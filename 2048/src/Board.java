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
					moveLeft();
					readyForNextTurn();
				}else if (key == KeyEvent.VK_RIGHT) {
					moveRight();
					readyForNextTurn();
				} else if (key == KeyEvent.VK_DOWN) {
					moveDown();
					readyForNextTurn();
				} else if (key == KeyEvent.VK_UP) {
					moveUp();

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
	static void moveLeft(){
		for(int i = 0; i<4;i++){
			for(int j = 0; j<tileArray.length; j+=4){
				if((j+i)%4!=0 && tileArray[(j+i)].getSum()!=0){
					moveTileLeft(j+i);
				}
			}
		}
	}
	static void moveRight(){
		for(int i = 0; i<4;i++){
			for(int j = 15; j>=0; j-=4){
				//check if not right side and not empty
				if((j-i)%4!=3 && tileArray[(j-i)].getSum()!=0){
					moveTileRight(j-i);
				}
			}
		}
	}

	static void moveUp(){
		for(int i = 0; i<tileArray.length;i++){
			if(i>3 && tileArray[i].getSum()!=0){
				moveTileUp(i);
			}
		}
	}
	static void moveDown(){
		for(int i = 15; i>=0;i--){
			if(i<12 && tileArray[i].getSum()!=0){
				moveTileDown(i);
			}
		}
		
	}
	static void moveTileUp(int i){
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
				emptyTile(startI);
				tileMoved = true;
				return;
			}else{
				break;
			}
		}
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
				i+=4;
				tileMoved = true;
				
			} else if (tileArray[lowerTile].getSum() == currentTileSum && !tileArray[lowerTile].getAdded()){
				i+=4;
				tileArray[lowerTile].setSum(currentTileSum*2);
				tileArray[lowerTile].setAdded(true);
				emptyTile(startI);
				tileMoved = true;
				return;
			}else{
				//i+=4;
				break;
			}
			
		}
		tileArray[i].setSum(currentTileSum);
		if(startI!=i){
			emptyTile(startI);
		}
	}
	
	static void moveTileRight(int i){
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
				emptyTile(startI); 
				tileMoved = true;
				return;
			}else{
				break;
			}
			
		}
		tileArray[i].setSum(currentTileSum);
		if(startI !=i){
			emptyTile(startI);
		}
	}
	
	static void moveTileLeft(int i){
		int startI = i;
		int currentTileSum = tileArray[i].getSum();
		while(i%4!=0){
			int leftTile = i-1;
			if (tileArray[leftTile].getSum() == 0){
				tileMoved = true;
				i--;
				
			} else if (tileArray[leftTile].getSum() == currentTileSum && !tileArray[leftTile].getAdded()){
				i--;
				tileArray[leftTile].setSum(currentTileSum*2);
				tileArray[leftTile].setAdded(true);
				emptyTile(startI);
				tileMoved = true;
				return;
			}else{
				break;
			}
			
		}
		
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
	/*public static void printArray(){
		
		for(int i = 0; i<tileArray.length; i++){
			if(i%4==0){
				System.out.println(" ");
			}
			System.out.print(tileArray[i].getSum() + " ");
			
		}
	}*/
	public static void main(String[] args) {
		Board();
		
 	}

}