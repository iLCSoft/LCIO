#include "RIO/RIOWriter.h" 

#include "EVENT/LCEvent.h"
#include "EVENT/LCRunHeader.h"
#include "EVENT/LCIO.h"
#include "EVENT/LCCollection.h"

#include "LCIOSTLTypes.h"

#include "EVENT/MCParticle.h"
#include "EVENT/SimCalorimeterHit.h"
#include "EVENT/SimTrackerHit.h"

#include "IMPL/LCIOExceptionHandler.h"


//#define DEBUG 1
#include "IMPL/LCTOOLS.h"

#include "IMPL/LCRelationImpl.h" 

#include <cstring>

//#define ROOTIO_FILE_EXTENSION ".lcio.root"
#define ROOTIO_FILE_EXTENSION ".rlcio"

#include "TFile.h"
#include "TTree.h"

#include "RIO/RIOLCCollectionHandler.h"
//#include "TSystem.h"
//#include "TClassTable.h"


using namespace EVENT ;
using namespace IO ;
using namespace IMPL ;

namespace RIO {

  RIOWriter::RIOWriter() : 
    _compressionLevel(-1) ,
    _file(0),
    _tree(0),_evtImpl(0),
    _haveBranches( false ) {
    
    	// file needs 


#ifdef DEBUG
#else
#endif
    
    LCIOExceptionHandler::createInstance() ;
  }
  

  RIOWriter::~RIOWriter(){
  }

  void RIOWriter::open(const std::string & filename) throw(IOException, std::exception){
    
    std::string rioFilename ;  
    getRIOFileName( filename, rioFilename ) ;
    
    // if the file exists we throw an exception
    _file = new TFile( rioFilename.c_str() , "NEW");    

    if( _file->IsOpen() ){ 

      _file->Close() ;      

      throw IOException( std::string( "[RIOWriter::open()] File already exists: " 
    				      + rioFilename
				      + " \n              open it in append or new mode !\n"
				      )) ;
    }

    // open new file for writing
    return open( filename, EVENT::LCIO::WRITE_NEW ) ;
  }


  void RIOWriter::getRIOFileName(const std::string& filename, 
				 std::string& rioFilename ) {
    
    
    if( filename.rfind(ROOTIO_FILE_EXTENSION) == std::string::npos ||  // .lcio.root not found at all
	!(  filename.rfind(ROOTIO_FILE_EXTENSION)
	    + strlen( ROOTIO_FILE_EXTENSION ) == filename.length() ) ) {  // found, but not at end 
      
      rioFilename = filename + ROOTIO_FILE_EXTENSION ;
    } 
    else 
      
      rioFilename = filename ;    
  } 
  
  void RIOWriter::open(const std::string& filename, int writeMode) throw(IOException, std::exception) {
    
    
    // make sure filename has the proper extension (.slcio) 
    std::string rioFilename ;  

    getRIOFileName( filename, rioFilename ) ;
    

    // does ROOT have some rules about valid names for streams, records,  ???
    //     std::string stream_name = LCRIO::getValidRIOName(rioFilename) ;
    //     _stream = RIO_streamManager::add(  stream_name.c_str() , 32*RIO_KBYTE*RIO_KBYTE ) ;
    //     if( _stream == 0 )
    //       throw IOException( std::string( "[RIOWriter::open()] Bad or duplicate stream name: " 
    //  				      + stream_name  )) ;
        
    
    //    unsigned int  status = 0  ;


    //    std::cout << " ******** open ( " << filename << ", " << writeMode << ") "  << std::endl ;

    switch( writeMode ) 
      {
      case EVENT::LCIO::WRITE_NEW : 

	_file = new TFile( rioFilename.c_str() , "RECREATE" , " LCIO file " , _compressionLevel );
	_tree = new TTree("LCIO", "lcio tree");
	break ;
	
      case EVENT::LCIO::WRITE_APPEND : 
	
	_file = new TFile( rioFilename.c_str() , "UPDATE" , " LCIO file " , _compressionLevel );
	_tree = (TTree*) _file->Get("LCIO") ;

	if( _tree == 0 ) {

	  throw IOException( std::string( "[RIOWriter::open()]  LCIO tree not found in file: " 
				      +  rioFilename ) ) ;
	} else {

	  _file->Delete( "LCIO;1") ; // delete the existing old cycle
	}

	break ;
      }
    
    if( _file->IsZombie() )
      throw IOException( std::string( "[RIOWriter::open()] zombie root file : " 
				      +  rioFilename ) ) ;
    
    
    if( !_file->IsOpen() )
      throw IOException( std::string( "[RIOWriter::open()] Couldn't open file: " 
				      +  rioFilename ) ) ;


  }
  

  void RIOWriter::setCompressionLevel(int level) {
    _compressionLevel = level ;
  }


  void RIOWriter::writeRunHeader(const EVENT::LCRunHeader * hdr)  throw(IOException, std::exception) {

//     // create a new handler for every new run 
    
// //     if( !_runHandler){
// //       _runHandler = dynamic_cast<RIORunHeaderHandler*> 
// // 	( RIO_blockManager::get( LCRIO::RUNBLOCKNAME  ) ) ;
      
// //       if( _runHandler == 0 ) 
// // 	_runHandler = new RIORunHeaderHandler( LCRIO::RUNBLOCKNAME  ) ;
      
//     _runRecord->disconnect(LCRIO::RUNBLOCKNAME ) ;
//     _runRecord->connect( _runHandler );
// //     }
//    _file->Close() ;



//     _runHandler->setRunHeader(  hdr ) ;
    
//     if( _stream->getState()== RIO_STATE_OPEN ){
      
//       _hdrRecord->setCompress( _compressionLevel != 0 ) ;
//       _evtRecord->setCompress( _compressionLevel != 0 ) ;
//       _runRecord->setCompress( _compressionLevel != 0 ) ; 

//       // write LCRunHeader record
//       unsigned int status =  _stream->write( LCRIO::RUNRECORDNAME    ) ;
      
//       if( !(status & 1)  )
// 	throw IOException( std::string( "[RIOWriter::writeRunHeader] couldn't write run header to stream: "
// 					+  *_stream->getName() ) ) ;
//     } else {
      
//       throw IOException( std::string( "[RIOWriter::writeRunHeader] stream not opened: "
// 				      +  *_stream->getName() ) ) ;
      
//     }

  }



