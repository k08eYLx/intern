package crawler.proxy.sites;

import java.io.IOException;
import java.util.ArrayList;

import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;
import org.jsoup.select.Elements;

/**
 * http://www.itmop.com/proxy/post/1166.html
 * 
 * @author xiangzi
 *
 */
public class Itmop extends ProxySite {

	private static final String PROXY_URL = "http://www.itmop.com/proxy/post/1166.html";

	@Override
	public void listProxy(ArrayList<String> list) {
		String url = PROXY_URL;
		try {
			System.out.println(url);
			Document htmlDoc = Jsoup.connect(url).timeout(0).get();
			Elements proxyItemElts = htmlDoc.select("div[class=NEW_CONTENT LEFT]>p");
			String[] proxyItems = proxyItemElts.toString().replaceAll("<p>|</p>", "").split("<br />");
			for (String proxyItem : proxyItems) {
				proxyItem = proxyItem.split("HTTPS?")[0].trim().replaceAll(" ", ":");
				if (checkProxy(proxyItem)) {
					System.out.println(proxyItem);
					list.add(proxyItem);
				}
			}
		}
		catch (IOException ioe) {
			System.out.println(ioe.getMessage() + " ===> " + url);
		}
	}
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		(new Itmop()).listProxy();
	}

}
