/*
 * Created on Jun 3, 2004
 *
 */
package hep.lcio.implementation.event;

import hep.lcio.event.LCObject;
import hep.lcio.event.TPCHit;
import hep.lcio.event.TrackerHit;

/**
 * @author gaede
 *
 */
public class ITrackerHit extends ILCObject implements TrackerHit {

	/* (non-Javadoc)
	 * @see hep.lcio.event.TrackerHit#getPosition()
	 */
	public double[] getPosition() {
		// TODO Auto-generated method stub
		return null;
	}

	/* (non-Javadoc)
	 * @see hep.lcio.event.TrackerHit#getCovMatrix()
	 */
	public float[] getCovMatrix() {
		// TODO Auto-generated method stub
		return null;
	}

	/* (non-Javadoc)
	 * @see hep.lcio.event.TrackerHit#getdEdx()
	 */
	public float getdEdx() {
		// TODO Auto-generated method stub
		return 0;
	}

	/* (non-Javadoc)
	 * @see hep.lcio.event.TrackerHit#getTime()
	 */
	public float getTime() {
		// TODO Auto-generated method stub
		return 0;
	}

	/* (non-Javadoc)
	 * @see hep.lcio.event.TrackerHit#getType()
	 */
	public String getType() {
		// TODO Auto-generated method stub
		return null;
	}

	/* (non-Javadoc)
	 * @see hep.lcio.event.TrackerHit#getRawDataHit()
	 */
	public LCObject getRawDataHit() {
		// TODO Auto-generated method stub
		return null;
	}

	/* (non-Javadoc)
	 * @see hep.lcio.event.TrackerHit#setPosition(double[])
	 */
	public void setPosition(double[] pos) {
		// TODO Auto-generated method stub

	}

	/* (non-Javadoc)
	 * @see hep.lcio.event.TrackerHit#setCovMatrix(float[])
	 */
	public void setCovMatrix(float[] cov) {
		// TODO Auto-generated method stub

	}

	/* (non-Javadoc)
	 * @see hep.lcio.event.TrackerHit#setdEdx(float)
	 */
	public void setdEdx(float dEdx) {
		// TODO Auto-generated method stub

	}

	/* (non-Javadoc)
	 * @see hep.lcio.event.TrackerHit#setTime(float)
	 */
	public void setTime(float time) {
		// TODO Auto-generated method stub

	}

	/* (non-Javadoc)
	 * @see hep.lcio.event.TrackerHit#setTPCHit(hep.lcio.event.TPCHit)
	 */
	public void setTPCHit(TPCHit hit) {
		// TODO Auto-generated method stub

	}

}
