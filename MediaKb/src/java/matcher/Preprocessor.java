package matcher;

import java.util.regex.Pattern;

/**
 * Created with IntelliJ IDEA.
 * User: YangLX
 * Date: 13-1-17
 * Time: 下午4:44
 * To change this template use File | Settings | File Templates.
 */

public abstract class Preprocessor {

    protected static String deleteSuffix(String fileName) {
        return deleteSuffix(fileName, true);
    }

    protected static String deleteSuffix(String fileName, boolean hasSuffix) {
        String result = fileName;
        if (hasSuffix) {
            result = fileName.substring(0, fileName.lastIndexOf("."));
        }
        return result;
    }

    /**
     * 删除包含在歌曲名称里的一些附加信息
     * 如：括号间的内容
     *     歌曲名前的序号
     *
     * @param input
     * @return
     */
    public static String rmAdditionalInfo(String input) {
        input = input.replaceAll("^[0-9]{2}\\.", "");              // 删除歌曲名前边的序号，如“01."
        input = input.replaceAll("（", "(").replaceAll("）", ")"); // 将中文的括号替换为英文的
        input = input.replaceAll("\\(.*\\)|\\[.*\\]", "");        // 删除掉匹配括号间的内容
        int lastIndex = input.length() - 1;
        if (lastIndex > 0 && input.lastIndexOf('(') == lastIndex) {
            input = input.substring(0, lastIndex);                  // 删除可能出现在末尾的左单括号
        }
        return input.trim();
    }

    public static void main(String[] args) {
        // 中英文括号是不同的
        String str = "03.Ldjfkd（d（Remix））[SA](";
        System.out.println(rmAdditionalInfo(str));
    }

}
