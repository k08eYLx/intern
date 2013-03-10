package kb.media

import db.H2Manager
import groovy.json.JsonBuilder
import groovy.json.JsonSlurper
import matcher.DefaultSplitParser
import matcher.Filter
import matcher.MaxFilterParser
import matcher.Spliter
import utils.Tracer

class MediaKbService implements IMediaKbManager {

    def static h2Sql = H2Manager.getSqlInstance()

    /**
     * 0    未知
     * 1    歌曲名
     * 2    歌手名
     */
    static String[] types = ["", "song_name", "singer_name"]
    private static final int SONG_COL   = 1;
    private static final int SINGER_COL = 2;

    private String makeStmt(String hint) {
        "select * from songs where ${makeWhereClause(hint)} order by song_id"
    }

    private String makeStmt(Hint hint) {
        "select * from songs where ${makeWhereClause(hint)} order by song_id"
    }

    private String makeStmt(Hint[] hints) {
        return makeStmt(Arrays.asList(hints))
    }

    private String makeStmt(List<Hint> hints) {
        String whereClause = makeWhereClause(hints)
        "select * from songs where ${whereClause} order by song_id"
    }

    private String makeWhereClause(String value) {
        "${makeWhereClause(value, SONG_COL)} or ${makeWhereClause(value, SINGER_COL)}"
    }

    /**
     * 做精确匹配 先不用like，like不支持索引
     *
     * @param value
     * @param type
     * @return
     */
    private String makeWhereClause(String value, int type) {
         // "${types[type]} like '%${value}%'"
        "${types[type]} = '${value}'"
    }

    private String makeWhereClause(Hint hint) {
        String whereClause = null
        String value = hint.getValue()
        int type = hint.getType()
        if (type == 0) {
            whereClause = makeWhereClause(value)
        }
        else {
            whereClause = makeWhereClause(value, type)
        }
        return whereClause
    }

    private String makeWhereClause(Hint[] hints) {
        return makeWhereClause(Arrays.asList(hints))
    }

    private String makeWhereClause(List<Hint> hints) {
        String whereClause = makeWhereClause(hints.remove(0))
        hints.each {
            whereClause += " and ${makeWhereClause(it)}"
        }
        return whereClause
    }

    /**
     * Songs
     * singerId         // song_id
     * songName         // song_name
     * hotNumber        // hot_number
     * releaseTime      // releaseTime
     * company          // company
     * tags             // tags
     *
     * // Albums
     * albumId          // album_id
     * albumName        // album_name
     * // release_time 应该与歌曲的相同
     * // styles       与歌曲的tags类似
     * // company      应该与歌曲的相同
     *
     * // Singers
     * singerId         // singer_id
     * singerName       // singer_name
     * type             // type
     * listeners        // listeners
     * region           // region
     * birthday         // birthday
     *
     * possibility = 1.0f;
     *
     * @param row
     * @return
     */
    private MediaInfo getMediaInfo(Object row) {
        MediaInfo info  = new MediaInfo()
        info.setSongId(row.song_id)
        info.setSongName(row.song_name)
        info.setHotNumber(row.hot_number)
        info.setReleaseTime(row.release_time.toString())
        info.setCompany(row.company)
        info.setTags(row.tags)
        info.setAlbumId(row.album_id)
        info.setAlbumName(row.album_name)
        info.setSingerId(row.singer_id)
        info.setSingerName(row.singer_name)
        h2Sql.eachRow("select * from Singers where singer_id=${row.singer_id}") {
            info.setType(it.type)
            info.setListeners(it.listeners)
            info.setRegion(it.region)
            info.setBirthday(it.birthday)
        }
        return info
    }

    private void getMediaInfo(List<MediaInfo> infos, Hint hint) {
        getMediaInfo(infos, makeStmt(hint))
    }

    private void getMediaInfo(List<MediaInfo> infos, String stmt) {
        println(stmt)
        h2Sql.eachRow(stmt) { row ->
            infos.add(getMediaInfo(row))
        }
    }

    private void getMediaInfoByIds(List<MediaInfo> infos, List<Integer> songIds, List<Integer> singerIds) {
        songIds.each { songId ->
            singerIds.each { singerId ->
                String stmt = "SELECT * FROM Songs WHERE song_id=${songId} and singer_id=${singerId}"
                getMediaInfo(infos, stmt)
            }
        }
    }

