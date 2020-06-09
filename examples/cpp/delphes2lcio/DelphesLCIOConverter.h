#ifndef DelphesLCIOConverter_h
#define DelphesLCIOConverter_h


class TTree;

namespace IO{ class LCWriter ; }
namespace EVENT{ class LCEvent ; }
namespace IMPL{ class LCEventImpl ; }


/** \class DelphesLCIOConverter
 *
 *  Class handling output of ROOT tree in LCIO
 *
 *  \author F.Gaede, DESY
 *  \date June 2020
 *
 */
class DelphesLCIOConverter {
public:

  ///c'tor intializes an LCWriter if valid non-empty file name given
  DelphesLCIOConverter(const char* fileName = "" );
  ~DelphesLCIOConverter();


  /// convert the Delphes tree to LCIO collections and add them to the event
  void convertTree2LCIO( TTree *tree , IMPL::LCEventImpl* evt) ;

  /// create a new LCIO event and write it to the writer (if valid)
  void writeEvent(TTree* tree) ;


  
private:
  IO::LCWriter *fWriter=nullptr;
};

#endif /* DelphesLCIOConverter */
