function openWait(){
  var wait = $('#wait-modal');
  wait.modal({backdrop: 'static', keyboard: false});
  $("#wait-modal").modal('show');
}

function closeWait(){
  $("#wait-modal").modal('hide');
}

$(function(){
  var csrftoken = getCookie('csrftoken');

  $.ajaxSetup({
    tryCount: 0,
    retryLimit: 3,
    beforeSend: function(xhr, settings) {
      if (!csrfSafeMethod(settings.type) && !this.crossDomain) {
        xhr.setRequestHeader("X-CSRFToken", csrftoken);
      }
    },
    error: function(x, t, m){
      bootbox.alert("Houve um erro ao executar esta ação. Status: " + x.status)
    }
  });

  //$('[data-toggle="tooltip"]').tooltip();
  $('[data-toggle="popover"]').popover();
});
