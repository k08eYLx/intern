package utils.db;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.util.ArrayList;

import org.h2.jdbcx.JdbcConnectionPool;
import org.h2.tools.Server;

import utils.file.PropertyUtils;
import app.Constants;

public class ConnectionPool {

	// For MySQL
    // a proper number no more than db's max_connections system variable.
	private static final int MAX_NUM  = 70;
	private static final int MIN_NUM  = 10;    // 最好不要小于子线程数目
	private static final int STEP_NUM = 10;

	private static boolean useMySQL = false;
	
	/*private static final String H2_DRIVER     = "org.h2.Driver";
	private static final String MYSQL_DRIVER  = "com.mysql.jdbc.Driver";*/
	
	private static final String H2_URL_FORMAT    = "jdbc:h2:tcp://%s/data/%s;DB_CLOSE_ON_EXIT=FALSE";
	private static final String MYSQL_URL_FORMAT = "jdbc:mysql://%s:3306/%s?autoReconnect=true";
	private static String urlFormat = "";
	
	// jdbc:h2:tcp://localhost/~/MusicCrawler
	// jdbc:mysql://localhost:3306/MusicCrawler
	private static String url      = null;
	
	private static String host     = "localhost";     // host
	private static String database = "MusicCrawler";  // database
	private static String user     = "crawler";       // user
	private static String password = "music";         // password
	
	private static ArrayList<Connection> pool = new ArrayList<Connection>(MAX_NUM);
	private static JdbcConnectionPool cp = null;
	
	private static ConnectionPool instance = new ConnectionPool();
	
	private ConnectionPool() { }
	
	public static ConnectionPool getInstance() {
		if (useMySQL) {
			if (pool.size() == 0) initial();
		}
		else {
			cp = JdbcConnectionPool.create(url, user, password);
		}
		return instance;
	}
	
	public static void config(boolean flag) {
		useMySQL = flag;
		
		PropertyUtils pus = new PropertyUtils(Constants.DB_CONFIG);
		
		host = pus.restoreStringField("host", host);
		database = pus.restoreStringField("database", database);
		
		if (useMySQL) {
			urlFormat = MYSQL_URL_FORMAT;
		}
		else {
			urlFormat = H2_URL_FORMAT;
			try {
				// 使用H2 TCP server mode
				Server.createTcpServer().start();
			}
			catch (SQLException sqle) {
				sqle.printStackTrace();
			}
		}
		url = String.format(urlFormat, host, database);
		
		user = pus.restoreStringField("user", user);
		password = pus.restoreStringField("password", password);
	}

	// 这里建立所有的数据库连接;
	private static void initial() {
		for (int i = 0; i < MAX_NUM; i++) {
			pool.add(createConnection());
		}
	}

	private static Connection createConnection() {
		Connection connection = null;

		try {
			connection = DriverManager.getConnection(url, user, password);
		}
		catch (SQLException sqle) {
			sqle.printStackTrace();
		}
		
		return connection;
	}
	
	private void enlarge() {
		for (int i = 0; i < STEP_NUM; i++) {
			Connection connection = createConnection();
			if (connection != null) {
				pool.add(connection);
			}
		}
	}
	
	private void shrink() {
		for (int i = 0; i < STEP_NUM; i++) {
			pool.remove(0);
		}
	}
	
	// 从连接池中获取一个可用的连接
	public Connection getConnection() {
		Connection connection = null;
		if (useMySQL) {
			synchronized (pool) {
				if (pool.size() < MIN_NUM) enlarge();
				else if (pool.size() > MAX_NUM) shrink();
		
				if (pool != null) connection = pool.remove(0);
			}
		}
		else {
			try {
				connection = cp.getConnection();
			}
			catch (SQLException sqle) {
				sqle.printStackTrace();
			}
		}

		return connection;
	}

	// 将使用完毕的连接放回到连接池当中
	public void release(Connection connection) {
		if (useMySQL) {
			pool.add(connection);	
		}
		else {
			try {
				connection.close();
			}
			catch (SQLException sqle) {
				sqle.printStackTrace();
			}
		}
	}

}
