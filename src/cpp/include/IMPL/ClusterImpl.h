#ifndef IMPL_CLUSTERIMPL_H
#define IMPL_CLUSTERIMPL_H 1


#include "EVENT/Cluster.h"
//#include "EVENT/CalorimeterHit.h"
#include "AccessChecked.h"
#include <map>


#define NERRPOS 6
#define NERRDIR 3
#define NSHAPE 6

namespace IMPL {


//   /**Internal helper struct to store indices and weights in map
//    */
//   struct WeightedIndices {
//     EVENT::IntVec* Indices ;
//     EVENT::FloatVec* Weights ;
//   } ;
//   typedef std::map< std::string , WeightedIndices* > IndexMap ; 


/** Implementation of Cluster.
 *
 * @see Cluster
 * @author gaede
 * @version Mar 26, 2003
 */
  class ClusterImpl : public EVENT::Cluster, public AccessChecked {
    
  public: 
    
    /** Default constructor, initializes values to 0.
     */
    ClusterImpl() ;
    
    /// Destructor.
    virtual ~ClusterImpl() ; 

    /** Type of cluster:<br>
     *  ------------- detector:(bits31-16): ECAL,HCAL,COMBINED,LAT,LCAL
     *  Cluster::UNKNOWN <br> 
     *  Cluster::ECAL <br> 
     *  Cluster::HCAL <br> 
     *  Cluster::COMBINED <br> 
     *  Cluster::LAT <br> 
     *  Cluster::LCAL <br> 
     *	--------- track cluster link (bits15-0):
     *  Cluster::NEUTRAL <br> 
     *  Cluster::CHARGED <br> 
     *  Cluster::UNDEFINED <br> 
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
    virtual float getTheta() const ;

    /** Intrinsic direction of cluster at position: Phi.
     */
    virtual float getPhi() const ;

    /** Covariance matrix of the direction (3 Parameters)
     */
    virtual const EVENT::FloatVec & getDirectionError() const ;

    /** Shape parameters (6 Parameters) - TO DO: definition
     */
    virtual const EVENT::FloatVec & getShape() const ;

    /** Type hypotheses: 3 Parameters: compatible with EM, HAD, muon cluster
     */
    virtual const EVENT::FloatVec & getParticleType() const ;


//     /** Returns the names of the hit collections that have been
//      *  used to create the cluster.
//      */
//     virtual const EVENT::StringVec & getHitCollectionNames() const ;

//     /** Returns the hit indices for the given collection name.
//      */
//     virtual const EVENT::IntVec & getHitIndicesForCollection(const std::string & colName) const ;

//     /** Returns the energy contribution from the hits in the given collection.
//      * Runs parallel to the IntVec from getHitIndicesForCollection()
//      */
//     virtual const EVENT::FloatVec & getHitContributionsForCollection(const std::string & colName) const ;
    
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

    //setters 
    void setType(int type ) ; 
    void setEnergy(float energy ) ;
    void setPosition(float* position) ;
    void setPositionError(const EVENT::FloatVec &errpos) ;
    void setPositionError(const float* errpos) ;
    void setTheta(float theta) ;
    void setPhi(float phi) ;
    void setDirectionError(const EVENT::FloatVec &errdir) ;
    void setDirectionError(const float* errdir) ;
    void setShape(const float* shape) ;
    void setShape(const EVENT::FloatVec &shape) ;

    void setEMWeight(float emWeight ) ;
    void setHADWeight(float hadWeight ) ;
    void setMuonWeight(float muonWeight ) ;
    void addCluster(EVENT::Cluster* cluster) ;

    void addHit(EVENT::CalorimeterHit* hit  , float contribution) ;

    //    void  addHitIndex( const std::string& colName, int index , float contribution) ;


  protected:
    int _type ;
    float _energy ;
    float _position[3] ;
    EVENT::FloatVec _errpos ;
    float _theta;
    float _phi ;
    EVENT::FloatVec _errdir ;
    EVENT::FloatVec _shape ;
    EVENT::FloatVec _particletype;
//     mutable  EVENT::StringVec _hitCollectionNames ;
//     mutable IndexMap _indexMap ;
    EVENT::ClusterVec _clusters ;
    EVENT::CalorimeterHitVec _hits ;
    EVENT::FloatVec _weights ;

}; // class

}; // namespace IMPL
#endif /* ifndef IMPL_CLUSTERIMLP_H */
