// -*- C++ -*-
#ifndef IOIMPL_LCFactory_h
#define IOIMPL_LCFactory_h 1


#include "IO/ILCFactory.h"
#include "IO/LCReader.h"
#include "IO/LCWriter.h"

namespace IOIMPL {

/**Factory to create LCWriter and Reader object for the known data 
 * formats, e.g. SIO. Singleton class 
 * that knows all concrete reader/writer implementations.
 * 
 * @author gaede
 * @version Mar 10, 2003
 */
class LCFactory : public IO::ILCFactory {


protected: 
    LCFactory() ; 

public: 

  /** Returns the instance of the factory.
   * In the future we need to specify the data format here...
   * This doesn't work for java as an interface can't have static members ...
   */
  static LCFactory* getInstance() ;
  
  /// Destructor.
  virtual ~LCFactory() ;
  

  /**Creates an LCWriter object for the current persistency type.
   */
  virtual IO::LCWriter * createLCWriter() ;
  
  /** Creates an LCReader object for the current persistency type.
   * lcReaderFlag: configuration options for the LCReader object -
   * combine multible options with '|'. So far only LCReader::directAccess.
   */
  virtual IO::LCReader * createLCReader(int lcReaderFlag=0 ) ;


private:
  
  static LCFactory * _me ;

}; // class
} // namespace IOIMPL
#endif /* ifndef IOIMPL_LCFactory_h */
