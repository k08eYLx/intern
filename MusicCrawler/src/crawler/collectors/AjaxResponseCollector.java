package crawler.collectors;

import java.io.IOException;
import java.net.Proxy;

import nbeans.Singer;

import org.json.JSONException;
import org.json.JSONObject;
import org.jsoup.Jsoup;

import utils.LogUtils;

public abstract class AjaxResponseCollector extends InfosCollector {
	
	protected Singer singer = null;
	
	/**
	 * @param singer
	 */
	public AjaxResponseCollector(Singer singer) {
		this.singer = singer;
	}

	protected JSONObject newJsonObj(String ajaxUrl) {
		return newJsonObj(ajaxUrl, true);
	}

	/**
	 * 通过JSoup请求URL，并使用返回的字符串构造JSON对象， 如果字符串不是合法的JSON格式，则返回null
	 * 
	 * @param ajaxUrl
	 * @return
	 */
	protected JSONObject newJsonObj(String ajaxUrl, boolean useProxy) {
		JSONObject jsonObj = null;
		try {
			slowdown();
			Proxy localProxy = null;
			if (useProxy) localProxy = proxy;
			String jsonStr = Jsoup.connect(ajaxUrl, localProxy).get().text();
			jsonObj = new JSONObject(jsonStr);
		}
		catch (JSONException jsone) {
			LogUtils.LOGGER.warn("Malformed JSON data from URL: " + ajaxUrl);
			// LogUtils.EXCEPTION_LOGGER.debug(LogUtils.getTrace(jsone));
		}
		catch (IOException ioe) {
			LogUtils.LOGGER.warn(ioe.getMessage() + " ===> " + ajaxUrl);
			// LogUtils.EXCEPTION_LOGGER.debug(LogUtils.getTrace(ioe));
		}
		return jsonObj;
	}

}
