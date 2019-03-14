/*added by Li*/
animationDelay=100;
minSearchTime = 100; 
GRID_LENGTH = 4;


// Wait till the browser is ready to render the game (avoids glitches)
window.requestAnimationFrame(function () {
  new GameManager(GRID_LENGTH, KeyboardInputManager, HTMLActuator, LocalStorageManager);
});
