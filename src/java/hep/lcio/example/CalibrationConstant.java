/*
 * Created on Feb 28, 2005
 */
package hep.lcio.example;

import hep.lcio.event.LCGenericObject;
import hep.lcio.event.LCObject;
import hep.lcio.implementation.event.ILCGenericObject;

/** Example for a user class based on LCGenericObject.<br>
 * Note: Once we switch to Java 1.5 we can provide a templated class
 * for fixed size LCGenericObjects that makes it easier for the users
 * to implement their classes.
 * @author gaede
 */
public class CalibrationConstant implements LCGenericObject {

	static final int NINT=1 ;
	static final int NFLOAT=2;
	static final int NDOUBLE=0;
	
	protected ILCGenericObject obj=null ;
	
	/** Convenience c'tor */
	public CalibrationConstant(int cellId, float offset, float gain){
	  obj = new ILCGenericObject(NINT,NFLOAT,NDOUBLE) ;
	  obj.setIntVal(cellId,0) ;
	  obj.setFloatVal(offset,0);
	  obj.setFloatVal(gain,1) ;
	}
	/** Convenience wrapper for LCGenericObjects read from file*/
	public CalibrationConstant(LCObject obj) throws Exception {
	    // should dop some safety checks here:
		boolean typeCheck = true ;
		if( obj instanceof ILCGenericObject ){
	      this.obj = (ILCGenericObject) obj ;
	      if(this.obj.getNInt() != NINT || 
	      	 this.obj.getNFloat() != NFLOAT ||
			 this.obj.getNDouble() != NDOUBLE )
             typeCheck = false ;
		}
	    else if( obj instanceof CalibrationConstant )
	       this.obj = ((CalibrationConstant)obj).obj ;
	    else
	    	typeCheck = false ;
		
	    if( ! typeCheck )	
	    	throw new Exception( "CalibrationConstant: cannot be instantiated from sth."
	    		             +" other than CalibrationConstant") ;
	}
	/** CellID */
	int getCellID() { return obj.getIntVal(0) ; }
    /** Offset */	
	float getOffset() { return obj.getFloatVal(0) ; }
	/** Gain */
	float getGain() { return obj.getFloatVal(1) ; }
	
	// ----- implement the LCGenericObject interface ----------
	public int getNInt() { return NINT; }
	public int getNFloat() { return NFLOAT ;}
	public int getNDouble() { return NDOUBLE; }

	public int getIntVal(int index) {
		return obj.getIntVal(index);
	}
	public float getFloatVal(int index) {
		return obj.getFloatVal(index);
	}
	public double getDoubleVal(int index) {
		return obj.getDoubleVal(index);
	}
	public boolean isFixedSize() {
		return true ;
	}
	public String getTypeName() {
		return "CalibrationConstant" ;
	}
	public String getDataDescription() {
		return "i:cellID,f:offset,f:gain" ;
	}
    // ------- end  LCGenericObject interface -------------
}
