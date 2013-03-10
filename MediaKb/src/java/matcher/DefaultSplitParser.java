package matcher;

import java.util.ArrayList;
import java.util.List;

/**
 * Created with IntelliJ IDEA.
 * User: YangLX
 * Date: 13-1-17
 * Time: 下午4:32
 * To change this template use File | Settings | File Templates.
 */

public class DefaultSplitParser {

    private static final NameIdsDict songNameDict = DictionaryMaker.getSongNameDict();
    private static final NameIdsDict singerNameDict = DictionaryMaker.getSingerNameDict();

    /**
     * 匹配字符串列表
     *
     * @param items 待匹配的字符串列表
     * @param outIds 匹配到的信息对应的ID列表
     * @param dict 依据的词典
     */
    public static void parse(List<String> items, List<Integer> outIds, NameIdsDict dict) {
        for (String item : items) {
            // 是否在词典里
            if (dict.containsKey(item)) {
                List<Integer> ids = dict.get(item);
                for (Integer id : ids) {
                    outIds.add(id);
                }
            }
        }
    }

    public static void parseSongName(List<String> items, List<Integer> songIds) {
        parse(items, songIds, songNameDict);
    }

    public static void parseSingerName(List<String> items, List<Integer> singerIds) {
        parse(items, singerIds, singerNameDict);
    }

    /**
     * @param args
     */
    public static void main(String[] args) {
        long start = System.currentTimeMillis();

        List<Integer> songIds = new ArrayList<Integer>();
        List<Integer> singerIds = new ArrayList<Integer>();

        String hint = "我是其他信息 - 爱冒险 - 元若蓝 [您还好吧]";  // 75
        List<String> items = Spliter.process(hint);    // 分隔字符串
        for (String item : items) {
            System.out.println(item);
        }

        DefaultSplitParser.parseSongName(items, songIds);
        for (Integer songId : songIds) {
            System.out.println(songId);
        }

        DefaultSplitParser.parseSingerName(items, singerIds);
        for (Integer singerId : singerIds) {
            System.out.println(singerId);
        }

        long finish = System.currentTimeMillis();
        System.out.println("Time ===> " + (finish - start));
    }

}
