package crawler.collectors;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;

import org.jsoup.nodes.Element;
import org.jsoup.select.Elements;

import app.Constants;

import utils.LogUtils;
import utils.file.FileUtils;
import utils.file.PropertyUtils;

public class UrlCollector extends InfosCollector {
	
	private static final int SIZE = 10;
	
	private static final int MAX_PRIORITY = Thread.MAX_PRIORITY - 1;
	private static final int HIGH_PRIORITY = 7;

	private static final String STORE_FILE = "tmp.ids";

	public UrlCollector(String url) {
		super(url);
	}
	
	/**
	 * 截取歌手分类URL的一部分，作为子线程名称
	 * 
	 * @param categoryUrl
	 * @return    子线程名称
	 */
	private String makeThreadName(String categoryUrl) {
		String identifier = "artist/";
		int pos = categoryUrl.lastIndexOf(identifier);
		pos += identifier.length();
		String threadName = categoryUrl.substring(pos);
		threadName = threadName.replace('/', '-');
		if (threadName.isEmpty()) {
			threadName = "main-url";    // As main URL
		}
		return threadName;
	}
	
	/**
	 * 调整线程的优先级，对于jpkr和other使用默认优先级
	 * 
	 * 不要假定高优先级的线程一定先于低优先级的线程被系统执行，不要有逻辑依赖于线程优先级
	 * 
	 * @param thread
	 * @return    返回传入的thread，便于链式调用
	 */
	private Thread adjustPriority(Thread thread) {
		String tn = thread.getName();
		if (tn.contains("western")) {
			thread.setPriority(MAX_PRIORITY);
		}
		else if (tn.contains("cn")) {
			thread.setPriority(HIGH_PRIORITY);
		}
		return thread;
	}

	/**
	 * 建立子线程，爬取各歌手分类下的信息
	 * 
	 * @param hrefs
	 * @param cyclicBarrier
	 */
	private void runSubJobs(List<String> urls, CyclicBarrier barrier) {
		PropertyUtils pus = new PropertyUtils(STORE_FILE);
		
		for (String url : urls) {
			SingersCollector cSingers = new SingersCollector(url, barrier, pus);
			while (!(cSingers.initialHtmlDoc())) {
				LogUtils.LOGGER.warn(url + " ===> Retry initial html document");
				slowdown();
			}
			
			String threadName = makeThreadName(url);
			Thread thread = new Thread(cSingers, threadName);
			adjustPriority(thread).start();
		}
	}

	/**
	 * 挂起主线程
	 * 
	 * @param cyclicBarrier
	 */
	private void mainAwait(CyclicBarrier cyclicBarrier) {
		try {
			cyclicBarrier.await();
		}
		catch (BrokenBarrierException bbe) {
			bbe.printStackTrace();
		}
		catch (InterruptedException ie) {
			ie.printStackTrace();
		}
	}
	
	/**
	 * 主页面地址 + 各歌手分类地址 + 额外的地址
	 * 
	 * @param urls
	 */
	private void collectUrls(List<String> urls) {
		urls.add(url);
		
		Elements hrefs = htmlDoc.select("dl.tree_main a[href]");
		
		int size = hrefs.size();
		if (SIZE != size) {
			System.out.println(size + "\n" + hrefs);
		}
		
		for (Element href : hrefs) {
			String absUrl = href.attr("abs:href");
			if (absUrl != null && !(absUrl.isEmpty())) {
				urls.add(absUrl);
			}
		}
		
		FileUtils.readByLine(Constants.ADDITIONAL_URLS, urls);
	}
	
	/**
	 * 根据各页面链接遍历所有歌手
	 */
	public void listAllUrls() {
		List<String> urls = new ArrayList<String>();
		collectUrls(urls);
		int size = urls.size();
		CyclicBarrier barrier = new CyclicBarrier(size + 1);    // +1 ，让主线程等待所有子线程结束
		runSubJobs(urls, barrier);
		mainAwait(barrier);    // 执行完所有子线程后会回到这里
	}

	public int countSingers() {
		int total = 0;
		List<String> urls = new ArrayList<String>();
		collectUrls(urls);
		for (String url : urls) {
			SingersCollector cSingers = new SingersCollector(url);
			while (!(cSingers.initialHtmlDoc())) {
				LogUtils.LOGGER.warn(url + " ===> Retry initial html document");
			}
			total += cSingers.countSingers();
		}
		return total;
	}
	
	public static void main(String[] args) {
		UrlCollector cUrls = new UrlCollector(Constants.MAIN_URL);
		cUrls.initialHtmlDoc();
		System.out.println(cUrls.countSingers());
	}
	
}
