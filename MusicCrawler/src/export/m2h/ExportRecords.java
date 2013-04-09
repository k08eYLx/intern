package export.m2h;

import java.io.IOException;
import java.sql.Connection;
import java.sql.SQLException;
import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;

import org.apache.ibatis.io.Resources;
import org.apache.ibatis.jdbc.ScriptRunner;
import org.h2.jdbcx.JdbcConnectionPool;

import utils.db.ConnectionPool;
import utils.db.SqlUtils;
import utils.file.PropertyUtils;
import app.Constants;

/**
 * Created with IntelliJ IDEA.
 * User: YangLX
 * Date: 13-1-15
 * Time: 下午5:28
 * To change this template use File | Settings | File Templates.
 */

/**
 * 从MySQL导出到H2
 * 
 * 计算id并转存到（MERGE）h2数据库里的EXTs表
 * 
 * @author YangLX
 *
 */
public abstract class ExportRecords implements Runnable {

    private static final String H2_URL = "jdbc:h2:tcp://localhost/data/Media_kb";
    protected static final JdbcConnectionPool h2Cp = JdbcConnectionPool.create(H2_URL, "media", "kb");
	
	private static final String MYSQL_DB    = "MySQL";
	static {
		configDatabase();
	}
	
    private CyclicBarrier barrier = null;
	
    public ExportRecords() { }
    
	public ExportRecords(CyclicBarrier barrier) {
		this.barrier = barrier;
	}

	public static void configDatabase() {
		PropertyUtils pus = new PropertyUtils(Constants.DB_CONFIG);
		
		String db = pus.restoreStringField("db", "");
		boolean useMySQL = db.equalsIgnoreCase(MYSQL_DB);
		ConnectionPool.config(useMySQL);

		runScript(Constants.DB_SCRIPT);
	}
  
	private static void runScript(String path) {
		try {
			Connection connection = h2Cp.getConnection();
			ScriptRunner runner = new ScriptRunner(connection);
			runner.runScript(Resources.getResourceAsReader(path));
			connection.close();
		}catch (IOException ioe) {
			ioe.printStackTrace();
		}
		catch (SQLException sqle) {
			sqle.printStackTrace();
		}
	}
	
	public int getCount(String table) {
		return SqlUtils.getCount(table);
	}

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
	
    protected abstract void action() throws SQLException;
    
    public void export() {
        try {
        	action();
        }
		catch (SQLException sqle) {
			sqle.printStackTrace();
		}
    }

	@Override
	public void run() {
		export();
		if (barrier != null) {
			try {
				barrier.await();
			}
			catch (InterruptedException ie) {
				ie.printStackTrace();
			}
			catch (BrokenBarrierException bbe) {
				bbe.printStackTrace();
			}
		}
	}

}
