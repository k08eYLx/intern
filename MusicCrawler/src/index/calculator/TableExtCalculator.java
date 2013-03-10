package index.calculator;

import index.word.CnWord;

import java.sql.ResultSet;
import java.util.concurrent.CyclicBarrier;

import nbeans.Record;

import utils.db.SqlUtils;

public abstract class TableExtCalculator {
	
	protected static final int STEP = 50;
	
	protected int count = 0;
	
	protected static final String SELECT = "SELECT * FROM %s WHERE (%s) LIMIT %d";
	protected static final String wClaussFormat = "%s not in (select %s from %s) and (%s > %d)";
	
	protected CyclicBarrier barrier = null;
	
	public TableExtCalculator() { }
	
	public TableExtCalculator(CyclicBarrier barrier) {
		this.barrier = barrier;
	}
	
	public int getRecordCount(String table, String colName, String extTable, int value) {
		int count = 0;
		String wClause = String.format(wClaussFormat, colName, colName, extTable, colName, value);
		count = SqlUtils.getCount(table, wClause);
		return count;
	}
	
	public ResultSet getRecords(String table, String colName, String extTable, int value) {
		String wClause = String.format(wClaussFormat, colName, colName, extTable, colName, value);
		String sql = String.format(SELECT, table, wClause, STEP);
		return SqlUtils.select(sql);
	}
	
	public abstract void doWork();
	
	public void calculate(Record record) {
		CnWord cnWord = new CnWord(record.getName());
		record.setfCnCharId(cnWord.getCharId(0));
		record.setsCnCharId(cnWord.getCharId(1));
		record.settCnCharId(cnWord.getCharId(2));
	}
}
