// -*- C++ -*-
#ifndef EVENT_LCRELATIONNAVIGATORIMPL_H
#define EVENT_LCRELATIONNAVIGATORIMPL_H 1

#include "IMPL/LCRelationNavigatorBaseImpl.h"

namespace IMPL {

/** Same as LCRelationNavigatorImpl except that the order of the relations is reversed
 *  when instantiated from a collection of LCWgtRelation objects.
 *
 * @author gaede 
 * @version $Id: LCReverseRelationNavigatorImpl.h,v 1.1 2004-07-13 11:54:27 gaede Exp $
 * @see LCRelationNavigatorImpl
 */
  class LCReverseRelationNavigatorImpl : public LCRelationNavigatorBaseImpl {
    
    
  public: 
    /**Default constructor.
     */
    LCReverseRelationNavigatorImpl() { /* nop */; }

    /** Use the given collection to create this navigator object but reverse the 
     *  direction, i.e. exchange 'from' and 'to' object.
     */
    LCReverseRelationNavigatorImpl( const EVENT::LCCollection* col ) { initialize(col) ; }
    
    /// Destructor.
    virtual ~LCReverseRelationNavigatorImpl() { /* nop */; }
    
  protected:

    /** Make this class abstract
     */
    virtual void initialize( const EVENT::LCCollection* col ) ;


}; // class
} // namespace EVENT
#endif /* ifndef EVENT_LCRELATIONNAVIGATOR_H */
