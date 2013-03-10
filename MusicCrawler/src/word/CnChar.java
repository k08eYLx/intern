package word;

import com.lerss.utils.CnToSpell;

public class CnChar implements Comparable<CnChar> {


	private static final int SPELL_MAX = 6;
	private static final int BASE      = 26;
	private static final int BITS      = 32;
	
	private String cn = "";
	
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

	private int py2Int(String py) {
		int i = 0;
		for ( ; i < py.length(); i++) {
			pyInt = pyInt * BASE + (py.charAt(i) - 'A');
		}
		return i;
	}
	
	/**
	 * 比较拼音需要右侧补齐
	 * 
	 * @param i
	 * @return
	 */
	private int append(int i) {
		for ( ; i < SPELL_MAX; i++) {
			pyInt *= BASE;
		}
		return pyInt;
	}
	
	public long toInt() {
		// 按全拼需要有一个最大对齐位数，否则不一定是首字母大的值就大，因为跟拼音总位数还有关系
		String py = CnToSpell.getFullSpell(cn).toUpperCase();
		pyInt = append(py2Int(py));
		
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
