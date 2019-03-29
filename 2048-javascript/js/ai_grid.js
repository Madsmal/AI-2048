function AIGrid(size, previousState){
  Grid.apply(this,arguments);
  //this.name = name || 'Tom';
}
AIGrid.prototype = deepClone(Grid.prototype) ;


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
          var merged = new AITile(positions.next, tile.value * 2);
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
AIGrid.prototype.FmgetVector = function (direction) {
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
    var tile = new AITile(this.grid.randomAvailableCell(), value);

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
//    console.log("0 0:");
//    console.log(this.cells[3][3]);
//    writeObj(this.cells);alert("fsfsa")
    for(i=0;i<4;i++){
        for(j=0;j<4;j++){
            if(this.cells[i][j] != null){
                var x = (this.cells[i][j]).x;
                var y = (this.cells[i][j]).y;
                if((this.cells[i][j].value >= largestValue)){
                    if(largestValue == 0 || (this.cells[i][j].value > largestValue) || ((x == 0 || x==3) && (y == 0 || y==3)) ){//Priority in the Corner
                        largestValue = this.cells[i][j].value;
                        largestCell = this.cells[i][j];
                       // console.log("i:"+i+",j:"+j)
                    }
                }
            }
            //console.log(this.cells[i][j]);
        }
    }
    return largestCell;
}

AIGrid.prototype.getTileNum = function(value){
    var num =0;
    for(i=0;i<4;i++){
        for(j=0;j<4;j++){
            if(this.cells[i][j] != null){
                if((this.cells[i][j].value == value)){
                    num ++;
                }
            }    
        }
    }
}

AIGrid.prototype.largestTilePositionEval = function(){
   var corners = [{x:0,y:0},{x:0,y:3},{x:3,y:0},{x:3,y:3}];
   var tile = this.findLargestTile();
   var tilePosition = {x:tile.x,y: tile.y};
   var score  =0;
   var x = tile.x;
   var y = tile.y;
   
   //score += tile.value/4;
   if(tile.x == 0 && tile.y ==0){
       //console.log("here2");
   //if((tile.x == 0 || tile.x == 3) && (tile.y==0 || tile.y == 3) && (tile.value >= 8)){
       score += 100 ;
       if(tile.value > MAX_TILE_VALUE){
           score += tile.value;
       }
        if(MAX_TILE_VALUE >=32 &&  this.getTileNum(MAX_TILE_VALUE) > MAX_TILE_NUM){
           score += tile.value/2;
       }
//        if(MAX_TILE_VALUE >=128 &&  this.getTileNum(MAX_TILE_VALUE/2) > 2){
//           score += 10;
//       }       
       
//       if(tile.y > 1 && this.cells[x][y-1] != null ){//up
//           if(this.cells[x][y-1].value == tile.value){
//                score += 20;
//            }
//           if(this.cells[x][y-1].value == tile.value/2){
//                score += 10;
//            }            
//        }
//       if(tile.y < 2 && this.cells[x][y+1] != null ){//down
//           if(this.cells[x][y+1].value == tile.value){
//                score += 20;
//            }
//           if(this.cells[x][y+1].value == tile.value/2){
//                score += 10;
//            }            
//        }  
//       if(tile.x > 1 && this.cells[x-1][y] != null ){//left
//           if(this.cells[x-1][y].value == tile.value){
//                score += 20;
//            }
//           if(this.cells[x-1][y].value == tile.value/2){
//                score += 10;
//            }            
//        } 
//       if(tile.x < 2 && this.cells[x+1][y] != null ){//right
//           if(this.cells[x+1][y].value == tile.value){
//                score += 20;
//            }
//           if(this.cells[x+1][y].value == tile.value/2){
//                score += 10;
//            }            
//        }
        
        //var vector = this.getVector(1);
//        if((tile.x == 0 && tile.y == 0)){ 
//                if(this.cells[x+1][y] && this.cells[x+1][y].value >= tile.value/2) && (this.cells[x+2][y] && this.cells[x+2][y] >= this.cells[x+1][y].value) && (this.cells[x+3][y] && this.cells[x+3][y] >= this.cells[x+2][y].value)){
//                     score += 30;
//                } 
//                if((this.cells[x][y+1] && this.cells[x][y+1].value >= tile.value/2) && (this.cells[x][y+2] && this.cells[x][y+2] >= this.cells[x][y+1].value) && (this.cells[x][y+3] && this.cells[x][y+3] >= this.cells[x][y+2].value)){
//                    score += 30;
//                }
//            }
//        }
           //console.log(this.cellMonotonicity(tile));
           score += 2* this.cellMonotonicity(tile);
   }else{
       if(tile.previousPosition && tile.previousPosition.x == 0 && tile.previousPosition.y == 0){
           score -= 1000;
       }
       //score = -100;
       //score = -100 * tile.value /8 ;
   }
   return score;
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
        if(arr.length == 4 && (arr[0]<=arr[1]<=arr[2]<=arr[3] || arr[0]>=arr[1]>=arr[2]>=arr[3]) && (arr[3]<=4*arr[0] || arr[0]<=4*arr[3])){
            //totalScore += 10;
            if(arr.length == 4 && arr[0]==arr[1]==arr[2]==arr[3]){
                totalScore += 10;
            }
        }

        if(arr.length == 3 && (arr[0]<=arr[1]<=arr[2] || arr[0]>=arr[1]>=arr[2])){
            //totalScore += 10;
            if(arr.length == 3 && arr[0]==arr[1]==arr[2]){
                totalScore += 10;
            } 
        }

        if(arr.length == 2 && arr[0]==arr[1]){
            totalScore += 1;
        }
        arr == Array();
    }
    
    for(i=0;i<4;i++){
        for(j=0;j<4;j++){
          if(this.cells[j][i] != null){
              arr.push(this.cells[j][i].value);
          }
        }
        if(arr.length == 4 && (arr[0]<=arr[1]<=arr[2]<=arr[3] || arr[0]>=arr[1]>=arr[2]>=arr[3]) && (arr[3]<=4*arr[0] || arr[0]<=4*arr[3]) ){
            //totalScore += 10;
            if(arr.length == 4 && arr[0]==arr[1]==arr[2]==arr[3]){
                 totalScore += 10;
            }
        }
        if(arr.length == 3 && (arr[0]<=arr[1]<=arr[2] || arr[0]>=arr[1]>=arr[2])){
            //totalScore += 10;
            if(arr.length == 3 && arr[0]==arr[1]==arr[2]){
                totalScore += 10;
             } 
        }

        if(arr.length == 2 && arr[0]==arr[1]){
            totalScore += 2;
        }
        arr = Array();
    }    

