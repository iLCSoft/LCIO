// -*- C++ -*-
#ifndef LCIO_NAMESPACE_H
#define LCIO_NAMESPACE_H 1


/**Declaration of the namespace lcio.
 * 
 * @author gaede
 * @version Mar 10, 2003
 * @see LCEvent
 */
#include "IOIMPL/LCFactory.h"

namespace EVENT{};
namespace IO{};
namespace IMPL{};

namespace lcio{

  using namespace EVENT ;

  using namespace IO ;

  using namespace IMPL ;

  using IOIMPL::LCFactory ;

} ;

#endif // LCIO_NAMESPACE_H
