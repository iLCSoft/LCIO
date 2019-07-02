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
    /// Constructor
    SIOVertexHandler() ;

    /// Init collection reading
    void initReading( sio::read_device &device, EVENT::LCCollection *collection, sio::version_type vers ) override ;

    /// Init collection reading
    void initWriting( sio::write_device &device, EVENT::LCCollection *collection ) override ;

    /// Reads lcio objects from an SIO stream.
    void read( sio::read_device& device, EVENT::LCObject* objP, sio::version_type vers ) override ;

    /// Writes lcio objects to an SIO stream.
    void write( sio::write_device& device, const EVENT::LCObject* obj ) override ;

    /// Factory method to create an object of the type of the collection
    EVENT::LCObject *create() const override ;

  protected:
    std::map<int,std::string>    _imr {}; //indexmap for reading
    std::set<std::string>        _set {};
    EVENT::StringVec             _parameters {}; //needed for putting the collection parameters into the STL Containers
  }; // class
} // namespace

#endif /* ifndef SIO_SIOVERTEXHANDLER_H */
