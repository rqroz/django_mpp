class Point {
  constructor(x, y){
    this.x = x;
    this.y = y;
  }
}

var selectedPoints = [];

$(function(){
  $(".matrix-btn-group button").on('click', function(event){
    event.preventDefault();
    let btn = $(this);
    let row = btn.data('row'), col = btn.data('col');
    let hoveredBackgroundColor = 'rgb(230, 230, 230)'
    let bgColor = btn.css('background-color');

    if(bgColor == hoveredBackgroundColor){
      // Not selected, set selected
      btn.css('background-color', '#000');
    }else{
      // Selected, unselect
      btn.css('background-color', '');
    }

    let point = new Point(row, col);
    let index = selectedPoints.findIndex(p => p.x === point.x && p.y === point.y);
    if(index === -1){
      selectedPoints.push(point);
    }else{
      selectedPoints.splice(index, 1);
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
