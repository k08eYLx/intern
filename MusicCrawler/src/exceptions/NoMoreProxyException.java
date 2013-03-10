package exceptions;

public class NoMoreProxyException extends AppException {

	private static final long serialVersionUID = 4988322123062334129L;

	private static final String NMPE_MESSAGE = "No more available proxy!";
		
		public NoMoreProxyException() {
			super(NMPE_MESSAGE);
		}
		
}
