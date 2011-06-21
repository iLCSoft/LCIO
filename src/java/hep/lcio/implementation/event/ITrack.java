package hep.lcio.implementation.event;

import hep.lcio.event.Track;
import hep.lcio.event.TrackState;
import hep.lcio.event.TrackerHit;

import java.util.ArrayList;
import java.util.Collections;

import java.util.List;

/**
 *
 * @author Tony Johnson
 */
public class ITrack extends ILCObject implements Track
{
   private static final int[] null0 = new int[0];

   //private static final float[] null3 = new float[3];         // stored in TrackState
   //private static final float[] null15 = new float[15];       // stored in TrackState
   //protected float d0;                                        // stored in TrackState
   //protected float phi;                                       // stored in TrackState
   //protected float omega;                                     // stored in TrackState
   //protected float z0;                                        // stored in TrackState
   //protected float tanLambda;                                 // stored in TrackState
   //protected float[] covMatrix = null15;                      // stored in TrackState
   //protected float[] referencePoint = null3;                  // stored in TrackState

   protected int type;
   protected float chi2;
   protected float dEdx;
   protected float dEdxError;
   protected int ndf;
   protected List tracks = new ArrayList();
   protected List trackStates = new ArrayList();
   protected List hits = new ArrayList();
   protected float radiusOfInnermostHit;
   
   //public final static int BITISREFERENCEPOINTPCA = 31;
   protected int[] subdetectorHitNumbers = null0;
   
   public float getD0()
   {
      //return d0;
      return ((ITrackState)trackStates.get(0)).getD0();
   }
   public void setD0(float d0)
   {
      //checkAccess();
      //this.d0 = d0;
      if (trackStates.size() == 0){
          // create a first TrackState for backwards compatibility
          trackStates.add( new ITrackState() ) ;
      }
      if (trackStates.size() != 1){
          throw new IllegalArgumentException("trying to use setD0 within Track object containing more than one TrackState.");
      }
      ((ITrackState)trackStates.get(0)).setD0( d0 ) ;
   }

   public float getPhi()
   {
      //return phi;
      return ((ITrackState)trackStates.get(0)).getPhi();
   }
   public void setPhi(float phi)
   {
      //checkAccess();
      //this.phi = phi;
      if (trackStates.size() == 0){
          // create a first TrackState for backwards compatibility
          trackStates.add( new ITrackState() ) ;
      }
      if (trackStates.size() != 1){
          throw new IllegalArgumentException("trying to use setPhi within Track object containing more than one TrackState.");
      }
      ((ITrackState)trackStates.get(0)).setPhi( phi ) ;
   }
   
   public float getOmega()
   {
      //return omega;
      return ((ITrackState)trackStates.get(0)).getOmega();
   }
   public void setOmega(float f)
   {
      //checkAccess();
      //omega = f;
      if (trackStates.size() == 0){
          // create a first TrackState for backwards compatibility
          trackStates.add( new ITrackState() ) ;
      }
      if (trackStates.size() != 1){
          throw new IllegalArgumentException("trying to use setOmega within Track object containing more than one TrackState.");
      }
      ((ITrackState)trackStates.get(0)).setOmega( f ) ;
   }
 
   public float getZ0()
   {
      //return z0;
      return ((ITrackState)trackStates.get(0)).getZ0();
   }
   public void setZ0(float z0)
   {
      //checkAccess();
      //this.z0 = z0;
      if (trackStates.size() == 0){
          // create a first TrackState for backwards compatibility
          trackStates.add( new ITrackState() ) ;
      }
      if (trackStates.size() != 1){
          throw new IllegalArgumentException("trying to use setZ0 within Track object containing more than one TrackState.");
      }
      ((ITrackState)trackStates.get(0)).setZ0( z0 ) ;
   }
  
   public float getTanLambda()
   {
      //return tanLambda;
      return ((ITrackState)trackStates.get(0)).getTanLambda();
   }
   public void setTanLambda(float f)
   {
      //checkAccess();
      //tanLambda = f;
      if (trackStates.size() == 0){
          // create a first TrackState for backwards compatibility
          trackStates.add( new ITrackState() ) ;
      }
      if (trackStates.size() != 1){
          throw new IllegalArgumentException("trying to use setTanLambda within Track object containing more than one TrackState.");
      }
      ((ITrackState)trackStates.get(0)).setTanLambda( f ) ;
   }

   public float[] getCovMatrix()
   {
      //return covMatrix;
      return ((ITrackState)trackStates.get(0)).getCovMatrix();
   }
   public void setCovMatrix(float[] covMatrix)
   {
      //checkAccess();
      //if (covMatrix.length != 15) throw new IllegalArgumentException("covMatrix.length != 15");
      //this.covMatrix = covMatrix;
      if (trackStates.size() == 0){
          // create a first TrackState for backwards compatibility
          trackStates.add( new ITrackState() ) ;
      }
      if (trackStates.size() != 1){
          throw new IllegalArgumentException("trying to use setCovMatrix within Track object containing more than one TrackState.");
      }
      ((ITrackState)trackStates.get(0)).setCovMatrix( covMatrix ) ;
   }
  
