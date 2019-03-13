import java.awt.Font;
import java.awt.GridLayout;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Random;

import javax.swing.BorderFactory;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.border.BevelBorder;

public class Board {
	static boolean tileMoved;
	public static Random random1;
	public static Random random2;
	public static JPanel panel; 
	static Tile tileArray[];
	private static void Board() {
		tileMoved = false;
		random1 = new Random();
		random2 = new Random();
		panel = new JPanel(new GridLayout(4, 4));
		JFrame frame = new JFrame();
		/*Random tempRand1 = new Random();
		int rand1 = tempRand1.nextInt(10);
		Random tempRand2 = new Random();
		if(rand1 == 9) {
			rand1 =4;
		}else {
			rand1 = 2;
		}
		int rand2 = tempRand1.nextInt(10);
		if(rand2 == 9) {
			rand2 =4;
		}else {
			rand2 = 2;
		}
		Random tempRandLabel1 = new Random();
		int randLabel1 = tempRandLabel1.nextInt(16);
		Random tempRandLabel2 = new Random();
		int randLabel2 = randLabel1;
		while(randLabel2 == randLabel1) {
			randLabel2 = tempRandLabel2.nextInt(16);
		}*/
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
//		System.out.println(randLabel1);
//		System.out.println(randLabel2);
		/*tileArray[randLabel1].setSum(rand1);
		JLabel temp = (JLabel) panel.getComponent(randLabel1);
		temp.setText("" + tileArray[randLabel1].getSum());
		
		tileArray[randLabel2].setSum(rand2);
		JLabel temp2 = (JLabel) panel.getComponent(randLabel2);
		temp2.setText(""+ tileArray[randLabel2].getSum());*/
		frame.addKeyListener(new KeyListener() {

			@Override
			public void keyPressed(KeyEvent arg0) {
				int key = arg0.getKeyCode();
				
				if (key == KeyEvent.VK_LEFT) {
					//JLabel x = (JLabel) (panel.getComponent(0));
					//x.setText("1");
					moveLeft();
					printArray();
					readyForNextTurn();
				}else if (key == KeyEvent.VK_RIGHT) {
					moveRight();
					printArray();
					readyForNextTurn();
				} else if (key == KeyEvent.VK_DOWN) {
					moveDown();
					printArray();
					readyForNextTurn();
				} else if (key == KeyEvent.VK_UP) {
					moveUp();
					printArray();
					readyForNextTurn();
				}
				//readyForNextTurn();
				//checkForVictory
				
			}

			@Override
			public void keyReleased(KeyEvent arg0) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void keyTyped(KeyEvent arg0) {
				// TODO Auto-generated method stub
				
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
	static void moveLeft(){
		System.out.println("move left");
		for(int i = 0; i<4;i++){
			for(int j = 0; j<tileArray.length; j+=4){
				if((j+i)%4!=0 && tileArray[(j+i)].getSum()!=0){
					moveTileLeft(j+i);
					//emptyTile(j+i);
				}
			}
		}
	}
	static void moveRight(){
		System.out.println("move right");
		for(int i = 0; i<4;i++){
			for(int j = 15; j>=0; j-=4){
				//check if not right side and not empty
				if((j-i)%4!=3 && tileArray[(j-i)].getSum()!=0){
					moveTileRight(j-i);
					//emptyTile(j-i);
				}
			}
		}
	}

	static void moveUp(){
		System.out.println("move up");
		for(int i = 0; i<tileArray.length;i++){
			if(i>3 && tileArray[i].getSum()!=0){
				moveTileUp(i);
				//emptyTile(i);
			}
		}
	}
	static void moveDown(){
		System.out.println("move down");
		for(int i = 15; i>=0;i--){
			if(i<12 && tileArray[i].getSum()!=0){
				moveTileDown(i);
				//emptyTile(i);
			}
		}
		
	}
	static void moveTileUp(int i){
		System.out.println("Move Tile Up");
		int startI = i;
		int currentTileSum = tileArray[i].getSum();
		while(i>3){
			int upperTile = i-4;
			if (tileArray[upperTile].getSum() == 0){
				System.out.println("if");
				i-=4;
				tileMoved = true;
			} else if (tileArray[upperTile].getSum() == currentTileSum && !tileArray[upperTile].getAdded()){
				System.out.println("else if");
				i-=4;
				tileArray[upperTile].setSum(currentTileSum*2);
				tileArray[upperTile].setAdded(true);
				emptyTile(startI);
				tileMoved = true;
				return;
			}else{
				System.out.println("else");
				break;
			}
			
		}
		System.out.println("finale setsum");
		tileArray[i].setSum(currentTileSum);
		if(startI!=i){
			emptyTile(startI);
		}
	}
	
	static void moveTileDown(int i){
		int startI = i;
		int currentTileSum = tileArray[i].getSum();
		while(i<12){
			int lowerTile = i+4;
			if (tileArray[lowerTile].getSum() == 0){
				System.out.println("if");
				i+=4;
				tileMoved = true;
				
			} else if (tileArray[lowerTile].getSum() == currentTileSum && !tileArray[lowerTile].getAdded()){
				System.out.println("else if");
				i+=4;
				tileArray[lowerTile].setSum(currentTileSum*2);
				tileArray[lowerTile].setAdded(true);
				emptyTile(startI);
				tileMoved = true;
				return;
			}else{
				System.out.println("else");
				//i+=4;
				break;
			}
			
		}
		System.out.println("finale setsum");
		tileArray[i].setSum(currentTileSum);
		if(startI!=i){
			emptyTile(startI);
		}
	}
	
	static void moveTileRight(int i){
		int startI = i;
		System.out.println("Move tile right");
		int currentTileSum = tileArray[i].getSum();
		while(i%4!=3){
			int rightTile = i+1;
			if (tileArray[rightTile].getSum() == 0){
				System.out.println("if");
				i++;
				tileMoved = true;
			} else if (tileArray[rightTile].getSum() == currentTileSum && !tileArray[rightTile].getAdded()){
				System.out.println("else if");
				i++;
				tileArray[rightTile].setSum(currentTileSum*2);
				tileArray[rightTile].setAdded(true);
				emptyTile(startI); 
				tileMoved = true;
				return;
			}else{
				System.out.println("else");
				break;
			}
			
		}
		System.out.println("finale setsum");
		tileArray[i].setSum(currentTileSum);
		if(startI !=i){
			emptyTile(startI);
		}
	}
	
	static void moveTileLeft(int i){
		int startI = i;
		System.out.println("i: " + i);
		System.out.println("Move til left");
		int currentTileSum = tileArray[i].getSum();
		while(i%4!=0){
			int leftTile = i-1;
			if (tileArray[leftTile].getSum() == 0){
				System.out.println("if");
				tileMoved = true;
				i--;
				
			} else if (tileArray[leftTile].getSum() == currentTileSum && !tileArray[leftTile].getAdded()){
				System.out.println("else if");
				i--;
				tileArray[leftTile].setSum(currentTileSum*2);
				tileArray[leftTile].setAdded(true);
				emptyTile(startI);
				tileMoved = true;
				return;
			}else{
				System.out.println("else");
				break;
			}
			
		}
		
		System.out.println("currentTileSum: " + currentTileSum);
		System.out.println("i: " + i);
		tileArray[i].setSum(currentTileSum);
		if(startI !=i){
			emptyTile(startI);
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
	
	static void emptyTile(int i){
		tileArray[i].setSum(0);
	}
	static void randomSpawn(){
		System.out.println("random spawn");
		ArrayList<Integer> emptyIndices = getEmptyIndices();
		int randomObject = random2.nextInt(emptyIndices.size());
		int placement = emptyIndices.get(randomObject);
		System.out.println(placement);
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
		System.out.println(Arrays.toString(emptyIndices.toArray()));
		return emptyIndices;
		
	}
	public static void printArray(){
		
		for(int i = 0; i<tileArray.length; i++){
			if(i%4==0){
				System.out.println(" ");
			}
			System.out.print(tileArray[i].getSum() + " ");
			
		}
	}
	public static void main(String[] args) {
		Board();
		
 	}

}