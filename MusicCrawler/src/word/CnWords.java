package word;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;

public class CnWords {

	private ArrayList<CnWord> words = new ArrayList<CnWord>();

	public ArrayList<CnWord> sortWords() {    
        Collections.sort(words, new Comparator<CnWord>() {
            public int compare(CnWord o1, CnWord o2) {      
                return o1.compareTo(o2);
            }
        });       
        return words;
    }
	
	public void add(CnWord cw) {
		words.add(cw);
	}
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		CnWords cws = new CnWords();
		cws.add(new CnWord("范伟"));
		cws.add(new CnWord("范怡文"));
		cws.add(new CnWord("范大伟"));
		cws.add(new CnWord("张玉"));
		cws.add(new CnWord("赵某"));
		cws.add(new CnWord("章鱼"));
		cws.add(new CnWord("张宇"));
		cws.add(new CnWord("张云"));
		cws.add(new CnWord("乌苏里"));
		cws.add(new CnWord("张宇文"));
		cws.add(new CnWord("章鱼爪"));
		cws.add(new CnWord("赵子龙"));
		ArrayList<CnWord> words = cws.sortWords();
		for (CnWord word : words) {
			System.out.println(word);
		}
		CnWord cw = new CnWord("乌苏里");
		System.out.println(cw.getCharId(0));
	}

}
