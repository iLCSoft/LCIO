/*
 * Created on Feb 26, 2005
 *
 */
package hep.lcio.implementation.event;

import hep.lcio.event.LCGenericObject;


/** Default Implementation of LCGenericObject.
 * 
 * @author gaede
 * @version $Id: ILCGenericObject.java,v 1.1 2005-02-28 14:49:51 gaede Exp $
 * 
 */
public class ILCGenericObject extends ILCObject implements LCGenericObject {
	
	private static int[] nullI = new int[0];
	private static float[] nullF = new float[0];
	private static double[] nullD = new double[0];

	protected boolean _isFixedSize ;
    protected int _intVec[] ;
    protected float _floatVec[] ;
    protected double _doubleVec[] ;
	
	/** Default C'tor - assumes fixedSize==false.*/
    public ILCGenericObject(){
		_isFixedSize = false ;
		_intVec = nullI ;
		_floatVec = nullF ;
		_doubleVec = nullD ;
	}
	
	/** C'tor for fixed size objects */
    public ILCGenericObject(int nInt,int nFloat,int nDouble){
		_isFixedSize = true ;
		_intVec = new int[nInt] ;
		_floatVec = new float[nFloat] ;
		_doubleVec = new double[nDouble] ;
	}
	
	
	/**
	 * @see hep.lcio.event.LCGenericObject#getNInt()
	 */
	public int getNInt() {
		return _intVec.length;
	}

	/**
	 * @see hep.lcio.event.LCGenericObject#getNFloat()
	 */
	public int getNFloat() {
		return _floatVec.length;
	}

	/**
	 * @see hep.lcio.event.LCGenericObject#getNDouble()
	 */
	public int getNDouble() {
		return _doubleVec.length ;
	}

	/**
	 * @see hep.lcio.event.LCGenericObject#getIntVal(int)
	 */
	public int getIntVal(int index) {
		return _intVec[index] ;
	}

	/**
	 * @see hep.lcio.event.LCGenericObject#getFloatVal(int)
	 */
	public float getFloatVal(int index) {
		return _floatVec[index];
	}

	/**
	 * @see hep.lcio.event.LCGenericObject#getDoubleVal(int)
	 */
	public double getDoubleVal(int index) {
		return _doubleVec[index] ;
	}

	/**
	 * @see hep.lcio.event.LCGenericObject#isFixedSize()
	 */
	public boolean isFixedSize() {
		return _isFixedSize ;
	}

	/**
	 * @see hep.lcio.event.LCGenericObject#getTypeName()
	 */
	public String getTypeName() {
		return "LCGenericObject" ;
	}

	/** 
	 * @see hep.lcio.event.LCGenericObject#getDataDescription()
	 */
	public String getDataDescription() {
		return "" ;
	}
	 
	/** Sets the int value at index - resizes the array if needed,
	 * thus it is more efficient to fill the largest index first. 
	 */
	public void setIntVal(int val,int index){
		checkAccess() ;
		if( index >= _intVec.length){
			_isFixedSize = false ;
			int[] old = _intVec ;
			_intVec = new int[ index+1 ] ;
			for (int i = 0; i < old.length; i++) {
				_intVec[i]=old[i] ;
			}
		}
		_intVec[index]=val;
	
	}

	/** Sets the float value at index - resizes the array if needed,
	 * thus it is more efficient to fill the largest index first. 
	 */
	public void setFloatVal(float val,int index){
		checkAccess() ;
		if( index >= _floatVec.length){
			_isFixedSize = false ;
			float[] old = _floatVec ;
			_floatVec = new float[ index+1 ] ;
			for (int i = 0; i < old.length; i++) {
				_floatVec[i]=old[i] ;
			}
		}
		_floatVec[index]=val;
	
	}
	/** Sets the double value at index - resizes the array if needed,
	 * thus it is more efficient to fill the largest index first. 
	 */
	public void setDoubleVal(double val,int index){
		checkAccess() ;
		if( index >= _doubleVec.length){
			_isFixedSize = false ;
			double[] old = _doubleVec ;
			_doubleVec = new double[ index+1 ] ;
			for (int i = 0; i < old.length; i++) {
				_doubleVec[i]=old[i] ;
			}
		}
		_doubleVec[index]=val;
	
	}

	/** Set all int values at once */
	public void setIntVals(int[] values ){
		checkAccess() ;
		if( values.length != _intVec.length){
			_isFixedSize = false ;
		}
		_intVec = values ;
	}
	/** Set all float values at once */
	public void setFloatVals(float[] values ){
		checkAccess() ;
		if( values.length != _floatVec.length){
			_isFixedSize = false ;
		}
		_floatVec = values ;
	}
	/** Set all double values at once */
	public void setDoubleVals(double[] values ){
		checkAccess() ;
		if( values.length != _doubleVec.length){
			_isFixedSize = false ;
		}
		_doubleVec = values ;
	}

}
