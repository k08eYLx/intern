package matcher;

import utils.JStrMgr;

import java.util.ArrayList;
import java.util.List;

public class MaxFilterParser {

	private static final NameIdsDict songNameDictFed = DictionaryMaker.getSongNameDictFed();
	private static final NameIdsDict singerNameDictFed = DictionaryMaker.getSingerNameDictFed();
	
	/**
	 * 解析字符串，返回剩余字符串
     *
     * 由于空格不再被过滤掉，现在可以匹配“张.学.友”，但不能匹配“张 学 友”
	 * 
	 * @param str 待解析的字符串
     * @param outIds 匹配到的信息对应的ID列表
	 * @param dict 依据的词典
	 * @return 匹配后的剩余的字符串
	 */
	private static String parse(String str, List<Integer> outIds, NameIdsDict dict) {
		String subStr = "";
		int len = str.length();
		for (int i = len; i > 0; --i) {
			for (int j = 0; j < (len - i + 1); j++) {
				subStr = str.substring(j, i + j).trim();
				// 是否在词典里
				if (dict.containsKey(subStr)) {
					List<Integer> ids = dict.get(subStr);
					for (Integer id : ids) {
						outIds.add(id);
					}
					return str.replace(subStr, "");
				}
			}
		}
		return str.replace(subStr, "");
	}

    /**
     * 解析字符串，以空格作为step，返回剩余字符串
     *
     * @param str 待解析的字符串
     * @param outIds 匹配到的信息对应的ID列表
     * @param dict 依据的词典
     * @return 匹配后的剩余的字符串
     */
    private static String stepParse(String str, List<Integer> outIds, NameIdsDict dict) {
        String subString = "";
        int[] poses = JStrMgr.getSpaces(str);
        int spaces = poses.length;
        for (int i = spaces - 1; i > 0; --i) {
            for (int j = 0; j <= spaces - i - 1; ++j) {
                subString = str.substring(poses[j], poses[i + j]).trim();
                // 是否在词典里
                if (dict.containsKey(subString)) {
                    List<Integer> ids = dict.get(subString);
                    for (Integer id : ids) {
                        outIds.add(id);
                    }
                    return JStrMgr.handleSpace(str.replace(subString, ""));
                }
            }
        }
        return JStrMgr.handleSpace(str.replace(subString, ""));
    }

    /**
     * 如果输入字符串含有空格，则执行按空格的step匹配
     * 如果不含有空格则，则按照逐字符的匹配
     *
     * @param str
     * @param songIds
     * @return
     */
	private static String parseSongName(String str, List<Integer> songIds) {
        /*if (str.contains(" "))
		    return stepParse(str, songIds, songNameDictFed);
        else*/
            return parse(str, songIds, songNameDictFed);
	}
	
	private static String parseSingerName(String str, List<Integer> singerIds) {
        /*if (str.contains(" "))
		    return stepParse(str, singerIds, singerNameDictFed);
        else*/
            return parse(str, singerIds, singerNameDictFed);
	}

    public static String parseHint(String hint, List<Integer> songIds, List<Integer> singerIds) {
        String left = Filter.process(hint);    // 替换标点符号
        System.out.println(left);
        left = parseSongName(left, songIds);
        System.out.println(left);
        return parseSingerName(left, singerIds);
    }
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		long start = System.currentTimeMillis();

		List<Integer> songIds = new ArrayList<Integer>();
		List<Integer> singerIds = new ArrayList<Integer>();

		//String hint = "我是其他信息*爱冒险&元若蓝[您还好吧]";  // 1ms
        String hint = "A.B. Quintanilla Iii Y Kumbia Kings Con Juan Gabriel Y El Gran Silencio"
                    + "- No Tengo Dinero (Album VersióN) - [英文]";  // 3ms ===> 2ms

		String left = MaxFilterParser.parseHint(hint, songIds, singerIds);
		for (Integer songId : songIds) {
			System.out.println(songId);
		}
		for (Integer singerId : singerIds) {
			System.out.println(singerId);
		}
        System.out.println(left);

        long finish = System.currentTimeMillis();
        System.out.println("Time ===> " + (finish - start));
	}

}
