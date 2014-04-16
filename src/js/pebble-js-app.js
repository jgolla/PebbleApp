Pebble.addEventListener("ready", function() {
  console.log("ready called!");
});

function getNews() {
    console.log("getNews");
  var req = new XMLHttpRequest();
  req.open("GET", "http://api.espn.com/v1/sports/hockey/nhl/news?apiKey=", true);

  //Pebble.sendAppMessage({ "title": "my title here" });

  req.onload = function(e) {
    if (req.readyState == 4) {
      if(req.status == 200) {
        var response = JSON.parse(req.responseText);
        if (response.headlines) {
            console.log(response.headlines[0].title);
            Pebble.sendAppMessage({ "title": response.headlines[0].title });//, "description": response.headlines[0].description });
        }
      } else {
        console.log("Request returned error code " + req.status.toString());
      }
    }
  }

  req.send(null);
}

// Set callback for appmessage events
Pebble.addEventListener("appmessage", function(e) {
    console.log("appmessage");
    if (e.payload.symbol) {
        //symbol = e.payload.symbol;
        //localStorage.setItem("symbol", symbol);
        //fetchStockQuote(symbol);
    }
    if (e.payload.fetch) {
        getNews();
    }
    if (e.payload.price) {
        //fetchStockQuote(symbol);
    }
});

/*Pebble.addEventListener("showConfiguration", function() {
  console.log("showing configuration");
  Pebble.openURL('http://assets.getpebble.com.s3-website-us-east-1.amazonaws.com/pebble-js/configurable.html');
});

Pebble.addEventListener("webviewclosed", function(e) {
  console.log("configuration closed");
  // webview closed
  var options = JSON.parse(decodeURIComponent(e.response));
  console.log("Options = " + JSON.stringify(options));
});*/