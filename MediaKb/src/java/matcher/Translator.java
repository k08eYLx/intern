package matcher;

import java.io.*;
import java.util.HashMap;
import java.util.Map;
import java.util.Properties;
import java.util.Set;

/**
 * Translation for inputs.
 */
public class Translator {

	private String fileName = "";
    private Map<String, String> dictionary = null;

	public Translator(String fileName) {
		this.fileName = fileName;
        initialDictionary();
	}

	private Properties restorePorperties() {
		Properties ps = new Properties();
		try {
			File file = new File(fileName);
			if (file.exists()) {
				InputStream ins = new FileInputStream(file);
                InputStreamReader reader = new InputStreamReader(ins, "UTF-8");
				ps.load(reader);
                reader.close();
				ins.close();
			}
		}
		catch (IOException ioe) {
			ioe.printStackTrace();
		}
		return ps;
	}

    private void initialDictionary() {
        Properties ps = restorePorperties();
        Set<String> keys = ps.stringPropertyNames();
        dictionary = new HashMap<String, String>(keys.size());
        for (String key : keys) {
            String value = ps.getProperty(key);
            dictionary.put(key, value);
        }
    }

    public String translate(String str) {
        Set<String> keys = dictionary.keySet();
        for (String key : keys) {
            // ignore case
            String regex = "(?i)" + key;
            String value = dictionary.get(key);
            str = str.replaceAll(regex, value);
        }
        return str;
    }

	public static void main(String[] args) {
		String fileName = "etc/map.translate";
    	Translator translator = new Translator(fileName);
        String str = "TensioN 国语-Tension-离歌 英语-黑眼豆豆-My Humps" +
                "英语-后街男孩-Larger Than Life 英语-麦当娜-Ray Of Light";
        System.out.println(translator.translate(str));
	}
	
}
