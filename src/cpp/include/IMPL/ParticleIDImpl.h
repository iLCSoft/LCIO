// -*- C++ -*-
#ifndef IMPL_PARTICLEIDIMPL_H
#define IMPL_PARTICLEIDIMPL_H 1

#include "EVENT/ParticleID.h"
#include "AccessChecked.h"
#include <functional>

namespace IMPL {


  /** Helper class to sort ParticleIDs wrt. their loglikelihood.
   */
  class PIDSort : public std::binary_function<EVENT::ParticleID*,EVENT::ParticleID*,bool>{
  public:
    bool operator()(const EVENT::ParticleID* p1, const EVENT::ParticleID* p2){
      return p1->getLoglikelihood() > p2->getLoglikelihood() ;
    }
  };


/** Implementation of ParticleID.
 *
 * @see ParticleID
 * @author gaede
 * @version $Id: ParticleIDImpl.h,v 1.6 2004-09-16 07:15:31 gaede Exp $
 */

  class ParticleIDImpl : public EVENT::ParticleID, public AccessChecked {
    
  public: 
    
    /** Default constructor, initializes values to 0.
     */
    ParticleIDImpl() ;
    
    /// Destructor.
    virtual ~ParticleIDImpl() ; 

    virtual int id() { return simpleUID() ; }

    /** Type - userdefined.
     */
    virtual int getType() const ;

    /** The PDG code of this id - UnknownPDG ( 999999 ) if unknown.
     */
    virtual int getPDG() const ;

    /**The log likelihood  of this hypothesis
     */
    virtual float getLoglikelihood() const ;

    /**Name of the algorithm/module that created this hypothesis.
     */
    virtual const std::string& getIdentifier() const ;

    /**Parameters associated with this hypothesis.
     */
    virtual const EVENT::FloatVec & getParameters() const ;

    // setters
    void setType( int type ) ;
    void setPDG( int pdg ) ;
    void setLoglikelihood( float logL ) ;
    void setIdentifier(std::string identifier ) ;
    void addParameter( float p ) ;
    
  protected:
    int _type ;
    int _pdg ;
    float _loglikelihood ;
    std::string _identifier ;
    EVENT::FloatVec _parameters ;

}; // class

}; // namespace IMPL
#endif /* ifndef IMPL_PARTICLEIDIMLP_H */
