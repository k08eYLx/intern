package nbeans;

import java.sql.ResultSet;
import java.sql.SQLException;

import utils.db.SqlUtils;

public class Album extends Record {

	public static final String TABLE       = "ALBUMS";
	private static final String EXT_TABLE  = "ALBUMS_Ext";
	//private static final String ID_COL     = "album_id";
	
	// 专辑ID
	// 专辑名
	private int    singerId    = ILLEGAL_ID;    // 歌手ID
	private String singerName  = "";            // 歌手名
	private String releaseTime = "";            // 发行时间
	private String styles      = "";            // 风格
	private String company     = "";            // 公司
	
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
	
	public String getReleaseTime() {
		return releaseTime;
	}
	public void setReleaseTime(String releaseTime) {
		this.releaseTime = releaseTime;
	}
	
	public String getStyles() {
		return styles;
	}
	public void setStyles(String styles) {
		this.styles = styles;
	}
	
	public String getCompany() {
		return company;
	}
	public void setCompany(String company) {
		this.company = company;
	}
	
	@Override
	public String toString() {
		return "专辑 ID：\t" 	+ id 		  + "\n"
			 + "专辑名：\t" 	+ name 		  + "\n"
			 + "歌手 ID：\t" 	+ singerId	  + "\n"
			 + "歌手名：\t" 	+ singerName  + "\n"
			 + "发行时间：\t" 	+ releaseTime + "\n"
			 + "风格：\t" 	+ styles	  + "\n"
			 + "公司：\t" 	+ company;
	}
	
	@Override
	public void saveToDB() {
		String vFormat = "%d,'%s',%d,'%s',%s,'%s','%s'";
		String values = String.format(vFormat
				, id, escape(name), singerId, escape(singerName)
				, assureDate(releaseTime), escape(styles), escape(company));
		
		String uFormat = "album_name='%s',singer_id=%d,singer_name='%s'"
				+ ",release_time=%s,styles='%s',company='%s'";
		String update = String.format(uFormat, 
				  escape(name), singerId, escape(singerName)
				, assureDate(releaseTime), escape(styles), escape(company));
		
		String sql = "";
		if (useMySQL) {
			sql = String.format(insertSql, TABLE, values, update);
		}
		else {
			String.format(insertSql, TABLE, values);
		}
		
		SqlUtils.executeUpdate(sql);
	}
	
	@Override
	public void saveToExt() {
		String vFormat = "%d,'%s',%d,%d,%d,%d,'%s',%s,'%s','%s'";
		String values = String.format(vFormat
				, id, escape(name)
				, fCnCharId, sCnCharId, tCnCharId
				, singerId, escape(singerName)
				, assureDate(releaseTime), escape(styles), company);
		
		String uFormat = "album_name='%s',f_cn_char_id=%d, s_cn_char_id=%d,t_cn_char_id=%d" 
				+ ",singer_id=%d,singer_name='%s',release_time=%s,styles='%s',company='%s'";
		String update = String.format(uFormat, 
				  escape(name), fCnCharId, sCnCharId, tCnCharId, singerId, escape(singerName)
				, assureDate(releaseTime), escape(styles), escape(company));
		
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
			setSingerId(rSet.getInt(3));
			setSingerName(rSet.getString(4));
			setReleaseTime(rSet.getString(5));
			setStyles(rSet.getString(6));
			setStyles(rSet.getString(7));
		}
		catch (SQLException sqle) {
			sqle.printStackTrace();
		}
	}
	
	/**
	 * 专辑播放次数变化才认为是需要更新的
	 */
	@Override
	public boolean needUpdate() {
		//String sql = String.format(SELECT_SQL, "play_num", TABLE, ID_COL, id);
		//return playNum != SqlUtils.selectInt(sql);
		return true;
	}

}
