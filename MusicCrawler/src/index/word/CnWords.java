package index.word;

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
		cws.add(new CnWord("张玉"));
		cws.add(new CnWord("章鱼"));
		cws.add(new CnWord("张宇"));
		ArrayList<CnWord> words = cws.sortWords();
		for (CnWord word : words) {
			System.out.println(word);
		}
	}

}
