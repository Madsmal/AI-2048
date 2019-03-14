var deepClone = function(source,target){
  source = source || {} ;
  target = target || {};
  var toStr = Object.prototype.toString ,
    arrStr = '[object array]' ;
  for(var i in source){
    if(source.hasOwnProperty(i)){
      var item = source[i] ;
      if(typeof item === 'object'){
        target[i] = (toStr.apply(item).toLowerCase() === arrStr) ? [] : {} ;
        deepClone(item,target[i]) ;
      }else{
        target[i] = item;
      }
    }
  }
  return target ;
} ;

function AIGrid(size, previousState){
  Grid.apply(this,arguments);
  //this.name = name || 'Tom';
}
AIGrid.prototype = deepClone(Grid.prototype) ;



AIGrid.prototype.test = function(){
    console.log("test");
}





//added by Li
// Move tiles on the grid in the specified direction
AIGrid.prototype.move = function (direction) {
  // 0: up, 1: right, 2: down, 3: left
  var self = this;

  //if (this.isGameTerminated()) return; // Don't do anything if the game's over

  var cell, tile;

  var vector     = this.getVector(direction);
  var traversals = this.buildTraversals(vector);
  var moved      = false;
  var score      = 0;
  var won = false;

  // Save the current tile positions and remove merger information
  this.prepareTiles();

  // Traverse the grid in the right direction and move tiles
  traversals.x.forEach(function (x) {
    traversals.y.forEach(function (y) {
      cell = { x: x, y: y };
      tile = self.cellContent(cell);

      if (tile) {
        var positions = self.findFarthestPosition(cell, vector);
        var next      = self.cellContent(positions.next);

        // Only one merger per row traversal?
        if (next && next.value === tile.value && !next.mergedFrom) {
          var merged = new Tile(positions.next, tile.value * 2);
          merged.mergedFrom = [tile, next];

          self.insertTile(merged);
          self.removeTile(tile);

          // Converge the two tiles' positions
          tile.updatePosition(positions.next);

          // Update the score
          score += merged.value;

          // The mighty 2048 tile
          if (merged.value === 2048) won = true;
        } else {
          self.moveTile(tile, positions.farthest);
        }

        if (!self.positionsEqual(cell, tile)) {
          moved = true; // The tile moved from its original cell!
        }
      }
    });
  });

  return {moved: moved, score: score, won: won};
  /*
  if (moved) {
    this.addRandomTile();

    if (!this.movesAvailable()) {
      this.over = true; // Game over!
    }

    this.actuate();
  }
  */
};

// Get the vector representing the chosen direction
AIGrid.prototype.getVector = function (direction) {
  // Vectors representing tile movement
  var map = {
    0: { x: 0,  y: -1 }, // Up
    1: { x: 1,  y: 0 },  // Right
    2: { x: 0,  y: 1 },  // Down
    3: { x: -1, y: 0 }   // Left
  };
  return map[direction];
};

// Save all tile positions and remove merger info
AIGrid.prototype.prepareTiles = function () {
  this.eachCell(function (x, y, tile) {
    if (tile) {
      tile.mergedFrom = null;
      tile.savePosition();
    }
  });
};

// Build a list of positions to traverse in the right order
AIGrid.prototype.buildTraversals = function (vector) {
  var traversals = { x: [], y: [] };

  for (var pos = 0; pos < this.size; pos++) {
    traversals.x.push(pos);
    traversals.y.push(pos);
  }

  // Always traverse from the farthest cell in the chosen direction
  if (vector.x === 1) traversals.x = traversals.x.reverse();
  if (vector.y === 1) traversals.y = traversals.y.reverse();

  return traversals;
};

AIGrid.prototype.findFarthestPosition = function (cell, vector) {
  var previous;

  // Progress towards the vector direction until an obstacle is found
  do {
    previous = cell;
    cell     = { x: previous.x + vector.x, y: previous.y + vector.y };
  } while (this.withinBounds(cell) &&
           this.cellAvailable(cell));

  return {
    farthest: previous,
    next: cell // Used to check if a merge is required
  };
};

AIGrid.prototype.moveTile = function (tile, cell) {
  this.cells[tile.x][tile.y] = null;
  this.cells[cell.x][cell.y] = tile;
  tile.updatePosition(cell);
};

AIGrid.prototype.positionsEqual = function (first, second) {
  return first.x === second.x && first.y === second.y;
};

