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
 * @version $Id: ParticleIDImpl.h,v 1.7 2004-09-16 10:46:34 gaede Exp $
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

    /** Type of the algorithm/module that created this hypothesis. 
     * Check/set collection parameters PIDAlgorithmTypeName and PIDAlgorithmTypeID.
     */
    virtual  int getAlgorithmType() const ;

    /** Parameters associated with this hypothesis.
     * Check/set collection paramter PIDParameterNames for decoding the indices.
     */
    virtual const EVENT::FloatVec & getParameters() const ;

    // setters
    void setType( int type ) ;
    void setPDG( int pdg ) ;
    void setLoglikelihood( float logL ) ;
    void setAlgorithmType(int algorithmType ) ;
    void addParameter( float p ) ;
    
  protected:
    int _type ;
    int _pdg ;
    float _loglikelihood ;
    int _algorithmType ;
    EVENT::FloatVec _parameters ;

}; // class

}; // namespace IMPL
#endif /* ifndef IMPL_PARTICLEIDIMLP_H */
