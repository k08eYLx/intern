package crawler.proxy;

import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.io.LineNumberReader;
import java.net.HttpURLConnection;
import java.net.InetSocketAddress;
import java.net.Proxy;
import java.net.URL;
import java.util.ArrayList;

import javax.swing.JOptionPane;

import utils.file.FileUtils;
import app.Constants;
import exceptions.NoMoreProxyException;

public class ProxyPool {

	private static final String IP_REGEX   = "(\\d{1,3}\\.){3}\\d{1,3}:\\d{2,5}";

	private static final int TIME_OUT = 1 * Constants.SECOND;
	
	private static ArrayList<Proxy> pool = new ArrayList<Proxy>();
	private static ArrayList<String> availables = new ArrayList<String>();
	
	private int maxCount = 0;
	
	private static final ProxyPool instance = new ProxyPool();
	
	private ProxyPool() {
		initialPool();
		poolMessage();
	}

	public static ProxyPool getInstance() {
		return instance;
	}
	
	private void initialPool() {
		try {
			System.out.println("Initializing available proxy list...");
        	
			File file = new File(Constants.PROXY_FILE);
        	if (!(file.exists())) return;
        	
        	FileReader in = new FileReader(file);
        	LineNumberReader reader = new LineNumberReader(in);
        	
        	String line = null;
			while ((line = reader.readLine()) != null) {
				Proxy proxy = createProxy(line);
			    if (checkProxy(proxy)) {
			    	pool.add(proxy);
			    	availables.add(line);
			    	System.out.println(line + "\t ===> an available proxy.");
				}
			}
			reader.close();
			
			maxCount = pool.size() * 2;
			saveAvailables();
		}
		catch (IOException ioe) {
			ioe.printStackTrace();
		}
	}
	
	private void poolMessage() {
		boolean flag = false;
		if (pool.size() <= 0) {
			JOptionPane.showMessageDialog(null, "没有可用代理！\n程序将自动尝试从网络获取。");
			flag = true;
		}
		else {
			System.out.println("Proxy list initialized ===> " + pool.size());
			if (pool.size() <= 10) {
				String message = "There are less than 10 proxies left.\n"
						+ "Do you want to fetch more?";
				int result = JOptionPane.showConfirmDialog(null, message, "WARNING!"
						, JOptionPane.YES_NO_OPTION, JOptionPane.WARNING_MESSAGE);
				flag = (result == JOptionPane.YES_OPTION);
			}
		}
		
		if (flag) enlarge();
	}

	private void enlarge() {
		ProxyHelper.fetchAvailableProxy(availables);
		for (String li : availables) {
			Proxy proxy = createProxy(li);
		    if (checkProxy(proxy)) {
		    	pool.add(proxy);
		    }
		}
		System.out.println("Proxy pool size has been enlarged to ===> " + pool.size());
		maxCount = pool.size() * 2;
		saveAvailables();
	}

	private void saveAvailables() {
		FileUtils.write("", Constants.PROXY_FILE);
		StringBuffer sBuffer = new StringBuffer();
		for (String available : availables) {
			sBuffer.append(available + "\r\n");
		}
		FileUtils.append(sBuffer.toString(), Constants.PROXY_FILE);
	}
	
	private Proxy createProxy(String proxyStr) {
		Proxy proxy = null;
		if (proxyStr.matches(IP_REGEX)) {
			String[] strs = proxyStr.split(":");
			String ipStr = strs[0];
			int    port  = Integer.valueOf(strs[1]);

			InetSocketAddress address = new InetSocketAddress(ipStr, port);
			proxy = new Proxy(Proxy.Type.HTTP, address);
		}
		return proxy;
	}
	
	public boolean checkProxy(Proxy proxy) {
		if (proxy == null) return false;
		
		try {
			URL url = new URL(Constants.MAIN_URL);
			HttpURLConnection conn = (HttpURLConnection) url.openConnection(proxy);
			conn.setConnectTimeout(TIME_OUT);
			conn.setReadTimeout(TIME_OUT);
			
			return (HttpURLConnection.HTTP_OK == conn.getResponseCode());
		}
		catch (IOException ioe) {
			System.out.println("Bad proxy ===> " + ioe.getMessage());
		}
		
		return false;
	}
	
	private boolean assureCheck(Proxy proxy) {
		for (int i = 0; i < Constants.RETRY_TIMES; i++) {
			if (checkProxy(proxy)) return true;
		}
		return false;
	}
	
	/**
	 * 获取一个可用的代理
	 * 
	 * @param proxy
	 * @return
	 * @throws NoMoreProxyException 
	 */
	public Proxy getProxy(Proxy proxy) throws NoMoreProxyException {
		synchronized (pool) {
			if (proxy != null) pool.add(proxy);
			int count = 0;
			int iCount = 0;
			while (pool.size() > 0) {
				proxy = pool.remove(0);
				if (assureCheck(proxy)) break;
				//else pool.add(proxy);
				if (++count > maxCount) {
					count = 0;
					if (++iCount > Constants.RETRY_TIMES) {
						throw new NoMoreProxyException();
					}
					enlarge();
				}
			}
			return proxy;
		}
	}
	
}
