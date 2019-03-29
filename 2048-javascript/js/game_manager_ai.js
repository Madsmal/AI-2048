/*inherit class GameManager*/


class GameManagerAI extends GameManager{
    constructor(size, InputManager, Actuator, StorageManager,aiRunButton){
        super(size, InputManager, Actuator, StorageManager);
        this.aiAutoRunning = false;
        this.aiRunButton = aiRunButton;
        self = this;
//        $("body").on("click", "#ai-auto-run", function() {
//            this.aiRunButton.onclick = function(){
//              if (self.aiAutoRunning) {
//                self.aiAutoRunning = false;
//                document.getElementById('ai-auto-run').innerHTML = "AI";
//              } else {
//                self.aiAutoRunning = true;
//                self.run();
//                document.getElementById('ai-auto-run').innerHTML = "stop";
//              }               
//        }});

        this.aiRunButton.onclick = function(){
          if (self.aiAutoRunning) {
            self.aiAutoRunning = false;
            document.getElementById('ai-auto-run').innerHTML = "AI";
          } else {
            self.aiAutoRunning = true;
            self.run();
            document.getElementById('ai-auto-run').innerHTML = "stop";
          }          
        }
        this.setup();
    }
    
}
//function GameManagerAI(size, InputManager, Actuator, StorageManager,aiRunButton) {
//  GameManager.apply(this,arguments);  
//  this.aiAutoRunning = false;  
//  
//  this.aiRunButton = aiRunButton;
//  this.aiRunButton.onclick = function(){
//    if (this.aiAutoRunning) {
//      this.aiAutoRunning = false;
//      document.getElementById('ai-auto-run').innerHTML = "AI";
//    } else {
//      this.aiAutoRunning = true;
//      this.run();
//      document.getElementById('ai-auto-run').innerHTML = "stop";
//    }  
//  }
//  this.setup();
//} 
//
//GameManagerAI.prototype = deepClone(GameManager.prototype) ;




// Set up the game
GameManagerAI.prototype.setup = function () {
//  var previousState = this.storageManager.getGameState();

  // Reload the game from a previous game if present
//  if (previousState) {
//    this.grid        = new AIGrid(previousState.grid.size,
//                                previousState.grid.cells); // Reload grid
//    this.score       = previousState.score;
//    this.over        = previousState.over;
//    this.won         = previousState.won;
//    this.keepPlaying = previousState.keepPlaying;
//  } else {
    this.grid        = new AIGrid(this.size);
    this.score       = 0;
    this.over        = false;
    this.won         = false;
    this.keepPlaying = false;

    // Add the initial tiles
    this.addStartTiles();
//  }

  //arguments.callee.prototype.constructor.prototype.setup(); //use the function in super class 
  /*added by Li */
  this.ai  = new AI(this.grid);
  this.aiAutoRunning = false;
  document.getElementById('ai-auto-run').innerHTML = "AI";
    
  // Update the actuator
  this.actuate();
};

/*
 * added by Li
// moves continuously until game is over
*/
GameManagerAI.prototype.run = function() {
  //this.aiAutoRunning = true;  
  var best = this.ai.getBest();
  this.move(best.move);
  var timeout = animationDelay;
  //console.log(this.aiAutoRunning);
  if (this.aiAutoRunning && !this.over && (!this.won || this.keepPlaying)) {
    var self = this;
    setTimeout(function(){
      self.run();
    }, timeout);
  }
}
