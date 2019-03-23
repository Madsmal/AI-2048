import java.awt.Font;
import java.awt.GridLayout;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Random;
import java.util.concurrent.TimeUnit;

import javax.swing.BorderFactory;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.border.BevelBorder;

public class Board {
	static int score;
	static boolean tileMoved;
	public static Random random1;
	public static Random random2;
	public static JPanel panel; 
	static Tile tileArray[];
	private static void Board() {
		score = 0;
		tileMoved = false;
		random1 = new Random();
		random2 = new Random();
		panel = new JPanel(new GridLayout(4, 4));
		JFrame frame = new JFrame();
		
		tileArray = new Tile[16];
		Tile tile;
	
		for (int i = 0; i < 16; i++) {
			tile = new Tile();
			tileArray[i] = tile;
			JLabel l = new JLabel("", JLabel.CENTER);
			l.setFont(new Font("Serif", Font.PLAIN, 40));
			//JLabel l = new JLabel(new ImageIcon("image_file.png"), JLabel.CENTER);
			l.setText("" + tileArray[i].getSum());
			l.setBorder(BorderFactory.createBevelBorder(BevelBorder.RAISED));
			panel.add(l);
		}
		randomSpawn();
		randomSpawn();
		readyForNextTurn();
		frame.addKeyListener(new KeyListener() {

			@Override
			public void keyPressed(KeyEvent arg0) {
				int key = arg0.getKeyCode();
				
				if (key == KeyEvent.VK_LEFT) {
					tileArray = moveLeft(tileArray);
					readyForNextTurn();
				}else if (key == KeyEvent.VK_RIGHT) {
					tileArray = moveRight(tileArray);
					readyForNextTurn();
				} else if (key == KeyEvent.VK_DOWN) {
					tileArray = moveDown(tileArray);
					readyForNextTurn();
				} else if (key == KeyEvent.VK_UP) {
					tileArray = moveUp(tileArray);

					readyForNextTurn();
				}
			}

			@Override
			public void keyReleased(KeyEvent arg0) {
				
			}

			@Override
			public void keyTyped(KeyEvent arg0) {
				
			}
			

			
		});
		frame.setSize(800,800);
		frame.setContentPane(panel);
		frame.setTitle("2048");
		frame.setLocationRelativeTo(null);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setResizable(false);
		frame.setVisible(true);
	}
	static Tile[] moveLeft(Tile[] tileArray2){
		//printArray(tileArray);
		for(int i = 0; i<4;i++){
			for(int j = 0; j<tileArray2.length; j+=4){
				if((j+i)%4!=0 && tileArray2[(j+i)].getSum()!=0){
					moveTileLeft(j+i, tileArray2);
				}
			}
		}
		return tileArray2;
	}
	static Tile[] moveRight(Tile[] tileArray){
		for(int i = 0; i<4;i++){
			for(int j = 15; j>=0; j-=4){
				//check if not right side and not empty
				if((j-i)%4!=3 && tileArray[(j-i)].getSum()!=0){
					moveTileRight(j-i, tileArray);
				}
			}
		}
		return tileArray;
	}

	static Tile[] moveUp(Tile[] tileArray){
		for(int i = 0; i<tileArray.length;i++){
			if(i>3 && tileArray[i].getSum()!=0){
				moveTileUp(i, tileArray);
			}
		}
		return tileArray;
	}
	static Tile[] moveDown(Tile[] tileArray){
		for(int i = 15; i>=0;i--){
			if(i<12 && tileArray[i].getSum()!=0){
				moveTileDown(i, tileArray);
			}
		}
		return tileArray;
		
	}
	static void moveTileUp(int i, Tile[] tileArray){
		int startI = i;
		int currentTileSum = tileArray[i].getSum();
		while(i>3){
			int upperTile = i-4;
			if (tileArray[upperTile].getSum() == 0){
				i-=4;
				tileMoved = true;
			} else if (tileArray[upperTile].getSum() == currentTileSum && !tileArray[upperTile].getAdded()){
				i-=4;
				tileArray[upperTile].setSum(currentTileSum*2);
				tileArray[upperTile].setAdded(true);
				emptyTile(tileArray,startI);
				tileMoved = true;
				score += currentTileSum*2;
				return;
			}else{
				break;
			}
		}
		tileArray[i].setSum(currentTileSum);
		if(startI!=i){
			emptyTile(tileArray,startI);
		}
	}
	
