package nbeans;

import java.sql.ResultSet;
import java.sql.SQLException;

import utils.db.SqlUtils;

public class Song extends Record {

	public static final String TABLE      = "SONGS";
	private static final String EXT_TABLE = "SONGS_Ext";
	private static final String ID_COL    = "song_id";

	// 以hotNumber作为是否进行更新的依据
	private static final String HOT_NUMBER = "hot_number";
	
	// 歌曲ID
	// 歌曲名
	private int    albumId     = ILLEGAL_ID;    // 专辑ID
	private String albumName   = "";            // 专辑名
	private int    singerId    = ILLEGAL_ID;    // 歌手ID
	private String singerName  = "";            // 歌手名
	private int    hotNumber   = 0;             // 热度
	private String releaseTime = "";        // 发行时间
	private String company     = "";            // 公司
	private String tags        = "";            // 标签
	
	public int getAlbumId() {
		return albumId;
	}
	public void setAlbumId(int albumId) {
		this.albumId = albumId;
	}
	
	public String getAlbumName() {
		return albumName;
	}
	public void setAlbumName(String albumName) {
		this.albumName = albumName;
	}
	
	public int getSingerId() {
		return singerId;
	}
	public void setSingerId(int singerId) {
		this.singerId = singerId;
	}
	
	public String getSingerName() {
		return singerName;
	}
	public void setSingerName(String singerName) {
		this.singerName = singerName;
	}
	
	public int getHotNumber() {
		return hotNumber;
	}
	public void setHotNumber(int hotNumber) {
		this.hotNumber = hotNumber;
	}
	
	public String getReleaseTime() {
		return releaseTime;
	}
	public void setReleaseTime(String releaseTime) {
		this.releaseTime = releaseTime;
	}
	
	public String getCompany() {
		return company;
	}
	public void setCompany(String company) {
		this.company = company;
	}
	
	public String getTags() {
		return tags;
	}
	public void setTags(String tags) {
		this.tags = tags;
	}
	
	@Override
	public String toString() {
		return "歌曲 ID：\t" 	+ id 		  + "\n"
			 + "歌曲名：\t" 	+ name 	 	  + "\n"
			 + "专辑 ID：\t" 	+ albumId 	  + "\n"
			 + "专辑名：\t"	+ albumName   + "\n"
			 + "歌手 ID：\t" 	+ singerId    + "\n"
			 + "歌手名：\t" 	+ singerName  + "\n"
			 + "热度：\t" 	+ hotNumber   + "\n"
			 + "发行时间：\t" 	+ releaseTime + "\n"
			 + "公司：\t" 	+ company 	  + "\n"
			 + "标签：\t" 	+ tags;
	}

	/**
	 * 歌曲热度值变化才认为是需要更新的
	 * 
	 * @return
	 */
	@Override
	public boolean needUpdate() {
		String sql = String.format(SELECT_SQL, HOT_NUMBER, TABLE, ID_COL, id);
		return (hotNumber != SqlUtils.selectInt(sql));
	}
	
	@Override
	public void saveToDB() {
		String vFormat = "%d,'%s',%d,'%s',%d,'%s',%d,%s,'%s','%s'";
		String values = String.format(vFormat
				, id, escape(name), albumId, escape(albumName), singerId, escape(singerName)
				, hotNumber, assureDate(releaseTime), escape(company), tags);
		
		String uFormat = "song_name='%s',album_id=%d,album_name='%s'" 
				+ ",singer_id=%d,singer_name='%s',hot_number=%d" 
				+ ",release_time=%s,company='%s',tags='%s'";
		String update = String.format(uFormat
				, escape(name), albumId, escape(albumName), singerId, escape(singerName)
				, hotNumber, assureDate(releaseTime), company, tags);
		
		String sql = "";
		if (useMySQL) {
			sql = String.format(insertSql, TABLE, values, update);
		}
		else {
			sql = String.format(insertSql, TABLE, values);	
		}
		
		SqlUtils.executeUpdate(sql);
	}
	
	@Override
	public void saveToExt() {
		String format = "%d,'%s',%d,%d,%d,%d,'%s',%d,'%s',%d,%s,'%s','%s'";
		String values = String.format(format
				, id, escape(name), fCnCharId, sCnCharId, tCnCharId
				, albumId, escape(albumName), singerId, escape(singerName)
				, hotNumber, assureDate(releaseTime), escape(company), tags);
		

		String uFormat = "song_name='%s'f_cn_char_id=%d,s_cn_char_id=%d,t_cn_char_id=%d"
				+ ",album_id=%d,album_name='%s',singer_id=%d"
				+ ",singer_name='%s',hot_number=%d,release_time=%s,company='%s',tags='%s'";
		String update = String.format(uFormat
				, escape(name), fCnCharId, sCnCharId, tCnCharId
				, albumId, escape(albumName), singerId, escape(singerName)
				, hotNumber, assureDate(releaseTime), company, tags);
		
		String sql = "";
		if (useMySQL) {
			sql = String.format(insertSql, EXT_TABLE, values, update);
		}
		else {
			sql = String.format(insertSql, EXT_TABLE, values);
		}
		
		SqlUtils.executeUpdate(sql);
	}

	@Override
	public void fromRset(ResultSet rSet) {
		try {
			setId(rSet.getInt(1));
			setName(rSet.getString(2));
			setAlbumId(rSet.getInt(3));
			setAlbumName(rSet.getString(4));
			setSingerId(rSet.getInt(5));
			setSingerName(rSet.getString(6));
			setHotNumber(rSet.getInt(7));
			setReleaseTime(rSet.getString(8));
			setCompany(rSet.getString(9));
			setTags(rSet.getString(10));
		}
		catch (SQLException sqle) {
			sqle.printStackTrace();
		}
	}
	
}
