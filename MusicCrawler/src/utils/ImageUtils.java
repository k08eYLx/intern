package utils;



public class ImageUtils {

	private ImageUtils() { }
	
	// java net
	public static boolean netFetchImg(String imgUrl, String outName) {
		return NetUtils.download(imgUrl, outName);
	}
	
}
