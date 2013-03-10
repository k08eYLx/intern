package crawler.collectors;

import nbeans.Singer;
import nbeans.Song;

import org.json.JSONException;
import org.json.JSONObject;
import org.jsoup.Jsoup;
import org.jsoup.nodes.Element;
import org.jsoup.select.Elements;

import utils.LogUtils;
import utils.StringUtils;
import app.Constants;
import app.Tracer;
import exceptions.NoMoreProxyException;

public class SongsCollector extends AjaxResponseCollector {
	
	private static final int SIZE = 20;
	
	/**
	 * @param singer
	 */
	public SongsCollector(Singer singer) {
		super(singer);
	}
	
	/**
	 * 从歌手页面的作品列表里获取歌曲的热度值
	 * 要去页面中分隔歌曲热度值的逗号
	 * Comma separated value to plain string value
	 * 
	 * @param el    包含歌曲热度字段的页面元素（没有歌曲的部分（li））
	 * @return      返回歌曲热度值
	 */
	private int fetchHotNumAsStr(Element el) {
		String strHotNumber = "";
		
		// 含有span.icon-new表示是新歌，新歌不包含span.hot-num
		Element iconNewEl = el.select("span.icon-new").first();
		if (iconNewEl == null) {
			Element hotNumEl = el.select("span.hot-num").first();
			if (hotNumEl != null) {
				strHotNumber = hotNumEl.text().trim();
				strHotNumber = strHotNumber.replaceAll(",", "");
			}	
			else {
				System.err.println(el);
			}
		}
		
		int hotNumber = StringUtils.toInt(strHotNumber); 
		return hotNumber != -1 ? hotNumber : 0;
	}
	
	private LyricCollector fetchSongInfoAndLyric(String absHref, Song song) {
		LyricCollector cLyric = new LyricCollector(absHref, song);
		for (int i = 0; i < Constants.RETRY_TIMES; i++) {
			if (cLyric.initialHtmlDoc()) {
				cLyric.fetchInfo();    // 专辑信息、发行时间、版权公司、歌曲标签以及歌词
				return cLyric;
			}
		}
		return null;
	}

	/**
	 * 获取一首歌的各字段组成的记录
	 * 会链接到歌曲页面获取歌曲基本信息，以及歌词
	 * 
	 * @param songEl    含有歌曲信息的页面元素
	 * @return
	 */
	private boolean makeSongRecord(Element songEl, Song song) {
		Element songLink = songEl.select("span.song-title>a[href]").first();
		
		// absolute hyper reference, 相对于BASE_URL
		String absHref = songLink.attr("abs:href").trim();
		
		int id = parseId(absHref);                      // 歌曲ID号
		/*
		 * 如果通过text获取到的歌曲名含有省略号（可能是名称过长，显示的不是全名），则再尝试通过attr获取
		 * 
		 * 通过attr获取到的字符串可能是损坏的格式
		 * 例如：attr是 星光伴我心电影\"老港正传\"，则只能获取到 星光伴我心电影\
		 */
		//String name = songLink.attr("title").trim();  // 歌曲名
		String name = songLink.text().trim();           // 歌曲名
		if (name.contains("..")) {
			name = songLink.attr("title").trim();
		}

		song.setId(id);
		song.setName(name);
		if (!(song.isLegal())) return false;
		
		song.setHotNumber(fetchHotNumAsStr(songEl));      // 热度值
		
		// 记录存在 并且 不需要更新，则返回（歌曲热度值有变化才更新）
		if (!song.needUpdate()) return false;
		
		// 记录不存在 或者 需要更新
		LogUtils.LOGGER.info(absHref);
		
		song.setSingerId(singer.getId());
		song.setSingerName(singer.getName());
		
		// 专辑信息、发行时间、版权公司、歌曲标签以及歌词
		LyricCollector cLyric = fetchSongInfoAndLyric(absHref, song);
		return (cLyric != null);
	}

	/**
	 * 具体解析AJAX返回页面上的歌曲信息
	 * 
	 * @param html
	 * @param content
	 */
	private void parseResponsePage(String html, StringBuffer content) {
		Elements songEls = Jsoup.parse(html, BASE_URL).getElementsByAttribute("data-songitem");
		for (Element songEl : songEls) {
			if (stop) break;
			Song song = new Song();
			if (makeSongRecord(songEl, song)) {
				Tracer.trace(song);
				song.saveToDB();
			}
		}
	}
	
	/**
	 * 列出一次AJAX请求返回的页面上的所有歌曲
	 * list songs of a single page
	 * 
	 * @param ajaxUrl    AJAX请求路径
	 * @throws NoMoreProxyException 
	 */
	private void listSongsInPage(String ajaxUrl) throws NoMoreProxyException {
		try {
			StringBuffer content = new StringBuffer();
			// 多次尝试后应该返回合法的JSON格式数据，但是要注意避免循环次数过多
			for (int i = 0; i < Constants.RETRY_TIMES; i++) {
				JSONObject jsonObj = newJsonObj(ajaxUrl);
				if (jsonObj == null) {    // 不使用代理再尝试一次
					jsonObj = newJsonObj(ajaxUrl, false);
				}
				if (jsonObj != null) {
					JSONObject innerObj = jsonObj.getJSONObject("data");
					if (innerObj != null) {
						String html = innerObj.getString("html");
				        parseResponsePage(html, content);
						//saveToFile(content.toString(), SONGS_PATH);
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
	 * 列出构造函数中传入的URL对应歌手的全部歌曲
	 * list songs of a single singer
	 * 
	 * @throws NoMoreProxyException 
	 */
	public void listSongsOfSinger() throws NoMoreProxyException {
		int numOfSongs = singer.getNumOfSongs();
		for (int i = 0; i < numOfSongs; i += SIZE) {
			if (stop) break;
			
			String getUrl = BASE_URL 
					+ String.format(GET_URL_FORMAT, SONG, singer.getId(), i, SIZE);
			listSongsInPage(getUrl);
		}
	}
	
	public static void main(String[] args) throws NoMoreProxyException {
		//String artistUrl = "http://music.baidu.com/artist/89010";
		Singer singer = new Singer();
		singer.setId(2374);
		singer.setNumOfSongs(4974);
		SongsCollector cSongs = new SongsCollector(singer);
		cSongs.listSongsOfSinger();
	}
	
}
