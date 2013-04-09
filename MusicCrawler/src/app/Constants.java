package app;

public abstract class Constants {

	/**
	 * h2-1.3.171:
	 *  Improved compatibility with MySQL: 
	 *  	support for "ENGINE=InnoDB charset=UTF8" when creating a table.
	 */
	public static final String DB_SCRIPT       = "db.tables";
	
	public static final String MAIN_URL        = "http://music.baidu.com/artist/";
	public static final String ADDITIONAL_URLS = "additional.urls";
	public static final String DB_CONFIG       = "db.cfg";
	public static final String PROXY_FILE      = "proxy.list";
	public static final String IDS_FILE        = "tmp.ids";

	public static final int ILLEGAL_VALUE = -1;
	
	public static final int RETRY_TIMES   = 3;
	
	// 设置主界面用于输出信息的text area显示的最大行数
	public static final int MAX_LINE = 3000;
	
	public static final int SECOND = 1000;
	public static final int MINUTE = SECOND * 60;
	
	public static final String DEF_CHARSET = "UTF-8";
	
}
