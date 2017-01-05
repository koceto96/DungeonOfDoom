import java.io.File;
import java.util.Random;
import java.util.concurrent.SynchronousQueue;


public class GameLogic implements IGameLogic{
    static{
    	System.load(System.getProperty("user.dir")+"/jni/liblogic.so");

    }
    private int[] playerPosition;
    private int collectedGold;
    private boolean active;
	private String playerName;
    //A char that stores the tile on the map on which the player had been standing before moving.
    private char lastPosition;

    public GameLogic(){
        playerPosition = new int[2];
        collectedGold = 0;

    }

    /**
     * A method that initialises a new player/bot on a random spot on the map,
	 * (or if the player is from the previous game on the same map loads the same attributes as before), 
     * places 'P' on that position on the map and makes the game active.
     */
    public native synchronized void startGame(String playerName);


    /**
     * Accessor method for the lastPosition field.
     * @return the lastPosition as a char.
     */
    public char getLastPosition() {
        return lastPosition;
    }


    /**
     * A method that loads the map from a text file.
     * @param file the name of the file.
     */
    public native void setMap(String mapName);



    /**
     * Prints how much gold is still required to win!
     */
    public native String hello();

    /**
     * By proving a character direction from the set of {N,S,E,W} the gamelogic
     * checks if this location can be visited by the player.
     * If it is true, the player is moved to the new location.
     * @param bot boolean that indicates if the method is called by the client or by the bot.
     * @param direction the direction of movement as a char.
     * @return If the move was executed Success is returned. If the move could not execute Fail is returned.
     */
    public native synchronized String move(char direction, boolean bot);


    /**
     * When a player disconnects the tile on which he had been standing is replaced by it's original.
     */
    public native void resetPosition();

    /**
     * A method that picks up the gold on the current tile(if any) and returns a message to the player.
     * @return an answer as a String.
     */
    public native synchronized String pickup(boolean bot);

    /**
     * The method shows the dungeon around the player location.
     */
    public native String look();

    /**
     * Prints the whole map directly to Standard out.
     */
    public native String printMap();

    /**
     * Sets the game status as inactive if someone wins.
     */
    public void quitGame(){
        active = false;
    }

    /**
     * Accessor to the active field.
     * @return if the game is active as a boolean.
     */
    public boolean gameRunning()
    {
        return active;
    }

    /**
     * A method that checks if the tile in certain direction is a wall.
     * @param direction the direction to check as a char.
     * @return whether the tile next to the player in that direction is wall.
     */
    public native boolean isWall(char direction);
    
    /**
     * Accessor for the map height.
	 * @return the value of the map height as integer.
     */	 
    public native int getMapHeight();
	
	/**
     * Accessor for the map width.
	 * @return the value of the map width as integer.
     */	 
    public native int getMapWidth();
    
  

}
