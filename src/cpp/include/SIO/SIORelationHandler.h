#ifndef SIO_SIORELATIONHANDLER_H
#define SIO_SIORELATIONHANDLER_H 1

#include <string>
#include "EVENT/LCRelation.h"
#include "IOIMPL/LCEventIOImpl.h"
#include "Exceptions.h"

#include "SIO_block.h"


namespace SIO {
  
  class SIOObjectHandler;
  
/** Handler for LCRelation/LCRelationIOImpl objects for SIO.
 * 
 * @author gaede
 * @version $Id: SIORelationHandler.h,v 1.2 2004-08-20 16:45:25 gaede Exp $
 */

  class SIORelationHandler : public SIO_block{
    
  private:
    SIORelationHandler() ;  // no default c'tor
    
  public:
    
    /** The default constructor needs the name, the type and a pointer 
     * the address of the relation. Throws an exception if handler
     * for given type doesn't exist.
     * @throws Exception
     */
    SIORelationHandler(const std::string& name, 
		       //			 const std::string& type, 
			 IOIMPL::LCEventIOImpl**  anEvtP=0 ) 
      throw (EVENT::Exception) ;

    virtual ~SIORelationHandler() ;
    
    // interface from SIO_block
    virtual unsigned int   xfer( SIO_stream*, SIO_operation, unsigned int ) ;
    virtual unsigned int   version() ;
    
    void setRelation( EVENT::LCRelation *rel) ; 
    void setEvent(IOIMPL::LCEventIOImpl**  anEvtP) ; 
    static void fillRelationMap( IOIMPL::LCEventIOImpl* evt ) ;
    
  private: 
    IOIMPL::LCEventIOImpl**  _evtP ;    // adress of the event that data is read into 
    EVENT::LCRelation *_rel ;   // for writing we use the data interface
    
    //    std::string _myType ;
    //    SIOObjectHandler* _myHandler  ;
    
  }; // class
  
}; // namespace
#endif /* ifndef SIO_SIORELATIONHANDLER_H */
