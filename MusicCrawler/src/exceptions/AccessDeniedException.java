package exceptions;

public class AccessDeniedException extends AppException {

	private static final long serialVersionUID = 1893198504817277896L;
	
	// http://verify.baidu.com/vcode?+{URL}
	private static final String ADE_MESSAGE = "Access denied!";    // "您的访问出错了";
	
	public AccessDeniedException() {
		super(ADE_MESSAGE);
	}
	
}