AIGrid.prototype.addRandomTile = function () {
  if (this.cellsAvailable()) {
    var value = Math.random() < 0.9 ? 2 : 4;
    var tile = new Tile(this.grid.randomAvailableCell(), value);

    this.insertTile(tile);
  }
};

AIGrid.prototype.movesAvailable = function () {
  return this.cellsAvailable() || this.tileMatchesAvailable();
};

// Check for available matches between tiles (more expensive check)
AIGrid.prototype.tileMatchesAvailable = function () {
  var self = this;

  var tile;

  for (var x = 0; x < this.size; x++) {
    for (var y = 0; y < this.size; y++) {
      tile = this.cellContent({ x: x, y: y });

      if (tile) {
        for (var direction = 0; direction < 4; direction++) {
          var vector = self.getVector(direction);
          var cell   = { x: x + vector.x, y: y + vector.y };

          var other  = self.cellContent(cell);

          if (other && other.value === tile.value) {
            return true; // These two tiles can be merged
          }
        }
      }
    }
  }

  return false;
};

AIGrid.prototype.clone = function() {
  newGrid = new AIGrid(this.size);
  //newGrid.playerTurn = this.playerTurn;
  for (var x = 0; x < this.size; x++) {
    for (var y = 0; y < this.size; y++) {
      if (this.cells[x][y]) {
        newGrid.insertTile(this.cells[x][y].clone());
      }
    }
  }
  return newGrid;
};

AIGrid.prototype.findLargestTile = function(){
    var i=0, j=0;
    var position = {x:0,y:0};
    var largestValue = 0;
    var largestCell = null;
    for(i=0;i<4;i++){
        for(j=0;j<4;j++){
            if(this.cells[i][j] != null && (this.cells[i][j].value > largestValue)){
                largestValue = this.cells[i][j].value;
                largestCell = this.cells[i][j];
            }
        }
    }
    return largestCell;
}

AIGrid.prototype.largestTilePositionEval = function(){
   var corners = [{x:0,y:0},{x:0,y:3},{x:3,y:0},{x:3,y:3}];
   var tile = this.findLargestTile();
   var tilePosition = {x:tile.x,y:tile.y};
   
   if((tile.x == 0 || tile.x == 3) && (tile.y==0 || tile.y == 3)){
       return 50;
   }
   return 0;
//   if(tilePosition in corners){
//       console.log(tilePosition);
//       return 20;
//   }else{
//       return 0;
//   }
    
}

AIGrid.prototype.smoothEval = function(){
    var totalScore = 0;
    var arr = Array();
    for(i=0;i<4;i++){
        for(j=0;j<4;j++){
          if(this.cells[i][j] != null){
              arr.push(this.cells[i][j].value);
          }
        }
        if(arr.length == 4 && (arr[0]<arr[1]<arr[2]<arr[3] || arr[0]>arr[1]>arr[2]>arr[3])){
            totalScore += 5;
        }
        if(arr.length == 4 && arr[0]==arr[1]==arr[2]==arr[3]){
            totalScore += 25;
        }
        if(arr.length == 3 && (arr[0]<arr[1]<arr[2] || arr[0]>arr[1]>arr[2])){
            totalScore += 10;
        }
        if(arr.length == 3 && arr[0]==arr[1]==arr[2]){
            totalScore += 22;
        } 
        if(arr.length == 2 && arr[0]==arr[1]){
            totalScore += 20;
        }
        arr == Array();
    }
    
    for(i=0;i<4;i++){
        for(j=0;j<4;j++){
          if(this.cells[j][i] != null){
              arr.push(this.cells[j][i].value);
          }
        }
        if(arr.length == 4 && (arr[0]<arr[1]<arr[2]<arr[3] || arr[0]>arr[1]>arr[2]>arr[3]) ){
            totalScore += 5;
        }
        if(arr.length == 4 && arr[0]==arr[1]==arr[2]==arr[3]){
            totalScore += 25;
        }
        if(arr.length == 3 && (arr[0]<arr[1]<arr[2] || arr[0]>arr[1]>arr[2])){
            totalScore += 10;
        }
        if(arr.length == 3 && arr[0]==arr[1]==arr[2]){
            totalScore += 22;
        } 
        if(arr.length == 2 && arr[0]==arr[1]){
            totalScore += 20;
        }
        arr == Array();
    }    
    
    return totalScore;
}


