var MAX_TILE_VALUE = 0;
var MAX_TILE_NUM = 0;
function AI(grid) {
    this.grid = grid;
}



//evaluate function 
AI.prototype.eval = function () {
    var emptyCellsWeight = 5;
    var smoothWeight = 1;
    var largestTilePositionWeight = 1;
    var emptyCells = this.grid.availableCells().length;
    var smoothScore = this.grid.smoothEval();
    var largestTilePositionScore = this.grid.largestTilePositionEval();
    if(largestTilePositionScore > 0){
//        console.log(this.grid.findLargestTile());
//        console.log(code);
//        console.log(largestTilePositionScore);
    }
    
    var monotonicity2Score = this.grid.monotonicity3();
    //console.log(this.grid.monotonicity3());
//    console.log(this.grid.monotonicity2());
    //console.log(monotonicity2Score);
    
    return emptyCells * emptyCellsWeight 
            + smoothScore * smoothWeight
            + largestTilePositionWeight * largestTilePositionScore +monotonicity2Score*2;
    
}


//depth -- 搜索深度
//alpha -- lower bound on what max can achieve when playing through the choice points leading to the current node
//beta --  upper bound on what min can achieve when playing through the choice points leading to the current node
//positions -- 存储已经到达过的位置状态
AI.prototype.search = function (depth, alpha, beta, positions, cutoffs,playerTurn = true) {
    var bestMove = -2;
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
                    var tempResult = newAI.search(depth-1,bestScore,beta,positions,cutoffs,false);
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
                //console.log(bestScore);
               // console.log("bestScore in alpha:"+tempResult.score);
               // console.log("tempResult.score:"+tempResult.score);
                
               if( bestScore == tempResult.score && bestMove < 0){
                    bestMove = direction;
               } 
               if( bestScore < tempResult.score){
                   bestScore = tempResult.score;
                   bestMove = direction;
                   searchBestResult = tempResult;
               }
               // console.log("beta:"+beta);
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
        var worstScore = 10000;
        var tiles = {};//worst tile
        var tiles2 = Array();//all tile
        
        for(var i=0;i<end;i++){
            addedNumber = 2;
            var newTile = new AITile(cells[i],addedNumber);
            this.grid.insertTile(newTile);
            var score = this.eval();
            if(worstScore > score){
                worstTile = newTile;
                worstScore = score;
            }
            this.grid.removeTile(newTile);
            if( (tiles[score] instanceof Array) === false){
                tiles[score] = new Array();
            }
            //console.log((tiles[score] instanceof Array));
            tiles[score].push(newTile);
            tiles2.push(newTile);
            
            addedNumber = 4;
            var newTile = new AITile(cells[i],addedNumber);
            this.grid.insertTile(newTile);
            var score = this.eval();
            if(worstScore > score){
                worstTile = newTile;
                worstScore = score;                
            }
           this.grid.removeTile(newTile);
           
            if( (tiles[score] instanceof Array) === false){
                tiles[score] = new Array();
            }
            tiles[score].push(newTile);
            tiles2.push(newTile);
        }
        
        
        var newGrid = this.grid.clone();
        var newAI = new AI(newGrid);
        //console.log(tiles[worstScore]);
       // for(i=0;i<tiles2.length;i++){
       tiles[worstScore] = tiles2;
        for(i=0;i<tiles[worstScore].length;i++){
            worstTile = tiles[worstScore][i];
            newAI.grid.insertTile(worstTile);
            var tempResult = newAI.search(depth,alpha,bestScore,positions,cutoffs,true);
            cutoffs = tempResult.cutoffs;
            positions = tempResult.positions;

            //console.log("bestScoreInBeta:"+ bestScore)
            if( bestScore > tempResult.score){//这里要取最大值中的最小值
                bestScore = tempResult.score;
                searchBestResult = tempResult;
            }


            if(alpha >= bestScore){
                cutoffs++;
                //console.log(i);
                return {move:-1,score:alpha,positions:positions,cutoffs:cutoffs};
            }
            newAI.grid.removeTile(worstTile);
        }
    }
    
    //console.log({ move: bestMove, score: bestScore, positions: positions, cutoffs: cutoffs });
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
    this.grid.insertTile(new AITile(cellLocation,addedNumber));
}


// performs a search and returns the best move
AI.prototype.getBest = function () {
    MAX_TILE_VALUE = this.grid.findLargestTile().value;
    MAX_TILE_NUM = this.grid.getTileNum(MAX_TILE_VALUE);
    return this.iterativeDeep();
}

// performs iterative deepening over the alpha-beta search
AI.prototype.iterativeDeep = function () {

    var start = (new Date()).getTime();
    var depth = 2;
    var best;
    //console.log(this.grid.cells[3][1].value);return;
// can't limit the excution time of function in javascript!
//    setTimeout(this.search(depth, -10000, 10000, 0, 0),minSearchTime); 
//    return;
    do {
        var newBest = this.search(depth, -10000, 10000, 0, 0,true);
        if (newBest.move == -1) {// deal with exception
            break;
        } else {
            best = newBest;
        }
        depth++;
       
    } while ((new Date()).getTime() - start < minSearchTime);
   //console.log(best);
    //alert("here")
   //console.log(depth);
    return best;
}


