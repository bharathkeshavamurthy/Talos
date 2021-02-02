
$(document).ready(function(){
	$("#forward").on("click", function() {
		$.ajax({
			url: '/command.json',
			dataType: 'json',
			method: 'POST',
			cache: false,
			headers: { 'X-Command': "forward" },
			data: { 'timestamp': Date.now()}
		});
	});
	$("#backward").on("click", function() {
		$.ajax({
			url: '/command.json',
			dataType: 'json',
			method: 'POST',
			cache: false,
			headers: { 'X-Command': "backward" },
			data: { 'timestamp': Date.now()}
		});
	});	
	$("#left").on("click", function() {
		$.ajax({
			url: '/command.json',
			dataType: 'json',
			method: 'POST',
			cache: false,
			headers: { 'X-Command': "left" },
			data: { 'timestamp': Date.now()}
		});
	});	
	$("#right").on("click", function() {
		$.ajax({
			url: '/command.json',
			dataType: 'json',
			method: 'POST',
			cache: false,
			headers: { 'X-Command': "right" },
			data: { 'timestamp': Date.now()}
		});
	});	
	$("#stop").on("click", function() {
		$.ajax({
			url: '/command.json',
			dataType: 'json',
			method: 'POST',
			cache: false,
			headers: { 'X-Command': "stop" },
			data: { 'timestamp': Date.now()}
		});
	});	
});	