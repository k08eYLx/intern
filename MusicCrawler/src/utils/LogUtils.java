package utils;

import java.io.PrintWriter;
import java.io.StringWriter;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

public class LogUtils {
	
	public static final Logger LOGGER = LogManager.getLogger(LogUtils.class.getName());
	//public static final Logger EXCEPTION_LOGGER = LogManager.getLogger("ExceptionLog");
	
	private LogUtils() { }
	
	public static String getTrace(Throwable t) {
        StringWriter stringWriter= new StringWriter();
        PrintWriter writer= new PrintWriter(stringWriter);
        t.printStackTrace(writer);
        StringBuffer buffer= stringWriter.getBuffer();
        return buffer.toString();
    }
	
}
