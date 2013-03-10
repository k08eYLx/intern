package export.m2h;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.concurrent.CyclicBarrier;

import nbeans.Record;

import utils.db.SqlUtils;
import word.CnWord;

public class ExportAlbums extends ExportRecords {

	public ExportAlbums() { }
	
	public ExportAlbums(CyclicBarrier barrier) {
		super(barrier);
	}

	@Override
    public void action() throws SQLException {
        String mSelect = "SELECT * FROM ALBUMS ORDER BY album_id";
        Connection mConn = SqlUtils.getConnection();
        ResultSet rSet = mConn.createStatement().executeQuery(mSelect);
        System.out.println("Export albums... ===> " + mSelect);
	    while (rSet.next()) {
	    	int    albumId     = rSet.getInt("album_id");
	        String albumName   = rSet.getString("album_name");
	        int    singerId    = rSet.getInt("singer_id");
	        String singerName  = rSet.getString("singer_name");
	        String releaseTime = rSet.getString("release_time");
	        String styles      = rSet.getString("styles");
	        String company     = rSet.getString("company");
	        	
	        String msg = String.format("Export album ===> %s : %d ...", albumName, albumId);
	        System.out.println(msg);

	        CnWord cnWord = new CnWord(albumName);
	    	long fCnCharId = cnWord.getCharId(0);
	    	long sCnCharId = cnWord.getCharId(1);
	    	long tCnCharId = cnWord.getCharId(2);
	        
	        String format = "MERGE INTO ALBUMS_EXT VALUES(%d, '%s', %d, %d, %d, %d, '%s', %s, '%s', '%s')";
	        String hInsert = String.format(format, albumId, escape(albumName)
	        		, fCnCharId, sCnCharId, tCnCharId
	        		, singerId, escape(singerName), Record.assureDate(releaseTime)
	        		, escape(styles), escape(company));
	        Connection hConn = h2Cp.getConnection();
	       	hConn.createStatement().execute(hInsert);
	       	hConn.close();
	    }
	    SqlUtils.release(mConn);
	    System.out.println("Export albums finished!");
    }
	
}
