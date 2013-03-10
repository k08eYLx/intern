package export.m2h;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.concurrent.CyclicBarrier;

import nbeans.Record;

import utils.db.SqlUtils;
import word.CnWord;

public class ExportSongs extends ExportRecords {
	
	public ExportSongs() { }

	public ExportSongs(CyclicBarrier barrier) {
		super(barrier);
	}

	@Override
    public void action() throws SQLException {
		/*int count = getCount("Songs");
		int startId = 0;
		int step = 10000;
		for (int i = 0; i < count; i += step) {
			String format = "SELECT * FROM Songs WHERE (song_id > %d) limit %d";
			String select = String.format(format, startId, step);
			System.out.println(select);
			ResultSet rSet = null;
			try {
				while (rSet.next()) {
					startId = 0;
				}
			}
			catch (SQLException sqle) {
				sqle.printStackTrace();
			}
		}*/

		String mSelect = "SELECT * FROM Songs ORDER BY song_id";
        Connection mConn = SqlUtils.getConnection();
        ResultSet rSet = mConn.createStatement().executeQuery(mSelect);
        System.out.println("Export songs... ===> " + mSelect);
	    while (rSet.next()) {
	        int songId = rSet.getInt("song_id");
	        String songName    = rSet.getString("song_name");
	        int    albumId     = rSet.getInt   ("album_id");
	        String albumName   = rSet.getString("album_name");
	        int    singerId    = rSet.getInt   ("singer_id");
	        String singerName  = rSet.getString("singer_name");
	        int    hotNumber   = rSet.getInt   ("hot_number");
	        String releaseTime = rSet.getString("release_time");
	        String company     = rSet.getString("company");
	        String tags        = rSet.getString("tags");
	        	
	        String msg = String.format("Export song ===> %s : %d ...", songName, songId);
	        System.out.println(msg);

	        CnWord cnWord = new CnWord(songName);
	    	long fCnCharId = cnWord.getCharId(0);
	    	long sCnCharId = cnWord.getCharId(1);
	    	long tCnCharId = cnWord.getCharId(2);
	        
	        String format = "MERGE INTO SONGS_EXT VALUES(%d, '%s', %d, %d, %d, %d, '%s', %d, '%s', %d, %s, '%s', '%s')";
	        String hInsert = String.format(format, songId, escape(songName)
	        		, fCnCharId, sCnCharId, tCnCharId
	        		, albumId, escape(albumName), singerId, escape(singerName)
	        		, hotNumber, Record.assureDate(releaseTime), escape(company), escape(tags));
	        Connection hConn = h2Cp.getConnection();
	       	hConn.createStatement().execute(hInsert);
	       	hConn.close();
	    }
	    SqlUtils.release(mConn);
	    System.out.println("Export songs finished!");
    }

}
