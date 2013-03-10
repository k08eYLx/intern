package index.calculator;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;

import nbeans.Singer;

public class SingersExtCalculator extends TableExtCalculator implements Runnable {

	private static final String TABLE     = "Singers";
	private static final String EXT_TABLE = "Singers_Ext";
	private static final String ID_COL    = "Singer_id";
	
	public SingersExtCalculator() {
		count = getRecordCount(TABLE, ID_COL, EXT_TABLE, 0);
	}
	
	public SingersExtCalculator(CyclicBarrier barrier) {
		super(barrier);
		count = getRecordCount(TABLE, ID_COL, EXT_TABLE, 0);
		System.out.println(this.getClass().getName() + "\t ===> " + count);
	}

	@Override
	public void doWork() {
		int startId = 0;
		Singer singer = new Singer();
		for (int i = 0; i < count; i += STEP) {
			System.out.println(this.getClass().getName() + "\t ===> " + i);
			ResultSet rSet = getRecords(TABLE, ID_COL, EXT_TABLE, startId);
			try {
				while (rSet.next()) {
					singer.fromRset(rSet);
					//System.out.println(singer);
					calculate(singer);
					singer.saveToExt();
				}
			}
			catch (SQLException sqle) {
				sqle.printStackTrace();
			}
			startId = singer.getId();
		}
	}

	@Override
	public void run() {
		System.out.println(TABLE);
		doWork();
		System.out.println(EXT_TABLE);
		if (barrier != null) {
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
	}

	public static void main(String[] args) {
		//System.out.println((new CalculateSingersExt()).count);
		/*Singer record = new Singer();
		record.setName("刘备");
		(new CalculateSingersExt()).calculate();
		System.out.println(record.getfCnCharId());
		System.out.println(record.getsCnCharId());
		System.out.println(record.gettCnCharId());*/
		(new SingersExtCalculator()).doWork();
	}

}
