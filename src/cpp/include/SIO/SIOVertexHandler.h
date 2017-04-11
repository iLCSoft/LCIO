#ifndef SIO_SIOVERTEXHANDLER_H
#define SIO_SIOVERTEXHANDLER_H 1

#include "SIO/SIOObjectHandler.h"
// #include "LCIOSTLTypes.h"
#include "EVENT/LCParameters.h"

#include <map>
#include <set>
#include <string>

namespace SIO {
    
    
/** Implementation of SIOObjectHandler to handle IO of Vertexes.
 *
 * @author gaede, engels
 * @version $Id: SIOVertexHandler.h,v 1.3 2006-12-06 19:09:18 hvogt Exp $
 */

  class SIOVertexHandler : public SIOObjectHandler {
	
  public:

    virtual ~SIOVertexHandler() ;
    
    virtual unsigned int init( SIO_stream* stream,
                             SIO_operation op,
                             EVENT::LCCollection* col,
                             unsigned int vers ) ;
	
    /** Reads lcio Vertex objects from an SIO stream.
     */
    virtual unsigned int read(SIO_stream* stream, EVENT::LCObject** objP)  ;
	
    /** Writes lcio Vertex objects to an SIO stream.
     */
    virtual unsigned int write(SIO_stream* stream, const EVENT::LCObject* obj) ;

  protected:
    std::map<int,std::string> imr{}; //indexmap for reading
    std::set<std::string> _set{};
    EVENT::StringVec parameters{}; //needed for putting the collection parameters into the STL Containers
    
  }; // class
} // namespace

#endif /* ifndef SIO_SIOVERTEXHANDLER_H */
