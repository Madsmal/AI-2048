import java.awt.GridLayout;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.util.Random;

import javax.swing.BorderFactory;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.border.BevelBorder;

public class Board {
	public static JPanel panel; 
	
	private static void Board() {
		panel = new JPanel(new GridLayout(4, 4));
		JFrame frame = new JFrame();
		Random tempRand1 = new Random();
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
			randLabel2 = tempRandLabel1.nextInt(16);
		}
		Tile tileArray[] = new Tile[16];
		Tile tile;
	
		for (int i = 0; i < 16; i++) {
			tile = new Tile();
			tileArray[i] = tile;
			JLabel l = new JLabel("", JLabel.CENTER);
			//JLabel l = new JLabel(new ImageIcon("image_file.png"), JLabel.CENTER);
			l.setText("" + tileArray[i].getSum());
			l.setBorder(BorderFactory.createBevelBorder(BevelBorder.RAISED));
			l.setFont(l.getFont().deriveFont(20f));
			panel.add(l);
		}
		tileArray[randLabel1].setSum(rand1);
		JLabel temp = (JLabel) panel.getComponent(randLabel1);
		temp.setText("" + tileArray[randLabel1].getSum());
		
		tileArray[randLabel2].setSum(rand2);
		JLabel temp2 = (JLabel) panel.getComponent(randLabel2);
		temp.setText(""+ tileArray[randLabel2].getSum());
		frame.addKeyListener(new KeyListener() {

			@Override
			public void keyPressed(KeyEvent arg0) {
				int key = arg0.getKeyCode();

				if (key == KeyEvent.VK_LEFT) {
					JLabel x = (JLabel) (panel.getComponent(0));
					x.setText("1");
				}
				
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

	public static void main(String[] args) {
		Board();
		
 	}

}