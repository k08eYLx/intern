package crawler.proxy.sites;

import java.io.IOException;
import java.util.ArrayList;

import org.apache.http.HttpHost;
import org.apache.http.HttpResponse;
import org.apache.http.HttpStatus;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.conn.params.ConnRoutePNames;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.params.CoreConnectionPNames;
import org.apache.http.params.CoreProtocolPNames;
import org.apache.http.params.HttpParams;

import utils.file.FileUtils;

import app.Constants;

public abstract class ProxySite {

	private static final String IP_REGEX = "(\\d{1,3}\\.){3}\\d{1,3}:\\d{2,5}";

	public abstract void listProxy(ArrayList<String> list);

	public void listProxy() {
		ArrayList<String> list = new ArrayList<String>();
		listProxy(list);
		StringBuffer sBuffer = new StringBuffer();
		for (String li : list) {
			sBuffer.append(li + "\r\n");
		}
		FileUtils.append(sBuffer.toString(), Constants.PROXY_FILE);
	}

	public static boolean checkProxy(String addrStr) {
		if (addrStr.matches(IP_REGEX)) {
			String[] strs = addrStr.split(":");
			
			String ipStr = strs[0];
			String portStr = strs[1];
			
			return checkProxy(ipStr, portStr);	
		}
		return false;
	}
	
	protected static boolean checkProxy(String ipStr, String portStr) {
		int port = new Integer(portStr);
		return checkProxy(ipStr, port);
	}
	
	/* 取得请求内容  
    HttpEntity entity = response.getEntity();
    // 显示内容  
    if (entity != null) {
        System.out.println(EntityUtils.toString(entity, "UTF-8"));
    }//*/
	/**
	 * 过滤出可用的HTTP代理
	 * 
	 * @param ipStr
	 * @param port
	 * @return
	 */
	protected static boolean checkProxy(String ipStr, int port) {
		try {
			HttpClient httpClient = new DefaultHttpClient();

			// 对HttpClient对象设置代理
			HttpHost proxyHost = new HttpHost(ipStr, port);
			httpClient.getParams().setParameter(ConnRoutePNames.DEFAULT_PROXY, proxyHost);
			
			HttpParams params = httpClient.getParams();
			
			// 设置连接超时和传输超时，单位是毫秒
			
			params.setParameter(CoreConnectionPNames.CONNECTION_TIMEOUT, Constants.SECOND);
			params.setParameter(CoreConnectionPNames.SO_TIMEOUT, Constants.SECOND);
			
			// 设置字符编码
			params.setParameter(CoreProtocolPNames.HTTP_CONTENT_CHARSET, Constants.DEF_CHARSET);
			
			// 如果不是具体地址不要忘记以'/'结尾
			HttpGet httpGet = new HttpGet(Constants.MAIN_URL);
			HttpResponse response = httpClient.execute(httpGet);

			return (HttpStatus.SC_OK == response.getStatusLine().getStatusCode());
		}
		catch (ClientProtocolException cpe) {
			System.out.println("Bad proxy ===> " + cpe.getMessage());
		}
		catch (IOException ioe) {
			System.out.println("Bad proxy ===> " + ioe.getMessage());
		}
		
		return false;
	}
	
}
