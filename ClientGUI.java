import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;

/**
 * This class makes a Client GUI object for the
 * DoD game with game control buttons ,look window display
 * and ip and port fields.
 * client serving.
 */
public class ClientGUI extends JFrame {

    private JTextField ipField = new JTextField(SwingConstants.NEXT);
    private JTextField portField = new JTextField(SwingConstants.WEST);
    private JLabel[] tiles = new JLabel[25];
    private JButton extBtn = new JButton("Exit");
    private JButton startBtn = new JButton("Start");
    private JButton upBtn = new JButton("N");
    private JButton downBtn = new JButton("S");
    private JButton leftBtn = new JButton("W");
    private JButton rightBtn = new JButton("E");
    private JButton pickupBtn = new JButton("PICKUP");
    private JTextArea gold = new JTextArea();
    private JPanel controlPanel = new JPanel() ;

    /**
     * A constructor for the Client GUI object.
     */
    public ClientGUI(){
        setName("Dungeon Of Doom");
        setTitle("Dungeon Of Doom");
        setPreferredSize(new Dimension(700,640));
        setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);

    }

    /**
     * A method that sets and configures the DoD server
     * GUI.
     * @throws Exception
     */
    private void createAndShowGUI(){
        /*
         The GUI's panels are created and configured.
         */
        JPanel infoPanel = new JPanel() ;
        JPanel gamePanel = new JPanel() ;

        infoPanel.setSize(new Dimension(700,100)) ;
        gamePanel.setSize(new Dimension(700,300)) ;
        controlPanel.setSize(new Dimension(700,200)) ;

        infoPanel.setBackground(new Color(204, 145, 96));

        getContentPane().add(infoPanel,BorderLayout.PAGE_START);
        getContentPane().add(gamePanel,BorderLayout.CENTER) ;
        getContentPane().add(controlPanel,BorderLayout.PAGE_END) ;

        infoPanel.setLayout(new FlowLayout(FlowLayout.LEADING));
        /*
         Labels and fields are created, configured and added to the infoPanel.
          */
        JLabel title = new JLabel("DoD  ") ;
        JLabel ip = new JLabel() ;
        JLabel port = new JLabel() ;

        title.setFont(new Font("Calibri",Font.BOLD,60));
        title.setSize(new Dimension(100,30));

        ipField.setSize(new Dimension(200,100));
        ipField.setFont(new Font("Times New Roman",Font.PLAIN,20));
        ipField.setText("localhost");



        ip.setText("IP: ");
        ip.setFont(new Font("Times New Roman",Font.BOLD,30));
        ip.setSize(new Dimension(100,30));

        port.setText("  Port: ");
        port.setFont(new Font("Times New Roman",Font.PLAIN,30));
        port.setSize(new Dimension(100,30));

        portField.setSize(new Dimension(100,100));
        portField.setFont(new Font("Times New Roman",Font.PLAIN,18));
        portField.setText("2245");


        infoPanel.add(title);
        infoPanel.add(ip);
        infoPanel.add(ipField);
        infoPanel.add(port);
        infoPanel.add(portField);
         /*
           Labels,panel and buttons are created, configured and added to the gamePanel.
          */
        extBtn.setPreferredSize(new Dimension(80,60));

        gamePanel.setLayout(new FlowLayout(FlowLayout.CENTER));
        gamePanel.add(extBtn);
        gamePanel.add(new JLabel("     "));



        gold.setSize(new Dimension(70,120));
        gold.setFont(new Font("calibri",Font.BOLD,21));
        gold.setBackground(new Color(100,70,82));
        gold.setForeground(Color.orange);
        gold.setEditable(false);


        //The DoD look window is created as a JPanel and added to the gamePanel.
        JPanel mapPanel = new JPanel();
        mapPanel.setLayout(new GridLayout(5,5));
        mapPanel.setSize(new Dimension(500,500));

        for(int i = 0 ; i < 25 ; i ++ ){
            tiles[i]  = new JLabel();
            tiles[i].setSize(new Dimension(25,25));
            tiles[i].setIcon(new ImageIcon("question-circle-128.png"));
            mapPanel.add(tiles[i]);

        }
        gamePanel.add(mapPanel);

        startBtn.setPreferredSize(new Dimension(80,60));
        startBtn.setBackground(new Color(30,150,80));
        startBtn.setCursor(new Cursor(12));

        extBtn.setBackground(new Color(150,10,30));
        extBtn.setCursor(new Cursor(12));
        // myPanel2.add(lookWindow);
        gamePanel.add(new JLabel("     "));
        gamePanel.add(startBtn);
        gamePanel.add(gold);
        /*
         The game control buttons are configured organized and added to
         the controlPanel of the client GUI.
         */
        upBtn.setPreferredSize(new Dimension(70,50));
        upBtn.setFont(new Font("Times New Roman" , Font.PLAIN, 20));
        upBtn.setCursor(new Cursor(12));

        downBtn.setPreferredSize(new Dimension(70,50));
        downBtn.setFont(new Font("Times New Roman" , Font.PLAIN, 20));
        downBtn.setCursor(new Cursor(12));

        leftBtn.setPreferredSize(new Dimension(70,50));
        leftBtn.setFont(new Font("Times New Roman" , Font.PLAIN, 20));
        leftBtn.setCursor(new Cursor(12));

        rightBtn.setPreferredSize(new Dimension(70,50));
        rightBtn.setFont(new Font("Times New Roman" , Font.PLAIN, 20));
        rightBtn.setCursor(new Cursor(12));

        pickupBtn.setPreferredSize(new Dimension(90,90));
        pickupBtn.setForeground(new Color(100, 68, 78));
        pickupBtn.setBackground(new Color(217, 205, 104));
        pickupBtn.setCursor(new Cursor(12));

        //Grid Bag Layout is used to organize the buttons in the panel in a proper way for the game to be more easily played.
        controlPanel.setLayout(new GridBagLayout());
        GridBagConstraints gbc = new GridBagConstraints();
        gbc.gridy = 0;
        gbc.gridx = 1;
        gbc.insets = new Insets(4,4,4,4);
        controlPanel.add(upBtn,gbc);
        gbc.gridy = 2;
        controlPanel.add(downBtn,gbc);
        gbc.gridy = 1;
        gbc.gridx = 0;
        controlPanel.add(leftBtn,gbc);
        gbc.gridx++;
        controlPanel.add(pickupBtn,gbc);
        gbc.gridx++;
        controlPanel.add(rightBtn,gbc);



        hotKeys();
        pack () ;
        setVisible (true) ;
    }

    /**
     * A method that starts the Server GUI on a priority thread.
     */
    public void startGUI(){
        javax.swing.SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                createAndShowGUI();
            }
        });
    }

    /**
     * A method that updates every label on the
     * map according to the tile.
     * @param lookWindow the visible part of the map as a sting.
     */
    public void setMapPanel(String lookWindow) {


        for(int i = 0; i < 25 && lookWindow.length() >= 25; i++){
            tiles[i].setIcon(tileCheck(lookWindow.charAt(i)));
        }

    }

    /**
     * A method that checks which icon is to be displayed
     * instead of the char on the string map.
     * @param tile a single tile of the map as a char.
     * @return the according icon.
     */
    private ImageIcon tileCheck(char tile){
        switch (tile){
            case '.':
                return new ImageIcon("rounded_square-512.png");
            case 'P':
                return new ImageIcon("r2.jpg");
            case 'B':
                return new ImageIcon("images(1).jpg");
            case '#':
                return new ImageIcon("wall.png");
            case 'G':
                return new ImageIcon("1l02r.png");
            case 'E':
                return new ImageIcon("user-exit-512.png");
            case 'X':
                return new ImageIcon("mist-fog-foggy-wave-waves-sea-256.png");
            default:
                return new ImageIcon(" ");
        }

    }

    /**
     *
     * A method that adds a key listener to every button on the GUI.
     */
    private void hotKeys(){
        AbstractAction moveUp = new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                upBtn.requestFocusInWindow();
                upBtn.doClick();
            }};
        controlPanel.getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke(KeyEvent.VK_UP, 0, true), "N");
        controlPanel.getActionMap().put("N", moveUp);

        AbstractAction moveDown = new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                downBtn.requestFocusInWindow();
                downBtn.doClick();
            }};
        controlPanel.getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke(KeyEvent.VK_DOWN, 0, true), "S");
        controlPanel.getActionMap().put("S", moveDown);

        AbstractAction moveLeft = new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                leftBtn.requestFocusInWindow();
                leftBtn.doClick();
            }};
        controlPanel.getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke(KeyEvent.VK_LEFT, 0, true), "W");
        controlPanel.getActionMap().put("W", moveLeft);

        AbstractAction moveRight = new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                rightBtn.requestFocusInWindow();
                rightBtn.doClick();
            }};
        controlPanel.getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke(KeyEvent.VK_RIGHT, 0, true), "E");
        controlPanel.getActionMap().put("E", moveRight);

        AbstractAction pickup = new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                pickupBtn.requestFocusInWindow();
                pickupBtn.doClick();
            }};
        controlPanel.getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke(KeyEvent.VK_ENTER, 0, true), "PICKUP");
        controlPanel.getActionMap().put("PICKUP" , pickup);


    }

    /**
     * Accessors for the GUI's fields and buttons.
     */

    public JTextField getIpField() {
        return ipField;
    }

    public JTextField getPortField() {
        return portField;
    }

    public JButton getDownBtn() {
        return downBtn;
    }

    public JButton getExtBtn() {
        return extBtn;
    }

    public JButton getLeftBtn() {
        return leftBtn;
    }

    public JButton getPickupBtn() {
        return pickupBtn;
    }

    public JButton getRightBtn() {
        return rightBtn;
    }

    public JButton getStartBtn() {
        return startBtn;
    }

    public JButton getUpBtn() {
        return upBtn;
    }

    public JTextArea getGold() {
        return gold;
    }
    public static void main(String [] args) throws Exception {
        new ClientGUI().startGUI();
    }



}
