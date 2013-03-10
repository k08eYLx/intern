package export.m2h;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.concurrent.CyclicBarrier;

import utils.db.SqlUtils;
import word.CnWord;

public class ExportSingers extends ExportRecords {

	public ExportSingers() { }
	
	public ExportSingers(CyclicBarrier barrier) {
		super(barrier);
	}

	@Override
    protected void action() throws SQLException {
        String mSelect = "SELECT * FROM Singers ORDER BY singer_id";
        Connection mConn = SqlUtils.getConnection();
        ResultSet rSet = mConn.createStatement().executeQuery(mSelect);
        System.out.println("Export singers... ===> " + mSelect);
	    while (rSet.next()) {
	    	int    singerId   = rSet.getInt("singer_id");
	        String singerName = rSet.getString("singer_name");
	        String type       = rSet.getString("type");
	        int    listeners  = rSet.getInt("listeners");
	        String region     = rSet.getString("region");
	        String birthday   = rSet.getString("birthday");
	        	
	        String msg = String.format("Export singer ===> %s : %d ...", singerName, singerId);
	        System.out.println(msg);
	        
	        CnWord cnWord = new CnWord(singerName);
	    	long fCnCharId = cnWord.getCharId(0);
	    	long sCnCharId = cnWord.getCharId(1);
	    	long tCnCharId = cnWord.getCharId(2);
	        
	        String format = "MERGE INTO SINGERS_EXT VALUES(%d, '%s', %d, %d, %d, '%s', %d, '%s', '%s')";
	        String hInsert = String.format(format, singerId, escape(singerName)
	        		, fCnCharId, sCnCharId, tCnCharId
	        		, type, listeners, region, birthday);
	        Connection hConn = h2Cp.getConnection();
	       	hConn.createStatement().execute(hInsert);
	       	hConn.close();
	    }
	    SqlUtils.release(mConn);
	    System.out.println("Export singers finished!");
    }

}