	static void moveTileDown(int i, Tile[] tileArray){
		int startI = i;
		int currentTileSum = tileArray[i].getSum();
		while(i<12){
			int lowerTile = i+4;
			if (tileArray[lowerTile].getSum() == 0){
				i+=4;
				tileMoved = true;
			} else if (tileArray[lowerTile].getSum() == currentTileSum && !tileArray[lowerTile].getAdded()){
				i+=4;
				tileArray[lowerTile].setSum(currentTileSum*2);
				tileArray[lowerTile].setAdded(true);
				emptyTile(tileArray,startI);
				tileMoved = true;
				score += currentTileSum*2;
				return;
			}else{
				//i+=4;
				break;
			}
			
		}
		tileArray[i].setSum(currentTileSum);
		if(startI!=i){
			emptyTile(tileArray,startI);
		}
	}
	
	static void moveTileRight(int i, Tile[] tileArray){
		int startI = i;
		int currentTileSum = tileArray[i].getSum();
		while(i%4!=3){
			int rightTile = i+1;
			if (tileArray[rightTile].getSum() == 0){
				i++;
				tileMoved = true;
			} else if (tileArray[rightTile].getSum() == currentTileSum && !tileArray[rightTile].getAdded()){
				i++;
				tileArray[rightTile].setSum(currentTileSum*2);
				tileArray[rightTile].setAdded(true);
				emptyTile(tileArray,startI); 
				tileMoved = true;
				score += currentTileSum*2;
				return;
			}else{
				break;
			}
			
		}
		tileArray[i].setSum(currentTileSum);
		if(startI !=i){
			emptyTile(tileArray,startI);
		}
	}
	
	static void moveTileLeft(int i, Tile[] tileArray2){
		//printArray(tileArray);
		int startI = i;
		int currentTileSum = tileArray2[i].getSum();
		while(i%4!=0){
			int leftTile = i-1;
			if (tileArray2[leftTile].getSum() == 0){
				tileMoved = true;
				i--;
				
			} else if (tileArray2[leftTile].getSum() == currentTileSum && !tileArray2[leftTile].getAdded()){
				i--;
				tileArray2[leftTile].setSum(currentTileSum*2);
				tileArray2[leftTile].setAdded(true);
				emptyTile(tileArray2,startI);
				tileMoved = true;
				score += currentTileSum*2;
				return;
			}else{
				break;
			}
			
		}
		
		tileArray2[i].setSum(currentTileSum);
		if(startI !=i){
			emptyTile(tileArray2,startI);
		}
	}
	
	static void readyForNextTurn(){
		if(tileMoved){
			randomSpawn();
		}
		for(int i = 0; i<tileArray.length; i++){
			//update view
			JLabel x = (JLabel) (panel.getComponent(i));
			x.setText("" + tileArray[i].getSum());
			
			//set added to falls
			if(tileArray[i].getAdded()){
				tileArray[i].setAdded(false); 
			}
		}
		tileMoved = false;
	}
	
	static void emptyTile(Tile[] tileArray3, int i){
		tileArray3[i].setSum(0);
	}
	static void randomSpawn(){
		ArrayList<Integer> emptyIndices = getEmptyIndices();
		int randomObject = random2.nextInt(emptyIndices.size());
		int placement = emptyIndices.get(randomObject);
		int randomNumber = random1.nextInt(10);
		if(randomNumber == 9) {
			tileArray[placement].setSum(4); 
		}else {
			tileArray[placement].setSum(2); 
		}
		
	}
	public static ArrayList<Integer> getEmptyIndices(){
		ArrayList<Integer> emptyIndices = new ArrayList();
		for(int i = 0; i<tileArray.length;i++){
			if(tileArray[i].getSum()==0){
				emptyIndices.add(i);
			}
		}
		return emptyIndices;
		
	}
	
