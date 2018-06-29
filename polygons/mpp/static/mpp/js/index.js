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

function addPointInSection(x, y, section){
  let added = tryNewPoint(x, y);
  if(added){
    section.push(new Point(x, y));
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
    if(lastPoint.y < endPoint.y){ // left to right
      for(var i = lastPoint.y + 1; i <= endPoint.y; ++i){
        addPointInSection(lastPoint.x, i, section);
      }
    }else{ // right to left
      for(var i = lastPoint.y - 1; i >= endPoint.y; --i){
        addPointInSection(lastPoint.x, i, section);
      }
    }
  }else if(lastPoint.y == endPoint.y){ // same col
    if(lastPoint.x < endPoint.x){ // top down
      for(var i = lastPoint.x + 1; i <= endPoint.x; ++i){
        addPointInSection(i, lastPoint.y, section);
      }
    }else{ // bottom up
      for(var i = lastPoint.x - 1; i >= endPoint.x; --i){
        addPointInSection(i, lastPoint.y, section);
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
    $("#reverse-btn-container").slideDown(500);
  }else{
    $("#reverse-btn-container").slideUp(500);
  }
}


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

$(function(){
  $('.popover-dismiss').popover({
    trigger: 'focus'
  });
  
  $("#mpp-popover").popover({
      html : true,
      content: function() {
        return "ha"
        // return $("#mpp-popover-content").html();
      },
      title: function() {
        return $("#mpp-popover-title").html();
      }
  });


  $(".matrix-btn-group button").on('click', function(event){
    event.preventDefault();
    let btn = $(this);
    let row = btn.data('row'), col = btn.data('col');

    let point = new Point(row, col);

    if($(".switch input").is(":checked")){
      fillSection(point);
    }else{
      tryNewPoint(row, col, true);
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
