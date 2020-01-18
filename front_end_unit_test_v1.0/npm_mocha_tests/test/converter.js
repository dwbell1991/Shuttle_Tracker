//Node style require()
var expect = require("chai").expect;
//Implementation code
var converter = require("../app/converter");

//Describe specific features we are implementing
describe("Color Code Converter", function() {
	//Feature 1
	describe("RGB to Hex conversion", function() {
		//'expectations' go in the 'it' function
  		it("converts the basic colors", function() {
  			//Inputs
  			var redHex   = converter.rgbToHex(255, 0, 0);
      	var greenHex = converter.rgbToHex(0, 255, 0);
     		var blueHex  = converter.rgbToHex(0, 0, 255);
     		
     		//Expected output tests
      		expect(redHex).to.equal("ff0000");
      		expect(greenHex).to.equal("00ff00");
      		expect(blueHex).to.equal("0000ff");
    	});
  	});
	//Feature 2
  	describe("Hex to RGB conversion", function() {
  		//'expectations' go in the 'it' function
  		it("converts the basic colors", function() {
  			//Inputs
  			var red   = converter.hexToRgb("ff0000");
      	var green = converter.hexToRgb("00ff00");
      	var blue  = converter.hexToRgb("0000ff");
		
			//Expected output tests
			//to.deep.equal for nested objects
      		expect(red).to.deep.equal([255, 0, 0]);
      		expect(green).to.deep.equal([0, 255, 0]);
      		expect(blue).to.deep.equal([0, 0, 255]);
    	});
  	});
});
