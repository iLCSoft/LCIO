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
#include "IOIMPL/LCFactory.h"


/** \mainpage LCIO
 * A persistency framework for linear collider simulation studies.
 */

/** The EVENT namespace holds all base interfaces of data entities.
 */
namespace EVENT{};

/** The IO namespace holds the base interfaces for io of data. 
 */
namespace IO{};

/** The IMPL namespace holds all default implementations of
 * the base interfaces that are defined in EVENT.
 */
namespace IMPL{};

/** The LCIO namespace combines EVENT, IO and IMPL for user convenience. 
 */
namespace lcio{

  using namespace EVENT ;

  using namespace IO ;

  using namespace IMPL ;

  using IOIMPL::LCFactory ;

} ;

/** The SIO namespace holds the persistency implementation using SIO.
 * Users should not use any of the classes defined here explicitly but through their
 * base interfaces defined in IO.
 */
namespace SIO{};


#endif // LCIO_NAMESPACE_H
