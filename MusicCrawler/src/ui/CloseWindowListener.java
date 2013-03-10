package ui;

import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;

import app.Tracer;

public class CloseWindowListener implements WindowListener {

	@Override
	public void windowOpened(WindowEvent e) {
		Tracer.trace("windowOpened");
	}

	@Override
	public void windowClosing(WindowEvent e) {
		Tracer.trace("windowClosing");
	}

	@Override
	public void windowClosed(WindowEvent e) {
		Tracer.trace("windowClosed");
	}

	@Override
	public void windowIconified(WindowEvent e) {
		Tracer.trace("windowIconified");
	}

	@Override
	public void windowDeiconified(WindowEvent e) {
		Tracer.trace("windowDeiconified");
	}

	@Override
	public void windowActivated(WindowEvent e) {
		Tracer.trace("windowActivated");
	}

	@Override
	public void windowDeactivated(WindowEvent e) {
		Tracer.trace("windowDeactivated");
	}

}
