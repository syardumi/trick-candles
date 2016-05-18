$('.toggleOutlet').on('click', function() {
	//ajax post call to run server side script for publishing Particle event to the cloud
	var dataParams = $( this ).attr('data-outletStatus')+','+$( this ).attr('data-outletLocation')+','+$( this ).attr('data-outletId');
	$.ajax({
		method: 'POST',
		url: 'particle.php',
		data: {
			action: 'switchLightsState',
			params: dataParams
		}
	}).done(function( msg ) {
    	console.log( "Data Saved: " + JSON.stringify(msg) );
  	});
});