import java.io.File;

public interface IGameLogic {
	
	
	
	public void setMap(String mapName);

    public void startGame(String playerName);

	public String hello();

	public String move(char direction,boolean bot);

	public String pickup(boolean bot);

	public String look();

    public char getLastPosition();

    public void resetPosition();

    public boolean gameRunning();

    public boolean isWall(char nextPosition);

	public void quitGame();
	
	public int getMapHeight();
	    
	public int getMapWidth();


	
}
