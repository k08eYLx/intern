package crawler.collectors;

import java.io.File;

import javax.swing.JOptionPane;

import nbeans.Singer;

import org.jsoup.nodes.Element;
import org.jsoup.select.Elements;

import utils.ImageUtils;
import utils.LogUtils;
import utils.StringUtils;
import app.Constants;
import exceptions.NoMoreProxyException;

public class SingerCollector extends InfosCollector {
	
	private static final String REGION = "地区：";
	private static final String BIRTHDAY = "生日：";
	
	private Singer singer =  null;
	
	public SingerCollector(String url, Singer singer) {
		super(url);
		this.singer = singer;
	}
	
	/**
	 * 填充歌手对象的各个字段
	 */
	private void fetchNumOfWorks() {
		int[] numOfWorks = new int[3];  // 0 - 歌曲数;   1 - 专辑数;   2 - 图片数
		Element singerWorks = htmlDoc.getElementById("singerWorks");
		if (singerWorks == null) return;
		Elements lists = singerWorks.select("ul.tab-list a.list");
		for (int i = 0; i < lists.size(); i++) {
			Element list = lists.get(i);
			String text = list.text();
			int sPos = text.indexOf('(') + 1;
			int ePos = text.indexOf(')');
			String strNum = text.substring(sPos, ePos).trim();
			numOfWorks[i] = new Integer(strNum); 
		}
		
		singer.setNumOfSongs(numOfWorks[0]);
		singer.setNumOfAlbums(numOfWorks[1]);
	}

	public boolean fetchInfo() {
		if (htmlDoc == null) return false;
		
		// 如果传入构造函数的singer对象的name为空，则再次尝试填充
		if (singer.getName().isEmpty()) {
			Element nameEl = htmlDoc.select("h2.singer-name").first();
			if (nameEl == null) return false;
			String name = nameEl.text().trim();
			if (name.isEmpty()) return false;
			singer.setName(name);
		}

		fetchType();
		fetchImage();
		fetchBaseInfo();
		fetchNumOfWorks();
		return fetchListenerNumber();
	}
	
	/**
	 * 确定歌手所属于的分类
	 */
	private void fetchType() {
		String identifier = "artist/";
		String type = htmlDoc.select("li.path-list-item>a[href]").last().attr("abs:href");
		int pos = type.lastIndexOf(identifier);
		pos += identifier.length();
		type = type.substring(pos).replace('/', '-');
		// 移除最后多余的字符‘-’
		if (!type.isEmpty() && type.lastIndexOf('-') == (type.length() - 1)) {
			type = type.substring(0, type.length() - 1);
		}
		singer.setType(type);
	}
	
	/**
	 * 获取歌手听众数
	 * 
	 * @return
	 */
	private boolean fetchListenerNumber() {
		Element listenerEl = htmlDoc.select("span.play-num>span.num").first();
		if (listenerEl != null) {
			String listenersStr = listenerEl.text().trim();
			listenersStr = listenersStr.replaceAll(",", "");
			int listeners = StringUtils.toInt(listenersStr);
			singer.setListeners(listeners);
			if (listeners < 0) {
				System.err.println(getClass().getName() + " : " + singer.getId() + " ===> " + listenersStr);
			}
		}
		else {
			// 认为是代理出错，返回的网页内容不正确
			getNextAvaliableProxy();
			System.err.println("Bad singer page ===> " + url);
			System.err.println(htmlDoc.text());
		}
		return (listenerEl != null);
	}

	/**
	 * 根据传入构造函数的URL，获取歌手的基本信息
	 */
	private void fetchBaseInfo() {
		String region   = "";
		String birthday = "";

		for (int i = 0; i < Constants.RETRY_TIMES; i++) {
			// infoLi可能为空，没有基本信息时，有的时候是用<li></li>标记占位的，有的时候又是没有占位的
			Element infoLi = htmlDoc.select("ul[class=c6 clearfix] li:not([class])").first();
			if (infoLi != null) {
				String infoStr = infoLi.text().trim();
				if (infoStr.contains(REGION) || infoStr.contains(BIRTHDAY)) {
					Elements infos = infoLi.select("span");
					for (Element info : infos) {
						if (info.text().matches("\\d{2}-\\d{2}.*")) { // 匹配生日，可能含有星座部分
							birthday = info.text().trim();
						}
						else {
							region = info.text().trim();
						}
					}
				}
				break;
			}
		}

		singer.setRegion(region);
		singer.setBirthday(birthday);
	}

	/**
	 * 下载歌手的头像图片，存放文件名为歌手id.jpg
	 * 需要使用图片时，先判断文件歌手id.jpg是否存在
	 * 如果不存在，则使用默认图片0.png
	 */
	private void fetchImage() {
		// 不应该为空
		Element imgEl = htmlDoc.select("div#baseInfo img[src]").first();
		if (imgEl != null) {
			String imgUrl = imgEl.attr("abs:src").trim(); // 获得绝对路径
			if (!(imgUrl.equals(DEFAULT_IMG_URL))) {      // 不是默认头像才下载图片
				String suffix = imgUrl.substring(imgUrl.lastIndexOf('.'));
				if (parseId(url) != singer.getId()) {
					System.out.println("!!! ID does not match !!!");
				}
				String path = IMG_BASE_PATH + singer.getId() + suffix;
				File img = new File(path);
				if (img.exists()) return;
				for (int j = 0; j < Constants.RETRY_TIMES; j++) {
					if (ImageUtils.netFetchImg(imgUrl, path)) break;
				}
			}
		}
		//singer.setImgPath(path);    // DEFAULT_IMG_PATH;
	}
	
	/**
	 * 根据传入构造函数的URL，获取此歌手的所有歌曲和专辑信息
	 */
	public void listWorks() {
		if (singer.needUpdate()) {
			try {
				AlbumsCollector cAlbums = new AlbumsCollector(singer);
				cAlbums.listAlbumsOfSinger();	

				SongsCollector cSongs = new SongsCollector(singer);
				cSongs.listSongsOfSinger();	
			}
			catch (NoMoreProxyException nmpe) {
				nmpe.printStackTrace();
				JOptionPane.showMessageDialog(null, "WARNING!\n" + nmpe.getMessage());
			}
		}
	}
	
	public static void main(String[] args) {
		String singersUrl = "http://music.baidu.com/artist/16183933";
		Singer singer = new Singer();
		singer.setId(16183933);
		SingerCollector cSinger = new SingerCollector(singersUrl, singer);
		while (!(cSinger.initialHtmlDoc())) {
			LogUtils.LOGGER.warn("Retry initial artist html document");
		}
		cSinger.listWorks();
		//cSinger.fetchListenerNumber();
		//cSinger.fetchBaseInfo();
		//cSinger.fetchImage();
		System.out.println(singer.toString());
	}

}
