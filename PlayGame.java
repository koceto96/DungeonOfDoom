import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.*;
import java.net.ConnectException;
import java.net.Socket;
import java.net.SocketException;
import java.net.UnknownHostException;


/**
 * A Play Game class that provides the functionality of a human DoD player.
 * The class also contains a client main method that provides the functionality of
 * a DoD client and uses string communication with the server.
 */


public class PlayGame {

	protected IGameLogic logic;
	protected ClientGUI client;
	protected String command;
	protected boolean running;

	public PlayGame() {
		logic = new GameLogic();
		client = new ClientGUI();
		running = false;
		command = "";
	}



	/**
	 * A method that returns the answer of a player action
	 * according to the DoD protocol and prints it to the screen.
	 * @param clientInput the input from the client as a String.
     * @return the Game Logic response of the client command as a String.
     */
	public String update(String clientInput) {
		String answer = "";
		while (logic.gameRunning() && !clientInput.equals("")) {
			answer = parseCommand(clientInput);
			printAnswer(answer + "\n==================");
			clientInput = "";
		}
		return answer;
	}

	/**
	 * A simple print method.
	 * @param answer what is to be printed to the screen as a String.
     */
	protected void printAnswer(String answer) {
		System.out.println(answer);
	}

	public void selectMap(String mapName) {
		logic.setMap(mapName);
	}

	/**
	 * Parsing and Evaluating the input from the client/bot.
	 *
	 * @param readUserInput input the user client/bot generates.
	 * @return answer of GameLogic.
	 */
	protected String parseCommand(String readUserInput) {

		String[] command = readUserInput.trim().split(" ");
		String answer = "FAIL";

		switch (command[0].toUpperCase()) {
			case "HELLO":
				answer = hello();
				break;
			case "MOVE":
				if (command.length == 2)
					answer = move(command[1].charAt(0));
				break;
			case "PICKUP":
				answer = pickup();
				break;
			case "LOOK":
				answer = look();
				break;
			case "QUIT":
				logic.resetPosition();
				return "SUCCESS";
			default:
				answer = "FAIL";
		}

		return answer;
	}


	public String hello() {
		return logic.hello();
	}

	public String move(char direction) {
		return logic.move(direction, false);
	}

	public String pickup() {
		return logic.pickup(false);
	}

	public String look() {
		return logic.look();
	}

	/**
	 * A method that updates the GUI on every player move.
	 * This method also reports when the client has won the game or if the server is
	 * not listening for clients.
	 * @param receivedMessage the update message from the server.
     */
	private void guiUpdate(String receivedMessage){
		if(receivedMessage.contains("WIN")){
			JOptionPane.showMessageDialog(null,"Congratulations!!! \nYOU HAVE ESCAPED THE DUNGEON OF DOOM,\n" +
					"THE GAME WILL NOW EXIT!!!","GAME WON!",JOptionPane.INFORMATION_MESSAGE);
			System.exit(111);
		}
		try {
			client.getGold().setText(receivedMessage.substring(receivedMessage.lastIndexOf("G"), receivedMessage.length()));
			receivedMessage.replace(receivedMessage.substring(receivedMessage.lastIndexOf("G"), receivedMessage.length()), "");
			client.setMapPanel(receivedMessage);
		}catch (StringIndexOutOfBoundsException e){
			JOptionPane.showMessageDialog(null,"The server is not listening for clients!","SERVER CLOSED",JOptionPane.ERROR_MESSAGE);
			System.exit(222);
		}
	}

