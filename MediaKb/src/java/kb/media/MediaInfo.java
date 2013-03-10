package kb.media;

/**
 * Created with IntelliJ IDEA.
 * User: YangLX
 * Date: 13-1-13
 * Time: 下午4:13
 * To change this template use File | Settings | File Templates.
 */

public class MediaInfo {

    // Songs
    private int songId = -1;                     // song_id
    private String songName    = "";             // song_name
    private int    hotNumber   = 0;              // hot_number
    private String releaseTime = "";             // releaseTime
    private String company     = "";             // company
    private String tags         = "";            // tags

    // Albums
    private int albumId = -1;                    // album_id
    private String albumName   = "";             // album_name
    // release_time 应该与歌曲的相同
    // styles       与歌曲的tags类似
    // company      应该与歌曲的相同

    // Singers
    private int singerId = -1;                   // singer_id
    private String singerName  = "";             // singer_name
    private String type         = "";             // type
    private int    listeners   = 0;              // listeners
    private String region       = "";            // region
    private String birthday     = "";            // birthday

    private float possibility = 1.0f;

    public int getSongId() {
        return songId;
    }

    public void setSongId(int songId) {
        this.songId = songId;
    }

    public String getSongName() {
        return songName;
    }

    public void setSongName(String songName) {
        this.songName = songName;
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

    public float getPossibility() {
        return possibility;
    }

    public void setPossibility(float possibility) {
        this.possibility = possibility;
    }

    @Override
    public String toString() {
        return "song_id : \t" + songId
                + "\n" + "song_name : \t" + songName
                + "\n" + "hot_number :\t" + hotNumber
                + "\n" + "release_time :\t" + releaseTime
                + "\n" + "company : \t" + company
                + "\n" + "tags : \t" + tags
                + "\n" + "album_name : \t" + albumName
                + "\n" + "singer_id : \t" + singerId
                + "\n" + "singer_name : \t" + singerName
                + "\n" + "type : \t" + type
                + "\n" + "listeners : \t" + listeners
                + "\n" + "region : \t" + region
                + "\n" + "birthday : \t" + birthday
                + "\n" + "possibility : \t" + possibility
                + "\n";
    }

}
