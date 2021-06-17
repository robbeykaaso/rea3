#!/usr/bin/env node
//https://medium.com/jspoint/creating-cli-executable-global-npm-module-5ef734febe32

var nopt = require("nopt"),
fs = require('fs'),
path = require("path"),
help = require("help")(path.resolve(path.dirname(__filename), "./usage.txt")),
knownOpts = {
    "version": Boolean,
    "init": Boolean,
    "help": Boolean
},
shortHands = {
    "v": ["--version"],
    "h": ["--help"]
}

function download(fileUrl, apiPath, callback) {
    var http = require('https')
                
    var timeout = 10000; 
    
    var file = fs.createWriteStream(apiPath);
    
    var timeout_wrapper = function( req ) {
        return function() {
            console.log('abort');
            req.abort();
            callback("Downloading timeout! ");
        };
    };

    var request = http.get(fileUrl).on('response', function(res) {         
        var len = parseInt(res.headers['content-length'], 10);
        var downloaded = 0;
        
        res.on('data', function(chunk) {
            file.write(chunk);
            downloaded += chunk.length;
            process.stdout.write("Downloading " + (100.0 * downloaded / len).toFixed(2) + "% " + downloaded + " bytes" + "\r");
            // reset timeout
            clearTimeout( timeoutId );
            timeoutId = setTimeout( fn, timeout );
        }).on('end', function () {
            // clear timeout
            clearTimeout( timeoutId );
            file.end();
            callback("Downloading finished! ");
        }).on('error', function (err) {
            // clear timeout
            clearTimeout( timeoutId );                
            callback(err.message + " ");
        });           
    });
    
    // generate timeout handler
    var fn = timeout_wrapper( request );

    // set initial timeout
    var timeoutId = setTimeout( fn, timeout );
}

var opts = nopt(knownOpts, shortHands, process.argv, 2)

function version(){
    const pkg = fs.readFileSync(path.resolve(path.dirname(__filename), "../package.json"))
    const js = JSON.parse(pkg)
    console.log(js["version"])
}

if (opts["init"])
    download("https://www.robbeykaaso.work:3000/test/viewupload/tools/rea.zip", "rea.zip", function(aMessage){
        console.log(aMessage)
    })
else if (opts["version"]){
    version()
}else if (opts["help"])
    help()
else
    console.log("please use --help to learn the usage")