package utils;

import java.io.FileNotFoundException;
import java.io.PrintStream;

/**
 * Created with IntelliJ IDEA.
 * User: YangLX
 * Date: 13-1-23
 * Time: 下午3:46
 * To change this template use File | Settings | File Templates.
 */
public class Tracer {

    private static final String TRACE_FILE = "etc/trace.log";
    private static final PrintStream STD_OUT = System.out;
    private static PrintStream ps = null;
    static {
        try {
            ps = new PrintStream(TRACE_FILE);
        }
        catch (FileNotFoundException fnfe) {
            fnfe.printStackTrace();
        }
    }

    public static void log(String msg) {
        System.setOut(ps);
        System.out.println(msg + "\r\n");
        ps.flush();
        System.setOut(STD_OUT);
    }

}
