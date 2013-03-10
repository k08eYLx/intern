package crawler.proxy.sites;

import java.io.IOException;
import java.util.ArrayList;

import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;
import org.jsoup.nodes.Element;
import org.jsoup.select.Elements;

/**
 * http://www.site-digger.com/html/articles/20110516/proxieslist.html
 *
 * @author xiangzi
 *
 */

public class GetProxyCn extends ProxySite {

	private static final String BASE_URL  = "http://www.getproxy.jp/cn/";
	private static final String[] PROXY_URLS = { 
											BASE_URL
										  , BASE_URL + "fastest"
										  ,	BASE_URL + "china"
										  , BASE_URL + "hongkong" };

	@Override
	public void listProxy(ArrayList<String> list) {
		try {
			for (String proxyUrl : PROXY_URLS) {
				System.out.println(proxyUrl);
				Document htmlDoc = Jsoup.connect(proxyUrl).timeout(0).get();
				Elements tableRowElts = htmlDoc.select("table#mytable>tbody>tr");
				tableRowElts.remove(0);
				if (tableRowElts != null) {
					parseProxy(tableRowElts, list);
				}	
			}
			System.out.println(list.size());
		}
		catch (IOException ioe) {
			System.out.println(ioe.getMessage() + " ===> " + BASE_URL);
		}
	}
	
	private void parseProxy(Elements tableRowElts, ArrayList<String> list) {
		for (Element tableRowElt : tableRowElts) {
			Element proxyElt = tableRowElt.select("td").first();
			if (proxyElt != null) {
				String addrStr = proxyElt.text().trim();
				if (checkProxy(addrStr)) {
					System.out.println(addrStr);
					list.add(addrStr);
				}	
			}
		}
	}
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		(new GetProxyCn()).listProxy();
	}

}
