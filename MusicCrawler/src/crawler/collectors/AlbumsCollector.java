package crawler.collectors;

import nbeans.Album;
import nbeans.Singer;

import org.json.JSONException;
import org.json.JSONObject;
import org.jsoup.Jsoup;
import org.jsoup.nodes.Element;
import org.jsoup.select.Elements;

import utils.LogUtils;
import app.Constants;
import exceptions.NoMoreProxyException;

public class AlbumsCollector extends AjaxResponseCollector {
	
	private static final int SIZE = 10;   // 每次查询的数量
	
	/**
	 * @param singer
	 */
	public AlbumsCollector(Singer singer) {
		super(singer);
	}
	
	private void recordReleaseTime(Element albumActive, Album album) {
		if (albumActive == null) return;
		Element time = albumActive.select("span.time").first();
		if (time != null) {
			String timeStr = time.text().trim();
			// private static final String UNKNOWN_TIME = "时间未知";
			// !timeStr.equals(UNKNOWN_TIME);
			if (timeStr.matches("\\d{4}-\\d{2}-\\d{2}")) {
				album.setReleaseTime(timeStr);         // 发行时间
			}
		}
	}
	
	private void recordStyles(Element albumActive, Album album) {
		if (albumActive == null) return;
		Element styles = albumActive.select("span.styles").first();
		if (styles != null) {
			album.setStyles(styles.text().trim());     // 唱片风格
		}
	}
	
	private void recordCompany(Element albumActive, Album album) {
		if (albumActive == null) return;
		Element company = albumActive.select("span.company").first();
		if (company != null) {
			album.setCompany(company.text().trim());   // 出品公司
		}
	}
	
	/**
	 * 获取一条专辑记录信息
	 * 
	 * @param album    含有专辑信息的页面元素
	 * @return
	 */
	private boolean makeAlbumRecord(Element albumEl, Album album) {
		Element albumLink = albumEl.select("a[href]").first();
		
		// absolute hyper reference
		String absHref = albumLink.attr("abs:href").trim();
		
		int id = parseId(absHref);                       // 专辑ID号
		String name = albumLink.attr("title").trim();    // 专辑名
	
		album.setId(id);
		album.setName(name);
		
	    // 由于不需要跟进链接获取信息，如果专辑信息不合法或者已存在，则可以在此处退出
		if (!album.isLegal()) return false;
		
		album.setSingerId(singer.getId());        // 歌手ID号
		album.setSingerName(singer.getName());    // 歌手名
		
		Element albumActive = albumEl.select("div.album-active").first();
		if (albumActive != null) {
			recordReleaseTime(albumActive, album);       // 发行时间
			recordStyles(albumActive, album);            // 唱片风格
			recordCompany(albumActive, album);           // 出品公司
		}
		
		return true;
	}

	/**
	 * 具体解析AJAX返回页面上的专辑信息
	 * 
	 * @param html
	 * @param content
	 */
	private void parseResponsePage(String html, StringBuffer content) {
		Elements albumEls = Jsoup.parse(html, BASE_URL).getElementsByClass("album-info");
		for (Element albumEl : albumEls) {
			Album album = new Album();
			if (makeAlbumRecord(albumEl, album)) {
				//System.out.println(album);
				album.saveToDB();
			}
		}
	}
	
	/**
	 * 列出一次AJAX请求返回的页面上的所有专辑
	 * list albums of a single page
	 * 
	 * @param ajaxUrl
	 * @throws NoMoreProxyException 
	 */
	private void listAlbumsInPage(String ajaxUrl) throws NoMoreProxyException {
		try {
			StringBuffer content = new StringBuffer();
			// 多次尝试后应该返回合法的JSON格式数据
			for (int i = 0; i < Constants.RETRY_TIMES; i++) {
				JSONObject jsonObj = newJsonObj(ajaxUrl);
				if (jsonObj == null) {    // 不使用代理再尝试一次
					newJsonObj(ajaxUrl, false);
				}
				if (jsonObj != null) {
					JSONObject innerObj = jsonObj.getJSONObject("data");
					if (innerObj != null) {
						String html = innerObj.getString("html");
				        parseResponsePage(html, content);
						//saveToFile(content.toString(), ALBUMS_PATH);
					}
					break;
				}
			}
		}
		catch (JSONException jsone) {
			LogUtils.LOGGER.warn(jsone.getMessage());
			//LogUtils.EXCEPTION_LOGGER.debug(LogUtils.getTrace(jsone));
		}
	}
	
	/**
	 * 列出构造函数中传入的URL对应歌手的全部专辑
	 * 
	 * @throws NoMoreProxyException 
	 */
	public void listAlbumsOfSinger() throws NoMoreProxyException {
		int numOfAlbums = singer.getNumOfAlbums();
		for (int i = 0; i < numOfAlbums; i += SIZE) {
			String getUrl = BASE_URL 
					+ String.format(GET_URL_FORMAT, ALBUM, singer.getId(), i, SIZE);
			listAlbumsInPage(getUrl);
		}
	}
	
	public static void main(String[] args) throws NoMoreProxyException {
		//String artistUrl = "http://music.baidu.com/artist/89010";
		Singer singer = new Singer();
		singer.setId(89010);
		singer.setNumOfAlbums(5);
		AlbumsCollector cAlbums = new AlbumsCollector(singer);
		cAlbums.listAlbumsOfSinger();
	}
	
}
