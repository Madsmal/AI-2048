animationDelay=50;
minSearchTime = 50; 

var aiRunButton = document.getElementById('ai-auto-run');

// Wait till the browser is ready to render the game (avoids glitches)
window.requestAnimationFrame(function () {
  new GameManagerAI(4, KeyboardInputManager, HTMLActuator, LocalStorageManager,aiRunButton);
});
