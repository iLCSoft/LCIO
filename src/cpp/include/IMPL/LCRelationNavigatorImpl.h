// -*- C++ -*-
#ifndef EVENT_LCRELATIONNAVIGATORIMPL_H
#define EVENT_LCRELATIONNAVIGATORIMPL_H 1

#include "IMPL/LCRelationNavigatorBaseImpl.h"

namespace IMPL {

/** Implementation of LCRelationNavigator that allows instantiation from 
 * a collection of LCWgtRelation objects.
 *
 * 
 * @author gaede 
 * @version $Id: LCRelationNavigatorImpl.h,v 1.1 2004-07-13 11:54:27 gaede Exp $
 * @see LCReverseRelationNavigatorImpl
 */
  class LCRelationNavigatorImpl : public LCRelationNavigatorBaseImpl {
    
    
  public: 
    /**Default constructor.
     */
    LCRelationNavigatorImpl() { /* nop */; }

    /** Use the given collection to create this navigator object.
     */
    LCRelationNavigatorImpl( const EVENT::LCCollection* col ) { initialize(col) ; }
    
    /// Destructor.
    virtual ~LCRelationNavigatorImpl() { /* nop */; }
    
  protected:

    /** Make this class abstract
     */
    virtual void initialize( const EVENT::LCCollection* col ) ;


}; // class
} // namespace EVENT
#endif /* ifndef EVENT_LCRELATIONNAVIGATOR_H */
