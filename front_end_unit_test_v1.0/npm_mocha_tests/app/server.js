/*
Description: The server application which makes calls
via the express() import which allows for the ability 
to send HTTP requests and sets up a mock server. 
*/
var express = require("express");
var app = express();	//simple API
var converter = require("./converter");	

app.get("/rgbToHex", function(req, res) {
	var red   = parseInt(req.query.red, 10);
  	var green = parseInt(req.query.green, 10);
  	var blue  = parseInt(req.query.blue, 10);
	
  	var hex = converter.rgbToHex(red, green, blue);

  res.send(hex);
});

app.get("/hexToRgb", function(req, res) {
	var hex = req.query.hex;

 	var rgb = converter.hexToRgb(hex);

 	res.send(JSON.stringify(rgb));
});

app.get("/latlng", function(req, res) {

	//URL variables
	var lat = req.query.lat;
	var lng = req.query.lng;

	var latlng = converter.latlng(lat, lng);

	//Verifying check
	if(lat.length < 11 && lng.length < 11 && lat.includes(".") && lng.includes(".") 
		&& correct_chars(lat) && correct_chars(lng)) {
			res.send(JSON.stringify(latlng));
	}else {
		var fail = "Failure";
		res.send(fail);
	}

});

function correct_chars(str) {
	return /^[0-9.-]*$/.test(str);
}


//Listening on port xxxx
app.listen(3000);