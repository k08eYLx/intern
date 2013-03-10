package crawler.proxy.sites;

import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;
import org.jsoup.nodes.Element;
import org.jsoup.select.Elements;

/**
 * http://www.cnproxy.com/proxy1.html
 *
 * @author xiangzi
 *
 */

public class CnProxy extends ProxySite {
	
	private static final String BASE_URL   = "http://www.cnproxy.com/";
	private static final String PROXY_URL = BASE_URL + "proxy1.html";

	private static final String START_MARKER = "<script type=\"text/javascript\">document.write(\":\"+";
	private static final String END_MARKER   = "</script></td>";
	
	private static final Map<String, String> v2i = new HashMap<String, String>();
	static {
		v2i.put("d", "0");
		v2i.put("c", "1");
		v2i.put("k", "2");
		v2i.put("z", "3");
		v2i.put("m", "4");
		v2i.put("b", "5");
		v2i.put("w", "6");
		v2i.put("i", "7");
		v2i.put("r", "8");
		v2i.put("l", "9");
	}
	
	@Override
	public void listProxy(ArrayList<String> list) {
		String url = PROXY_URL;
		try {
			Document htmlDoc = Jsoup.connect(url).timeout(0).get();
			Element pageNavEl = htmlDoc.select("div.proxylistnav").first();
			Elements pageEls = pageNavEl.select("ul>li>a[href]");
			for (Element pageEl : pageEls) {
				String pageUrl = pageEl.attr("abs:href");
				listProxyInPage(pageUrl, list);
			}
		}
		catch (IOException ioe) {
			System.out.println(ioe.getMessage() + " ===> " + url);
		}
	}
	
	private void listProxyInPage(String url, ArrayList<String> list) {
		try {
			System.out.println(url);
			Document htmlDoc = Jsoup.connect(url).timeout(0).get();
			Element proxyListTbEl = htmlDoc.select("div#proxylisttb>table").last();
			Elements proxyListItemEls = proxyListTbEl.select("tr");
			proxyListItemEls.remove(0);
			for (Element proxyListItemEl : proxyListItemEls) {
				Element proxyItemEl = proxyListItemEl.select("td").first();
				String addrStr = proxyItemEl.text() + ":" + parsePort(proxyItemEl);
				System.out.println(addrStr);
				if (checkProxy(addrStr)) {
					list.add(addrStr);
				}
			}
		}
		catch (IOException ioe) {
			ioe.printStackTrace();
		}
	}
	
	private String parsePort(Element proxyItemEl) {
		String itemStr = proxyItemEl.toString();
		int sPos = itemStr.indexOf(START_MARKER) + START_MARKER.length();
		int ePos = itemStr.indexOf(END_MARKER) - 1;
		String portVarStr = itemStr.substring(sPos, ePos);
		String[] bits = portVarStr.split("\\+");
		String portStr = "";
		for (String bit : bits) {
			portStr += v2i.get(bit.trim());
		}
		return portStr;
	}
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		(new CnProxy()).listProxy();
	}

}
