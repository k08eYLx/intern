package app;

import nbeans.Singer;

public class Tracer {
	
	private static boolean isDebug = true;

	public static void trace(Object obj) {
		if (isDebug) {
			System.out.println(obj);	
		}
	}

	/**
	 * connect object left and right with " ===> "
	 * 
	 * @param left
	 * @param right
	 */
	public static void trace(Object left, Object right) {
		trace(left + " ===> " + right);
	}
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		Tracer.trace("Singer", new Singer());
	}

}
