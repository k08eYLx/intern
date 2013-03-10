package db;

import groovy.sql.Sql;
import org.h2.jdbcx.JdbcConnectionPool;
import org.h2.tools.Server;

import java.sql.Connection;
import java.sql.SQLException;

/**
 * Created with IntelliJ IDEA.
 * User: lihongzhen
 * Date: 13-1-16
 * Time: 下午2:08
 * To change this template use File | Settings | File Templates.
 */
public class H2ConnectionPool {

    // server mode
    private static final String DB_URL = "jdbc:h2:tcp://localhost/data-dev/Media_kb";
    private static final String DB_USER = "media";
    private static final String DB_PSWD = "kb";
    private static final String DB_DRIVER = "org.h2.Driver";

    private static JdbcConnectionPool cp = JdbcConnectionPool.create(DB_URL, DB_USER, DB_PSWD);

    public static Sql getSqlInstance() {
        Sql sqlInstance = null;
        try {
            sqlInstance = Sql.newInstance(DB_URL, DB_USER, DB_PSWD);
        }
        catch (SQLException sqle) {
            sqle.printStackTrace();
        }
        return sqlInstance;
    }

    public static Connection getConnection() {
        Connection connection = null;
        try {
            connection = cp.getConnection();
        } catch (SQLException sqle) {
            sqle.printStackTrace();
        }
        return connection;
    }

    public static void dispose() {
        cp.dispose();
    }

}