//    for(i=0;i<4;i++){
//        for(j=0;j<4;j++){
//            if(this.cells[j][i] != null){
//                 v = this.cells[j][i].value;
//                 
//            } 
//        }
//    }
    return totalScore;
}

AIGrid.prototype.cellMonotonicity  = function (cell){
    if (!this.withinBounds(cell)){
        return 0;
    }
    var x= cell.x;
    var y = cell.y;
    var arrX = Array();
    var arrY = Array();
    var factorX = 1;
    var factorY = 1;
    var i,j;


    for(j=0;j<4;j++){
        if(this.cells[x][j]){
            arrX.push(this.cells[x][j].value);
        }
    }
    for(i=0;i<4;i++){
        if(this.cells[i][y]){
            arrY.push(this.cells[i][y].value);
        }
    }
    
    if(arrX.length >= 5){
        if(x == 0){//top->down
            for(i=1;i<arrX.length;i++){
                if(arrX[i-1] == arrX[i] || arrX[i-1] == 2* arrX[i]){
                    factorX += (arrX.length-i)*(arrX.length-i);
                }
            }
            if(arrX[0] == arrX[1] || arrX[0] == 2*arrX[1]){
                factorX += 20;
                if(arrX[1] == arrX[2] || arrX[1] == 2*arrX[2]){
                    factorX += 5;
                    if(arrX[1] == arrX[2] || arrY[1] == 2*arrX[2]){
                        factorX += 3;
                    }
                }                
            }
            if(arrX[1] <= arrX[0] / 8){
                //factorX -= 10;
            }
        }
        if( x==3){//bottom
            for(i=arrX.length-1;i>0;i--){
                if(arrX[i] == arrX[i-1] || arrX[i] == 2* arrX[i-1]){
                    factorX += 2;
                }
            }
            if(arrX[arrX.length-1] == arrX[arrX.length-2] || arrX[arrX.length-1] == 2*arrX[arrX.length-2]){
                factorX += 10;
            }             
        }
    }

    if(arrY.length >= 3){
        if(y == 0){//left->right
            for(i=1;i<arrY.length;i++){
                if(arrY[i-1] == arrY[i] || arrY[i-1] == 2* arrY[i]){
                    factorY += Math.log(arrY[i])*(arrY.length-i);
                }
            }
            if(arrY[0] == arrY[1] || arrY[0] == 2*arrY[1]){
                factorY += 20;
                if(arrY[1] == arrY[2] || arrY[1] == 2*arrY[2]){
                    factorY += 5;
                    if(arrY[1] == arrY[2] || arrY[1] == 2*arrY[2]){
                        factorY += 3;
                    }
                }
            }
            if(arrY[1] <= arrY[0] / 8){
                //factorY -= 10;
                //console.log("here");
            }            
        }
        if( y==3){//right
            for(i=arrY.length-1;i>0;i--){
                if(arrY[i] == arrY[i-1] || arrY[i] == 2* arrY[i-1]){
                    factorY += 2;
                }
            }
            if(arrY[arrY.length-1] == arrY[arrY.length-2]){
                factorY += 10;
            }            
        }
    }    
    
    return factorX * factorY;
}


