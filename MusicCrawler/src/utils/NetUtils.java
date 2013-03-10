package utils;

import java.io.DataInputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLConnection;

import utils.file.FileUtils;

public class NetUtils {

	private static final int BUFFER_SIZE = 1024;
	
	private NetUtils() { }
	
	public static boolean isUrlOk(String strURL) {
		boolean flag = false;
		
		try {
			URL url = new URL(strURL);
			URLConnection connection = url.openConnection();
			HttpURLConnection httpConn = (HttpURLConnection) connection;
			int code = httpConn.getResponseCode();
			flag = (code == 200);
		}
		catch (MalformedURLException mue) {
			LogUtils.LOGGER.warn(mue.getMessage() + " ===> " + strURL);
		}
		catch (IOException ioe) {
			LogUtils.LOGGER.warn("Can't access URL ===> " + strURL);
		}

		return flag;
	}

	// java net
	public static boolean download(String url, String outName, int size) {
		FileUtils.initialBasePath(outName);
		File out = new File(outName);

		InputStream in = null;
		DataInputStream dataIn = null;
		FileOutputStream fileOut = null;

		try {
			in = new URL(url).openStream();
			dataIn = new DataInputStream(in);
			fileOut = new FileOutputStream(out);
			int length = 0;
			byte[] buffer = new byte[size];
			while ((length = dataIn.read(buffer)) > 0) {
				fileOut.write(buffer, 0, length);
			}
			return true;
		}
		catch (IOException ioe) {
			LogUtils.LOGGER.warn(ioe.getMessage() + " ===> " + url);
			//LogUtils.EXCEPTION_LOGGER.debug(LogUtils.getTrace(ioe));
		}
		finally {
			if (in != null) {
				try {
					in.close();
				}
				catch (IOException e) {
					e.printStackTrace();
				}
			}
			if (dataIn != null) {
				try {
					dataIn.close();
				}
				catch (IOException e) {
					e.printStackTrace();
				}
			}
			if (fileOut != null) {
				try {
					fileOut.close();
				}
				catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
		return false;
	}
	
	public static boolean download(String url, String outName) {
		return download(url, outName, BUFFER_SIZE);
	}
	
}
