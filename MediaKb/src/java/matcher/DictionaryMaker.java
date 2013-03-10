package matcher;

import com.lerss.utils.Trad2Simp;
import db.H2Manager;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;

public class DictionaryMaker {

    class Runner implements Runnable {

        private String sql = "";
        private NameIdsDict dict = null;
        private NameIdsDict dictFed = null;
        private CyclicBarrier barrier = null;

        Runner(String sql, NameIdsDict dict, NameIdsDict dictFed, CyclicBarrier barrier) {
            this.sql = sql;
            this.dict = dict;
            this.dictFed = dictFed;
            this.barrier = barrier;
        }

        @Override
        public void run() {
            initDictFromDB(sql, dict, dictFed);
            await(barrier);
        }

    }

	// 由于名称可能有相同的情况，将id存入ArrayList
	private static NameIdsDict songNameDict = new NameIdsDict();
	private static NameIdsDict singerNameDict = new NameIdsDict();

    // filtered 将标点字符过滤掉的字典
    private static NameIdsDict songNameDictFed = new NameIdsDict();
    private static NameIdsDict singerNameDictFed = new NameIdsDict();

    private static final DictionaryMaker instance = new DictionaryMaker();
    private DictionaryMaker() {
        System.out.println("Initial dict ...");
        long start = System.currentTimeMillis();
        initDict();
        long finish = System.currentTimeMillis();
        System.out.println("Initial time ===> " + (finish - start));
    }

    public static NameIdsDict getSongNameDict() {
		return songNameDict;
	}
	
	public static NameIdsDict getSingerNameDict() {
		return singerNameDict;
	}

    public static NameIdsDict getSongNameDictFed() {
        return songNameDictFed;
    }

    public static NameIdsDict getSingerNameDictFed() {
        return singerNameDictFed;
    }

    public void await(CyclicBarrier barrier) {
        try {
            barrier.await();
        }
        catch (InterruptedException ie) {
            ie.printStackTrace();
        }
        catch (BrokenBarrierException bbe) {
            bbe.printStackTrace();
        }
    }

    public void insertWord(String str, int id, NameIdsDict dict) {
        List<Integer> ids = null;
        str = str.trim();
        // 繁体字转成简体字，小写字母转成大写字母
        str = Trad2Simp.convert(str).toUpperCase();
        if (!(dict.containsKey(str))) {
            ids = new ArrayList<Integer>();
        }
        else {
            ids = dict.get(str);
        }
        ids.add(id);
        dict.put(str, ids);
    }

	public void insertWord(String str, int id, NameIdsDict dict, NameIdsDict dictFed) {
		if (str != null) {
            if (!str.equals("")) {
                str = Preprocessor.rmAdditionalInfo(str);
                insertWord(str, id, dict);
			    str = Filter.process(str);
			    insertWord(str, id, dictFed); // 过滤处理后的key
            }
		}
	}

	public void initDictFromDB(String sql, NameIdsDict dict, NameIdsDict dictFed) {
		try {
            System.out.println(sql);
            // DriverManager.getConnection(MYSQL_URL, USER_NAME, PASSWORD);
            Connection connection = H2Manager.getConnection();
			ResultSet resultSet = connection.createStatement().executeQuery(sql);
			while (resultSet.next()) {
                String name = resultSet.getString(2);
                int id = resultSet.getInt(1);
                System.out.println(name + " ===> " + id);
				insertWord(name, id, dict, dictFed);
			}
			connection.close();
		}
		catch (SQLException sqle) {
			sqle.printStackTrace();
		}
	}

    /**
     * 初始化词典    大概 2 ~ 3 min
     */
	public void initDict() {
        // songs, singers
        String[] tables = { "song", "singer" };
        int parties = tables.length + 1;
        CyclicBarrier barrier = new CyclicBarrier(parties);

        Thread thread = null;
        String sql = "";
		String format = "SELECT %s_id, %s_name FROM %ss";

        String song   = tables[0];
        sql = String.format(format, song, song, song);
        Runner songRunner = new Runner(sql, songNameDict, songNameDictFed, barrier);
        thread = new Thread(songRunner);
        thread.start();

        String singer = tables[1];
        sql = String.format(format, singer, singer, singer);
        Runner singerRunner = new Runner(sql, singerNameDict, singerNameDictFed, barrier);
        thread = new Thread(singerRunner);
        thread.start();

        await(barrier);
    }

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		NameIdsDict songNameDictFed = DictionaryMaker.getSongNameDictFed();
		NameIdsDict singerNameDictFed = DictionaryMaker.getSingerNameDictFed();
		System.out.println(songNameDictFed.size());
		System.out.println(singerNameDictFed.size());
		System.out.println(songNameDictFed.containsKey("爱冒险"));
		System.out.println(singerNameDictFed.containsKey("刘德华"));
		System.out.println(songNameDictFed.get("爱冒险"));
		System.out.println(singerNameDictFed.get("刘德华"));
	}

}
