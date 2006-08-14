// -*- C++ -*-
#ifndef IMPL_VERTEXIMPL_H
#define IMPL_VERTEXIMPL_H 1


#include "EVENT/Vertex.h"
#include "AccessChecked.h"


#define NCOVARIANCE 10


namespace IMPL {


/** Implementation of Vertex Class.
 *
 * @see Vertex
 * @author gaede, engels
 * @version Aug 09, 2006
 */
  class VertexImpl : public EVENT::Vertex, public AccessChecked {
    
  public: 
    
    /** Default constructor, initializes values to 0.
     */
    VertexImpl() ;
    
    /// Destructor.
    virtual ~VertexImpl() ; 

    virtual int id() const { return simpleUID() ; }
    
    /** Checks if the Vertex is the primary vertex of the event.
     *  Only one primary vertex per event is allowed
     */
    virtual bool isPrimary() const ;
    
    /** Position of the vertex 
     */
    virtual const float* getPosition() const;
                                                                                                         
    /** Covariance matrix of the position (stored as lower triangle matrix, i.e.
     *  cov(xx),cov(y,x),cov(y,y) ).
     */
    virtual const EVENT::FloatVec & getCovMatrix() const;
                                                                                                         
    /** Chi squared of the vertex fit.
     */
    virtual float getChi2() const;
                                                                                                         
    /** Probability of the vertex fit.
     */
    virtual float getProbability() const;
                                                                                                         
    /** Additional parameters related to this vertex - check/set the collection
     *  parameter "VertexParameterNames" for the parameters' meaning.
     */
    virtual const EVENT::FloatVec & getParameters() const;

    /** Returns Reconstructed Particle associated to the Vertex
     */
    virtual EVENT::ReconstructedParticle * getAssociatedParticle() const;

    // setters
    void setPrimary(bool primary) ;
    void setPosition( float vpos[3] ) ;
    void setPosition( float px, float py, float pz ) ;
    void setChi2( float chi2 ) ;
    void setProbability( float probability ) ;
    void setCovMatrix( const float* cov ) ;
    void setCovMatrix( const EVENT::FloatVec & ) ;
    void setParameters( const float* par ) ;
    void setParameters( const EVENT::FloatVec& ) ;
    void setAssociatedParticle( EVENT::ReconstructedParticle * aP ) ;

  protected:
    bool _primary ;
    float _vpos[3] ;
    float _chi2 ;
    float _probability ;
    EVENT::FloatVec _cov ;
    EVENT::FloatVec _par ;
    EVENT::ReconstructedParticle* _aParticle ;
   
}; // class

} // namespace IMPL
#endif /* ifndef IMPL_VERTEXIMLP_H */
