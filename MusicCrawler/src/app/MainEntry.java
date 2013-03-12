package app;

import java.io.IOException;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.jsoup.Jsoup;
import org.jsoup.nodes.Element;

import utils.file.PropertyUtils;
import crawler.collectors.SingersCollector;
import crawler.collectors.UrlCollector;

public class MainEntry {
	
	private static final Logger LOGGER = LogManager.getLogger(MainEntry.class.getName());

	private static void addShutdownHook() {
		Runtime.getRuntime().addShutdownHook(new Thread() {  
            public void run() {  
                System.err.println("Shutdown Hook!");  
            }  
        });
	}
	
	private static String calculateTime(long start, long finish) {
		long total = (finish - start) / (1000 * 60);    // minutes
		long hours = total / 60;
		long minutes = total % 60;
		return String.format("Total time: %d hours - %d minutes.", hours, minutes);
	}
	
	// if part is false, ids should be Record.ILLEGAL_ID
	public static void runWork(boolean part, int sId, int eId, int interval, String url) {
		LOGGER.info("App started.");

		addShutdownHook();
		
		long start = System.currentTimeMillis();

		SingersCollector.fetchDefaultImage();
		if (part) runPartWrok(sId, eId, interval, url);
		else runFullWork(interval, url);

		long finish = System.currentTimeMillis();
		LOGGER.info("Work finished." + " ===> " + calculateTime(start, finish));
	}
	
	private static void runPartWrok(int sId, int eId, int interval, String url) {
		String message = "Retry initial singers html document";    // "重新尝试初始化页面访问!";
		PropertyUtils pus = new PropertyUtils(Constants.IDS_FILE);
		SingersCollector cSingers = new SingersCollector(url, pus);
		if (cSingers.doInitial(interval, message)) {
			cSingers.listSingers(sId, eId);
		}
	}

	private static void runFullWork(int interval, String url) {
		String message = "Retry initial main html document";       // "重新尝试初始化页面访问!";
		UrlCollector cUrls = new UrlCollector(url);
		if (cUrls.doInitial(interval, message)) {
			cUrls.listAllUrls();
		}
	}
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		//runWork(false, Record.ILLEGAL_ID, Record.ILLEGAL_ID, AppConstants.SECOND, AppConstants.MAIN_URL);
		try {
			Element element = Jsoup.connect("http://music.baidu.com/song/35104979").get().select("span.icon-new").first();
			if (element != null) {
				System.out.println(element);
			}
		}
		catch (IOException ioe) {
			ioe.printStackTrace();
		}
	}

}