  /** Creates Handlers needed for writing the event on this stream.
   * Needs to be called for every event.
   */
  void RIOWriter::setUpHandlers(const LCEvent * evt){
   
    if( !_haveBranches ) {
      

      // first we create a branch for the event (header) 
      
      TBranch* br = (TBranch*) _tree->GetBranch( "LCEvent" ) ;
      
      if( br != 0 ){  // branch allready exists -> update/append  mode 
	
	br->SetAddress( &_evtImpl ) ;
	
      } else {
	
	//FIXME: make split level and 'record size' parameters ....
	_tree->Branch( "LCEvent"  , &_evtImpl, 16000, 2 );
      }



      // loop over all collections in first event ...
      typedef std::vector< std::string > StrVec ; 

      const StrVec* strVec = evt->getCollectionNames() ;

      for(  StrVec::const_iterator name = strVec->begin() ; name != strVec->end() ; name++){
	
	LCCollection* col = evt->getCollection( *name ) ;
	
	//	const LCParameters&  params =  col->getParameters()  ;
	std::string typeName = col->getTypeName() ;

	std::cout << " registering collection " << *name << " of " <<  typeName <<  std::endl ;

	_branches.push_back(  new RIO::RIOLCCollectionHandler( *name, typeName, _tree) ) ;	 

// 	//FIXME: these should be held by  a singleton handler manager (registry) 
// 	if( typeName == LCIO::MCPARTICLE ){

// 	  _branches.push_back(  new RIO::RIOLCCollectionHandler<EVENT::MCParticle>(name->c_str() ,_tree) ) ;	  
// 	}      
// 	if( typeName == LCIO::SIMCALORIMETERHIT ){

// 	  _branches.push_back(  new RIO::RIOLCCollectionHandler<EVENT::SimCalorimeterHit>(name->c_str() ,_tree) ) ;	  
// 	}      
// 	if( typeName == LCIO::SIMTRACKERHIT ){

// 	  _branches.push_back(  new RIO::RIOLCCollectionHandler<EVENT::SimTrackerHit>(name->c_str() ,_tree) ) ;	  
// 	}      
	

	// ToDo:  add all other LCIO types ....

      }

      _haveBranches = true ;
    }
  }
  
  
  void RIOWriter::writeEvent(const LCEvent* evt)  throw(IOException, std::exception) {
    
    std::cout <<  " RIOWriter::writeEvent : " << std::endl ;
    
    if( !_haveBranches ) {
      
      setUpHandlers( evt ) ;
    }    
    

    _evtImpl = dynamic_cast<const IMPL::LCEventImpl*> ( evt ) ;


    for( BranchVector::iterator it=_branches.begin() ; it!=_branches.end() ; ++it){

      (*it)->toBranch( evt ) ;
    }

    _tree->Fill() ;
    _file->Flush() ;


    //     //here we set up the collection handlers 
    //     try{   setUpHandlers( evt) ;
    
    //     }catch(...){
    //       throw IOException(  "[RIOWriter::writeEvent] could not set up handlers " ) ;
    //     }
    
    //     _hdrRecord->setCompress( _compressionLevel != 0 ) ;
    //     _evtRecord->setCompress( _compressionLevel != 0 ) ;
    //     _runRecord->setCompress( _compressionLevel != 0 ) ; 
    
    //     if( _stream->getState()== RIO_STATE_OPEN ){
    
    //       // need to set the event in event header handler
    //       _hdrHandler->setEvent( evt ) ;
    
    //       unsigned int  status = 0  ;
    
    //       // write LCEventHeader record
    //       status =  _stream->write( LCRIO::HEADERRECORDNAME    ) ;
    
    //       if( ! (status & 1) )
    // 	throw IOException(  std::string("[RIOWriter::writeEvent] couldn't write event header to stream: "
    // 					+  *_stream->getName() )) ;
    
    
    //       // write the event record
    //       status =  _stream->write( LCRIO::EVENTRECORDNAME    ) ;
    
    //       if( ! (status & 1) )
    // 	throw IOException(  std::string("[RIOWriter::writeEvent] couldn't write event header to stream: "
    // 					+  *_stream->getName() )) ;
    //     }
    //     else      
    
    //       throw IOException(  std::string("[RIOWriter::writeEvent] stream not opened : "
    // 				      +  *_stream->getName()  )) ;
    
  }
  
  
  void RIOWriter::close() throw (IOException, std::exception) {
    
    
    _file->Write() ;
    _file->Close() ;
    
    //     if(! (status &1) ) 
    //       throw IOException(  std::string("[RIOWriter::close] couldn't close stream  : "
    // 				      + *streamName  )) ;
    
  }
  
  void RIOWriter::flush() throw (IOException, std::exception) {
    
    _file->Flush() ;
    
    
    //     const std::string* streamName  = _stream->getName() ;
    //     int status =  _stream->flush() ;
    //     if(! (status &1) ) 
    //       throw IOException(  std::string("[RIOWriter::flush] couldn't flush stream  : "
    // 				      + *streamName  )) ;
    
  }
  
} // namespace

