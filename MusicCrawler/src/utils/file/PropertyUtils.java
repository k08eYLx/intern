package utils.file;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Map.Entry;
import java.util.Properties;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;

import utils.StringUtils;

/*
 * 在方法前面加上synchronized修饰符即可以将一个方法声明为同步化方法，同步化方法在执行之前需要获得一个锁。
 * 如果这是一个类方法，那么获得的锁是和声明方法的类相关的java.lang.Class对象(唯一)的锁，即类锁实际上是由对象锁来实现。
 * 如果这是一个实例方法，那么此锁是this对象的锁。对象锁的同步是在类实例级别上的，会同时限制对该类实例的所有资源的访问。
 */

public class PropertyUtils {

	private String fileName = "";
	private int size = 0;     // initial table capacity
	
	public PropertyUtils(String fileName) {
		this.fileName = fileName;
	}
	
	public PropertyUtils(String fileName, int size) {
		this.fileName = fileName;
		this.size = size;
	}

	/*
	 * Properties类的store和load方法本身是线程安全的
	 * 不需要额外的synchronized同步
	 */
	
	private Properties restorePorperties() {
		Properties ps = new Properties();
		try {
			File file = new File(fileName);
			if (file.exists()) {
				InputStream in = new FileInputStream(file);
				ps.load(in);
				in.close();
			}
		}
		catch (IOException ioe) {
			ioe.printStackTrace();
		}
		return ps;
	}
	
	public String restoreStringField(String field, String defValue) {
		Properties ps = restorePorperties();
		String value = ps.getProperty(field);
		if (value == null || value.isEmpty()) {
			value = defValue;
		}
		return value;
	}
	
	public String restoreStringField(String field) {
		Properties ps = restorePorperties();
		return ps.getProperty(field);
	}
	
	public int restoreIntField(String field) {
		String strValue = restoreStringField(field);
		return StringUtils.toInt(strValue);
	}

	public ConcurrentHashMap<String, Integer> restoreIntFields() {
		Properties ps = restorePorperties();
		Set<Entry<Object, Object>> entries = ps.entrySet();
		size = Math.max(size, entries.size());
		ConcurrentHashMap<String, Integer> ints = new ConcurrentHashMap<String, Integer>(size);
		for (Entry<Object, Object> entry : entries) {
			String key = (String) entry.getKey();
			int value = StringUtils.toInt(ps.getProperty(key));
			ints.put(key, value);
		}
		return ints;
	}
	
	private void storeProperties(Properties ps) {
		try {
			File file = new File(fileName);
			if (!(file.exists())) {
				file.createNewFile();
			}
			OutputStream out = new FileOutputStream(file);
			ps.store(out, null);
			out.close();
		}
		catch (IOException ioe) {
			ioe.printStackTrace();
		}
	}
	
	public void storeStringFiled(String field, String value) {
		synchronized (fileName) {
			Properties ps = restorePorperties();			
			ps.setProperty(field, value);
			storeProperties(ps);
		}
	}

	// 将多线程转化为单线程
	public void storeIntFields(ConcurrentHashMap<String, Integer> ints) {
		Properties ps = restorePorperties();
		Set<Entry<String, Integer>> entries = ints.entrySet();
		for (Entry<String, Integer> entry : entries) {
			ps.setProperty(entry.getKey(), "" + entry.getValue());
		}
		storeProperties(ps);
	}
	
	public void storeIntField(String field, int value) {
		storeStringFiled(field, "" + value);	
	}

	public static void main(String[] args) {
		String fileName = "tmp.ids";
    	PropertyUtils pus = new PropertyUtils(fileName);
    	//pus.storeIntField("cn-female", -1);
		//System.out.println(pus.restoreIntField("cn-female"));
    	ConcurrentHashMap<String, Integer> ints = pus.restoreIntFields();
    	Set<Entry<String, Integer>> entries = ints.entrySet();
    	for (Entry<String, Integer> entry : entries) {
			System.out.println(entry.getKey() + " ===> " + entry.getValue());
			ints.put(entry.getKey(), -1);
		}
    	pus.storeIntFields(ints);
	}
	
}
