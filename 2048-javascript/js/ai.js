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
        bestScore = alpha;
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
                //positions ++;
                if(depth > 0){
                    if(initialDirection !== -1){
                        var tempResult = newAI.search(depth-1,bestScore,beta,positions,cutoffs,false,initialDirection);
                    }else{
                        var tempResult = newAI.search(depth-1,bestScore,beta,positions,cutoffs,false,direction);
                    }
                    cutoffs = tempResult.cutoffs;
                    positions = tempResult.positions;
                }
                
                if(depth == 0){
                    var evalScore = newAI.eval();
                    tempResult = { move: direction, score: evalScore};
                    //alpha = evalScore;
                    //continue;
                }
                
//                if(alpha > bestScore){//max节点，取所有节点中的最大分数
//                    bestScore = alpha;
//                }

//                if(searchBestResult == {} && initialDirection === -1){
//                    bestScore = evalScore;
//                    bestMove = moveResult.move;
//                    searchBestResult = { move: direction, score: bestScore,positions:positions,cutoffs:cutoffs };
//                }else if(bestScore < evalScore){
//                    //console.log(bestScore);
//                    bestScore = evalScore;
//                    bestMove = moveResult.move;
//                    searchBestResult = { move: direction, score: bestScore,positions:positions,cutoffs:cutoffs};
//                    if(initialDirection !== -1){
//                        searchBestResult.move = initialDirection;
//                    }
//                }
                //console.log(tempResult.score);
               if( bestScore < tempResult.score){
                   bestScore = tempResult.score;
                   bestMove = direction;
                   searchBestResult = tempResult;
               }
               if(bestScore >= beta){
                   cutoffs++;
                  // positions++;                         
                   return {move:direction,score:beta,positions:positions,cutoffs:cutoffs};//减枝,不去访问邻居节点，直接返回
               }               
            }
  
        }
       
    }
    
    
    //computer turn 相当于min节点，在这里beta要取子节点最大值中的最小值
    if(!playerTurn){
        bestScore = beta;
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
        var tempResult = newAI.search(depth,alpha,bestScore,positions,cutoffs,true,initialDirection);
        cutoffs = tempResult.cutoffs;
        positions = tempResult.positions;
        
        if( bestScore > tempResult.score){//这里要取最大值中的最小值
            bestScore = tempResult.score;
            searchBestResult = tempResult;
        }
        
        
        if(alpha >= bestScore){
            cutoffs++;
            //positions++;
            return {move:-1,score:alpha,positions:positions,cutoffs:cutoffs};       
        }
        //result = newAI.search(depth,alpha,beta,positions,0,true,initialDirection);
    }
    
    return { move: bestMove, score: bestScore, positions: positions, cutoffs: cutoffs };
    //return searchBestResult;
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
    var depth = 1;
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
        depth++;
       
    } while ((new Date()).getTime() - start < minSearchTime);
    console.log(best);
     console.log(depth);
    return best;
}