	/**
	 * A method that starts a client for the Dungeon of Doom game.
	 * Input from the GUI is taken from the client as actions
	 * and sent to the server as a string - the server
	 * response is displayed on the clients's screen as GUI.
	 * @throws IOException
     */
	private void startClient() throws IOException {
		String receivedMessage = "";
		try {
			//The port and the IP are selected from the GUI then are make not changeable.
			Socket sock = new Socket(client.getIpField().getText(),Integer.parseInt(client.getPortField().getText().replaceAll("[^0-9]", "")));
			client.getIpField().setEditable(false);
			client.getPortField().setEditable(false);
			BufferedWriter toServer = new BufferedWriter(new OutputStreamWriter(sock.getOutputStream()));
			BufferedReader fromServer = new BufferedReader(new InputStreamReader(sock.getInputStream()));
			//A message is shown when a client is connected and he is required to enter his name.
			toServer.write(JOptionPane.showInputDialog(null,"You have connected to the server!\nPlease enter your name:","Login",JOptionPane.INFORMATION_MESSAGE) +"\n");
			client.getGold().setText(fromServer.readLine());
			while (running) {
				//Sends a non-null message to the server.
				if (receivedMessage != null && !command.equals("")){
					receivedMessage = command;
					String sendMessage = receivedMessage + "\n";
					toServer.write(sendMessage);
					toServer.flush();
					receivedMessage="";
					//Get the return message from the server.
					do {
						receivedMessage += fromServer.readLine();
					}
					while (fromServer.ready());
					guiUpdate(receivedMessage);
					command = "";
				}
				//A message is sent only when action is performed on the GUI.
				actionPerformed();
			}
			fromServer.close();
			toServer.close();

		} catch (ConnectException | UnknownHostException CE) {
			//When connection can not be established.
			JOptionPane.showMessageDialog(null,"Cannot connect to the server.","Connection error",JOptionPane.ERROR_MESSAGE);
		} catch (SocketException SE){
			//When the connection is lost.
			JOptionPane.showMessageDialog(null,"Connection to the server is lost.","Connection error",JOptionPane.ERROR_MESSAGE);
		} finally{
			//When the game is over or error occurs.
			JOptionPane.showMessageDialog(null,"The game will now exit.","GAME OVER!",JOptionPane.WARNING_MESSAGE);
			client.setVisible(false);
		}
	}

	/**
	 * A main method that starts new client.
	 * @param args
	 * @throws IOException
     */
	public static void main(String [] args) throws IOException {
		PlayGame game = new PlayGame();
		game.login();
		game.updateWhenConnected();
		game.startClient();

	  }

	/**
	 * A method that sets the GUI for login mode only.
	 */
    private void login(){
		client.startGUI();
		client.getUpBtn().setVisible(false);
		client.getDownBtn().setVisible(false);
		client.getLeftBtn().setVisible(false);
		client.getRightBtn().setVisible(false);
		client.getPickupBtn().setVisible(false);
		client.getExtBtn().setVisible(false);
	}

	/**
	 * A method that checks if the game is started and
	 * updates the client GUI accordingly.
	 */
    private void updateWhenConnected(){
		while (!running) {
			client.getStartBtn().addActionListener(new ActionListener() {
				@Override
				public void actionPerformed(ActionEvent e) {
					running = true;
					client.getExtBtn().setVisible(true);
					client.getUpBtn().setVisible(true);
					client.getDownBtn().setVisible(true);
					client.getLeftBtn().setVisible(true);
					client.getRightBtn().setVisible(true);
					client.getPickupBtn().setVisible(true);
					client.getStartBtn().setVisible(false);
				}
			});
		}

	}

	/**
	 * A method that checks if a button is clicked
	 * and adds a delay after each client action in order
	 * to make the game more playable.
	 */
	private void actionPerformed(){

		client.getExtBtn().addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				command = "QUIT";
				running = false;

			}
		});



		client.getUpBtn().addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				command = "MOVE N";
			}
		});

		client.getDownBtn().addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				command = "MOVE S";
			}
		});

		client.getLeftBtn().addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				command = "MOVE W";
			}
		});

		client.getRightBtn().addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				command = "MOVE E";
			}
		});

		client.getPickupBtn().addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				command = "PICKUP";
			}
		});

		try {
			Thread.sleep(280);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}


	}

}
