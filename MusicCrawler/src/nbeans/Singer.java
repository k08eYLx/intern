package nbeans;

import java.sql.ResultSet;
import java.sql.SQLException;

import utils.db.SqlUtils;

public class Singer extends Record {

	private static final String TABLE     = "SINGERS";
	private static final String EXT_TABLE = "SINGERS_Ext";
	private static final String ID_COL    = "singer_id";

	// 以listeners作为是否进行更新的依据
	private static final String LISTENERS = "listeners";
	
	// 歌手ID
	// 歌手名
	private String type      = "";   // 类别
	private int    listeners = 0;    // 听众数
	private String region    = "";   // 地区
	private String birthday  = "";   // 生日星座
	
	// 不存入数据库的字段
	private int numOfSongs  = 0;
	private int numOfAlbums = 0;
	
	public String getType() {
		return type;
	}
	public void setType(String type) {
		this.type = type;
	}
	
	public int getListeners() {
		return listeners;
	}
	public void setListeners(int listeners) {
		this.listeners = listeners;
	}
		
	public String getRegion() {
		return region;
	}
	public void setRegion(String region) {
		this.region = region;
	}
	
	public String getBirthday() {
		return birthday;
	}
	public void setBirthday(String birthday) {
		this.birthday = birthday;
	}

	public int getNumOfSongs() {
		return numOfSongs;
	}
	public void setNumOfSongs(int numOfSongs) {
		this.numOfSongs = numOfSongs;
	}
	
	public int getNumOfAlbums() {
		return numOfAlbums;
	}
	public void setNumOfAlbums(int numOfAlbums) {
		this.numOfAlbums = numOfAlbums;
	}

	@Override
	public String toString() {
		return "歌手 ID：\t"   	    + id       	+ "\n"
			 + "歌手名：\t" 		+ name     	+ "\n"
			 + "歌手类别：\t"		+ type	   	+ "\n"
			 + "听众数：\t"		    + listeners + "\n"
			 + "地区：\t"      	+ region   	+ "\n"
			 + "生日(星座)：\t" 	+ birthday;
	}
	
	@Override
	public void saveToDB() {
		if (type.isEmpty()) {
			System.err.print("type is empty!");
		}
		
		String vFormat = "%d,'%s','%s',%d,'%s','%s'";
		String values = String.format(vFormat
				, id, escape(name), type, listeners, region, birthday);
		
		String uFormat = "singer_name='%s',type='%s',listeners=%d,region='%s',birthday='%s'";
		String update = String.format(uFormat
				, escape(name), type, listeners, region, birthday);
		
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
		if (type.isEmpty()) {
			System.err.print("type is empty!");
		}
		
		String vFormat = "%d,'%s',%d,%d,%d,'%s',%d,'%s','%s'";
		String values = String.format(vFormat
				, id, escape(name)
				, fCnCharId, sCnCharId, tCnCharId
				, type, listeners, region, birthday);
		
		String uFormat = "singer_name='%s',f_cn_char_id=%d,s_cn_char_id=%d,t_cn_char_id=%d,type='%s',listeners=%d,region='%s',birthday='%s'";
		String update = String.format(uFormat
				, escape(name), fCnCharId, sCnCharId, tCnCharId
				, type, listeners, region, birthday);
		
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
			setType(rSet.getString(3));
			setListeners(rSet.getInt(4));
			setRegion(rSet.getString(5));
			setBirthday(rSet.getString(6));
		}
		catch (SQLException sqle) {
			sqle.printStackTrace();
		}
	}

	/**
	 * 听众数变化才更新
	 * 
	 * @return
	 */
	@Override
	public boolean needUpdate() {
		String sql = String.format(SELECT_SQL, LISTENERS, TABLE, ID_COL, id);
		return listeners != SqlUtils.selectInt(sql);
	}
	
}
