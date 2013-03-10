package app;

import java.io.File;
import java.io.IOException;

import org.ini4j.Ini;
import org.ini4j.InvalidFileFormatException;
import org.ini4j.Profile.Section;

public class Configurator {
	
	public static final String CONFIG_FILE = "data/config.ini";

	/**
	 * @param args
	 * @throws IOException 
	 * @throws InvalidFileFormatException 
	 */
	public static void main(String[] args) throws InvalidFileFormatException, IOException {
		String filename = (args.length > 0) ? args[0] : CONFIG_FILE;
		Ini ini = new Ini(new File(filename));
		
		Section dataSect = ini.get("Data");
		String dataPath = dataSect.get("path", "D:/MusicData");
		System.out.println(dataPath);
		
		Section mysqlSec = ini.get("MySQL");
		String url = mysqlSec.get("url", "");
		System.out.println(url);
		String usr = mysqlSec.get("usr", "");
		System.out.println(usr);
		String pswd = mysqlSec.get("pswd", "");
		System.out.println(pswd);
		
		Section h2Sect = ini.get("H2");
		System.out.println(h2Sect.get("url", ""));
		
		/*mysqlSect.put("usr", "usr");
		ini.store();*/
	}

}
