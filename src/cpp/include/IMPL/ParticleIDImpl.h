// -*- C++ -*-
#ifndef IMPL_PARTICLEIDIMPL_H
#define IMPL_PARTICLEIDIMPL_H 1

#include "EVENT/ParticleID.h"
#include "AccessChecked.h"
#include <functional>

namespace IMPL {


  /** Helper class to sort ParticleIDs wrt. their probability
   */
  class PIDSort : public std::binary_function<EVENT::ParticleID*,EVENT::ParticleID*,bool>{
  public:
    bool operator()(const EVENT::ParticleID* p1, const EVENT::ParticleID* p2){
      return p1->getProbability() > p2->getProbability() ;
    }
  };


/** Implementation of ParticleID.
 *
 * @see ParticleID
 * @author gaede
 * @version Mar 30, 2003
 */

  class ParticleIDImpl : public EVENT::ParticleID, public AccessChecked {
    
  public: 
    
    /** Default constructor, initializes values to 0.
     */
    ParticleIDImpl() ;
    
    /// Destructor.
    virtual ~ParticleIDImpl() ; 

    virtual int id() { return simpleUID() ; }

    /** TypeID, if applicable this is the PDG code of the particle.
     */
    virtual int getTypeID() const ;

    /**The probability of this hypothesis (0.<=p<=1.). 
     */
    virtual float getProbability() const ;

    /**Name of the algorithm/module that created this hypothesis.
     */
    virtual const std::string& getIdentifier() const ;

    /**Parameters associated with this hypothesis.
     */
    virtual const EVENT::FloatVec & getParameters() const ;

    // setters
    void setTypeID( int typeID ) ;
    void setProbability( float probability ) ;
    void setIdentifier(std::string identifier ) ;
    void addParameter( float p ) ;
    
  protected:
    int _typeID ;
    float _probability ;
    std::string _identifier ;
    EVENT::FloatVec _parameters ;

}; // class

}; // namespace IMPL
#endif /* ifndef IMPL_PARTICLEIDIMLP_H */
