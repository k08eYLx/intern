package matcher;

import com.lerss.utils.Trad2Simp;
import utils.JStrMgr;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Filter extends Preprocessor {

    /**
     * 如果过滤空格，空格可以作为遍历字符串时的step标识
     * 并且空格还可以进一步降低中文里前后信息连接成新的有用信息的的可能性
     * 效率上也有一定提升
     * 但是如果原始信息间有空格将不能匹配，如无法匹配到“张 学 友”
      */

    // 最前面有一个空格
    private static final String FILTER_REGEX
            = "[ （）【】《》、，。；：！？‘’“”－—……￥"
            + "`_~#%={}|/@&*+^$()<>,.;:!?'\\\"\\-\\[\\]\\\\]";

    private static final Pattern FILTER_PATTERN = Pattern.compile(FILTER_REGEX);

    /**
     * 过滤掉标点符号，并将多个连续的空格转化为一个
     *
     * @param str
     * @return
     */
    public static String process(String str) {
        // 繁体字转成简体字，小写字母转成大写字母
        str = Trad2Simp.convert(str).toUpperCase();
		Matcher matcher = FILTER_PATTERN.matcher(str);
        str = matcher.replaceAll("");    // 过滤掉标点符号
		return JStrMgr.handleSpace(str);
	}
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		String text = "爱冒险 & [其他信息] -  元若蓝";
        System.out.println(Filter.process(text));
	}

}
