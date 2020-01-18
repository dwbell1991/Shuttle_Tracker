/*
Description: This is the file where the test descriptions lie.
This file is broken into three sets of tests. 
1) From a website tutorial 
2) For latitutde values
3) For longitude values
*/

var expect  = require("chai").expect;
var request = require("request");

/*Color Converter Tutorial ------------------------------------------
https://semaphoreci.com/community/tutorials/getting-started-with-node-js-and-mocha*/
describe("Color Code Converter API", function() {

  describe("RGB to Hex conversion", function() {

    var url = "http://localhost:3000/rgbToHex?red=255&green=255&blue=255";

    it("returns status 200", function(done) {
      request(url, function(error, response, body) {
        expect(response.statusCode).to.equal(200);
        done(); //Wait for callback
      });
    });

    it("returns the color in hex", function(done) {
      request(url, function(error, response, body) {
        expect(body).to.equal("ffffff");
        done(); //Wait for callback
      });
    });

  });

  describe("Hex to RGB conversion", function() {
    var url = "http://localhost:3000/hexToRgb?hex=00ff00";

    it("returns status 200", function(done) {
      request(url, function(error, response, body) {
        expect(response.statusCode).to.equal(200);
        done(); //Wait for callback
      });
    });

    it("returns the color in RGB", function(done) {
      request(url, function(error, response, body) {
        expect(body).to.equal("[0,255,0]");
        done(); //Wait for callback
      });
    });
  });

});

/*Latitude Checks --------------------------------------------------*/
describe("Latitude Checks API", function() {

      //Ensure connection status
      describe("Connection Check", function() {
        var url = "http://localhost:3000/latlng?lat=0&lng=0";
        it("returns status 200", function(done) {
            request(url, function(error, response, body) {
            expect(response.statusCode).to.equal(200);
            done(); //Wait for callback
            });
          });
        });
  
      //Correct Length String
      describe("Latitude Check 1 (Pass):", function(done) {
        var url = "http://localhost:3000/latlng?lat=41.247104&lng=0.001";
        it("returns latitude with correct length", function(done) {
            request(url, function(error, response, body) {
          expect(body).to.equal('["41.247104","0.001"]');
            done(); //Wait for callback
          });
      });
    });

      //Incorrect Length String
      describe("Latitude Check 1 (Fail):", function(done) {
        var url = "http://localhost:3000/latlng?lat=41.24799104&lng=0.001";
        it("returns latitude with correct length", function(done) {
            request(url, function(error, response, body) {
          expect(body).to.equal('Failure');
            done(); //Wait for callback
          });
      });
    });
  
    //Correct Characters in string
    describe("Latitude Check 2 (Pass):", function(done) {
      var url = "http://localhost:3000/latlng?lat=41.247104&lng=0.001";
      it("returns latitude with correct characters", function(done) {
          request(url, function(error, response, body) {
          expect(body).to.equal('["41.247104","0.001"]');
          done(); //Wait for callback
        });
      });
    });

    //Incorrect Characters in string
    describe("Latitude Check 2 (Fail):", function(done) {
      var url = "http://localhost:3000/latlng?lat=41.247104&lng=0.**001";
      it("returns latitude with correct characters", function(done) {
          request(url, function(error, response, body) {
          expect(body).to.equal('Failure');
          done(); //Wait for callback
        });
      });
    });
  
    //Contains period symbol
    describe("Latitude Check 3 (Pass):", function(done) {
      var url = "http://localhost:3000/latlng?lat=41.247104&lng=0.001";
      it("returns latitude with period", function(done) {
          request(url, function(error, response, body) {
          expect(body).to.equal('["41.247104","0.001"]');
          done(); //Wait for callback
        });
      });
  });

     //Contains period symbol
    describe("Latitude Check 3 (Fail):", function(done) {
      var url = "http://localhost:3000/latlng?lat=41247104&lng=0.001";
      it("returns latitude with period", function(done) {
          request(url, function(error, response, body) {
          expect(body).to.equal('Failure');
          done(); //Wait for callback
        });
      });
    });
});


/*Longitude Checks --------------------------------------------------*/
describe("Longitude Checks API", function() {

      //Ensure connection status
      describe("Connection Check", function() {
        var url = "http://localhost:3000/latlng?lat=0&lng=0";
        it("returns status 200", function(done) {
            request(url, function(error, response, body) {
            expect(response.statusCode).to.equal(200);
            done(); //Wait for callback
            });
          });
        });
  
      //Correct Length String
      describe("Longitude Check 1 (Pass):", function(done) {
        var url = "http://localhost:3000/latlng?lat=0.001&lng=-96.016709";
        it("returns longitude with correct length", function(done) {
            request(url, function(error, response, body) {
          expect(body).to.equal('["0.001","-96.016709"]');
            done(); //Wait for callback
          });
      });
    });

      //Incrrect Length String
      describe("Longitude Check 1 (Fail):", function(done) {
        var url = "http://localhost:3000/latlng?lat=0.001&lng=-96.09916709";
        it("returns longitude with correct length", function(done) {
            request(url, function(error, response, body) {
          expect(body).to.equal('Failure');
            done(); //Wait for callback
          });
      });
    });
  
    //Correct characters in string
    describe("Longitude Check 2 (Pass):", function(done) {
      var url = "http://localhost:3000/latlng?lat=0.001&lng=-96.016709";
      it("returns longitude with correct characters", function(done) {
          request(url, function(error, response, body) {
          expect(body).to.equal('["0.001","-96.016709"]');
          done(); //Wait for callback
        });
      });
    });

    //Incorrect characters in string
    describe("Longitude Check 2 (Fail):", function(done) {
      var url = "http://localhost:3000/latlng?lat=0.001&lng=-96.01!709";
      it("returns longitude with correct characters", function(done) {
          request(url, function(error, response, body) {
          expect(body).to.equal('Failure');
          done(); //Wait for callback
        });
      });
    });
  
    //Contains period in string
    describe("Longitude Check 3 (Pass):", function(done) {
      var url = "http://localhost:3000/latlng?lat=0.001&lng=-96.016709";
      it("returns latitude with period", function(done) {
          request(url, function(error, response, body) {
          expect(body).to.equal('["0.001","-96.016709"]');
          done(); //Wait for callback
        });
    });
  });

    //Does not contain period
    describe("Longitude Check 3 (Fail):", function(done) {
      var url = "http://localhost:3000/latlng?lat=0.001&lng=-96016709";
      it("returns latitude with period", function(done) {
          request(url, function(error, response, body) {
          expect(body).to.equal('Failure');
          done(); //Wait for callback
        });
    });
  });
});
