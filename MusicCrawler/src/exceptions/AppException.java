package exceptions;

public class AppException extends Exception {

	private static final long serialVersionUID = 1546705656349831652L;
	
	public AppException() {	
		super();
	}
	
	public AppException(String message) {
		super(message);
	}
	
	public AppException(Throwable cause) {
		super(cause);
	}
	
	public AppException(String message, Throwable cause) {
		super(message, cause);
	}

}
