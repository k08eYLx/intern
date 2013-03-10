package crawler.collectors;

import java.io.File;

import nbeans.Record;
import nbeans.Song;

import org.jsoup.nodes.Element;
import org.jsoup.select.Elements;

import utils.LogUtils;
import utils.StringUtils;
import utils.file.FileUtils;

public class LyricCollector extends InfosCollector {

	private Song song = null;
	
	public LyricCollector(String url) {
		super(url);
	}
	
	public LyricCollector(String url, Song song) {
		super(url);
		this.song = song;
	}
	
	/**
	 * 保存歌词到文件
	 * 
	 * @param lyric    歌词内容
	 * @return         歌词路径    歌曲id.txt
	 */
	private String saveLyric(Element lyricCont) {
		String folder = LYRICS_BASE_PATH + song.getSingerId() + SEPARATOR;
		String path = folder + song.getId() + ".txt";
		File lyric = new File(path);
		if (!(lyric.exists())) {
			String lyricStr = fetchLyric(lyricCont).trim();
			FileUtils.write(lyricStr, path);
		}
		return path;
	}
	
	/**
	 * 解析页面上歌词的具体操作
	 * 
	 * @param lyricCont    包含歌词部分的页面元素
	 * @return             歌词内容
	 */
	private String fetchLyric(Element lyricCont) {
		String lyric = lyricCont.toString();
		int beginPos = lyric.indexOf(">") + 1;
		int endPos = lyric.lastIndexOf("<");
		lyric = lyric.substring(beginPos, endPos);
		return lyric.replaceAll("<br /> ", NEW_LINE);
	}
	
	/**
	 * 获取歌词
	 * 
	 * @return
	 */
	private void listLyricOfSong() {
		Element lyricCont = htmlDoc.getElementById("lyricCont");
		if (lyricCont != null) {    // 可能没有歌词
			saveLyric(lyricCont);
		}
		//song.setLyricPath(path);
	}

	/**
	 * 尝试获取歌曲的专辑信息
	 * 
	 * @param infos
	 */
	private void fetchAlbumInfo(Elements infos) {
		Element album = infos.select("li[class=clearfix]>a").first(); 
		if (album != null) {
			String href = album.attr("href").trim();
			if (!(href.isEmpty() || href.contains(BAD_LINK_STRING))) {
				song.setAlbumId(parseId(href));
			}
			else {
				song.setAlbumId(Record.ILLEGAL_ID);
			}
			song.setAlbumName(album.text().trim());          // 专辑名
		}
	}
	
	/**
	 * 确保存储的是歌曲真正属于的歌手ID和名字
	 * 如果歌曲页面的歌手ID与歌手页面的不匹配则进行歌手信息更新
	 * 
	 * 虽然是从歌手页面获取到歌曲页面，但从歌曲页面获取到的歌手信息可能不同
	 * 例如：
	 * 从中国好声音学员页面链接到的歌曲页面上的歌手可能是具体的某一个歌手
	 * http://music.baidu.com/artist/16183933
	 * 
	 * @param infos
	 */
	private void fetchAuthorInfo(Elements infos) {
		Element singerHrefElt = infos.select("span.author_list>a[href]").first();
		if (singerHrefElt != null) {
			String singerHref = singerHrefElt.attr("href").trim();
			int singerId = parseId(singerHref);
			if (song.getSingerId() != singerId) {
				song.setSingerId(singerId);
				String singerName = singerHrefElt.text().trim();
				song.setSingerName(singerName);
			}
		}
	}
	
	/**
	 * 尝试获取歌曲的发行时间信息
	 * 
	 * @param infos
	 */
	private void fetchReleaseTime(Elements infos) {
		Element time = infos.select("li:contains(发行时间)").first();
		if (time != null) {
			String timeStr = StringUtils.getValue(time.text());
			// 跳过 "XXXX年" 这种特殊情况
			if (timeStr.matches("\\d{4}-\\d{2}-\\d{2}")) {
				song.setReleaseTime(timeStr);    // 发行时间
			}
		}
	}
	
	/**
	 * 尝试获取歌曲的版权公司信息
	 * 
	 * @param infos
	 */
	private void fetchCompany(Elements infos) {
		Element company = infos.select("li:contains(所属公司)").first();
		if (company != null) {
			song.setCompany(StringUtils.getValue(company.text()));     // 版权公司
		}
	}

	/**
	 * 尝试获取歌曲的标签
	 * 
	 * @param infos
	 */
	private void fetchTags(Elements infos) {
		String tagStr = "";
		Elements tags = infos.select("a.tag-list");
		if (tags.size() != 0) {
			tagStr += tags.remove(0).text().trim();
			for (Element tag : tags) {
				tagStr += "," + tag.text().trim();
			}
		}
		song.setTags(tagStr);    // 标签
	}
	
	/**
	 * 获取并记录页面上的歌曲基本信息
	 */
	private void fetchBaseInfo() {
		String queryStr = "ul[class=base-info c6] li:not([class=link-src clearfix])";
		Elements infos = htmlDoc.select(queryStr);

		fetchAlbumInfo(infos);         // 专辑信息
		fetchAuthorInfo(infos);        // 歌手信息
		
		fetchReleaseTime(infos);       // 发行时间
		fetchCompany(infos);           // 公司
		fetchTags(infos);              // 标签
	}

	public void fetchInfo() {
		if (htmlDoc == null) return;
		fetchBaseInfo();
		listLyricOfSong();
	}
	
	public static void main(String[] args) {
		String aSongUrl = "http://music.baidu.com/song/27672372";
		Song song = new Song();
		LyricCollector cLyric = new LyricCollector(aSongUrl, song);
		while (!(cLyric.initialHtmlDoc())) {
			LogUtils.LOGGER.warn("Retry initial song html document");
			System.out.println(cLyric.htmlDoc);
		}
		cLyric.listLyricOfSong();
		cLyric.fetchBaseInfo();
		System.out.println(song);
	}
	
}
