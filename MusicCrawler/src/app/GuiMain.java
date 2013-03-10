package app;

import java.awt.Container;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.HashMap;
import java.util.Map;

import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.UIManager;
import javax.swing.UnsupportedLookAndFeelException;

import nbeans.Record;
import ui.CloseWindowListener;
import ui.GuiPrintStream;
import ui.MainMenu;
import utils.StringUtils;
import crawler.collectors.SingersCollector;

public class GuiMain extends JFrame {

	class StartListener implements ActionListener {

		@Override
		public void actionPerformed(ActionEvent e) {
			switchStatus();	
			initialParameters();

			Thread worker = new Thread(new Runnable() {
				@Override
				public void run() { work(); }
			});
			worker.setName(category);
			worker.start();
		}
		
	}
	
	class StopListener implements ActionListener {

		@Override
		public void actionPerformed(ActionEvent e) {
			SingersCollector.stop();
		}

	}
	
	class CheckListener implements ActionListener {

		@Override
		public void actionPerformed(ActionEvent e) {
			setEnableAndEditable();
		}

	}

	private static final long serialVersionUID = 4239641484970843276L;
	
	/*private static final int INTERVAL = 5;    // 默认为5秒*/
	
	private static final Dimension winSize = new Dimension(930, 680);

	private static final String MAIN_URL = Constants.MAIN_URL;
	private static final String ALL      = "all";

	private static final String[] items = { 
					"cn-male", "cn-female", "cn-group"
				  , "jpkr-male", "jpkr-female", "jpkr-group"
				  , "western-male", "western-female", "western-group"
				  , "other" };

	private int interval = 0;    // -1

	private boolean partial = false;
	
	private JLabel    categoriesLabel = new JLabel("歌手类别：");
	private JCheckBox categoriesCheck = new JCheckBox();
	private JComboBox categoriesCombo = new JComboBox(items);
	
	// 此sId和eId只对分类别爬取数据有效
	private int sId = Record.ILLEGAL_ID;
	private JLabel sIdJLabel   = new JLabel("起始ID：");
	private JTextField sIdText = new JTextField(10);
	private int eId = Record.ILLEGAL_ID;
	private JLabel eIdJLabel   = new JLabel("截止ID：");
	private JTextField eIdText = new JTextField(10);
	
	/*private JLabel intervalLabel    = new JLabel("访问间隔（秒）：");
	private JTextField intervalText = new JTextField(5);*/

	private JButton startBtn = new JButton("开始爬取数据");
	private JButton stopBtn  = new JButton("停止爬取数据");
	
	private JTextArea outArea      = new JTextArea(27, 112);
	private JScrollPane scrollPane = new JScrollPane(outArea);

	private String url      = MAIN_URL;
	private String category = ALL;
	
	private static Map<String, String> urlMap = new HashMap<String, String>();
	static {
		initialUrlMap();
	}
	
	public GuiMain() {
		super("DeltaCrawler through proxy");
		
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		addWindowListener(new CloseWindowListener());
		setResizable(false);
		
		MainMenu.addTo(this);
		
		Container container = getContentPane();
		container.setLayout(new FlowLayout(FlowLayout.CENTER, 10, 10));
		
		container.add(categoriesLabel);
		categoriesCheck.setSelected(false);    // true
		container.add(categoriesCheck);
		categoriesCheck.addActionListener(new CheckListener());
		container.add(categoriesCombo);
		
		container.add(sIdJLabel);
		container.add(sIdText);
		container.add(eIdJLabel);
		container.add(eIdText);

		setEnableAndEditable();
		
		/*container.add(intervalLabel);
		container.add(intervalText);*/
		
		startBtn.addActionListener(new StartListener());
		container.add(startBtn);
		stopBtn.addActionListener(new StopListener());
		stopBtn.setEnabled(false);
		container.add(stopBtn);
		
		outArea.setAutoscrolls(true);
		outArea.setLineWrap(true);
		scrollPane.setViewportView(outArea);
		System.setOut(new GuiPrintStream(System.out, outArea));
		//System.setErr(System.out);    // 将System.err也定向到outArea
		container.add(scrollPane);
		
		moveToCenter();
		setVisible(true);
	}
	
	private void setEnableAndEditable() {
		boolean selected = categoriesCheck.isSelected();
		categoriesCombo.setEnabled(selected);
		sIdText.setEditable(selected);
		eIdText.setEditable(selected);	
	}
	
	private static void initialUrlMap() {
		urlMap.put(ALL, MAIN_URL);
		for (int i = 0; i < items.length; i++) {
			String key = items[i];
			String value = MAIN_URL + key.replaceAll("-", "/");
			urlMap.put(key, value);
		}
	}

	private void moveToCenter() {
		Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
		setBounds(new Rectangle(winSize));
		int x = (screenSize.width - winSize.width) / 2;
		int y = (screenSize.height - winSize.height) / 2;
		Point pt = new Point(x, y);
		setLocation(pt);
	}
	
	private static void setSystemLookAndFeel() {
		try {
			UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
		}
		catch (ClassNotFoundException cnfe) {
			cnfe.printStackTrace();
		}
		catch (InstantiationException ie) {
			ie.printStackTrace();
		}
		catch (IllegalAccessException iae) {
			iae.printStackTrace();
		}
		catch (UnsupportedLookAndFeelException ulafe) {
			ulafe.printStackTrace();
		}
	}
	
	private void initialParameters() {
		partial = categoriesCheck.isSelected();
		if (partial) {
			category = (String) categoriesCombo.getSelectedItem();
		}
		else {
			category = ALL;
		}
		url = urlMap.get(category);
		System.out.println(url);
		
		String idStr = "";
		idStr = sIdText.getText();
		sId = StringUtils.toInt(idStr);
		idStr = eIdText.getText();
		eId = StringUtils.toInt(idStr);
		
		/*String intervalStr = intervalText.getText();
		String retryStr = retryText.getText();
		interval = StringUtils.toInt(intervalStr);
		retry = StringUtils.toInt(retryStr);
		interval = (interval != -1) ? interval : INTERVAL;
		retry = (retry != -1) ? retry : RETRY;*/
	}
	
	private void switchStatus() {
		boolean isRunning = !(startBtn.isEnabled());
		startBtn.setEnabled(isRunning);
		stopBtn.setEnabled(!isRunning);
	}
	
	private void work() {
		SingersCollector.start();
		MainEntry.runWork(partial, sId, eId, interval * Constants.SECOND, url);
		switchStatus();
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		setSystemLookAndFeel();
		new GuiMain();
	}

}
