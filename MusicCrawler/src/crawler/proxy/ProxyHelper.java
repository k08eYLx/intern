package crawler.proxy;

import java.util.ArrayList;
import java.util.List;

import crawler.proxy.sites.CnProxy;
import crawler.proxy.sites.GetProxyCn;
import crawler.proxy.sites.Itmop;
import crawler.proxy.sites.ProxySite;
import crawler.proxy.sites.SiteDigger;

import utils.file.FileUtils;

public class ProxyHelper {
	
	public static final String PROXY_FILE = "proxy.list";
	
	public static void empty() {
		FileUtils.write("", PROXY_FILE);
	}
	
	public static boolean saveProxy(String addrStr) {
		if (ProxySite.checkProxy(addrStr)) {
			FileUtils.append(addrStr + "\r\n", PROXY_FILE);
			return true;
		}
		return false;
	}

	/**
	 * 从网络获取可用代理
	 */
	public static void fetchAvailableProxy() {
		List<ProxySite> proxySites = new ArrayList<ProxySite>();
		proxySites.add(new CnProxy());
		proxySites.add(new GetProxyCn());
		proxySites.add(new Itmop());
		proxySites.add(new SiteDigger());
		for (ProxySite proxySite : proxySites) {
			proxySite.listProxy();	
		}
	}
	
	/**
	 * 从网络获取可用代理
	 */
	public static void fetchAvailableProxy(ArrayList<String> list) {
		List<ProxySite> proxySites = new ArrayList<ProxySite>();
		proxySites.add(new CnProxy());
		proxySites.add(new GetProxyCn());
		proxySites.add(new Itmop());
		proxySites.add(new SiteDigger());
		for (ProxySite proxySite : proxySites) {
			proxySite.listProxy(list);	
		}
	}
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		ProxyHelper.fetchAvailableProxy();
	}

}
