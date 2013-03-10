package index.word;

import com.lerss.utils.CnToSpell;
import com.lerss.utils.Trad2Simp;

public class CnChar implements Comparable<CnChar> {

	private static final int BASE = 26;
	
	private String cn = "";
	
	private static final int BITS = 32;
	
	private int pyInt = 0;
	private int uniInt = 0;

	private long id = 0;

	public CnChar() { }
	
	public CnChar(String cn) {
		this.cn = cn;
		this.toInt();
	}
	
	public String getCn() {
		return cn;
	}

	public void setCn(String cn) {
		this.cn = cn;
	}

	@Override
	public int compareTo(CnChar c) {
		if (c instanceof CnChar) {
			int pyDelta = pyInt - c.pyInt;
			return ((pyDelta != 0) ? pyDelta : (uniInt - c.uniInt));
		}
		else {
			// 与非Word对象比较，则抛出异常
			throw new ClassCastException("Can't compare");
		}
	}

	public long toInt() {
		String py = CnToSpell.getFullSpell(Trad2Simp.convert(cn)).toUpperCase();
		for (int i = 0; i < py.length(); i++) {
			//pyInt = pyInt * BASE + (py.charAt(i) - 'A');
			pyInt = pyInt * BASE + py.charAt(i);
		}
		uniInt = (int)cn.charAt(0);
		// 注意 << 与 + 的优先级
		id = ((long)pyInt << BITS) + uniInt;
		return id;
	}
	
	public int getPyInt() {
		return pyInt;
	}

	public void setPyInt(int pyInt) {
		this.pyInt = pyInt;
	}

	public int getUniInt() {
		return uniInt;
	}

	public void setUniInt(int uniInt) {
		this.uniInt = uniInt;
	}

	public long getId() {
		return id;
	}

	public void setId(long id) {
		this.id = id;
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		CnChar cc = new CnChar("杨");
		System.out.println(cc.getPyInt() + " " + cc.getUniInt() + " " + cc.getId());
	}

}
