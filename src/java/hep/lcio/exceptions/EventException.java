package hep.lcio.exceptions;


/**
 * An exception thrown when adding a item which already exists to an event
 * @author Tony Johnson
 * @version $Id: EventException.java,v 1.3 2007-02-02 09:34:26 gaede Exp $
 */
public class EventException extends RuntimeException {
	public EventException(String s) {
		super(s) ;
	}
}