	public static int evaluate(Tile[] tileArray) {
		//TODO: Align same numbers 
		//TODO: Max score 
		//TODO: if not possible to move other ways than down
		//TODO: Ascending order of second and third line 
		//TODO When checking for ascending order, check if something i between them and not ust that everything i ascending 
		// It try to align the same numbers as there might spawn something and ruin the ascending order 
		//TODO: Make sure that it will align big numbers from the two top lines.
		int scorePoints = 0;
		int scoreValue = 10;
		float sameNumbersAlignedRatio = 4;
		float ascendingValue = 1;
		int numberOfPossibleSpawns = 0;
		float tempEval = 0;
		float sameNumbersAlignedPoints;
		int tempScore = score;
		float[] weightMatrix = {
				30,25,20,19,
				8,10,10,10,
				6,5,5,5,
				2,2,2,2};
		int direction = 0;
		
		//check left 
		Tile[] moveLeftTiles = moveLeft(cloneTiles(tileArray));
		float leftEval = 0;
		float weightBeforeSpawn = 0;	
		float ascendingEval = 0;
		for(int j = 0; j<moveLeftTiles.length; j++){	
			weightBeforeSpawn += moveLeftTiles[j].getSum()*weightMatrix[j];
		}
		numberOfPossibleSpawns = 0;
		tempEval = 0;
		scorePoints = (score-tempScore)*scoreValue;
		for (int i = 0; i<moveLeftTiles.length; i++){
			if (moveLeftTiles[i].getSum() == 0){
				numberOfPossibleSpawns += 2;
				//do this with other measurable parameters 
				moveLeftTiles[i].setSum(2);
				ascendingEval = getAscendingPoints(moveLeftTiles, weightMatrix)*ascendingValue;
				
				sameNumbersAlignedPoints = getSameNumbersAlignedPoints(moveLeftTiles, weightMatrix);
				//some eval algorithm
				//System.out.println("sameNumbersAlignedPoints: " + sameNumbersAlignedPoints);
			
		
				//compare all evals here 
				
				tempEval += (float) ((scorePoints + sameNumbersAlignedPoints/sameNumbersAlignedRatio + ascendingEval 
						+ weightMatrix[i]*2+weightBeforeSpawn)*0.9);
				
				/*if (tempEval>leftEval){
					leftEval = tempEval;
				}*/
				
				
				moveLeftTiles[i].setSum(4);
				ascendingEval = getAscendingPoints(moveLeftTiles, weightMatrix)*ascendingValue;
				sameNumbersAlignedPoints = getSameNumbersAlignedPoints(moveLeftTiles, weightMatrix);
				tempEval += (float) ((scorePoints + sameNumbersAlignedPoints/sameNumbersAlignedRatio + ascendingEval + weightMatrix[i]*4+weightBeforeSpawn)*0.1);
				/*if (tempEval>leftEval){
					leftEval = tempEval;
				}*/
				moveLeftTiles[i].setSum(0);
			}
		}
		if(numberOfPossibleSpawns !=0){
			leftEval = tempEval/numberOfPossibleSpawns;
		} else{
			leftEval = 0;
		}
		
		score = tempScore;
		Tile[] moveRightTiles = moveRight(cloneTiles(tileArray));
		float rightEval = 0;
		weightBeforeSpawn = 0;	
		for(int j = 0; j<moveRightTiles.length; j++){	
			weightBeforeSpawn += moveRightTiles[j].getSum()*weightMatrix[j];
		}
		tempEval = 0;
		numberOfPossibleSpawns = 0;
		scorePoints = (score-tempScore)*scoreValue;
		for (int i = 0; i<moveRightTiles.length; i++){
			
			if (moveRightTiles[i].getSum() == 0){
				numberOfPossibleSpawns += 2;
				//do this with other measurable parameters 
				moveRightTiles[i].setSum(2);
				ascendingEval = getAscendingPoints(moveRightTiles, weightMatrix)*ascendingValue;
				sameNumbersAlignedPoints = getSameNumbersAlignedPoints(moveRightTiles, weightMatrix);
				//moveLeftTiles[i].setSum(4);
				//some eval algorithm
				//compare all evals here 
				tempEval += (float) ((scorePoints + sameNumbersAlignedPoints/sameNumbersAlignedRatio + ascendingEval + weightMatrix[i]*2+weightBeforeSpawn)*0.9);
				
				
				/*if (tempEval>rightEval){
					rightEval = tempEval;
				}*/
				moveRightTiles[i].setSum(4);
				ascendingEval = getAscendingPoints(moveRightTiles, weightMatrix)*ascendingValue;
				sameNumbersAlignedPoints = getSameNumbersAlignedPoints(moveRightTiles, weightMatrix);
				
				tempEval += (float) ((scorePoints + sameNumbersAlignedPoints/sameNumbersAlignedRatio + ascendingEval + weightMatrix[i]*4+weightBeforeSpawn)*0.1);
				/*if (tempEval>rightEval){
					rightEval = tempEval;
				}*/
				
				moveRightTiles[i].setSum(0);
			}
			if(numberOfPossibleSpawns !=0){
				rightEval = tempEval/numberOfPossibleSpawns;
			} else{
				rightEval = 0;
			}
		}
		score = tempScore;
		Tile[] moveDownTiles = moveDown(cloneTiles(tileArray));
		float downEval = 0;
		weightBeforeSpawn = 0;	
		for(int j = 0; j<moveDownTiles.length; j++){	
			weightBeforeSpawn += moveDownTiles[j].getSum()*weightMatrix[j];
		}
		tempEval = 0;
		numberOfPossibleSpawns = 0;
		scorePoints = (score-tempScore)*scoreValue;
		for (int i = 0; i<moveDownTiles.length; i++){
			if (moveDownTiles[i].getSum() == 0){
				numberOfPossibleSpawns += 2;
				//do this with other measurable parameters 
				/*moveLeftTiles[i].setSum(2);
				moveLeftTiles[i].setSum(4);
				//some eval algorithm
			
				moveLeftTiles[i].setSum(0);*/
				//compare all evals here 
				moveDownTiles[i].setSum(2);
				ascendingEval = getAscendingPoints(moveDownTiles, weightMatrix)*ascendingValue;
				
				sameNumbersAlignedPoints = getSameNumbersAlignedPoints(moveDownTiles, weightMatrix);
				
				tempEval += (float) ((scorePoints + sameNumbersAlignedPoints/sameNumbersAlignedRatio + ascendingEval + weightMatrix[i]*2+weightBeforeSpawn)*0.9);
				/*if (tempEval>downEval){
					downEval = tempEval;
				}*/
				
				
				moveDownTiles[i].setSum(4);
				ascendingEval = getAscendingPoints(moveDownTiles, weightMatrix)*ascendingValue;
				
				sameNumbersAlignedPoints = getSameNumbersAlignedPoints(moveDownTiles, weightMatrix);
				
				tempEval += (float) ((scorePoints + sameNumbersAlignedPoints/sameNumbersAlignedRatio + ascendingEval + weightMatrix[i]*4+weightBeforeSpawn)*0.1);
				/*if (tempEval>downEval){
					downEval = tempEval;
				}*/
			}
			if(numberOfPossibleSpawns !=0){
				downEval = tempEval/numberOfPossibleSpawns;
			} else{
				downEval = 0;
			}
			
		}
		score = tempScore;
		Tile[] moveUpTiles = moveUp(cloneTiles(tileArray));
		float upEval = 0;	
		weightBeforeSpawn = 0;	
		for(int j = 0; j<moveUpTiles.length; j++){	
			weightBeforeSpawn += moveUpTiles[j].getSum()*weightMatrix[j];
		}
		tempEval = 0;
		numberOfPossibleSpawns = 0;
		scorePoints = (score-tempScore)*scoreValue;
		for (int i = 0; i<moveUpTiles.length; i++){
			if (moveUpTiles[i].getSum() == 0){
				numberOfPossibleSpawns += 2;
				//do this with other measurable parameters 
				/*moveLeftTiles[i].setSum(2);
				moveLeftTiles[i].setSum(4);
				//some eval algorithm
				
				moveLeftTiles[i].setSum(0);*/
				//compare all evals here 
				moveUpTiles[i].setSum(2);
				ascendingEval = getAscendingPoints(moveUpTiles, weightMatrix)*ascendingValue;
				
				sameNumbersAlignedPoints = getSameNumbersAlignedPoints(moveUpTiles, weightMatrix);
				
				tempEval += (float) ((scorePoints + sameNumbersAlignedPoints/sameNumbersAlignedRatio + ascendingEval + weightMatrix[i]*2+weightBeforeSpawn)*0.9);
				/*if (tempEval>upEval){
					upEval = tempEval;
				}*/
				moveUpTiles[i].setSum(4);
				ascendingEval = getAscendingPoints(moveUpTiles, weightMatrix)*ascendingValue;
				sameNumbersAlignedPoints = getSameNumbersAlignedPoints(moveUpTiles, weightMatrix);
				tempEval += (float) ((scorePoints + sameNumbersAlignedPoints/sameNumbersAlignedRatio + ascendingEval + weightMatrix[i]*4+weightBeforeSpawn)*0.1);
				/*if (tempEval>upEval){
					upEval = tempEval;
				}*/
			}
			if(numberOfPossibleSpawns !=0){
				upEval = tempEval/numberOfPossibleSpawns;
			} else{
				upEval = 0;
			}
		}
		score = tempScore;
		/*System.out.println("leftEval: " + leftEval);
		System.out.println("rightEval: " + rightEval);
		System.out.println("upEval: " + upEval);
		System.out.println("downEval: " + downEval);
		System.out.println("ascendingEval: " + ascendingEval);*/
		float max = Math.max(Math.max(Math.max(leftEval,rightEval),downEval),upEval);
		if (max == leftEval){
			tileArray = moveLeft(tileArray);
			readyForNextTurn();
		}
		else if( max == rightEval){
			tileArray = moveRight(tileArray);
			readyForNextTurn();
		}
		else if( max == downEval){
			tileArray = moveDown(tileArray);
			readyForNextTurn();
		}
		else if (max == upEval){
			tileArray = moveUp(tileArray);
			readyForNextTurn();
		} else {
			System.out.println("error");
		}
		//System.out.println(getSameNumbersAlignedPoints(tileArray, weightMatrix));
		System.out.println("score: " +  score);
		return direction;
	}
	public static float getAscendingPoints(Tile[] tiles, float[] weights){ 
		float ascendingPoints = 0;
		if (tiles[0].getSum()>=tiles[1].getSum()&& tiles[1].getSum()>=tiles[2].getSum()&& tiles[2].getSum()>=tiles[3].getSum()){
			ascendingPoints += tiles[0].getSum()*weights[0];
		}
		/*if ((tiles[4].getSum()>=tiles[5].getSum()&& tiles[5].getSum()>=tiles[6].getSum()&& tiles[6].getSum()>=tiles[7].getSum()) ||
				(tiles[7].getSum()>=tiles[6].getSum()&& tiles[6].getSum()>=tiles[5].getSum()&& tiles[5].getSum()>=tiles[4].getSum()) ){
			ascendingPoints += tiles[0].getSum()*weights[0]/2;
		}*/
		return ascendingPoints;
	}
	 public static float getSameNumbersAlignedPoints(Tile[] tiles, float[] weights){
		float points = 0; 
		boolean checkNext;
		for(int i = 0; i < tiles.length; i++){
			if(tiles[i].getSum()!=0){
			checkNext = false;;
			//check right
			for(int j = 1; j < 4; j++){
				if ((i+j)%4 ==0){
					break;
				}
				if(tiles[i+j].getSum()!=0){
					if(tiles[i].getSum()==tiles[i+j].getSum()){	
						points += tiles[i].getSum()*weights[i];
						checkNext = true;
					}
					break;					
				}
			}
			if (checkNext){
				continue;
			}
			
			//checkLeft
			for(int k = 1; k < 4; k++){
				if ((i-k)%4 ==3 || (i-k)<0){
					break;
				}
				if(tiles[i-k].getSum()!=0){
					if(tiles[i].getSum()==tiles[i-k].getSum()){	
						points += tiles[i].getSum()*weights[i];
						checkNext = true;
					}
					break;					
				}
			}
			if (checkNext){
				continue;
			}
			
			//check down
			for(int l = 4; l < 13; l+=4){
				if ((i+l)>15){
					break;
				}
				if(tiles[i+l].getSum()!=0){
					if(tiles[i].getSum()==tiles[i+l].getSum()){	
						points += tiles[i].getSum()*weights[i];
						checkNext = true;
					}
					break;					
				}
			}
			if (checkNext){
				continue;
			}
			
			//check up
			for(int m = 4; m < 13; m+=4){
				if ((i-m)<0){
					break;
				}
				if(tiles[i-m].getSum()!=0){
					if(tiles[i].getSum()==tiles[i-m].getSum()){	
						points += tiles[i].getSum()*weights[i];
					}
					break;					
				}
			}
			}
		
		
		}
		return points;
	 }
	
