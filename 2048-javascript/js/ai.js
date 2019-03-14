function AI(grid) {
    this.grid = grid;
}



//evaluate function 
AI.prototype.eval = function () {
    var emptyCellsWeight = 2;
    var smoothWeight = 3;
    var largestTilePositionWeight = 2;
    var emptyCells = this.grid.availableCells().length;
    var smoothScore = this.grid.smoothEval();
    var largestTilePositionScore = this.grid.largestTilePositionEval();
//    if(largestTilePositionScore !=0)
//        console.log(largestTilePositionScore);
    //return emptyCells * emptyCellsWeight;
    //return Math.floor(Math.random() * 100);
     //bestMove = Math.floor(Math.random() * (end - start + 1) + start);
    //return emptyCells;
    return emptyCells * emptyCellsWeight 
            + smoothScore * smoothWeight
            + largestTilePositionWeight * largestTilePositionScore;
    
}


//depth -- 搜索深度
//alpha -- lower bound on what max can achieve when playing through the choice points leading to the current node
//beta --  upper bound on what min can achieve when playing through the choice points leading to the current node
//positions -- 存储已经到达过的位置状态
AI.prototype.search = function (depth, alpha, beta, positions, cutoffs,playerTurn = true,initialDirection= -1) {
    var bestMove = -1;
    var bestScore = 0;
    var searchBestResult = {};
    searchBestResult.score = bestScore;
    
    
    
    if(playerTurn){
        // 0: up, 1: right, 2: down, 3: left
        for (var direction in [0, 1, 2, 3]) {
            //store all direction in this depth
            var result = [];

            //create a new grid  
            var newGrid = this.grid.clone();
            
            //create a new AI
            var newAI = new AI(newGrid);        
            var moveResult = newGrid.move(direction);
            //return {moved: moved, score: score, won: won};
            if(moveResult.moved ){
                var evalScore = newAI.eval();

                //result[direction] = {score:evalScore };
                if(searchBestResult == {} && initialDirection === -1){
                    bestScore = evalScore;
                    bestMove = moveResult.move;
                    searchBestResult = { move: direction, score: bestScore,positions:positions };
                }else if(bestScore < evalScore){
                   bestScore = evalScore;
                    bestMove = moveResult.move;
                    searchBestResult = { move: direction, score: bestScore,positions:positions };
                    if(initialDirection !== -1){
                        searchBestResult.move = initialDirection;
                        //alert(initialDirection);
                    }
                }
               // console.log(evalScore + " " + direction);
                 
                if(depth == 0){
                    continue;
                }
                if(depth > 0){
                    //newAI.computerTurnSearch();
                    if(initialDirection !== -1){
                        var tempResult = newAI.search(depth-1,0,0,positions+1,0,false,initialDirection);
                    }else{
                        var tempResult = newAI.search(depth-1,0,0,positions+1,0,false,direction);
                    }
                    //var tempResult = newAI.search(depth-1,0,0,positions+1,0,evalScore);
                    if(searchBestResult.score < tempResult.score){
                        searchBestResult = tempResult;
                    }
                }                  
            }
  
        }
       
    }
    
    
    //computer turn
    if(!playerTurn){
        var cells = this.grid.availableCells();
        var start = 0; 
        var  end = cells.length;
        var addedNumber;
        var worstTile = null;
        var worstScore = 0;
        for(var i=0;i<end;i++){
            addedNumber = 2;
            var newTile = new Tile(cells[i],addedNumber);
            this.grid.insertTile(newTile);
            var score = this.eval();
            if(worstTile == null){
                worstTile = newTile;
                worstScore = score;
            }
            this.grid.removeTile(newTile);
            
            
            addedNumber = 4;
            var newTile = new Tile(cells[i],addedNumber);
            this.grid.insertTile(newTile);
            var score = this.eval();
            if(worstScore > score){
                worstTile = newTile;
                worstScore = score;                
            }
           this.grid.removeTile(newTile);
        }
//        var index = Math.floor(Math.random() * (end - start + 1) + start);
//        var cell = this.grid.availableCells()[index];
//        
//        var r = Math.random() * 4;
//        
//        if(r<2){
//            addedNumber = 2;
//        }else{
//            addedNumber = 4;
//        }
//        
        var newGrid = this.grid.clone();
        newGrid.insertTile(worstTile);
        var newAI = new AI(newGrid);

        return newAI.search(depth,0,0,positions,0,true,initialDirection);
//        result =   newAI.search(depth,0,0,positions,0,true);
//            if (result.score > searchBestResult.score) {
//          searchBestResult = result;
//        }
//        return result;
        
    }
    return searchBestResult;
    
//    var end = 3, start = 0;
//    bestMove = Math.floor(Math.random() * (end - start + 1) + start);
//    return {move: bestMove, score: 0, positions: 0, cutoffs: 0};
}


// calculate computer's strategy, maybe just choose the worst one for human
AI.prototype.computerTurnSearch = function(){
    var start = 0; 
    var  end = this.grid.availableCells().length - 1;
    var index = Math.floor(Math.random() * (end - start + 1) + start);
    var cellLocation = this.grid.availableCells()[index];
    var r = Math.random() * 4;
    var addedNumber;
    if(r<2){
        addedNumber = 2;
    }else{
        addedNumber = 4;
    }
    this.grid.insertTile(new Tile(cellLocation,addedNumber));
}


// performs a search and returns the best move
AI.prototype.getBest = function () {

    return this.iterativeDeep();
}

// performs iterative deepening over the alpha-beta search
AI.prototype.iterativeDeep = function () {

    var start = (new Date()).getTime();
    var depth = 5;
    var best;

// can't limit the excution time of function in javascript!
//    setTimeout(this.search(depth, -10000, 10000, 0, 0),minSearchTime); 
//    return;
    do {
        var newBest = this.search(depth, -10000, 10000, 0, 0, true);
        if (newBest.move == -1) {// deal with exception
            break;
        } else {
            best = newBest;
        }
        //depth++;
    } while ((new Date()).getTime() - start < minSearchTime);
    console.log(best);
    return best;
}