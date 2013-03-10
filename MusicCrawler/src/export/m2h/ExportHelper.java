package export.m2h;

import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;

import org.h2.tools.Server;

/**
 * Created with IntelliJ IDEA.
 * User: YangLX
 * Date: 13-1-15
 * Time: 下午5:35
 * To change this template use File | Settings | File Templates.
 */
public class ExportHelper {

	private static final int SIZE = 3;
	
	public static void work() {
		Server server = null;
		try {
			server = Server.createTcpServer().start();
		}
		catch (SQLException sqle) {
			sqle.printStackTrace();
		}
		
		if (server != null) {
			// +1 ，让主线程等待所有子线程结束
			CyclicBarrier barrier = new CyclicBarrier(SIZE + 1);

			List<ExportRecords> eRecords = new ArrayList<ExportRecords>(SIZE);

			eRecords.add(new ExportSingers(barrier));
			eRecords.add(new ExportAlbums(barrier));
			eRecords.add(new ExportSongs(barrier));

			Thread thread = null;
			for (ExportRecords eRecord : eRecords) {
				thread = new Thread(eRecord);
				thread.start();
			}

			mainAwait(barrier);

			server.stop();
		}
		    
	}
	
	/**
	 * 挂起主线程
	 * 
	 * @param cyclicBarrier
	 */
	private static void mainAwait(CyclicBarrier cyclicBarrier) {
		try {
			cyclicBarrier.await();
		}
		catch (BrokenBarrierException bbe) {
			bbe.printStackTrace();
		}
		catch (InterruptedException ie) {
			ie.printStackTrace();
		}
	}

    public static void main(String[] args) {
		ExportHelper.work();
    }

}
