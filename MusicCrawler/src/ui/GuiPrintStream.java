package ui;

import java.io.OutputStream;
import java.io.PrintStream;

import javax.swing.SwingUtilities;
import javax.swing.text.JTextComponent;

import app.Constants;

public class GuiPrintStream extends PrintStream {

	private static int lines = 0;
	
	private JTextComponent component = null;
	private StringBuffer sb = new StringBuffer();

	public GuiPrintStream(OutputStream out, JTextComponent component) {
		super(out);
		this.component = component;
	}

	/**
	 * 重写write()方法，将输出信息填充到GUI组件。
	 * 
	 * @param buf
	 * @param off
	 * @param len
	 */
	@Override
	public void write(byte[] buf, int off, int len) {
		final String message = new String(buf, off, len);
		SwingUtilities.invokeLater(new Runnable() {
			@Override
			public void run() {
				sb.append(message);
				component.setText(sb.toString());
				if (++lines > Constants.MAX_LINE) {
					sb.delete(0, sb.length());
					lines = 0;
				}
			}
		});
	}

}
