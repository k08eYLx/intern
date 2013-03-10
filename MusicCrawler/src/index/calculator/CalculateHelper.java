package index.calculator;

import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;

import utils.LogUtils;

public class CalculateHelper {

	private static final int SUB_THREAD_SIZE = 3;
	
	public static void runWork() {
		LogUtils.LOGGER.info("Start!");
		CyclicBarrier barrier = new CyclicBarrier(SUB_THREAD_SIZE + 1);    // +1 ，让主线程等待所有子线程结束
		Thread thread = null; 
		SingersExtCalculator cSingersExt = new SingersExtCalculator(barrier);
		AlbumsExtCalculator  cAlbumsExt  = new AlbumsExtCalculator(barrier);
		SongsExtCalculator   cSongsExt   = new SongsExtCalculator(barrier);
		thread = new Thread(cSingersExt);
		thread.start();
		thread = new Thread(cAlbumsExt);
		thread.start();
		thread = new Thread(cSongsExt);
		thread.start();
		mainAwait(barrier);
		LogUtils.LOGGER.info("Finish!");
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
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		CalculateHelper.runWork();
	}

}
