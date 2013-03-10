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

public class SiteDigger extends ProxySite {

	private static final String BASE_URL  = "http://www.site-digger.com/";
	private static final String PROXY_URL = BASE_URL + "html/articles/20110516/proxieslist.html";

	@Override
	public void listProxy(ArrayList<String> list) {
		String url = PROXY_URL;
		try {
			Document htmlDoc = Jsoup.connect(url).timeout(0).get();
			Elements tableRowElts = htmlDoc.select("table#proxies_table>tbody>tr");
			if (tableRowElts != null) {
				parseProxy(tableRowElts, list);
			}
		}
		catch (IOException ioe) {
			System.out.println(ioe.getMessage() + " ===> " + url);
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
		(new SiteDigger()).listProxy();
	}

}
