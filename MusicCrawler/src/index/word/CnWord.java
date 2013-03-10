package index.word;

import java.util.ArrayList;
import java.util.Iterator;

import app.Constants;

public class CnWord implements Comparable<CnWord> {

	private ArrayList<CnChar> chars = new ArrayList<CnChar>();
		
	public CnWord(String str) {
		int len = str.length();
		for (int i = 0; i < len; i++) {
			CnChar c = new CnChar("" + str.charAt(i));
			chars.add(c);
		}
	}
	
	public CnChar get(int index) {
		return chars.get(index);
	}
	
	public long getCharId(int index) {
		if (index >= chars.size()) {
			return Constants.ILLEGAL_VALUE;
		}
		return get(index).getId();
	}
	
	public int size() {
		return chars.size();
	}
	
	@Override
	public int compareTo(CnWord o) {
		int tsize = this.size();
		int osize = o.size();
		int size = Math.min(tsize, osize);
		for (int i = 0; i < size; i++) {
			int tmp = chars.get(i).compareTo(o.get(i));
			if (tmp != 0) {
				return tmp;
			}
		}
		return (tsize - osize);
	}

	@Override
	public String toString() {
		String str = "";
		for (Iterator<CnChar> iter = chars.iterator(); iter.hasNext();) {
			CnChar c = iter.next();
			str += c.getCn();
		}
		return str;
	}
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		CnWord gc = new CnWord("张宇");
		CnWord lz = new CnWord("章鱼");
		System.out.println(gc.get(0).getUniInt());
		System.out.println(lz.get(0).getUniInt());
		System.out.println(gc.compareTo(lz));
	}

}
