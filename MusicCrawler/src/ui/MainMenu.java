package ui;

import index.calculator.CalculateHelper;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JFrame;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;

import nbeans.Record;

import crawler.proxy.ProxyHelper;
import export.m2h.ExportHelper;



public class MainMenu {

	static class FromNetListener implements ActionListener {

		@Override
		public void actionPerformed(ActionEvent e) {
			(new Thread(new Runnable() {
				@Override
				public void run() {
					ProxyHelper.fetchAvailableProxy();
				}
			})).start();
		}

	}
	
	static class ByHandListener implements ActionListener {

		@Override
		public void actionPerformed(ActionEvent e) {	
			String message = "Input your proxy (ip:port):";
			String proxyStr = JOptionPane.showInputDialog(message);
			if (proxyStr != null) {
				if (!(ProxyHelper.saveProxy(proxyStr))) {
					JOptionPane.showMessageDialog(null, "Bad proxy! Please check!");
				}
			}
		}

	}

	static class EmptyListener implements ActionListener {

		@Override
		public void actionPerformed(ActionEvent e) {
			int result = JOptionPane.showConfirmDialog(null, "Do you really want to empty the proxy list?");
			if (result == JOptionPane.OK_OPTION) {
				ProxyHelper.empty();	
			}
		}

	}

	static class CalListener implements ActionListener {

		@Override
		public void actionPerformed(ActionEvent e) {
			Thread thread = new Thread(new Runnable() {
				@Override
				public void run() {
					CalculateHelper.runWork();
				}
			});
			thread.setName("Calculation");
			thread.start();
		}

	}
	
	static class ExportListener implements ActionListener {

		@Override
		public void actionPerformed(ActionEvent e) {
			Thread thread = new Thread(new Runnable() {
				@Override
				public void run() {
					ExportHelper.work();
				}
			});
			thread.setName("ExportData");
			thread.start();
		}

	}
	
	private static JMenuBar  menuBar    = new JMenuBar();
	private static JMenu     proxyMenu  = new JMenu("添加代理");
	private static JMenuItem fromNet    = new JMenuItem("网络");
	private static JMenuItem byHand     = new JMenuItem("手动");
	private static JMenuItem empty      = new JMenuItem("清空");
	private static JMenu     calMenu    = new JMenu("计算索引");
	private static JMenuItem cal        = new JMenuItem("计算");
	private static JMenu     exportMenu = new JMenu("导出数据");
	private static JMenuItem export     = new JMenuItem("导出");
	
	public static void addTo(JFrame frame) {
		initialProxyMenu();
		
		/**
		 * 如果正在使用MySQL数据库，则加载“导出数据”菜单；
		 * 如果正在使用          H2数据库，则加载“计算索引”菜单。
		 */
		if (Record.isUsingMySql()) {
			initialExportMenu();	
		}
		else {
			initialCalMenu();	
		}
		
		frame.setJMenuBar(menuBar);
	}
	
	private static void initialProxyMenu() {
		proxyMenu.addSeparator();

		fromNet.addActionListener(new FromNetListener());
		proxyMenu.add(fromNet);

		proxyMenu.addSeparator();

		byHand.addActionListener(new ByHandListener());
		proxyMenu.add(byHand);

		proxyMenu.addSeparator();

		empty.addActionListener(new EmptyListener());
		proxyMenu.add(empty);

		proxyMenu.addSeparator();
		menuBar.add(proxyMenu);
	}
	
	private static void initialCalMenu() {
		calMenu.addSeparator();
		
		cal.addActionListener(new CalListener());
		calMenu.add(cal);
		
		calMenu.addSeparator();
		menuBar.add(calMenu);
	}
	
	private static void initialExportMenu() {
		exportMenu.addSeparator();
		
		export.addActionListener(new ExportListener());
		exportMenu.add(export);
		
		exportMenu.addSeparator();
		menuBar.add(exportMenu);
	}
	
}
