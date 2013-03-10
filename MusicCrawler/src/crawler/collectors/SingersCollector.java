package crawler.collectors;

import java.io.File;
import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;

import nbeans.Record;
import nbeans.Singer;

import org.jsoup.nodes.Element;
import org.jsoup.select.Elements;

import utils.ImageUtils;
import utils.LogUtils;
import utils.file.PropertyUtils;
import app.Constants;
import app.Tracer;

public class SingersCollector extends InfosCollector implements Runnable {
	
	private String threadName = "";
	
	private int sId = Record.ILLEGAL_ID;
	private int eId = Record.ILLEGAL_ID;
	
	private CyclicBarrier barrier = null;

	private PropertyUtils pus = null;
	
	/**
	 * 单线程
	 * 
	 * @param url
	 */
	public SingersCollector(String url) {
		super(url);
	}
	
	public SingersCollector(String url, PropertyUtils pus) {
		super(url);
		this.pus = pus;
	}

	/**
	 * 多线程    使用CyclicBarrier
	 * 
	 * @param url
	 * @param barrier
	 */
	public SingersCollector(String url, CyclicBarrier barrier, PropertyUtils pus) {
		super(url);
		this.barrier = barrier;
		this.pus = pus;
	}
	
	public void restoreId() {
		// thread name ===> id
		if (pus != null) {
			System.out.println(threadName);
			sId = pus.restoreIntField(threadName);
		}
	}
	
	public void saveId() {
		// thread name <=== id
		if (pus != null) {
			pus.storeIntField(threadName, sId);	
		}
	}

	/**
	 * 停止爬取前，将curId暂存到sId当中
	 * 
	 * @param curId
	 * @return
	 */
	private boolean stop(int curId) {
		sId = curId;
		return false;
	}
	
	/**
	 * 下载默认的歌手头像图片
	 */
	public static void fetchDefaultImage() {
		String imgUrl = DEFAULT_IMG_URL;
		String imgPath = DEFAULT_IMG_PATH;
		
		File defImg = new File(imgPath);
		if (defImg.exists()) return;

		while (!(ImageUtils.netFetchImg(imgUrl, imgPath))) {
			LogUtils.LOGGER.warn(imgUrl + " ===> Retry fetch default image");;
		}
	}
	
	/**
	 * 链接到歌手页面去爬取信息
	 * 歌手  头像图片 + 听众数 + 地区 + 生日（星座）
	 * 
	 * @param absHref
	 */
	private boolean linkToSingerPage(String absHref, SingerCollector cSinger) {
		LogUtils.LOGGER.info(absHref);
		for (int i = 0; i < Constants.RETRY_TIMES; i++) {
			if (cSinger.initialHtmlDoc()) {
				return cSinger.fetchInfo();
			}
		}
		return false;
	}
	
	/**
	 * 记录歌手信息
	 * 
	 * @param link
	 */
	private void recordSinger(int curId, Element link, String absHref) {
		Singer singer = new Singer();

		singer.setId(curId);                        // 歌手ID号
		// 不按照link.text()获取，因为如果内容过长text可能是以'...'结束的省略形式
		singer.setName(link.attr("title").trim());  // 歌手名	
		if (!(singer.isLegal())) return;

		// 根据歌手页面的li.path-list-item确定歌手分类
		//singer.setType(threadName);
		
		/*
		 * 由于进一步抓取专辑和歌曲信息都需要通过歌手页面，所以歌手页面是一定要访问的
		 * 而且歌手的听众数应该是随时间变化（增加?）的
		 * 这样一来，根据数据库已有记录来决定是否对页面上的歌手基本信息进行解析的意义变得就不大了（不会节省太多处理时间）
		 */
		SingerCollector cSinger = new SingerCollector(absHref, singer);
		if (linkToSingerPage(absHref, cSinger)) {
			cSinger.listWorks();
		}
		
		Tracer.trace(singer);
		// 可能需要更新数据库记录中 听众数、地区和生日（星座）等字段
		// 如果cSinger为null，则表示没有成功链接到歌手页面，不进行更新，以免覆盖了可能正确的已有记录信息
		if (cSinger != null) singer.saveToDB();
	}
	
