// Wait till the browser is ready to render the game (avoids glitches)
window.requestAnimationFrame(function () {
  new GameManagerAI(4, KeyboardInputManager, HTMLActuator, LocalStorageManager,aiRunButton);
});


