#ifndef SIO_SIOCOLLECTIONHANDLER_H
#define SIO_SIOCOLLECTIONHANDLER_H 1

#include <string>
#include "EVENT/LCCollection.h"
#include "IOIMPL/LCEventIOImpl.h"
#include "Exceptions.h"

#include "SIO_block.h"


namespace SIO {
  
  class SIOObjectHandler;
  
/** Handler for LCCollection/LCCollectionIOVec objects for SIO.
 * 
 * @author gaede
 * @version $Id: SIOCollectionHandler.h,v 1.8 2005-04-15 08:37:42 gaede Exp $
 */

  class SIOCollectionHandler : public SIO_block{
    
  private:
    SIOCollectionHandler() ;  // no default c'tor
    
  public:
    
    /** The default constructor needs the name, the type and a pointer 
     * the address of the collection. Throws an exception if handler
     * for given type doesn't exist.
     * @throws Exception
     */
    SIOCollectionHandler(const std::string& name, 
			 const std::string& type, 
			 IOIMPL::LCEventIOImpl**  anEvtP=0 );

    virtual ~SIOCollectionHandler() ;

    const std::string &getTypeName() const;

    // interface from SIO_block
    virtual unsigned int   xfer( SIO_stream*, SIO_operation, unsigned int ) ;
    virtual unsigned int   version() ;
    
    void setCollection(const EVENT::LCCollection *col) ; 
    void setEvent(IOIMPL::LCEventIOImpl**  anEvtP) ; 
    
    
  private: 
    IOIMPL::LCEventIOImpl**  _evtP ;    // adress of the event that data is read into 
    const EVENT::LCCollection *_col ;   // for writing we use the data interface
    
    std::string _myType ;
    SIOObjectHandler* _myHandler  ;
    
  }; // class
  
} // namespace
#endif /* ifndef SIO_SIOCOLLECTIONHANDLER_H */
