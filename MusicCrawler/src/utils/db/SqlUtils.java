package utils.db;

import java.io.IOException;
import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import org.apache.ibatis.io.Resources;
import org.apache.ibatis.jdbc.ScriptRunner;

import app.Constants;

public final class SqlUtils {

	private static final ConnectionPool cp = ConnectionPool.getInstance();

	private SqlUtils() { }

	public static Connection getConnection() {
		Connection connection = null;
		if (cp != null) {
			connection = cp.getConnection();
		}
		return connection;
	}
	
	private static void close(ResultSet rSet) {
		if (rSet != null) {
			try {
				rSet.close();
			}
			catch (SQLException sqle) {
				sqle.printStackTrace();
			}
		}
	}
	
	private static void close(Statement stmt) {
		if (stmt != null) {
			try {
				stmt.close();
			}
			catch (SQLException sqle) {
				sqle.printStackTrace();
			}
		}
	}
	
	public static void release(Connection connection) {
		if (connection != null) {
			cp.release(connection);
		}
	}
	
	public static void runScript(String path) {
		try {
			Connection connection = getConnection();
			ScriptRunner runner = new ScriptRunner(connection);
			runner.runScript(Resources.getResourceAsReader(path));
			release(connection);
		}catch (IOException ioe) {
			ioe.printStackTrace();
		}
	}
	
	/**
	 * INSERT, UPDATE, DELETE
	 * 
	 * @param sql
	 * @return
	 */
	public static void executeUpdate(String sql) {
		Connection connection = getConnection();
		if (connection != null) {
			Statement stmt = null;
			try {
				stmt = connection.createStatement();
				stmt.executeUpdate(sql);
			}
			catch (SQLException sqle) {
				//String msg = "Lock wait timeout exceeded; try restarting transaction";
				sqle.printStackTrace();
			}
			finally {
				close(stmt);
				release(connection);
			}
		}
	}
	
	private static boolean isExist(String sql) {
		Connection connection = getConnection();
		if (connection != null) {
			Statement stmt = null;
			ResultSet rSet = null;
			try {
				stmt = connection.createStatement();
				rSet = stmt.executeQuery(sql);
				if (rSet.next()) {
					return (rSet.getInt(1) > 0);
				}
			}
			catch (SQLException sqle) {
				sqle.printStackTrace();
			}
			finally {
				close(rSet);
				close(stmt);
				release(connection);
			}
		}
		
		return true;    // 查询失败也认为已存在，避免存的时候发生冲突
	}
	
	public static boolean isExist(String table, String sCol, String wCol, int value) {
		String format = "SELECT COUNT(%s) FROM %s WHERE %s=%d";
		String sql = String.format(format, sCol, table, wCol, value);
		return isExist(sql);
	}
	
	public static boolean isExist(String table, String column, int value) {
		return isExist(table, column, column, value);
	}
	
	public static int selectInt(String sql) {
		int value = Constants.ILLEGAL_VALUE;
		Connection connection = getConnection();
		if (connection != null) {
			Statement stmt = null;
			ResultSet rSet = null;
			try {
				stmt = connection.createStatement();
				rSet = stmt.executeQuery(sql);
				if (rSet.next()) {
					value = rSet.getInt(1);
				}
			}
			catch (SQLException sqle) {
				sqle.printStackTrace();
				System.out.println(sql);
			}
			finally {
				close(rSet);
				close(stmt);
				release(connection);
			}
		}
		return value;
	}

	private static int getRecordCount(String sql) {
		int count = 0;
		Connection connection = getConnection();
		if (connection != null) {
			Statement stmt = null;
			ResultSet rSet = null;
			try {
				stmt = connection.createStatement();
				rSet = stmt.executeQuery(sql);
				if (rSet.next()) {
					count = rSet.getInt(1);
				}
			}
			catch (SQLException sqle) {
				sqle.printStackTrace();
			}
			finally {
				close(rSet);
				close(stmt);
				release(connection);
			}
		}
		return count;
	}
	
	public static int getRecordCount(String table, String sCol, String wCol, int value) {
		String format = "SELECT COUNT(%s) FROM %s WHERE %s=%d";
		String sql = String.format(format, sCol, table, wCol, value);
		return getRecordCount(sql);
	}
	
	public static int getRecordCount(String table, String column, int value) {
		return getRecordCount(table, column, column, value);
	}
	
	public static int getCount(String table) {
		String format = "SELECT COUNT(*) FROM %s";
		String sql = String.format(format, table);
		return getRecordCount(sql);
	}
	
	public static int getCount(String table, String wClause) {
		String format = "SELECT COUNT(*) from %s where (%s)";
		String sql = String.format(format, table, wClause);
		return getRecordCount(sql);
	}
	
	public static ResultSet select(String sql) {
		ResultSet rSet = null;
		Connection connection = getConnection();
		if (connection != null) {
			Statement stmt = null;
			try {
				stmt = connection.createStatement();
				rSet = stmt.executeQuery(sql);
			}
			catch (SQLException sqle) {
				sqle.printStackTrace();
				System.out.println(sql);
			}
			finally {
				release(connection);
			}
		}
		return rSet;
	}
	
	public static void showTables() throws SQLException {
		Connection connection = cp.getConnection();
		if (connection != null) {
			Statement stmt = connection.createStatement();
			ResultSet rSet = stmt.executeQuery("show tables");
			while (rSet.next()) {
				System.out.println(rSet.getString(1));
			}
			stmt.close();
			cp.release(connection);
		}
	}
	
	public static void main(String[] args) throws SQLException {
		// script路径从src目录开始
		SqlUtils.runScript("create.tables");		
	}
	
}
