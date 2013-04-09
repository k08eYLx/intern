package nbeans;

import java.sql.ResultSet;

import utils.db.ConnectionPool;
import utils.db.SqlUtils;
import utils.file.PropertyUtils;
import app.Constants;

public abstract class Record {

	private static final String MYSQL      = "MySQL";
	public  static final int    ILLEGAL_ID = -1;

	// H2 as default
	private static String db = "H2";       // db
	
	protected static boolean useMySQL = false;
	
	protected static String insertSql = null;
	
	protected static final String SELECT_SQL = "SELECT %s FROM %s WHERE %s=%d";
	
	static {
		config();
	}
	
	protected int    id   = ILLEGAL_ID;    // -1 is illegal
	protected String name = "";
	protected long   fCnCharId = 0;
	protected long   sCnCharId = 0;
	protected long   tCnCharId = 0;
	
	public int getId() {
		return id;
	}
	public void setId(int id) {
		this.id = id;
	}
	
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}

	public long getfCnCharId() {
		return fCnCharId;
	}
	public void setfCnCharId(long fCnCharId) {
		this.fCnCharId = fCnCharId;
	}
	
	public long getsCnCharId() {
		return sCnCharId;
	}
	public void setsCnCharId(long sCnCharId) {
		this.sCnCharId = sCnCharId;
	}
	
	public long gettCnCharId() {
		return tCnCharId;
	}
	public void settCnCharId(long tCnCharId) {
		this.tCnCharId = tCnCharId;
	}
	
	public static void config() {
		PropertyUtils pus = new PropertyUtils(Constants.DB_CONFIG);
		
		db = pus.restoreStringField("db", db);
		useMySQL = db.equalsIgnoreCase(MYSQL);
		ConnectionPool.config(useMySQL);

		if (useMySQL) {
			// Insert on duplicate key update
			insertSql = "INSERT INTO %s VALUES (%s) ON DUPLICATE KEY UPDATE %s";
		}
		else {
			insertSql = "MERGE INTO %s VALUES (%s)";    // MERGE
		}
		
		SqlUtils.runScript(Constants.DB_SCRIPT);
	}
	
	public static boolean isUsingMySql() {
		return useMySQL;
	}
	
	public abstract String toString();
	
	public abstract boolean needUpdate();
	
	public abstract void saveToDB();
	public abstract void saveToExt();
	public abstract void fromRset(ResultSet rSet);
	
	/**
	 * 对输入字符串中的特殊字符进行转义
	 * 
	 * @param value
	 * @return
	 */
	protected String escape(String value) {
		// 移除字符串末尾的孤转义字符'\'
		if (value.contains("\\") && value.lastIndexOf('\\') == (value.length() - 1)) {
			if (value.length() < 2 || value.length() >= 2 && value.charAt(value.length() - 2) != '\\') {
				value = value.substring(0, value.length() - 1); 	
			}
		}

		// 为SQL语句，escape single quotes
		value = value.replaceAll("'", "''");
		
		// 对将转义字符'\'本身进行转义
		return value.replaceAll("\\\\", "\\\\");
	}
	
	/**
	 * DATE字段如果为空应该使用不带引号的null，
	 * 不为空时使用字符串形式可以自动转换为DATE
	 * 
	 * 将不合法的日期格式转化为合法的，
	 * 例如："2005-00-00" 到 "2005-01-01"
	 * 
	 * @param date
	 * @return
	 */
	public static String assureDate(String date) {
		// "2005-00-00" 对于H2来说是不合法的日期格式，但是MySQL不会报错
		// 将“-00”替换成“-01”，以便对于H2来说成为合法格式
		if (date != null) date = date.replaceAll("-0{2}", "-01");
		return (date == null || date.isEmpty()) ? "NULL" : ("'" + date + "'");
	}
	
	/**
	 * 判断记录是否合法
	 * 
	 * @return
	 */
	public boolean isLegal() {
		return (!(id == ILLEGAL_ID || name.isEmpty()));
	}
	
}
