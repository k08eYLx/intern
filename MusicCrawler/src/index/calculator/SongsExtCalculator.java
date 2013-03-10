package index.calculator;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;

import app.Tracer;

import nbeans.Song;

public class SongsExtCalculator extends TableExtCalculator implements Runnable {

	private static final String TABLE     = "Songs";
	private static final String EXT_TABLE = "Songs_Ext";
	private static final String ID_COL    = "Song_id";
	
	public SongsExtCalculator() {
		count = getRecordCount(TABLE, ID_COL, EXT_TABLE, 0);
	}
	
	public SongsExtCalculator(CyclicBarrier barrier) {
		super(barrier);
		count = getRecordCount(TABLE, ID_COL, EXT_TABLE, 0);
		Tracer.trace(this.getClass().getName(), count);
	}
	
	
	@Override
	public void doWork() {
		int startId = 0;
		Song song = new Song();
		for (int i = 0; i < count; i += STEP) {
			Tracer.trace(this.getClass().getName(), i);
			ResultSet rSet = getRecords(TABLE, ID_COL, EXT_TABLE, startId);
			try {
				while (rSet.next()) {
					song.fromRset(rSet);
					Tracer.trace(song);
					calculate(song);
					Tracer.trace(song);
					song.saveToExt();
				}
			}
			catch (SQLException sqle) {
				sqle.printStackTrace();
			}
			startId = song.getId();
		}
	}

	@Override
	public void run() {
		doWork();
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
		(new SongsExtCalculator()).doWork();
	}

}
