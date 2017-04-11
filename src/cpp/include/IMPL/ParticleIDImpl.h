// -*- C++ -*-
#ifndef IMPL_PARTICLEIDIMPL_H
#define IMPL_PARTICLEIDIMPL_H 1

#include "EVENT/ParticleID.h"
#include "AccessChecked.h"
#include <functional>

namespace IMPL {


  /** Helper class to sort ParticleIDs wrt. their likelihood.
   */
  struct PIDSort{
    bool operator()(const EVENT::ParticleID* p1, const EVENT::ParticleID* p2){
      return p1->getLikelihood() > p2->getLikelihood() ;
    }
  };


/** Implementation of ParticleID.
 *
 * @see ParticleID
 * @author gaede
 * @version $Id: ParticleIDImpl.h,v 1.11 2008-05-30 13:23:27 gaede Exp $
 */

  class ParticleIDImpl : public EVENT::ParticleID, public AccessChecked {
    
  public: 
    
    /** Default constructor, initializes values to 0.
     */
    ParticleIDImpl() ;
    
    /// Destructor.
    virtual ~ParticleIDImpl() ; 

    virtual int id() const { return simpleUID() ; }

    /** Type - userdefined.
     */
    virtual int getType() const ;

    /** The PDG code of this id - UnknownPDG ( 999999 ) if unknown.
     */
    virtual int getPDG() const ;

    /**The likelihood  of this hypothesis - in a user defined normalization.
     */
    virtual float getLikelihood() const ;

    /** Type of the algorithm/module that created this hypothesis - NOTE: must be unique within one 
     *  collection.  
     *  Check/set collection parameters PIDAlgorithmTypeName and PIDAlgorithmTypeID.
     */
    virtual  int getAlgorithmType() const ;

    /** Parameters associated with this hypothesis.
     * Check/set collection parameter ParameterNames_PIDAlgorithmTypeName for decoding the indices.
     */
    virtual const EVENT::FloatVec & getParameters() const ;

    /**Access to parameters associated with this hypothesis.
     */
    virtual EVENT::FloatVec& parameters() ;

    // setters
    void setType( int type ) ;
    void setPDG( int pdg ) ;
    void setLikelihood( float logL ) ;
    void setAlgorithmType(int algorithmType ) ;
    void addParameter( float p ) ;
    
  protected:
    int _type ;
    int _pdg ;
    float _likelihood ;
    int _algorithmType ;
    EVENT::FloatVec _parameters{} ;

}; // class

} // namespace IMPL
#endif /* ifndef IMPL_PARTICLEIDIMLP_H */
