package index.calculator;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;

import nbeans.Album;

public class AlbumsExtCalculator extends TableExtCalculator implements Runnable {
	
	private static final String TABLE     = "Albums";
	private static final String EXT_TABLE = "Albums_Ext";
	private static final String ID_COL    = "Album_id";
	
	public AlbumsExtCalculator() {
		count = getRecordCount(TABLE, ID_COL, EXT_TABLE, 0);
	}
	
	public AlbumsExtCalculator(CyclicBarrier barrier) {
		super(barrier);
		count = getRecordCount(TABLE, ID_COL, EXT_TABLE, 0);
		System.out.println(this.getClass().getName() + "\t ===> " + count);
	}
	
	@Override
	public void doWork() {
		int startId = 0;
		Album album = new Album();
		for (int i = 0; i < count; i += STEP) {
			System.out.println(this.getClass().getName() + "\t ===> " + i);
			ResultSet rSet = getRecords(TABLE, ID_COL, EXT_TABLE, startId);
			try {
				while (rSet.next()) {
					album.fromRset(rSet);
					//System.out.println(album);
					calculate(album);
					album.saveToExt();
				}
			}
			catch (SQLException sqle) {
				sqle.printStackTrace();
			}
			startId = album.getId();
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
		//System.out.println((new CalculateAlbumsExt()).count);
		(new AlbumsExtCalculator()).doWork();
	}

}
