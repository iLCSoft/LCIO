#ifndef IMPL_CLUSTERIMPL_H
#define IMPL_CLUSTERIMPL_H 1


#include "EVENT/Cluster.h"
//#include "EVENT/CalorimeterHit.h"
#include "AccessChecked.h"
#include <map>
#include <bitset>

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

    virtual int id() { return simpleUID() ; }

    /** Flagword that defines the type of cluster. Bits 0-15 can be used to denote the subdetectors
     *  that have contributed hits to the cluster. The definition of the bits has to be done 
     *  elsewhere, e.g. in the run header. Bits 16-31 are used internally.
     *  @see testType()
     */
    virtual int getType() const ;

    /** Returns true if the corresponding bit in the type word is set.
     */
    virtual bool testType(int bitIndex) const ;


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


    virtual const EVENT::FloatVec & getSubdetectorEnergies() const ;

    //setters 
    void setTypeBit( int index , bool val=true) ;
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

    /** To be used for modifying the subdetector energies, e.g.<br>
     *  clu->subdetectorEnergies().resize(3) ;
     *  clu->subdetectorEnergies()[2] = 1234.567 ;
     */
    EVENT::FloatVec& subdetectorEnergies() ;

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
    EVENT::FloatVec _particletype;
    EVENT::ClusterVec _clusters ;
    EVENT::CalorimeterHitVec _hits ;
    EVENT::FloatVec _weights ;
    EVENT::FloatVec _subdetectorEnergies ;

}; // class

}; // namespace IMPL
#endif /* ifndef IMPL_CLUSTERIMLP_H */
