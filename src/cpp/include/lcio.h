// -*- C++ -*-
#ifndef LCIO_NAMESPACE_H
#define LCIO_NAMESPACE_H 1


/*Declaration of the namespace lcio. Also serves as a source for package/namespace 
 * documentation with doxygen. 
 * 
 * @author gaede
 * @version Mar 10, 2003
 * @see LCEvent
 */
#include "EVENT/LCIO.h"
#include "IOIMPL/LCFactory.h"


/** \mainpage <a href="http://www-it.desy.de/physics/projects/simsoft/lcio/index.html">LCIO</a> (v00-09) 
 * A persistency framework for linear collider simulation studies.
 * \section Overview
 * This is the API documentation for the C++ version of LCIO. For general documetation on 
 * LCIO see the <a href="http://www-it.desy.de/physics/projects/simsoft/lcio/index.html" target="_blank">homepage</a>.
 * There you can also find the API documentation of the Java version.
 * \section Usage
 * A good starting point to browse this documentation is the <a href="./namespaces.html">Namespace List</a>.
 * As all data entities inherit from DATA::LCObject, the corresponding class documentation shows the complete
 * hierarchy of all objects that can be stored with LCIO.
 * The namespace IO contains the abstract interfaces for reading and writing data.
 */

/** The Data namespace holds all base interfaces of data entities.
 * These interfaces are used to write the data, i.e. all user classes 
 * that implement them can be made persistent with LCIO.
 */
namespace DATA{};

/** The EVENT namespace holds all base interfaces of the event.
 *  These interfaces extend the bare data interfaces with convienient 
 *  methods for analysis. They are used when lcio events/data are read in.
 */
namespace EVENT{};

/** The IO namespace holds the base interfaces for io of data. 
 */
namespace IO{};

/** The IMPL namespace holds all default implementations of
 * the base interfaces that are defined in EVENT.
 */
namespace IMPL{};




/** The LCIO namespace combines DATA, EVENT, IO and IMPL for user convenience. 
 */
namespace lcio{

  using namespace DATA ;

  using namespace EVENT ;

  using namespace IO ;

  using namespace IMPL ;

  using IOIMPL::LCFactory ;

} ;

/** The IOIMPL namespace holds extensions to the default implementations
 * needed for IO. For internal use only - with the exception of IOIMPL::LCFactory.
 */
namespace IOIMPL{};


/** The SIO namespace holds the persistency implementation using SIO.
 * Users should not use any of the classes defined here explicitly but through their
 * base interfaces defined in IO.
 */
namespace SIO{};


#endif // LCIO_NAMESPACE_H
