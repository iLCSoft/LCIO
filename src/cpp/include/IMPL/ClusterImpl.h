// -*- C++ -*-
#ifndef IMPL_CLUSTERIMPL_H
#define IMPL_CLUSTERIMPL_H 1


#include "EVENT/Cluster.h"
#include "AccessChecked.h"
#include "IMPL/LCRefVec.h"
#include <map>
#include <bitset>

#define NERRPOS 6
#define NERRDIR 3

#define NSHAPE_OLD 6

namespace IMPL {


/** Implementation of Cluster.
 *
 * @see Cluster
 * @author gaede
 * @version $Id: ClusterImpl.h,v 1.13.16.1 2009-11-20 15:12:04 gaede Exp $
 */
  class ClusterImpl : public EVENT::Cluster, public AccessChecked {
    
  public: 
    
    /** Default constructor, initializes values to 0.
     */
    ClusterImpl() ;
    
    /// Destructor.
    virtual ~ClusterImpl() ; 

    virtual int id() const { return simpleUID() ; }

    /** Flagword that defines the type of cluster. Bits 0-15 can be used to denote the subdetectors
     *  that have contributed hits to the cluster. The definition of the bits has to be done 
     *  elsewhere, e.g. in the run header. Bits 16-31 are used internally.
     */
    virtual int getType() const ;

    /** Energy of the cluster.
     */
    virtual float getEnergy() const ;

    /** Position of the cluster.
     */
    virtual const float* getPosition() const ;

    /** Covariance matrix of the position (6 Parameters)
    */
    virtual const EVENT::FloatVec & getPositionError() const ;

    /** Intrinsic direction of cluster at position: Theta.
     */
    virtual float getITheta() const ;

    /** Intrinsic direction of cluster at position: Phi.
     */
    virtual float getIPhi() const ;

    /** Covariance matrix of the direction (3 Parameters)
     */
    virtual const EVENT::FloatVec & getDirectionError() const ;

    /** Shape parameters - check/set  collection parameter
     *  ClusterShapeParameters for size and names of parameters.
     */
    virtual const EVENT::FloatVec & getShape() const ;

//     /** Type hypotheses: 3 Parameters: compatible with EM, HAD, muon cluster
//      */
//     virtual const EVENT::FloatVec & getParticleType() const ;


    /** The particle Id's sorted by their probability.
     * @see ParticleID
     */
    virtual const EVENT::ParticleIDVec & getParticleIDs() const ;


    /** The clusters that have been combined to this cluster.
     */
    virtual const EVENT::ClusterVec & getClusters() const ;


    /** The hits that have been combined to this cluster.
     */
    virtual const EVENT::CalorimeterHitVec & getCalorimeterHits() const ;

    /** Returns the energy contribution of the hits 
     * Runs parallel to the CalorimeterHitVec from getCalorimeterHits()
     */
    virtual const EVENT::FloatVec & getHitContributions() const ;


    /** A vector that holds the energy observed in a particular subdetectors.
     *  The mapping of indices to subdetectors is implementation dependent.
     *  To be used as convenient information or if hits are not stored in 
     *  the data set, e.g. DST or FastMC. 
     *  Check/set collection parameter ClusterSubdetectorNames for decoding the
     *  indices of the array.
     */
    virtual const EVENT::FloatVec & getSubdetectorEnergies() const ;

    //setters 
    void setTypeBit( int index , bool val=true) ;
    void setEnergy(float energy ) ;
    void setPosition(float* position) ;
    void setPositionError(const EVENT::FloatVec &errpos) ;
    void setPositionError(const float* errpos) ;
    void setITheta(float theta) ;
    void setIPhi(float phi) ;
    void setDirectionError(const EVENT::FloatVec &errdir) ;
    void setDirectionError(const float* errdir) ;
    //    void setShape(const float* shape) ;
    void setShape(const EVENT::FloatVec &shape) ;

    void addParticleID( EVENT::ParticleID*  pid ) ;

//     void setEMWeight(float emWeight ) ;
//     void setHADWeight(float hadWeight ) ;
//     void setMuonWeight(float muonWeight ) ;

    void addCluster(EVENT::Cluster* cluster) ;
    void addHit(EVENT::CalorimeterHit* hit  , float contribution) ;


    /** To be used for modifying the subdetector energies, e.g.<br>
     *  clu->subdetectorEnergies().resize(3) ;
     *  clu->subdetectorEnergies()[0] = 123.4567 ;
     *  clu->subdetectorEnergies()[1] = 12.34567 ;
     *  clu->subdetectorEnergies()[2] = 1.234567 ;
     */
    EVENT::FloatVec& subdetectorEnergies() ;

    void ptrToIndex() {
      _clusters.ptrToIndex() ;
      _hits.ptrToIndex() ;
    }

  protected:
    void setType(int type ) ; 

    //    int _type ;
    //    std::string _type ;
    std::bitset<32> _type ;
    float _energy ;
    float _position[3] ;
    EVENT::FloatVec _errpos ;
    float _theta;
    float _phi ;
    EVENT::FloatVec _errdir ;
    EVENT::FloatVec _shape ;
    EVENT::ParticleIDVec _pid ;
    //    EVENT::ClusterVec _clusters ;
    mutable LCRefVec<EVENT::Cluster*> _clusters ;
    //    EVENT::CalorimeterHitVec _hits ;
    mutable LCRefVec<EVENT::CalorimeterHit*> _hits ;
    EVENT::FloatVec _weights ;
    EVENT::FloatVec _subdetectorEnergies ;

}; // class

} // namespace IMPL
#endif /* ifndef IMPL_CLUSTERIMLP_H */
