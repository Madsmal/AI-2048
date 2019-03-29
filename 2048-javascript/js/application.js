animationDelay=50;
minSearchTime = 50;

function loadScript(name){
    
    var myScript= document.createElement("script");
    myScript.type = "text/javascript";
    myScript.src= name;
    document.body.appendChild(myScript);    
}

function loadCss(url) {
        var head = document.getElementsByTagName('head')[0];
        var link = document.createElement('link');
        link.type='text/css';
        link.rel = 'stylesheet';
        link.href = url;
        head.appendChild(link);
    }


//loadScript("js/jquery-3.2.1.min.js");
loadScript("js/common_function.js");
loadScript("js/ai_tile.js");
loadScript("js/ai_grid.js");
loadScript("js/ai.js");
loadScript("js/game_manager_ai.js");
loadCss("style/ai.css");


document.getElementsByClassName("container")[0].innerHTML +=  '<hr>'+'<p class="my_note">' +
                    '<strong class="important">Note:</strong> This ai-version is an updated version (we add AI function) on the official version of 2048. You can find the original one <a href="https://github.com/gabrielecirulli/2048?fbclid=IwAR08tJg7dZhlLuL7aBq7MAKkJjckZx6I8RrxIMcj_MUNXvahZHECxVPTWpk" target="_blank" class="my_note">here</a>.'+
                    'you can see <a target="blank" href="README.md">Readme </a> for detail changes.'
                '</p>'+
                '<p  class="my_note">'+
                'Created by <a href="https://leven87.github.io/" target="_blank"  class="my_note">Li Wang</a>'+
                '</p>';
        
document.getElementsByClassName("above-game")[0].innerHTML +=  '<p><a id="ai-auto-run" class="run-button">AI</a></p>';     

var aiRunButton = document.getElementById('ai-auto-run');


function sleep(delay) {
  var start = (new Date()).getTime();
  while ((new Date()).getTime() - start < delay) {
    continue;
  }
}

sleep(100);
loadScript("js/start.js");