package utils;

/**
 * Created with IntelliJ IDEA.
 * User: YangLX
 * Date: 13-1-22
 * Time: 下午4:25
 * To change this template use File | Settings | File Templates.
 */
public class JStrMgr {

    private static int countSpace(String str) {
        int count = 0;
        for (int i = 0; i < str.length(); ++i) {
            if (str.charAt(i) == ' ') ++count;
        }
        return count;
    }

    /**
     * 获取字符串中所有空格的位置索引，
     * 将起始算作第一个空格，结束算作最后一个空格
     *
     * @param str
     * @return
     */
    public static int[] getSpaces(String str) {
        int[] poses = new int[countSpace(str)+2];
        int count = 0;
        int pos = 0;
        poses[count++] = pos;
        for ( ; pos < str.length(); ++pos) {
            if (str.charAt(pos) == ' ') {
                poses[count++] = pos;
            }
        }
        poses[count] = pos;
        return poses;
    }

    /**
     * trim and replace multi spaces with single
     *
     * @param str
     * @return
     */
    public static String handleSpace(String str) {
        return str.trim().replaceAll(" {2,}", " ");
    }

    public static void stepBySpace(String str) {
        str = handleSpace(str);
        int[] poses = getSpaces(str);
        int spaces = poses.length;
        for (int i = spaces - 1; i > 0; --i) {
            for (int j = 0; j <= spaces - i - 1; ++j) {
                System.out.println(str.substring(poses[j], poses[i + j]).trim());
            }
        }
    }

    public static void main(String[] args) {
        String str = "abc 杨连祥   hijack 是我";
        stepBySpace(str);
    }

}