   public float[] getReferencePoint()
   {
      //return referencePoint;
      return ((ITrackState)trackStates.get(0)).getReferencePoint();
   }
   public void setReferencePoint(float[] referencePoint)
   {
      //checkAccess();
      //if (referencePoint.length != 3) throw new IllegalArgumentException("referencePoint.lenhgth != 3");
      //this.referencePoint = referencePoint;
      if (trackStates.size() == 0){
          // create a first TrackState for backwards compatibility
          trackStates.add( new ITrackState() ) ;
      }
      if (trackStates.size() != 1){
          throw new IllegalArgumentException("trying to use setReferencePoint within Track object containing more than one TrackState.");
      }
      ((ITrackState)trackStates.get(0)).setReferencePoint( referencePoint ) ;
   }

   public List getTrackerHits()
   {
      return hits == null ? Collections.EMPTY_LIST : hits;
   }
   
   public List getTracks()
   {
      return tracks == null ? Collections.EMPTY_LIST : tracks;
   }

   public List getTrackStates()
   {
      return trackStates == null ? Collections.EMPTY_LIST : trackStates;
   }

   public TrackState getClosestTrackState( float x, float y, float z )
   {
        TrackState closest = (TrackState)trackStates.get(0) ;

        float [] refP = ((ITrackState)trackStates.get(0)).getReferencePoint() ;
        double shortest_distance_square = Math.pow( ( x - refP[0] ) , 2 ) + Math.pow( ( y - refP[1] ) , 2 ) + Math.pow( ( z - refP[2] ) , 2 ) ;
        double current_distance_square = 0 ;

        for( int i=1 ; i < trackStates.size() ; i++ ){
            refP = ((ITrackState)trackStates.get(i)).getReferencePoint() ;
            current_distance_square = Math.pow( ( x - refP[0] ) , 2 ) + Math.pow( ( y - refP[1] ) , 2 ) + Math.pow( ( z - refP[2] ) , 2 ) ;
            if( current_distance_square < shortest_distance_square ){
                closest = (TrackState)trackStates.get(i) ;
                shortest_distance_square = current_distance_square ;
            }   
        }
        return closest ;
   }
 
   public TrackState getTrackState( int location )
   {
       // FIXME check for valid location
      for( int i=0 ; i<trackStates.size() ; i++ ){
        if( ((ITrackState)trackStates.get(i)).getLocation() == location ){
            return (TrackState)trackStates.get(i) ;
        }
      }
      return null ;
   }
 
   public int getType()
   {
      return type;
   }
   
   public float getChi2()
   {
      return chi2;
   }
   
   public float getdEdx()
   {
      return dEdx;
   }
   
   public float getdEdxError()
   {
      return dEdxError;
   }

   public void setChi2(float chi2)
   {
      checkAccess();
      this.chi2 = chi2;
   }
 
   public void setDEdx(float dEdx)
   {
      checkAccess();
      this.dEdx = dEdx;
   }
   
   public void setDEdxError(float dEdxError)
   {
      checkAccess();
      this.dEdxError = dEdxError;
   }
  
   protected void setType(int type)
   {
      checkAccess();
      this.type = type;
   }
  
   public int getNdf()
   {
      return ndf;
   }
   
   public void setNdf(int i)
   {
      checkAccess();
      ndf = i;
   }
   
   //public boolean isReferencePointPCA()
   //{
   //   return bitTest(type,BITISREFERENCEPOINTPCA);
   //}
   //
   //public void setReferencePointPCA(boolean b)
   //{
   //   type = bitSet(type,BITISREFERENCEPOINTPCA, b);
   //}
   public void setTypeBit(int bit)
   {
      setTypeBit(bit,true) ;
   }
   public void setTypeBit(int bit, boolean val)
   {
      checkAccess();
      type = bitSet(type,bit,val);
   }  
   public void setTracks(List tracks)
   {
      checkAccess();
      this.tracks = tracks;
   }
   
   public void setTrackerHits(List hits)
   {
      checkAccess();
      this.hits = hits;
   }
   
   public float getRadiusOfInnermostHit()
   {
      return radiusOfInnermostHit;
   }
   
   public void setRadiusOfInnermostHit(float f)
   {
      checkAccess();
      radiusOfInnermostHit = f;
   }
   
   public int[] getSubdetectorHitNumbers()
   {
      return subdetectorHitNumbers ;
   }
   
   public void setSubdetectorHitNumbers(int[] is)
   {
      checkAccess();
      subdetectorHitNumbers = is;
   }
}
