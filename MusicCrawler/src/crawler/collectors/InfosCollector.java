package crawler.collectors;

import java.io.File;
import java.io.IOException;
import java.net.Proxy;

import nbeans.Record;

import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;

import crawler.proxy.ProxyPool;

import app.Constants;

import utils.LogUtils;
import utils.StringUtils;
import utils.file.FileUtils;
import exceptions.NoMoreProxyException;

public abstract class InfosCollector {

	protected static final String BASE_URL       = "http://music.baidu.com";
	protected static final String GET_URL_FORMAT = "/data/user/get%ss?ting_uid=%d&start=%d&size=%d&order=hot";
	
	protected static final String SONG  = "song";
	protected static final String ALBUM = "album";

	protected static final String SEPARATOR = File.separator;
	protected static final String BASE_PATH = "D:" + SEPARATOR + "MusicData" + SEPARATOR;
	protected static final String SINGERS_PATH = BASE_PATH + "singers.txt";
	protected static final String SONGS_PATH   = BASE_PATH + "songs.txt";
	protected static final String ALBUMS_PATH  = BASE_PATH + "albums.txt";
	
	protected static final String LYRICS_BASE_PATH = BASE_PATH + "lyrics" + SEPARATOR;
	protected static final String IMG_BASE_PATH    = BASE_PATH + "imgs" + SEPARATOR;
	
	protected static final String DEFAULT_IMG_PATH = IMG_BASE_PATH + "0.png";
	protected static final String DEFAULT_IMG_URL  = BASE_URL + "/static/images/default/artist_default_130.png";
	
	protected static final String NEW_LINE = System.getProperty("line.separator");
	
	protected static final String BAD_LINK_STRING = "/search?key=";
	
	protected static final String BAIDU_VERIFY = "您的访问出错了";
	
	protected static final int TIME_OUT = 5 * Constants.SECOND;
	protected static int sleepMillis = 10;     // 控制访问速度

	protected static boolean stop = false;    // 停止爬取的标记

	private   static final String    ASY_STR = "An arbitrary string just for asynchronous!";
	protected static final ProxyPool pp      = ProxyPool.getInstance();
	private   static boolean available = false;
	protected static Proxy   proxy     = null;
	
	protected String url = "";
	
	protected Document htmlDoc = null;
	
	protected InfosCollector() { }
	
	public InfosCollector(String url) {
		this.url = url;
	}
	
	protected void getNextAvaliableProxy() {
		try {
			System.out.println("Try to get an available proxy.");
			proxy = pp.getProxy(proxy);
			available = true;
		}
		catch (NoMoreProxyException nmpe) {
			nmpe.printStackTrace();
		}
	}

	/**
	 * 可能在initialHtmlDoc的时候返回的htmlDoc就是verify页面的内容
	 * 
	 * @return    是否通过初始化得到了合法的htmlDoc
	 */
	protected boolean initialHtmlDoc() {
		try {
			Thread.sleep(sleepMillis);
			synchronized (ASY_STR) {
				if (!available) {
					getNextAvaliableProxy();
				}
			}
			//htmlDoc = Jsoup.connect(url, proxy).timeout(0).get();    // 应该避免将timeout设置为0
			htmlDoc = Jsoup.connect(url, proxy).get();
			if (!checkHtml(htmlDoc.text())) htmlDoc = null;
		}
		catch (IOException ioe) {
			LogUtils.LOGGER.warn(ioe.getMessage() + " ===> " + url);
			//LogUtils.EXCEPTION_LOGGER.debug(LogUtils.getTrace(ioe));
		}
		catch (InterruptedException ie) {
			ie.printStackTrace();
		}
		
		synchronized (ASY_STR) {
			available = (htmlDoc != null);
			return available;
		}
	}
	
	/**
	 * 应该只在第一次实例化CollectUrls或CollectSingers的时候才被调用
	 * 
	 * @param interval
	 * @param message
	 * @return
	 */
	public boolean doInitial(int interval, String message) {
		sleepMillis = interval;

		for (int i = 0; i < Constants.RETRY_TIMES * 10; i++) {
			if (initialHtmlDoc()) return true;
			else {
				LogUtils.LOGGER.warn(message);
				slowdown();
			}
		}

		return false;
	}
	
	protected boolean checkHtml(String htmlText) {
		String[] responseErrors = { BAIDU_VERIFY
							   	  , "部分网站无法正常访问"
							   	  , "Server erfolgreich erreicht."
							   	  , "Wowza Media Server 3 Perpetual"
							   	  , "善品网-个人/企业捐赠"
							   	  , "您访问的网站被机房安全管理系统拦截" };
		for (String re : responseErrors) {
			if (htmlText.contains(re)) {
				return false;
			}
		}
		return true;
	}
	
	/**
	 * 开始爬取
	 */
	public static void start() {
		stop = false;
	}

	/**
	 * 停止爬取
	 */
	public static void stop() {
		stop = true;
	}
	
	/**
	 * 以append模式将新内容追加到文件中
	 * 
	 * @param content    要追加的新内容
	 * @param path       文件路径：文件夹+文件名
	 */
	protected void saveToFile(String content, String path) {
		if (!content.isEmpty()) {
			FileUtils.append(content, path);
		}
	}
	
	protected int parseId(String url) {
		int id = 0;
		try {
			id = new Integer(StringUtils.lastPart(url, '/'));
		}
		catch (NumberFormatException nfe) {
			id = Record.ILLEGAL_ID;
		}
		return id;
	}

	public void slowdown() {
		try {
			Thread.sleep(sleepMillis);
		}
		catch (InterruptedException ie) {
			ie.printStackTrace();
		}
	}
	
}
