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
    var btn = $(this);
    var row = btn.data('row'), col = btn.data('col');
    var hoveredBackgroundColor = 'rgb(230, 230, 230)'
    var bgColor = btn.css('background-color');

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
    let stringifiedPoints = JSON.stringify(selectedPoints)
    let data = {'points': stringifiedPoints};

    $.post("/", data, function(response){
      if(response.points !== undefined){
        let points = JSON.parse(response.points)
        console.log(points)
      }
    })
  });
})