	/**
	 * 遍历传入的歌手链接
	 * 爬取歌手信息、专辑、歌曲
	 * 
	 * @param links
	 */
	private boolean traverseLinks(Elements links) {
		for (Element link : links) {
			// absolute hyper reference
			String absHref = link.attr("abs:href").trim();
			
			int curId = parseId(absHref);
			if (stop) return stop(curId);
			if (curId == sId) sId = Record.ILLEGAL_ID;

			if (sId == Record.ILLEGAL_ID) {
				recordSinger(curId, link, absHref);
			}
			
			if (eId != Record.ILLEGAL_ID && curId == eId) {
				return false;    // 结束爬取过程
			}
		}
		return true;
	}
	
	private Elements getListItems() {
		return htmlDoc.getElementsByClass("list-item");
	}
	
	public int countSingers() {
		int size = 0;
		Elements listItems = getListItems();
		if (listItems.size() > 0) {
			// 虽然会有部分重复，但是为了简化统一处理方式，不移除热门歌手部分了
			//listItems.remove(0);    // 去除热门歌手部分，这些歌手已经在和后面的完整歌手列表里。
			Elements singerEls = listItems.select("a[href]");
			size = singerEls.size();
			System.out.println(url + " ===> " + size);
		}
		else {
			System.out.println(htmlDoc);
		}
		return size;
	}

	/**
	 * 根据传入构造函数的URL，列举出页面中的所有歌手
	 * 并根据每个歌手的页面路径链接进一步获取出更多的详细信息
	 * 
	 * 如果传入的inId等于Record.ILLEGAL_ID，才会尝试从文件恢复id
	 */
	public void listSingers(int sInId, int eInId) {
		threadName = Thread.currentThread().getName();
		
		if (sInId == Record.ILLEGAL_ID) restoreId();
		else sId = sInId;
		System.out.println(threadName + " ===> Initial ID: " + sId);

		if (eInId != Record.ILLEGAL_ID) eId = eInId;
		
		Elements listItems = getListItems();
		if (listItems.size() > 0) {
			// 虽然会有部分重复，但是为了简化统一处理方式，不移除热门歌手部分了
			//listItems.remove(0);    // 去除热门歌手部分，这些歌手已经在和后面的完整歌手列表里。
		}
		else {
			System.out.println(htmlDoc);
		}
			
		for (Element listItem : listItems) {
			// "ul.clearfix a[href]"
			Elements links = listItem.select("a[href]");
			if (!(traverseLinks(links))) break;
		}
		
		 /*
	     *  如果正常爬取完整个页面，id应该为Record.ILLEGAL_ID
	     *  如果是由于抛出异常而中断执行，id应该记录的是中断处的歌手ID
	     */
		System.out.println(threadName + " ===> Final ID: " + sId);
		saveId();
	}

	@Override
	public void run() {
		try {
			LogUtils.LOGGER.info(url);
			listSingers(Record.ILLEGAL_ID, Record.ILLEGAL_ID);
			barrier.await();
		}
		catch (InterruptedException ie) {
			LogUtils.LOGGER.error(ie.getMessage());
		}
		catch (BrokenBarrierException bbe) {
			LogUtils.LOGGER.error(bbe.getMessage());
		}
	}
	
	public static void main(String[] args) throws InterruptedException {
		Thread.currentThread().setName("western-group");
		String singersUrl = "http://music.baidu.com/artist/western/group";
		SingersCollector cSingers = new SingersCollector(singersUrl);
		while (!(cSingers.initialHtmlDoc())) {
			LogUtils.LOGGER.warn("Retry initial singers html document");
		}
		cSingers.listSingers(Record.ILLEGAL_ID, Record.ILLEGAL_ID);
	}

}
