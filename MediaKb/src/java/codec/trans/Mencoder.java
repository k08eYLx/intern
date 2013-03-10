package codec.trans;

import java.io.IOException;
import java.io.LineNumberReader;
import java.util.HashMap;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Mencoder {

    private static final Pattern bracket = Pattern.compile("[()]");
    private static final Pattern pattern = Pattern.compile("\\(( ?[0-9]{1,2}%)|(100%)\\)");

	/**
	 * vcodec=<value>		Employ the specified codec (default: mpeg4).
	 * asv1 asv2 dvvideo ffv1 ffvhuff flv h261 h263 h263p huffyuv libtheora libx264 libxvid ljpeg
	 * mjpeg mpeg1video mpeg2video mpeg4 msmpeg4 msmpeg4v2 roqvideo rv10 snow svq1 wmv1 wmv2
	 */
	private static Map<String, String> vcodec = new HashMap<String, String>();
	static {
		// .asf/.wmv <=== wmv1/wmv2
		vcodec.put(".wmv", "wmv2");
		vcodec.put(".asf", "wmv2");
		// .avi <=== msmpeg4/msmpeg4v1/msmpeg4v2
		vcodec.put(".avi", "msmpeg4");
		// .mpeg/.mpg <=== mpeg4
		vcodec.put(".mpeg", "mpeg4");
		vcodec.put(".mpg", "mpeg4");
		vcodec.put(".flv", "flv");
	}
	
	private static final String CONVERT_FORMAT = "etc/mencoder %s -o %s -oac mp3lame -ovc lavc";
	private static final String VCODEC_FORMAT  = " -lavcopts vcodec=%s";

    private static String getCmd(String src, String des) {
        String suffix = des.substring(des.lastIndexOf('.'));
        String codec = vcodec.get(suffix);
        System.out.println(suffix + "\t ===> " + codec);
        String format = CONVERT_FORMAT + VCODEC_FORMAT;
        return (String.format(format, src, des, codec));
    }

	public static void convert(String src, String des) {
		String cmd = getCmd(src, des);
        System.out.println(cmd);
		//CmdExecuter.execute(cmd);
        LineNumberReader reader = CmdExecuter.getLineReader(cmd);
        try {
            String line = null;
            while ((line = reader.readLine()) != null) {
                Matcher matcher = pattern.matcher(line);
                if (matcher.find()) {
                    String m = matcher.group();
                    String percentage = bracket.matcher(m).replaceAll("").trim();
                    System.out.println(percentage);
                }
            }
        }
        catch (IOException ioe) {
            ioe.printStackTrace();
        }
	}
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		Mencoder.convert("etc/input.avi", "etc/output.mpeg");
	}

}