    /**
     * 将json格式的数据转换为List<Hint>
     *
     * @param jsnHint
     * @return
     */
    public List<Hint> toHints(String jsnHint) {
        List<Hint> hints = new ArrayList<Hint>()
        def obj = new JsonSlurper().parseText(jsnHint)
        obj.hints.each {
            Hint hint = new Hint(it.hint.value, it.hint.type)
            hints.add(hint)
        }
        return hints
    }

    private void calculatePossibility(List<MediaInfo> infos) {
        int size = infos.size()
        if (size > 0) {
            float possibility = 1 / infos.size()
            infos.each { it.setPossibility(possibility) }
        }
    }

    List<MediaInfo> getMediaInfosByHint(List<MediaInfo> infos, Hint hint) {
        String stmt = makeStmt(hint)
        getMediaInfo(infos, stmt)

        calculatePossibility(infos)
        return infos
    }

    private void splitParser(String hint, List<MediaInfo> infos, List<Integer> songIds, List<Integer> singerIds) {
        println("Split parser...")
        long start = System.currentTimeMillis()

        List<String> items = Spliter.process(hint)
        items.each { println(it) }

        DefaultSplitParser.parseSongName(items, songIds)
        DefaultSplitParser.parseSingerName(items, singerIds)

        getMediaInfoByIds(infos, songIds, singerIds)

        long finish = System.currentTimeMillis()
        println("Split parse time ===> ${(finish - start)}")
    }

    private String filterParser(String hint, List<MediaInfo> infos, List<Integer> songIds, List<Integer> singerIds) {
        println("Filter parser...")
        long start = System.currentTimeMillis()

        songIds.clear()
        singerIds.clear()

        String left = MaxFilterParser.parseHint(hint, songIds, singerIds)
        getMediaInfoByIds(infos, songIds, singerIds)

        long finish = System.currentTimeMillis()
        println("Filter parse time ===> ${(finish - start)}")

        return left
    }

    private void showIds(List<Integer> songIds, List<Integer> singerIds) {
        println(songIds)
        println(singerIds)
    }

    @Override
    List<MediaInfo> getMediaInfosByHint(String hint) {
        List<MediaInfo> infos     = new ArrayList<MediaInfo>()
        List<Integer>   songIds   = new ArrayList<Integer>()
        List<Integer>   singerIds = new ArrayList<Integer>()

        splitParser(hint, infos, songIds, singerIds)
        showIds(songIds, singerIds)

        String left = ""
        if (infos.isEmpty()) {
            left = filterParser(hint, infos, songIds, singerIds)
            showIds(songIds, singerIds)
        }
        else {
            MediaInfo info = infos.first()
            left = hint.replace(info.getSongName(), "").replace(info.getSingerName(), "")
            left = Filter.process(left)
        }
        Tracer.log("Left : ${hint} ===> ${left}")

        filtIfNeeded(infos)
        calculatePossibility(infos)
        return infos;
    }

    @Override
    List<List<MediaInfo>> getMediaInfosByHints(String[] hints) {
        List<List<MediaInfo>> infosList = new ArrayList<ArrayList<MediaInfo>>();
        hints.each {
            List<MediaInfo> infos = getMediaInfosByHint(it);
            if (infos != null && infos.size() > 0) {
                infosList.add(infos)
            }
        }
        return infosList;
    }

    @Override
    List<MediaInfo> getMediaInfosByJson(String json) {
        return getMediaInfosByHints(toHints(json))
    }

    @Override
    List<MediaInfo> getMediaInfosByHint(Hint hint) {
        List<MediaInfo> infos = new ArrayList<MediaInfo>()
        getMediaInfo(infos, hint)
        calculatePossibility(infos)
        return infos
    }

    @Override
    List<MediaInfo> getMediaInfosByHints(Hint[] hints) {
        return getMediaInfosByHints(Arrays.asList(hints))
    }

    @Override
    List<MediaInfo> getMediaInfosByHints(List<Hint> hints) {
        List<MediaInfo> infos = new ArrayList<MediaInfo>()

        String stmt = makeStmt(hints)
        getMediaInfo(infos, stmt)

        calculatePossibility(infos)
        return infos
    }

    @Override
    MediaInfo getMediaInfoBySHA1(String sha1) {
        MediaInfo info = null
        String stmt = "SELECT * FROM MV_SHA1 WHERE org_sha1='${sha1}'"
        h2Sql.eachRow(stmt) {
            int songId = it.song_id
            stmt = "SELECT * FROM Songs WHERE song_id=${songId}"
            h2Sql.eachRow(stmt) { row ->
                info = getMediaInfo(row)
            }
        }
        return info
    }

