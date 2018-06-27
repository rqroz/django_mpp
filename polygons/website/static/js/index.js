class Point {
  constructor(x, y){
    this.x = x;
    this.y = y;
  }
}

var selectedPoints = [];
var pointSections = [];

function tryNewPoint(x, y, remove=false){
  let point = new Point(x, y);
  let index = selectedPoints.findIndex(p => p.x === point.x && p.y === point.y);
  if(index === -1){
    selectedPoints.push(point);
    $("#mb"+point.x+"-"+point.y).css('background-color', '#000');
    return true;
  }else if(remove){
    selectedPoints.splice(index, 1);
    $("#mb"+point.x+"-"+point.y).css('background-color', '');
    return false;
  }
}

function fillSection(endPoint){
  if(selectedPoints.length < 1){
    tryNewPoint(endPoint.x, endPoint.y, true);
    return;
  }

  let lastPoint = selectedPoints[selectedPoints.length - 1];
  let section = [];

  let btnGroup = $(".matrix-btn-group")
  if(lastPoint.x == endPoint.x){ // same row
    if(lastPoint.y < endPoint.y){
      for(var i = lastPoint.y + 1; i <= endPoint.y; ++i){
        let added = tryNewPoint(lastPoint.x, i);
        if(added){
          section.push(new Point(lastPoint.x, i));
        }
      }
    }else{
      for(var i = lastPoint.y - 1; i >= endPoint.y; --i){
        let added = tryNewPoint(lastPoint.x, i);
        if(added){
          section.push(new Point(lastPoint.x, i));
        }
      }
    }
  }else if(lastPoint.y == endPoint.y){ // same row
    if(lastPoint.x < endPoint.x){
      for(var i = lastPoint.x + 1; i <= endPoint.x; ++i){
        let added = tryNewPoint(i, lastPoint.y);
        if(added){
          section.push(new Point(i, lastPoint.y));
        }
      }
    }else{
      for(var i = lastPoint.x - 1; i >= endPoint.x; --i){
        let added = tryNewPoint(i, lastPoint.y);
        if(added){
          section.push(new Point(i, lastPoint.y));
        }
      }

    }
  }
  if(section.length > 0){
    pointSections.push(section);
  }
  toggle_reverse_btn()
}

function toggle_reverse_btn(){
  if(pointSections.length > 0){
    $("#reverse-btn").show(500);
  }else{
    $("#reverse-btn").hide(500);
  }
}

$(function(){
  $(".matrix-btn-group button").on('click', function(event){
    event.preventDefault();
    let btn = $(this);
    let row = btn.data('row'), col = btn.data('col');

    let point = new Point(row, col);
    let sel_type = $("#selection-type-btn").data('sel-type');

    if(sel_type == 'single'){
      tryNewPoint(row, col, true);
    }else{
      fillSection(point);
    }

  });

  $("#submit-matrix").on('click', function(event){
    event.preventDefault();
    if(selectedPoints.length < 1){
        bootbox.alert("<center>Escolha um contorno...</center>");
        return;
    }

    let stringifiedPoints = JSON.stringify(selectedPoints)
    let data = {'points': stringifiedPoints};

    $.post("/", data, function(response){
      if(response.error !== undefined){
        bootbox.alert(response.error);
      }else if(response.points !== undefined){
        let points = JSON.parse(response.points);
        console.log(points);
        populateModal(points);
      }
    });
  });

  $("#selection-type-btn").on('click', function(event){
    event.preventDefault();
    let btn = $(this);
    let sel_type = btn.data('sel-type');
    let text_span = btn.find('span');

    if(sel_type == 'single'){
      btn.text("Seleção Multipla");
      btn.data('sel-type', 'multiple');
      btn.removeClass('btn-warning');
      btn.addClass('btn-success');
    }else{
      btn.text("Seleção Unitária");
      btn.data('sel-type', 'single');
      btn.removeClass('btn-success');
      btn.addClass('btn-warning');
    }
  });

  $("#reverse-btn").on('click', function(event){
    if(pointSections.length > 0){
      lastSection = pointSections.pop();
      console.log(lastSection);
      for(var i = 0; i < lastSection.length; ++i){
        let x = lastSection[i].x, y = lastSection[i].y;
        let index = selectedPoints.findIndex(p => p.x === x && p.y === y);
        console.log("Trying to remove (",+x+", "+y+"), found index: "+index);
        if(index !== -1){
          $("#mb"+x+"-"+y).css('background-color', '');
          selectedPoints.splice(index, 1);
        }
      }
    }
    toggle_reverse_btn();
  });
});

function populateModal(points){
  let modal = $("#result-modal");
  let resulting_list = modal.find('#resulting-points-list');
  var points_list_html = "Pontos Resultantes:";

  for(var i = 0; i < points.length; ++i){
    let p = points[i];
    points_list_html += "<li class='list-item'> ("+p.x+", "+p.y+") </li>"
  }

  resulting_list.html(points_list_html);

  document.getElementById("result-img").src = img_url + "?t=" + new Date().getTime();

  modal.modal('show');
}
