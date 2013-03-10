package utils;

import java.io.UnsupportedEncodingException;
import java.util.ArrayList;

import app.Constants;

public class StringUtils {

	private StringUtils() { }
	
	/**
	 * 以TAB为分隔符连接多个字符串
	 * 
	 * @param strings    字符串链表
	 * @return           连接后的字符串
	 */
	public static String joinStrings(ArrayList<String> strings) {
		return joinStrings(strings, "\t");
	}
	
	private static ArrayList<String> toArrayList(String[] strings) {
		int length = strings.length;
		ArrayList<String> stringsList = new ArrayList<String>(length);
		for (int i = 0; i < length; i++) {
			stringsList.add(strings[i]);
		}
		return stringsList;
	}
	
	/**
	 * 以逗号分隔字符串数组
	 * 
	 * @param strings
	 * @return
	 */
	public static String joinStrings(String[] strings) {
		ArrayList<String> stringsList = toArrayList(strings);
		return joinStrings(stringsList, ",");
	}
	
	/**
	 * 以指定的分隔符连接多个字符串
	 * 
	 * @param strings    字符串链表
	 * @param sep        分隔符
	 * @return           连接后的字符串
	 */
	public static String joinStrings(ArrayList<String> strings, String sep) {
		String out = "";

		if (strings.size() > 0) {
			out = strings.remove(0);
			for (String string : strings) {
				// 字符串为空也要用TAB进行站位
				out += sep + string;
			}
		}
		
		return out.trim();
	}

	/**
	 * 从末尾获取字符串str中出现在字符c之后的子字符串
	 * 
	 * @param url
	 * @param c
	 * @return
	 */
	public static String lastPart(String str, char c) {
		int pos = str.lastIndexOf(c) + 1;
		return str.substring(pos).trim();
	}
	
	public static String getValue(String str) {
		return getByIndex(str, 1);
	}
	
	public static String getByIndex(String str, int idx) {
		return getByIndex(str, "：", idx);    // 中文冒号
	}
	
	public static String getByIndex(String str, String regex, int idx) {
		String res = "";
		String[] strs = str.trim().split(regex);    // 正则
		if (strs.length > idx) {
			res = strs[idx];
		}
		return res.trim();
	}
	
	public static int toInt(String str) {
		int value = Constants.ILLEGAL_VALUE;
		try {
			if (str != null && !str.isEmpty()) {
				value = new Integer(str.trim());	
			}
		}
		catch (NumberFormatException nfe) {
			value = Constants.ILLEGAL_VALUE;
		}
		return value;
	}
	
	/**
	 * 使用指定的编码方式对字符串进行编码
	 * 
	 * @param message
	 * @return
	 */
	public static String encode(String message, String charset) {
		try {
			message = new String(message.getBytes(), charset);
		}
		catch (UnsupportedEncodingException usee) {
			usee.printStackTrace();
		}
		return message;
	}
	
	/**
	 * 使用AppConstants中指定的默认编码方式对字符串进行编码
	 * 
	 * @param message
	 * @return
	 */
	public static String encode(String message) {
		return encode(message, Constants.DEF_CHARSET);
	}
	
	public static void main(String[] args) {
		/*Runtime.getRuntime().addShutdownHook(new Thread() {
			@Override
			public void run() {
				System.out.println("Exit!");
			}
		});*/
	}
	
}