//evaluate the all grid monotonicity in up-to-down direction
AIGrid.prototype.monotonicity3 = function(){
    var score =0;
    for( var x=0;x<4;x++){
        for(var current=0; current<3;current++){
            var next = current+1;
            if(!this.cellOccupied( {x:x,y:current})){
                continue;
            }
            //var currentValue = this.cellOccupied({x:x, y:current}) ?
        //Math.log(this.cellContent( this.indexes[x][current] ).value) / Math.log(2) :
        //0;  
            var currentValue = Math.log(this.cellContent( this.indexes[x][current] ).value) / Math.log(2);
            while(next < 4){
                if(!this.cellOccupied({x:x,y:next})){
                    next ++;
                    continue;
                }
//                var nextValue = this.cellOccupied({x:x, y:next}) ?
//        Math.log(this.cellContent( this.indexes[x][next] ).value) / Math.log(2) :
//        0;  
                var nextValue = Math.log(this.cellContent( this.indexes[x][next] ).value) / Math.log(2); 
                //if (currentValue > nextValue) {
                if (nextValue > currentValue) {
                    if(x == 0){
                        score += (currentValue - nextValue)*2;
                    }else{
                        score += (currentValue - nextValue)*2;
                    }
                }
                next ++;
            }
        }
    }
    
    for( var y=0;y<2;y++){
        for(var current=0; current<3;current++){
            var next = current+1;
            if(!this.cellOccupied( {x:current,y:y})){
                continue;
            }

            var currentValue = Math.log(this.cellContent( this.indexes[current][y] ).value) / Math.log(2);
            while(next < 4){
                if(!this.cellOccupied({x:next,y:y})){
                    next ++;
                    continue;
                }
    //                var nextValue = this.cellOccupied({x:x, y:next}) ?
    //        Math.log(this.cellContent( this.indexes[x][next] ).value) / Math.log(2) :
    //        0;  
                var nextValue = Math.log(this.cellContent( this.indexes[next][y] ).value) / Math.log(2); 
                //if (currentValue > nextValue) {
                if (nextValue > currentValue && y == 0) {    
                    if(y == 0){
                        score += (currentValue - nextValue)*2;
                    }
                }
                if(nextValue < currentValue && y == 1){
                     score += (nextValue - currentValue);
                }
                next ++;
            }
        }
    }
    //console.log(score);
    return score;
}


//getNeighborTileValue
//direction  0: up, 1: right, 2: down, 3: left
//AIGrid.prototype.getNeighborTileValue = function(x,y,direction){
//    if(this.cells[x][y] == null){
//        return null;
//    }
//    if(direction == 0 && y > 1 && this.cells[x][y-1] != null){
//        return 
//    }
//        
//}
    


function writeObj(obj){ 
 var description = ""; 
 for(var i in obj){ 
 var property=obj[i]; 
 description+=i+" = "+property+"\n"; 
 } 
 console.log(description); 
}










//can't use i

//  use a simple datastructure to store the value in an array
AIGrid.prototype.indexes = [];
for (var x=0; x<4; x++) {
  AIGrid.prototype.indexes.push([]);
  for (var y=0; y<4; y++) {
    AIGrid.prototype.indexes[x].push( {x:x, y:y} );
  }
}