    String buildJson(MediaInfo info) {
        println(info)
        JsonBuilder jb = new JsonBuilder()
        jb.Information {
            SongId "${info.getSongId()}"
            SongName "${info.getSongName()}"
            HotNumber "${info.getHotNumber()}"
            ReleaseTime "${info.getReleaseTime()}"
            Company "${info.getCompany()}"
            Tags "${info.getTags()}"
            AlbumId "${info.getAlbumId()}"
            AlbumName "${info.getAlbumName()}"
            SingerId "${info.getSingerId()}"
            SingerName "${info.getSingerName()}"
            Type "${info.getType()}"
            Listeners "${info.getListeners()}"
            Region "${info.getRegion()}"
            Birthday "${info.getBirthday()}"
            Possibility "${info.getPossibility()}"
        }
        jb.toPrettyString()
    }

    String buildJson(List<MediaInfo> infos) {
        infos.each { println(it) }

        /*StringWriter sw = new StringWriter()
        infos.each { info ->
            sw << buildJson(info) << ","
        }
        sw.toString()*/

        JsonBuilder jBuilder = new JsonBuilder()
        jBuilder {
            "Informations" infos.each { info ->
                [
                    SongId : "${info.getSongId()}",
                    SongName : "${info.getSongName()}",
                    HotNumber : "${info.getHotNumber()}",
                    ReleaseTime : "${info.getReleaseTime()}",
                    Company : "${info.getCompany()}",
                    Tags : "${info.getTags()}",
                    AlbumId : "${info.getAlbumId()}",
                    AlbumName : "${info.getAlbumName()}",
                    SingerId : "${info.getSingerId()}",
                    SingerName : "${info.getSingerName()}",
                    Type : "${info.getType()}",
                    Listeners : "${info.getListeners()}",
                    Region : "${info.getRegion()}",
                    Birthday : "${info.getBirthday()}",
                    Possibility : "${info.getPossibility()}"
                ]
            }
        }
        jBuilder.toPrettyString()
    }

    @Override
    String getMediaInfosBySha1AndName(String sha1, String fileName) {
        MediaInfo info = getMediaInfoBySHA1(sha1)
        if (info != null) {
            return buildJson(info)
        }
        else {
            List<MediaInfo> infos = getMediaInfosByHint(fileName)
            if (infos.size() > 0) {
                saveSongBySha1(infos.get(0), sha1)
            }
            return buildJson(infos)
        }
    }

    private void saveSongBySha1(MediaInfo info, String sha1) {
        // song_id，album_id和singer_id也许应该重新分配，不然一首歌的两个版本直接可能冲突
        String mergeSha1 = """MERGE INTO MV_SHA1 VALUES(
                    '${sha1}', '${sha1}', ${info.getSongId()})"""
        println(mergeSha1)
        h2Sql.executeInsert(mergeSha1)
        String mergeSong = """MERGE INTO CONFIRMED_SONGS VALUES(
                      ${info.getSongId()}, '${info.getSongName()}'
                    , ${info.getAlbumId()}, '${info.getAlbumName()}'
                    , ${info.getSingerId()}, '${info.getSingerName()}'
                    , ${info.getHotNumber()}, ${info.getReleaseTime()}
                    , '${info.getCompany()}', '${info.getTags()}'
                )"""
        println(mergeSong)
        h2Sql.executeInsert(mergeSong)
        String mergeSinger = """MERGE INTO CONFIRMED_SINGERS VALUES(
                      ${info.getSingerId()}, '${info.getSingerName()}'
                    , '${info.getType()}', ${info.getListeners()}
                    , '${info.getRegion()}', '${info.getBirthday()}'
                )"""
        println(mergeSinger)
        h2Sql.executeInsert(mergeSinger)
    }

    /**
     * 过滤可能出现的歌曲与歌手同名
     *
     * @param infos    待过滤的List
     */
    private void filtIfNeeded(List<MediaInfo> infos) {
        Iterator<MediaInfo> iter = infos.iterator();
        while(iter.hasNext() && infos.size() > 1){
            MediaInfo info = iter.next()
            String songName = info.getSongName()
            String singerName = info.getSingerName()
            if (songName.equalsIgnoreCase(singerName)) {
                // iter.remove()
                infos.remove(info)
                iter = infos.iterator()    // 重新获取迭代器
            }
        }
    }

    @Override
    String getMediaInfosBySha1sAndNames(Map<String, String> sha1sNames) {
        String infos = ""
        sha1sNames.each {
            infos += getMediaInfosBySha1AndName(it.key, it.value)
        }
        return infos
    }

}
