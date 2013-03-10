package kb.media;

import java.util.List;
import java.util.Map;

/**
 * Created with IntelliJ IDEA.
 * User: YangLX
 * Date: 13-1-13
 * Time: 下午3:51
 * To change this template use File | Settings | File Templates.
 */

public interface IMediaKbManager {

    /**
     * @param hint     普通字符串（文件名）
     * @return
     */
    public List<MediaInfo> getMediaInfosByHint(String hint);
    public List<List<MediaInfo>> getMediaInfosByHints(String[] hints);

    /**
     * @param json       json 格式
     * @return
     */
    public List<MediaInfo> getMediaInfosByJson(String json);

    /**
     * maybe by name
     *
     * @param hint
     * @return
     */
    public List<MediaInfo> getMediaInfosByHint(Hint hint);

    public List<MediaInfo> getMediaInfosByHints(Hint[] hints);

    public List<MediaInfo> getMediaInfosByHints(List<Hint> hints);

    public MediaInfo getMediaInfoBySHA1(String sha1);

    /**
     * Get media infos by sha1 and name.
     * 先尝试通过文件的SHA1获取信息，如果返回为空再通过文件名获取
     *
     * @param sha1
     * @param fileName
     * @return 返回JSON字符串
     */
    public String getMediaInfosBySha1AndName(String sha1, String fileName);

    /**
     * Get media infos by sha1s and names.
     *
     * @param sha1sNames 一系列sha1和name组成的map
     * @return 返回JSON字符串
     */
    public String getMediaInfosBySha1sAndNames(Map<String, String> sha1sNames);

}