	public static void AIActivate() throws InterruptedException {
			//Math.max(moveLeft(tileArray), b)

			//Tile[] copiedTiles = new Tile[tileArray.length];
			//System.arraycopy(tileArray, 0, copiedTiles, 0, tileArray.length);
		while(true){
		Thread.sleep(50);
		evaluate(tileArray);	
			/*int evalLeft = evaluate(moveLeft(cloneTiles(tileArray)));
			int evalRight = evaluate(moveRight(cloneTiles(tileArray)));
			int evalDown = evaluate(moveDown(cloneTiles(tileArray)));
			int evalUp =  evaluate(moveUp(cloneTiles(tileArray)));
			int max = Math.max(Math.max(Math.max(evalLeft,evalRight),evalDown),evalUp);
			if (max == evalLeft){
				tileArray = moveLeft(tileArray);
				readyForNextTurn();
			}
			else if( max == evalRight){
				tileArray = moveRight(tileArray);
				readyForNextTurn();
			}
			else if( max == evalDown){
				tileArray = moveDown(tileArray);
				readyForNextTurn();
			}
			else{
				tileArray = moveUp(tileArray);
				readyForNextTurn();
			}*/
		//i++;
		}
			//printArray(copiedTiles);
	}
	public static void printArray(Tile[] tileArray2){
		
		for(int i = 0; i<tileArray2.length; i++){
			if(i%4==0){
				System.out.println(" ");
			}
			System.out.print(tileArray2[i].getSum() + " ");
		}
	}
	public static Tile[] cloneTiles(Tile[] tileArray2){
		Tile[] clonedTiles = new Tile[tileArray2.length];
		for(int i = 0; i<tileArray2.length; i++){
			Tile newTile = new Tile();
			newTile.setSum(tileArray2[i].getSum());
			newTile.setAdded(tileArray2[i].getAdded());
			clonedTiles[i] = newTile;
		}
		return clonedTiles;
	}
	public static void main(String[] args) throws InterruptedException {
		Board();
		AIActivate();
		
 	}

}