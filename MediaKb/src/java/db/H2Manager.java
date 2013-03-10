package db;

import groovy.sql.Sql;
import org.h2.tools.Server;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;

/**
 * Created with IntelliJ IDEA.
 * User: lihongzhen
 * Date: 13-1-16
 * Time: 下午2:16
 * To change this template use File | Settings | File Templates.
 */
public class H2Manager {

    private static Server h2Server = null;
    static {
        startServer();
    }

    public static void startServer() {
        try {
            h2Server = Server.createTcpServer();
            h2Server.start();
        }
        catch (SQLException sqle) {
            sqle.printStackTrace();
        }
    }

    public static void stopServer() {
        h2Server.stop();
    }

    public static Sql getSqlInstance() {
        return H2ConnectionPool.getSqlInstance();
    }

    public static Connection getConnection() {
        return H2ConnectionPool.getConnection();
    }

    public void showTables() {
        String sql = "show tables";
        Connection conn = getConnection();
        try {
            ResultSet resultSet = conn.createStatement().executeQuery(sql);
            while (resultSet.next()) {
                System.out.println(resultSet.getString(1));
            }
            resultSet.close();
            conn.close();
        } catch (SQLException sqle) {
            sqle.printStackTrace();
        }
    }

    public static void main(String[] args) {
        (new H2Manager()).showTables();
    }

}
