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


/** \mainpage <a href="http://lcio.desy.de">LCIO</a> (v01-00) 
 * A persistency framework for linear collider simulation studies.
 * \section Overview
 * This is the API documentation for the C++ version of LCIO. For general documetation on 
 * LCIO see the <a href="http://lcio.desy.de" target="_blank">homepage</a>.
 * There you can also find the API documentation of the Java version.
 * \section Usage
 * A good starting point to browse this documentation is the <a href="./namespaces.html">Namespace List</a>.
 * As all data entities inherit from DATA::LCObject, the corresponding class documentation shows the complete
 * hierarchy of all objects that can be stored with LCIO.
 * The namespace IO contains the abstract interfaces for reading and writing data.
 */

/** The namespace DATA holds all base interfaces of data entities.
 * These interfaces are used to write the data, i.e. all user classes 
 * that implement them can be made persistent with LCIO.
 */
namespace DATA{};

/** The namespace EVENT holds all base interfaces of the event.
 *  These interfaces extend the bare data interfaces with convienient 
 *  methods for analysis. They are used when lcio events/data are read in.
 */
namespace EVENT{};

/** The namespace IO  holds the base interfaces for io of data. 
 */
namespace IO{};

/** The namespace IMPL holds all default implementations of
 * the base interfaces that are defined in EVENT.
 */
namespace IMPL{};

/** The namespace UTIL holds convenient and support classes and 
 *  methods for the LCIO objects.
 */
namespace UTIL{};



/** The LCIO namespace combines DATA, EVENT, IO and IMPL for user convenience. 
 */
namespace lcio{

  using namespace DATA ;

  using namespace EVENT ;

  using namespace IO ;

  using namespace IMPL ;

  using namespace UTIL ;

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
