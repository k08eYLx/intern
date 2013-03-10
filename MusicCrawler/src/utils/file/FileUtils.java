package utils.file;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.LineNumberReader;
import java.io.OutputStreamWriter;
import java.io.UnsupportedEncodingException;
import java.util.List;

import utils.StringUtils;

public class FileUtils {

	public static final String UTF_8 = "UTF-8";
	
	private FileUtils() { }
	
    public static String read(String fileName, String encoding) { 
		StringBuffer fileContent = new StringBuffer();
		FileInputStream fis = null;
		InputStreamReader isr = null;
		BufferedReader br = null;
		try {
			fis = new FileInputStream(fileName);
			isr = new InputStreamReader(fis, encoding);
			br = new BufferedReader(isr);
			String line = null;
			while ((line = br.readLine()) != null) {
				fileContent.append(line);
				fileContent.append(System.getProperty("line.separator"));
			}
		}
		catch (FileNotFoundException fnfe) {
			fnfe.printStackTrace();
		}
		catch (UnsupportedEncodingException uee) {
			uee.printStackTrace();
		}
		catch (IOException ioe) {
			ioe.printStackTrace();
		}
		finally {
			if (br != null) {
				try {
					br.close();
				}
				catch (IOException e) {
					e.printStackTrace();
				}
			}
			if (isr != null) {
				try {
					isr.close();
				}
				catch (IOException e) {
					e.printStackTrace();
				}
			}
			if (fis != null) {
				try {
					fis.close();
				}
				catch (IOException e) {
					e.printStackTrace();
				}
			}
		}

        return fileContent.toString(); 
    }

    public static void write(String content, String fileName, String encoding, boolean append) {
		initialBasePath(fileName);
		
    	FileOutputStream fos = null;
    	OutputStreamWriter osw = null;
        try { 
            fos = new FileOutputStream(fileName, append);
            osw = new OutputStreamWriter(fos, encoding);
            osw.write(content);
			osw.flush();
        }
		catch (Exception e) {
            e.printStackTrace();
        }
        finally {
        	if (osw != null) {
                try {
	                osw.close();
				}
				catch (IOException e) {
					e.printStackTrace();
				}
			}
        	if (fos != null) {
                try {
					fos.close();
				}
				catch (IOException e) {
					e.printStackTrace();
				}
			}
        }
    }
    
    public static void append(String content, String fileName, String encoding) {
    	write(content, fileName, encoding, true);
    }
    
    public static void write(String content, String fileName, String encoding) {
    	write(content, fileName, encoding, false);
    }
    
    public static void append(String content, String fileName) { 
        append(content, fileName, UTF_8);
    }
    
    public static void write(String content, String fileName) { 
        write(content, fileName, UTF_8);
    }

    /**
     * 初始化文件的各上级目录
     * 按照File.separator='\'作为路径分隔符
     * 
     * @param path    文件路径：文件夹+文件名
     */
    public static void initialBasePath(String path) {
    	int pos = path.lastIndexOf(File.separator);
    	if (pos > 0) {
    		String basePath = path.substring(0, pos);
    		File dir = new File(basePath);
    		if (!dir.exists()) dir.mkdirs();
		}
	}
    
    /**
     * 获取文件后缀
     * 
     * @param fn
     * @return
     */
    public static String suffix(String fn) {
		return StringUtils.lastPart(fn, '.');
	}

    public static void readByLine(String path, List<String> lines) {
		try {
			File file = new File(path);
			if (!(file.exists())) return;
			FileReader fileReader = new FileReader(file);
			LineNumberReader lineReader = new LineNumberReader(fileReader);
			String line = null;
			while ((line = lineReader.readLine()) != null) {
				if (!(line.isEmpty())) {
					lines.add(line);
				}
			}
			lineReader.close();
			fileReader.close();
		}
		catch (FileNotFoundException fnfe) {
			fnfe.printStackTrace();
		}
		catch (IOException ioe) {
			ioe.printStackTrace();
		}
	}
    
    public static void main(String[] args) {
    	
	}
    
}
