import java.util.Random;

/**
 * A Bot class that Extends PlayGame which provides the
 * functionality of a computer player of the DoD.
 */


public class Bot extends PlayGame {
	private Random random;
	private static final char [] DIRECTIONS = {'N','S','E','W'};
	
	public Bot(){
		super();
		random = new Random();
	}

    /**
     * A method that parses and evaluates a bot action using
     * functionality to pickup the gold if the bot steps over it and
     * to avoid wall collisions.
     * @param lastAnswer the last response from the game logic.
     * @return the next action of the bot as a String.
     */
	public String botAction(String lastAnswer){
		if(logic.getLastPosition() == 'G'){
			return "PICKUP";
		}
		switch (lastAnswer.split(" ")[0]){
		case "":
			return "HELLO";
		case "GOLD:":
		case "FAIL":
			return "LOOK";
		default:
			char direction = DIRECTIONS[random.nextInt(4)];
			while (logic.isWall(direction)){
				direction = DIRECTIONS[random.nextInt(4)];
			}
			return "MOVE " + direction;
		}
	}

    /**
     * A method that performs the actions of a bot and prints
     * the consequences to the screen. A delay is added between each bot action
     * so the bot could be playable.
     * @param thread the number of the thread that calls the method.
     * @throws InterruptedException
     */
	public void update(int thread) throws InterruptedException {
		String answer = "";
		while (logic.gameRunning()){
                Thread.sleep(random.nextInt(400));
				answer = parseCommand(botAction(answer));
                //The "name" of the Bot is printed before it's action
				printAnswer("BOT - " + thread + ":\n" + answer + "\n==================");
                //If the bot has won no delay is added after it's action.
                if(!logic.gameRunning()){
                    break;
                }
                Thread.sleep(2600);
        }

	}

	@Override
	public String move(char direction) {
		return logic.move(direction,true);
	}

	@Override
	public String pickup() {
		return logic.pickup(true);
	}


}
