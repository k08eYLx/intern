package matcher;

import com.lerss.utils.Trad2Simp;

import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Spliter extends Preprocessor {

    /*
     * ！！！歌手名也可能含有 '-'、'()'、'[]'，
     * 但是由于这些作为分隔符的情况太常见了，所以还是优先算作分隔符
     *
     * 空格 作为信息的组成部分的情况更多，所以不算作分隔符
     *
     * 点号和英文单引号真的不应该算作分隔符
     */
    private static final String SPLITER_REGEX
            = "[（）【】《》，、。；：！？‘’“”－—……￥"
            + "_~`={}|/^$()<>+&'\"@#%*!?,.;:\\-\\[\\]\\\\]";

    /**
     * 过滤掉标点等符号信息，返回纯文字内容
     *
     * @param str
     * @return
     */
	public static List<String> process(String str) {
        List<String> strs = new ArrayList<String>();
        // 繁体字转成简体字，小写字母转成大写字母
        str = Trad2Simp.convert(str).toUpperCase();
        String[] parts = str.split(SPLITER_REGEX);
        for (String part : parts) {
            part = part.trim();
            if (!part.isEmpty()) {
                strs.add(part);
            }
        }
		return strs;
	}
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
        Spliter spliter = new Spliter();
		String text = "爱冒险&[其他信息]-元若蓝 ";
        List<String> parts = spliter.process(text);
        for (String part : parts) {
            System.out.println(part);
        }
	}

}
