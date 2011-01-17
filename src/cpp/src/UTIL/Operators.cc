#include <iostream>
#include <ios> 

#include "UTIL/Operators.h"

#include "EVENT/ReconstructedParticle.h"

////////////////////////////////////////////////////////////////////////////////
// Operators for convenient printing of LCIO objects

////////////////////////////////////////////////////////////////////////////////

using namespace std;
using namespace EVENT;


//hauke: for the LCIOTOOLS.cc code
#include "EVENT/SimTrackerHit.h"
#include "UTIL/LCTOOLS.h"

#include "EVENT/TPCHit.h"
#include "EVENT/TrackerRawData.h"
#include "EVENT/TrackerData.h"
#include "EVENT/TrackerPulse.h"
#include "EVENT/LCIO.h"
#include "EVENT/MCParticle.h"
#include "EVENT/LCFloatVec.h"
#include "EVENT/LCIntVec.h"
#include "IMPL/LCFlagImpl.h"
#include "EVENT/Track.h"
#include "EVENT/Cluster.h"
#include "EVENT/ReconstructedParticle.h"
#include "EVENT/Vertex.h"
#include "EVENT/TrackerHit.h"
#include "EVENT/LCGenericObject.h"
#include "EVENT/LCRelation.h"
#include "LCIOSTLTypes.h"

#include <sstream>

#include "UTIL/LCObjectHandle.h"
#include "UTIL/LCTime.h"
#include "UTIL/CellIDDecoder.h"
#include "UTIL/PIDHandler.h"
#include <map>
#include <set>
#include <cstdio>

#include "EVENT/LCEvent.h"

typedef std::vector<std::string> LCStrVec ;

using namespace std ;
using namespace EVENT ;
using namespace IMPL ;

//end hauke


namespace UTIL{

/* 
  // EXP: INDEX MAP - UNDER DEVELOPMENT
  
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::Vertex, EVENT::LCCollection>& sV){
    const EVENT::Vertex* v = sV.obj;
    const EVENT::LCCollection* col = sV.cobj;
    
    //TODO: PROBLEM HERE!!!
    UTIL::IndexMap im(col,"AlgorithmNames","AlgorithmTypes");
    
    out << setfill('0');
    out << " [" << setw(8) << hex << v->id() << "] | " << v->isPrimary()<< " | ";
    out << setfill(' ') << setw(17) << left << im.decode( v->getAlgorithmType() ) << " | ";
    
    out << setfill('0') << right << scientific << setprecision(3) << v->getChi2() << " | " << v->getProbability() << " | " <<
	v->getPosition()[0] << "," <<
        v->getPosition()[1] << "," <<
        v->getPosition()[2] << " | [" ;

//    for(int i=0;i<VTXCOVMATRIX;i++)
//      out << v->getCovMatrix()[i] << (i<(VTXCOVMATRIX-1)?",":" | [");
    out << setw(3) << v->getParameters().size() << "] | [";
    out << setw(8) << hex << (v->getAssociatedParticle()!=NULL?v->getAssociatedParticle()->id():0) << "]\n";
    
    return out;

  }
*/



//--
//-- begin new ----
//-------------------------
  const ostream_flags saveflags(std::ostream& stream){
        ostream_flags flags;
        flags.oldf = stream.flags();
        flags.precision =  stream.precision();
        flags.fill = stream.fill();
        return(flags);
  }
  void setflags(std::ostream& stream, ostream_flags flags){
        stream.flags(flags.oldf);
        stream.precision(flags.precision);
        stream.fill(flags.fill);

  }

  const std::string& header(const EVENT::LCCollection &){ //hauke
    static std::string _vtxh("| Number of elements | Type name |    Flag     | Is transient | Is default | Is subset |\n");
    return _vtxh;
  }

  const std::string& tail(const EVENT::LCCollection &){ //hauke
    static std::string _vtxt("|--------------------|-----------|-------------|--------------|------------|-----------|\n");
    return _vtxt;
  }

  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::LCCollection>& sV){ //hauke
    const EVENT::LCCollection* hit = sV.obj;
    using namespace std;

    ostream_flags flags=saveflags(out);
    out << noshowpos;
    out << "|" << dec << setw(20) << setfill(' ') << hit->getNumberOfElements();
    out << "|" << dec << setw(11) << setfill(' ') << hit->getTypeName();
    out << "|" << hex << setw(13) << setfill(' ') << hit->getFlag();
    out << "|" << dec << setw(14) << setfill(' ') << hit->isTransient();
    out << "|" << dec << setw(12) << setfill(' ') << hit->isDefault();
    out << "|" << dec << setw(11) << setfill(' ') << hit->isSubset() << "|" << endl;
    
    setflags(out, flags);
    return out;
  }



  std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::LCCollection> l) { //hauke
    const EVENT::LCCollection *hit = l.object();
    const EVENT::LCCollection *col = l.collection();

    using namespace std;
    ostream_flags flags=saveflags(out);

    stringstream sstream;

    out << noshowpos;
    out << setw(41) << setfill('-') << right << " LCCollection " << setfill('-') << setw(29) << "-" << endl;

    if(col != NULL){
        if(col->getTypeName() != LCIO::LCCOLLECTION){
            out << "Warning: collection not of type " << LCIO::LCCOLLECTION << endl ;
            setflags(out, flags);
            return(out);
        }
        //print collection flags
    }

    //print object attributs
    out << setw(30) << setfill(' ') << left << "Number of elements" << right <<  setw(40)<< dec << hit->getNumberOfElements() << endl;
    out << setw(30) << setfill(' ') << left << "Type name"<< setfill(' ') << right << setw(40) << hit->getTypeName() << endl;
    out << setw(30) << setfill(' ') << left << "Flag"<< setfill(' ') << right << setw(40) << hex << hit->getFlag() << endl;
    out << setw(30) << setfill(' ') << left << "Is transient"<< setfill(' ') << right << setw(40) << dec << hit->isTransient()  << endl;
    out << setw(30) << setfill(' ') << left << "Is default"<< setfill(' ') << right << setw(40) << hit->isDefault() << endl;
    out << setw(30) << setfill(' ') << left << "Is subset"<< setfill(' ') << right << setw(40) << hit->isSubset() << endl;

    setflags(out, flags);
    return out;

}

  std::ostream& operator<<( std::ostream& out, const EVENT::LCCollection &hit){
    out<<lcio_long(hit,NULL);
    return out;
  }

//-------------------------
 const std::string& header(const EVENT::LCEvent &){ //hauke
    static std::string _vtxh("| Run number  |Event number |Detector name| Time stamp  |  Weight     |\n");
    return _vtxh;
  }

  const std::string& tail(const EVENT::LCEvent &){ //hauke
    static std::string _vtxt("|-------------|-------------|-------------|-------------|-------------|\n");
    return _vtxt;
  }
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::LCEvent>& sV){ //hauke
    const EVENT::LCEvent* hit = sV.obj;
    using namespace std;
    ostream_flags flags=saveflags(out);

    out << noshowpos;
    out << "|" << hex << setw(13) << setfill(' ') << hit->getRunNumber();
    out << "|" << hex << setw(13) << setfill(' ') << hit->getEventNumber();
    out << "|" << dec << setw(13) << setfill(' ') << hit->getDetectorName();
    out << "|" << dec << setw(13) << setfill(' ') << hit->getTimeStamp();
    out << "|" << dec << setw(13) << setfill(' ') << hit->getWeight() << "|" << endl;
    setflags(out, flags);

    return out;
  }


  std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::LCEvent> l) { //hauke
    const EVENT::LCEvent *hit = l.object();
    const EVENT::LCCollection *col = l.collection();

    using namespace std;
    ostream_flags flags=saveflags(out);

    stringstream sstream;

    out << noshowpos;
    out << setw(41) << setfill('-') << right << " LCEvent " << setfill('-') << setw(29) << "-" << endl;

    if(col != NULL){
        if(col->getTypeName() != LCIO::LCEVENT){
            out << "Warning: collection not of type " << LCIO::LCEVENT << endl ;
            setflags(out, flags);

            return(out);

        }
        //print collection flags
    }

    //print object attributs
    //sstream << "0x" << hex << hit->id();
    out << setw(30) << setfill(' ') << left << "Run number" << right << setw(40) <<hex << hit->getRunNumber() << endl;
    out << setw(30) << setfill(' ') << left << "Event number"<< setfill(' ') << right << setw(40) << dec << hit->getEventNumber() << endl;
    out << setw(30) << setfill(' ') << left << "Detector name"<< setfill(' ') << right << setw(40) << hit->getDetectorName() << endl;
    out << setw(30) << setfill(' ') << left << "Time stamp"<< setfill(' ') << right << setw(40) << hit->getTimeStamp() << endl;
    out << setw(30) << setfill(' ') << left << "Weight"<< setfill(' ') << right << setw(40) << hit->getWeight() << endl;

    out << setw(30) << setfill(' ') << left << "Collection Names" << endl;
    const std::vector< std::string >* strVec = hit->getCollectionNames() ;
    for(std::vector< std::string >::const_iterator name = strVec->begin() ; name != strVec->end() ; name++){
        out << "      " << "Name: " << setw(20) << left << *name;
        out << " Type: " << left << setw(10) <<  hit->getCollection( *name )->getTypeName();
        out << " Number of Elements: "<< left << setw(20) << hit->getCollection( *name )->getNumberOfElements() << endl;
    }
    out << setfill(' ') << setprecision(6)  <<  dec;
    setflags(out, flags);

    return out;
}

  std::ostream& operator<<( std::ostream& out, const EVENT::LCEvent  &hit){
    out<<lcio_long(hit,NULL);
    return out;
  }


//-------------------------
  const std::string& header(const EVENT::LCFlag &){ //hauke
    static std::string _vtxh("|    Flag    |");
    return _vtxh;
  }
  const std::string& tail(const EVENT::LCFlag &){ //hauke
    static std::string _vtxt("|------------|\n");
    return _vtxt;
  }
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::LCFlag>& sV){ //hauke
    const EVENT::LCFlag* hit = sV.obj;
    using namespace std;
    ostream_flags flags=saveflags(out);

    out << noshowpos;
    out << "|" << hex << setw(8) << setfill(' ') << hit->getFlag() << "|" << endl;
    setflags(out, flags);
    return out;
  }


  std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::LCFlag> l) { //hauke
    const EVENT::LCFlag *hit = l.object();
    const EVENT::LCCollection *col = l.collection();

    using namespace std;
    ostream_flags flags=saveflags(out);

    stringstream sstream;

    out << noshowpos;
    out << setw(41) << setfill('-') << right << " LCFlag " << setfill('-') << setw(29) << "-" << endl;

    if(col != NULL){
      // if(col->getTypeName() != LCIO::LCFLAG){
      //      out << "Warning: collection not of type " << LCIO::LCFLAG << endl ;
      //      return(out);
      //  }
      //not part of lcio?
        //print collection flags
    }

    //print object attributs
    out << setw(30) << setfill(' ') << left << "Flag" << right << hex << setw(40) << hit->getFlag() << dec << endl;

    setflags(out, flags);
    return out;
}

  std::ostream& operator<<( std::ostream& out, const EVENT::LCFlag  &hit){
    out<<lcio_long(hit,NULL);
    return out;
  }

//-------------------------
//-------------------------
  const std::string& header(const EVENT::LCGenericObject &){ //hauke
    static std::string _vtxh(" [   id   ] | Number of ints|Number of floats|     Type   |\n");
    return _vtxh;
  }
  const std::string& tail(const EVENT::LCGenericObject &){ //hauke
    static std::string _vtxt(" -------------|---------------|----------------|------------|\n");
    return _vtxt;
  }
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::LCGenericObject>& sV){ //hauke
    const EVENT::LCGenericObject* hit = sV.obj;
    using namespace std;
    ostream_flags flags=saveflags(out);

    out << noshowpos;
    out << " [" << hex << setw(8) << setfill('0') << hit->id() << "] ";
    out << " |" << dec << setw(15) << setfill(' ') << hit->getNInt();
    out << "|" << setw(16) << setfill(' ') << hit->getNDouble();
    out << "|" << setw(12) << setfill(' ') << hit->getDataDescription() << "|" << endl;


    setflags(out, flags);
    return out;
  }


  std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::LCGenericObject> l) { //hauke
    const EVENT::LCGenericObject *hit = l.object();
    const EVENT::LCCollection *col = l.collection();

    using namespace std;
    ostream_flags flags=saveflags(out);

    stringstream sstream;

    out << noshowpos;
    out << setw(41) << setfill('-') << right << " LCGenericObject " << setfill('-') << setw(29) << "-" << endl;

    if(col != NULL){
        if(col->getTypeName() != LCIO::LCGENERICOBJECT){
            out << "Warning: collection not of type " << LCIO::LCGENERICOBJECT << endl ;
            setflags(out, flags);
            return(out);

        }
        //print collection flags
    }

    //print object attributs
    sstream << "0x" << hex << hit->id();
    out << setw(30) << setfill(' ') << left << "Id" << right << setw(40) << sstream.str() << endl;
    out << setw(30) << setfill(' ') << left << "Number of integer values"<< setfill(' ') << right << setw(40) << hit->getNInt() << endl;
    out << setw(30) << setfill(' ') << left << "Number of float values"<< setfill(' ') << right << setw(40) << hit->getNDouble() << endl;
    out << setw(30) << setfill(' ') << left << "Type name"<< setfill(' ') << right << setw(40) << hit->getTypeName() << endl;
    out << setw(30) << setfill(' ') << left << "Description"<< setfill(' ') << right << setw(40) << hit->getDataDescription() << endl;

    setflags(out, flags);
    return out;
}

  std::ostream& operator<<( std::ostream& out, const EVENT::LCGenericObject  &hit){
    out<<lcio_long(hit,NULL);
    return out;
  }


//-------------------------
//-------------------------
  const std::string& header(const EVENT::LCIntVec &){ //hauke
    static std::string _vtxh(" [  Id   ] \n");
    return _vtxh;
  }
  const std::string& tail(const EVENT::LCIntVec &){ //hauke
    static std::string _vtxt("-----------\n");
    return _vtxt;
  }
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::LCIntVec>& sV){ //hauke
    const EVENT::LCIntVec* hit = sV.obj;
    using namespace std;
    ostream_flags flags=saveflags(out);

    out << noshowpos;
    out << " [" << hex << setw(8) << setfill('0') << hit->id() << "] ";

    setflags(out, flags);
    return out;
  }


  std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::LCIntVec> l) { //hauke
    const EVENT::LCIntVec *hit = l.object();
    const EVENT::LCCollection *col = l.collection();

    using namespace std;
    ostream_flags flags=saveflags(out);

    stringstream sstream;

    out << noshowpos;
    out << setw(41) << setfill('-') << right << " LCIntVec " << setfill('-') << setw(29) << "-" << endl;

    if(col != NULL){
        if(col->getTypeName() != LCIO::LCINTVEC){
            out << "Warning: collection not of type " << LCIO::LCINTVEC << endl ;
            setflags(out, flags);
            return(out);

        }
        //print collection flags
    }

    //print object attributs
    sstream << "0x" << hex << hit->id();
    out << setw(30) << setfill(' ') << left << "Id" << right << setw(40) << sstream.str() << endl;

    setflags(out, flags);
    return out;
}

  std::ostream& operator<<( std::ostream& out, const EVENT::LCIntVec  &hit){
    out<<lcio_long(hit,NULL);
    return out;
  }


//-------------------------
//-------------------------
  const std::string& header(const EVENT::LCObject &){ //hauke
    static std::string _vtxh(" [  Id   ] \n");
    return _vtxh;
  }

  const std::string& tail(const EVENT::LCObject &){ //hauke
    static std::string _vtxt("-----------\n");
    return _vtxt;
  }

  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::LCObject>& sV){ //hauke
    const EVENT::LCObject* hit = sV.obj;
    using namespace std;
    ostream_flags flags=saveflags(out);

    out << noshowpos;
    out << " [" << hex << setw(8) << setfill('0') << hit->id() << "] ";

    setflags(out, flags);
    return out;
  }



  std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::LCObject> l) { //hauke
    const EVENT::LCObject *hit = l.object();
    const EVENT::LCCollection *col = l.collection();

    using namespace std;
    ostream_flags flags=saveflags(out);

    stringstream sstream;

    out << noshowpos;
    out << setw(41) << setfill('-') << right << " LCObject " << setfill('-') << setw(29) << "-" << endl;

    if(col != NULL){
        //if(col->getTypeName() != LCIO::LCOBJECT){
        //    out << "Warning: collection not of type " << LCIO::LCOBJECT << endl ;
        //    return(out);
        //}
        //not part of lcio?

        //print collection flags
    }

    //print object attributs
    sstream << "0x" << hex << hit->id();
    out << setw(30) << setfill(' ') << left << "Id" << right << setw(40) << sstream.str() << endl;

    setflags(out, flags);
    return out;
}

  std::ostream& operator<<( std::ostream& out, const EVENT::LCObject  &hit){
    out<<lcio_long(hit,NULL);
    return out;
  }

//-------------------------
  const std::string& header(const EVENT::LCParameters &){ //hauke
    static std::string _vtxh("-----\n");
    return _vtxh;
  }
  const std::string& tail(const EVENT::LCParameters &){ //hauke
    static std::string _vtxt("-----\n");
    return _vtxt;
  }
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::LCParameters>& sV){ //hauke
    //const EVENT::LCParameters* hit = sV.obj;
    using namespace std;
    ostream_flags flags=saveflags(out);

    out << noshowpos;
    out << "| " << hex << setw(8) << setfill(' ')<< " |";

    setflags(out, flags);
    return out;
  }



  std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::LCParameters> l) { //hauke
    //const EVENT::LCParameters *params = l.object();
    const EVENT::LCCollection *col = l.collection();

    using namespace std;
    ostream_flags flags=saveflags(out);

    stringstream sstream;

    out << noshowpos;
    out << setw(41) << setfill('-') << right << " LCParameters " << setfill('-') << setw(29) << "-" << endl;

    if(col != NULL){
        //if(col->getTypeName() != LCIO::LCPARAMETERS){
        //    out << "Warning: collection not of type " << LCIO::LCPARAMETERS << endl ;
        //    return(out);
        //}
        //not part of LCIO?

        //print collection flags

    }


    // from lctools
/*
    StringVec intKeys ;
    int nIntParameters = params.getIntKeys( intKeys ).size() ;
    for(int i=0; i< nIntParameters ; i++ ){
      IntVec intVec ;
      params.getIntVals(  intKeys[i], intVec ) ;
      int nInt  = intVec.size()  ;
      out << " parameter " << intKeys[i] << " [int]: " ;

      if( nInt == 0 ){
    out << " [empty] " << std::endl ;
      }
      for(int j=0; j< nInt ; j++ ){
    out << intVec[j] << ", " ;
      }
      out << endl ;
    }
    StringVec floatKeys ;
    int nFloatParameters = params.getFloatKeys( floatKeys ).size() ;
    for(int i=0; i< nFloatParameters ; i++ ){
      FloatVec floatVec ;
      params.getFloatVals(  floatKeys[i], floatVec ) ;
      int nFloat  = floatVec.size()  ;
      out << " parameter " << floatKeys[i] << " [float]: " ;
      if( nFloat == 0 ){
    out << " [empty] " << std::endl ;
      }
      for(int j=0; j< nFloat ; j++ ){
    out << floatVec[j] << ", " ;
      }
      out << endl ;
    }
    StringVec stringKeys ;
    int nStringParameters = params.getStringKeys( stringKeys ).size() ;
    for(int i=0; i< nStringParameters ; i++ ){
      StringVec stringVec ;
      params.getStringVals(  stringKeys[i], stringVec ) ;
      int nString  = stringVec.size()  ;
      out << " parameter " << stringKeys[i] << " [string]: " ;
      if( nString == 0 ){
    out << " [empty] " << std::endl ;
      }
      for(int j=0; j< nString ; j++ ){
    out << stringVec[j] << ", " ;
      }
      out << endl ;
   }
*/
    // end

    //sstream << "0x" << hex << hit->id();
    //out << setw(30) << setfill(' ') << left << "Id" << right << setw(40) << sstream.str() << endl;
    //out << setw(30) << setfill(' ') << left << "Type"<< setfill(' ') << right << setw(40) << hit->getType() << endl;
    //out << setw(30) << left << "Energy [GeV]" << right << setw(40) << hit->getEnergy() << endl;

    setflags(out, flags);
    return out;

}

  std::ostream& operator<<( std::ostream& out, const EVENT::LCParameters  &hit){
    out<<lcio_long(hit,NULL);
    return out;
  }

//-------------------------
  const std::string& header(const EVENT::LCRunHeader &){ //hauke
    static std::string _vtxh(" [ Run number ] | Detector name | Description | Parameters |\n");
    return _vtxh;
  }
  const std::string& tail(const EVENT::LCRunHeader &){ //hauke
    static std::string _vtxt("----------------|---------------|-------------|------------|\n");
    return _vtxt;
  }
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::LCRunHeader>& sV){ //hauke
    const EVENT::LCRunHeader* hit = sV.obj;
    using namespace std;
    ostream_flags flags=saveflags(out);

    out << noshowpos;
    out << " [" << hex << setw(8) << setfill('0') << hit->getRunNumber() << "] ";
    out << "|" << dec << setw(8) << setfill(' ') << hit->getDetectorName();
    out << "|" << dec << setw(8) << setfill(' ') << hit->getDescription();
    out << "|" << dec << setw(8) << setfill(' ') << hit->getParameters() << "|" << endl;

    setflags(out, flags);
    return out;
  }



  std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::LCRunHeader> l) { //hauke
    const EVENT::LCRunHeader *hit = l.object();
    const EVENT::LCCollection *col = l.collection();

    using namespace std;
    ostream_flags flags=saveflags(out);

    stringstream sstream;

    out << noshowpos;
    out << setw(41) << setfill('-') << right << " LCRunHeader " << setfill('-') << setw(29) << "-" << endl;

    if(col != NULL){
        if(col->getTypeName() != LCIO::LCRUNHEADER){
            out << "Warning: collection not of type " << LCIO::LCRUNHEADER << endl ;
            return(out);

        }
        //print collection flags
    }

    //print object attributs
    sstream << "0x" << hex << hit->id();
    out << setw(30) << setfill(' ') << left << "Run number"<< setfill(' ') << right << setw(40) << hex << hit->getRunNumber() <<dec << endl;
    out << setw(30) << setfill(' ') << left << "Detector name"<< setfill(' ') << right << setw(40) << hex << hit->getDetectorName() <<dec << endl;
    out << setw(30) << setfill(' ') << left << "Description"<< setfill(' ') << right << setw(40) << hex << hit->getDescription() <<dec << endl;
    out << setw(30) << setfill(' ') << left << "Parameters"<< setfill(' ') << right << setw(40) << hex << hit->getParameters() <<dec << endl;

    setflags(out, flags);
    return out;
}

  std::ostream& operator<<( std::ostream& out, const EVENT::LCRunHeader  &hit){
    out<<lcio_long(hit,NULL);
    return out;
  }

//-------------------------
/*
  const std::string& header(const EVENT::LCStrVec &){ //hauke
    static std::string _vtxh("");
    return _vtxh;
  }
  const std::string& tail(const EVENT::LCStrVec &){ //hauke
    static std::string _vtxt("\n");
    return _vtxt;
  }
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::LCStrVec>& sV){ //hauke
    const EVENT::LCStrVec* hit = sV.obj;
    using namespace std;
    out << noshowpos;
    out << " [" << hex << setw(8) << setfill('0');// << hit->id() << "] ";
  }



  std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::LCStrVec> l) { //hauke
    const EVENT::LCStrVec *hit = l.object();
    const EVENT::LCCollection *col = l.collection();

    using namespace std;
    stringstream sstream;

    out << noshowpos;
    out << setw(41) << setfill('-') << right << " LCStrVec " << setfill('-') << setw(29) << "-" << endl;

    if(col != NULL){
        if(col->getTypeName() != LCIO::LCSTRVEC){
            out << "Warning: collection not of type " << LCIO::LCSTRVEC << endl ;
            return(out);

        }
        //print collection flags
    }

    //print object attributs
    sstream << "0x" << hex << hit->id();
    out << setw(30) << setfill(' ') << left << "Id" << right << setw(40) << sstream.str() << endl;
    out << setw(30) << setfill(' ') << left << "Type"<< setfill(' ') << right << setw(40) << hit->getType() << endl;
    out << setw(30) << left << "Energy [GeV]" << right << setw(40) << hit->getEnergy() << endl;
}

  std::ostream& operator<<( std::ostream& out, const EVENT::LCStrVec  &hit){
    out<<lcio_long(hit,NULL);
    return out;
  }

*/
//-------------------------
  const std::string& header(const EVENT::ParticleID &){ //hauke
    static std::string _vtxh("|   Type   |    PDG   | Likelihood |Algorithm type|");
    return _vtxh;
  }
  const std::string& tail(const EVENT::ParticleID &){ //hauke
    static std::string _vtxt("\n");
    return _vtxt;
  }
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::ParticleID>& sV){ //hauke
    const EVENT::ParticleID* hit = sV.obj;
    using namespace std;
    ostream_flags flags=saveflags(out);

    out << noshowpos;
    out << "|" << dec << setw(10) << setfill(' ') << hit->getType();
    out << "|" << dec << setw(10) << setfill(' ') << hit->getPDG();
    out << "|" << dec << setw(12) << setfill(' ') << hit->getLikelihood();
    out << "|" << dec << setw(14) << setfill(' ') << hit->getAlgorithmType();

    setflags(out, flags);
    return out;
  }


  std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::ParticleID> l) { //hauke
    const EVENT::ParticleID *hit = l.object();
    const EVENT::LCCollection *col = l.collection();

    using namespace std;
    ostream_flags flags=saveflags(out);

    stringstream sstream;

    out << noshowpos;
    out << setw(41) << setfill('-') << right << " ParticleID " << setfill('-') << setw(29) << "-" << endl;

    if(col != NULL){
        if(col->getTypeName() != LCIO::PARTICLEID){
            out << "Warning: collection not of type " << LCIO::PARTICLEID << endl ;
            setflags(out, flags);
            return(out);

        }
        //print collection flags
    }

    //print object attributs
    out << setw(30) << setfill(' ') << left << "Type"<< setfill(' ') << right << setw(40) << dec << hit->getType() << endl;
    out << setw(30) << setfill(' ') << left << "PDG"<< setfill(' ') << right << setw(40) << dec << hit->getPDG() << endl;
    out << setw(30) << setfill(' ') << left << "Likelihood  "<< setfill(' ') << right << setw(40) << dec << hit->getLikelihood() << endl;
    out << setw(30) << setfill(' ') << left << "Algorithm type"<< setfill(' ') << right << setw(40) << dec << hit->getAlgorithmType() << endl;

    setflags(out, flags);
    return out;
}

  std::ostream& operator<<( std::ostream& out, const EVENT::ParticleID  &hit){
    out<<lcio_long(hit,NULL);
    return out;
  }

//-------------------------
  const std::string& header(const EVENT::RawCalorimeterHit &){ //hauke
    static std::string _vtxh(" CellID0 | CellID1 | Amplitude | TimeStamp \n");
    return _vtxh;
  }
  const std::string& tail(const EVENT::RawCalorimeterHit &){ //hauke
    static std::string _vtxt("---------|---------|-----------|-----------\n");
    return _vtxt;
  }
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::RawCalorimeterHit>& sV){ //hauke
    const EVENT::RawCalorimeterHit* hit = sV.obj;
    using namespace std;
    ostream_flags flags=saveflags(out);

    out << noshowpos;
    out << "| " << hex << setw(8) << setfill('0') << hit->getCellID0();
    out << "| " << hex << setw(8) << setfill('0') << hit->getCellID1();
    out << "|" << dec << setw(11) << setfill(' ') << hit->getAmplitude();
    out << "|" << dec << setw(11) << setfill(' ') << hit->getTimeStamp() << "|" << endl;

    setflags(out, flags);
    return out;
  }


  std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::RawCalorimeterHit> l) { //hauke
    const EVENT::RawCalorimeterHit *hit = l.object();
    const EVENT::LCCollection *col = l.collection();

    using namespace std;
    ostream_flags flags=saveflags(out);

    stringstream sstream;

    out << noshowpos;
    out << setw(41) << setfill('-') << right << " RawCalorimeterHit " << setfill('-') << setw(29) << "-" << endl;

    if(col != NULL){
        if(col->getTypeName() != LCIO::RAWCALORIMETERHIT){
            out << "Warning: collection not of type " << LCIO::RAWCALORIMETERHIT << endl ;
            setflags(out, flags);
            return(out);
        }
        LCFlagImpl flag( col->getFlag() ) ;
        out << "     LCIO::RCHBIT_ID1    : " << flag.bitSet( LCIO::RCHBIT_ID1 ) << endl ;
        out << "     LCIO::RCHBIT_TIME   : " << flag.bitSet( LCIO::RCHBIT_TIME ) << endl ;
        out << "     LCIO::RCHBIT_NO_PTR : " << flag.bitSet( LCIO::RCHBIT_NO_PTR ) << endl ;
        //print collection flags
    }

    //print object attributs
    out << setw(30) << setfill(' ') << left << "CellID0"<< setfill(' ') << right << setw(40) << hex << hit->getCellID0() << endl;
    out << setw(30) << setfill(' ') << left << "CellID1"<< setfill(' ') << right << setw(40) << hex << hit->getCellID1() << endl;
    out << setw(30) << setfill(' ') << left << "Amplitude"<< setfill(' ') << right << setw(40) << dec << hit->getAmplitude() << endl;
    out << setw(30) << setfill(' ') << left << "TimeStamp"<< setfill(' ') << right << setw(40) << dec << hit->getTimeStamp() << endl;

    setflags(out, flags);
    return out;
}

  std::ostream& operator<<( std::ostream& out, const EVENT::RawCalorimeterHit  &hit){
    out<<lcio_long(hit,NULL);
    return out;
  }

//-------------------------
// //TPCHit is depricated
//  const std::string& header(const EVENT::TPCHit &){ //hauke
//    static std::string _vtxh("");
//    return _vtxh;
//  }
//  const std::string& tail(const EVENT::TPCHit &){ //hauke
//    static std::string _vtxt("\n");
//    return _vtxt;
//  }
//  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::TPCHit>& sV){ //hauke
//    const EVENT::TPCHit* hit = sV.obj;
//    using namespace std;
//    out << noshowpos;
//    out << " [" << hex << setw(8) << setfill('0');// << hit->id() << "] ";
//  }
//
//
//  std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::TPCHit> l) {
//    const EVENT::TPCHit *hit = l.object();
//    const EVENT::LCCollection *col = l.collection();
//
//    using namespace std;
//    stringstream sstream;
//
//    out << noshowpos;
//    out << setw(41) << setfill('-') << right << " TPCHit " << setfill('-') << setw(29) << "-" << endl;
//
//    if(col != NULL){
//        if(col->getTypeName() != LCIO::TPCHIT){
//            out << "Warning: collection not of type " << LCIO::TPCHIT << endl ;
//            return(out);
//
//        }
//        //print collection flags
//    }
//
//    //print object attributs
//    //sstream << "0x" << hex << hit->id();
//    //out << setw(30) << setfill(' ') << left << "Id" << right << setw(40) << sstream.str() << endl;
//    //out << setw(30) << setfill(' ') << left << "Type"<< setfill(' ') << right << setw(40) << hit->getType() << endl;
//    //out << setw(30) << left << "Energy [GeV]" << right << setw(40) << hit->getEnergy() << endl;
//}
//
//  std::ostream& operator<<( std::ostream& out, const EVENT::TPCHit  &hit){
//    out<<lcio_long(hit,NULL);
//    return out;
//  }
//
//
//-------------------------
  const std::string& header(const EVENT::TrackerData &){ //hauke
    static std::string _vtxh(" CellID0 | CellID1 | Time      \n");
    return _vtxh;
  }
  const std::string& tail(const EVENT::TrackerData &){ //hauke
    static std::string _vtxt("---------|---------|-----------\n");
    return _vtxt;
  }
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::TrackerData>& sV){ //hauke
    const EVENT::TrackerData* hit = sV.obj;
    using namespace std;
    ostream_flags flags=saveflags(out);

    out << noshowpos;
    out << "| " << hex << setw(8) << setfill('0') << hit->getCellID0();
    out << "| " << hex << setw(8) << setfill('0') << hit->getCellID1();
    out << "|" << dec << setw(11) << setfill(' ') << hit->getTime() << "|" << endl;

    setflags(out, flags);
    return out;

  }

  std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::TrackerData> l) { //hauke
    const EVENT::TrackerData *hit = l.object();
    const EVENT::LCCollection *col = l.collection();

    using namespace std;
    ostream_flags flags=saveflags(out);

    stringstream sstream;

    out << noshowpos;
    out << setw(41) << setfill('-') << right << " TrackerData " << setfill('-') << setw(29) << "-" << endl;

    if(col != NULL){
        if(col->getTypeName() != LCIO::TRACKERDATA){
            out << "Warning: collection not of type " << LCIO::TRACKERDATA << endl ;
            setflags(out, flags);
            return(out);

        }
        //print collection flags
    }

    //print object attributs
    out << setw(30) << setfill(' ') << left << "CellID0"<< setfill(' ') << right << setw(40) << hex << hit->getCellID0() << endl;
    out << setw(30) << setfill(' ') << left << "CellID1"<< setfill(' ') << right << setw(40) << hex << hit->getCellID1() << endl;
    out << setw(30) << setfill(' ') << left << "Time"<< setfill(' ') << right << setw(40) << dec << hit->getTime() << endl;

    setflags(out, flags);
    return out;
}
  std::ostream& operator<<( std::ostream& out, const EVENT::TrackerData  &hit){
    out<<lcio_long(hit,NULL);
    return out;
  }

//-------------------------
//-------------------------
  const std::string& header(const EVENT::TrackerPulse &){ //hauke
    static std::string _vtxh(" CellID0 | CellID1 | Time   | Charge | Quality \n");
    return _vtxh;
  }
  const std::string& tail(const EVENT::TrackerPulse &){ //hauke
    static std::string _vtxt("---------|---------|--------|--------|---------\n");
    return _vtxt;
  }
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::TrackerPulse>& sV){ //hauke
    const EVENT::TrackerPulse* hit = sV.obj;
    using namespace std;
    ostream_flags flags=saveflags(out);

    out << noshowpos;
    out << "| " << hex << setw(8) << setfill('0') << hit->getCellID0();
    out << "| " << hex << setw(8) << setfill('0') << hit->getCellID1();
    out << "|" << dec << setw(8) << setfill(' ') << hit->getTime();
    out << "|" << dec << setw(8) << setfill(' ') << hit->getCharge();
    out << "|" << dec << setw(8) << setfill(' ') << hit->getQuality() << endl;


    setflags(out, flags);
    return out;
  }



  std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::TrackerPulse> l) {//hauke
    const EVENT::TrackerPulse *hit = l.object();
    const EVENT::LCCollection *col = l.collection();

    using namespace std;
    ostream_flags flags=saveflags(out);

    stringstream sstream;

    out << noshowpos;
    out << setw(41) << setfill('-') << right << " TrackerPulse " << setfill('-') << setw(29) << "-" << endl;

    if(col != NULL){
        if(col->getTypeName() != LCIO::TRACKERPULSE){
            out << "Warning: collection not of type " << LCIO::TRACKERPULSE << endl ;
            setflags(out, flags);
            return(out);
        }
        //print collection flags
        LCFlagImpl flag( col->getFlag() ) ;
        out << "     LCIO::TRAWBIT_ID1    : " << flag.bitSet( LCIO::TRAWBIT_ID1 ) << endl ;
    }

    //print object attributs
    out << setw(30) << setfill(' ') << left << "CellID0"<< setfill(' ') << right << setw(40) << hex << hit->getCellID0() << endl;
    out << setw(30) << setfill(' ') << left << "CellID1"<< setfill(' ') << right << setw(40) << hex << hit->getCellID1() << endl;
    out << setw(30) << setfill(' ') << left << "Time"<< setfill(' ') << right << setw(40) << dec << hit->getTime() << endl;
    out << setw(30) << setfill(' ') << left << "Charge"<< setfill(' ') << right << setw(40) << dec << hit->getCharge() << endl;
    out << setw(30) << setfill(' ') << left << "Quality"<< setfill(' ') << right << setw(40) << dec << hit->getQuality() << endl;

    setflags(out, flags);
    return out;
}
  std::ostream& operator<<( std::ostream& out, const EVENT::TrackerPulse  &hit){
    out<<lcio_long(hit,NULL);
    return out;
  }

//-------------------------
//-------------------------
  const std::string& header(const EVENT::TrackerRawData &){ //hauke
    static std::string _vtxh(" CellID0 | CellID1 | Time   \n");
    return _vtxh;
  }
  const std::string& tail(const EVENT::TrackerRawData &){ //hauke
    static std::string _vtxt("\n");
    return _vtxt;
  }
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::TrackerRawData>& sV){ //hauke
    const EVENT::TrackerRawData* hit = sV.obj;
    using namespace std;
    ostream_flags flags=saveflags(out);

    out << noshowpos;
    out << "| " << hex << setw(8) << setfill('0') << hit->getCellID0();
    out << "| " << hex << setw(8) << setfill('0') << hit->getCellID1();
    out << "|" << dec << setw(8) << setfill(' ') << hit->getTime() << endl;

    setflags(out, flags);
    return out;
  }


  std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::TrackerRawData> l) { //hauke
    const EVENT::TrackerRawData *hit = l.object();
    const EVENT::LCCollection *col = l.collection();

    using namespace std;
    ostream_flags flags=saveflags(out);

    stringstream sstream;

    out << noshowpos;
    out << setw(41) << setfill('-') << right << " TrackerRawData " << setfill('-') << setw(29) << "-" << endl;

    if(col != NULL){
        if(col->getTypeName() != LCIO::TRACKERRAWDATA){
            out << "Warning: collection not of type " << LCIO::TRACKERRAWDATA << endl ;
            setflags(out, flags);
            return(out);

        }
        //print collection flags
    }

    //print object attributs
    out << setw(30) << setfill(' ') << left << "CellID0"<< setfill(' ') << right << setw(40) << hex << hit->getCellID0() << endl;
    out << setw(30) << setfill(' ') << left << "CellID1"<< setfill(' ') << right << setw(40) << hex << hit->getCellID1() << endl;
    out << setw(30) << setfill(' ') << left << "Time"<< setfill(' ') << right << setw(40) << dec << hit->getTime() << endl;

    setflags(out, flags);
    return out;
}
   std::ostream& operator<<( std::ostream& out, const EVENT::TrackerRawData  &hit){
    out<<lcio_long(hit,NULL);
    return out;
  }


//-------------------------
//-------------------------
  const std::string& header(const EVENT::LCIO &){ //hauke
    static std::string _vtxh("-------------------");
    return _vtxh;
  }
  const std::string& tail(const EVENT::LCIO &){ //hauke
    static std::string _vtxt("-------------------\n");
    return _vtxt;
  }
  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::LCIO>& sV){ //hauke
    //const EVENT::LCIO* hit = sV.obj;
    using namespace std;
    ostream_flags flags=saveflags(out);

    out << noshowpos;
    out << " " << endl;

    setflags(out, flags);
    return out;
  }


  std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::LCIO> l) { //hauke
    //const EVENT::LCIO *hit = l.object();
    const EVENT::LCCollection *col = l.collection();


    using namespace std;
    ostream_flags flags=saveflags(out);

    stringstream sstream;

    out << noshowpos;
    out << setw(41) << setfill('-') << right << " LCIO " << setfill('-') << setw(29) << "-" << endl;

    if(col != NULL){
        //if(col->getTypeName() != LCIO::LCIO){
        //    out << "Warning: collection not of type " << LCIO::LCIO << endl ;
        //    return(out);
        //}
        //???

        //print collection flags
    }

    //print object attributs

    setflags(out, flags);
    return out;
}
  std::ostream& operator<<( std::ostream& out, const EVENT::LCIO  &hit){ //hauke
    out<<lcio_long(hit,NULL);
    return out;
  }

  const std::string& header(const EVENT::SimCalorimeterHit &){ //hauke
        static std::string _vtxh("    [id]    | cellId0| cellId1|  energy  |energyerr |        position (x,y,z)          |nMCParticles|\n\t\t-> MC contribution: prim. PDG|  energy  |   time   | sec. PDG\n");
    return _vtxh;
  }
  
  const std::string& tail(const EVENT::SimCalorimeterHit &){ //hauke
        static std::string _vtxt("------------|--------|--------|----------|----------|----------------------------------|-------------\n");
    return _vtxt;
  }

  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::SimCalorimeterHit>& sV){ //hauke
    const EVENT::SimCalorimeterHit* hit = sV.obj;
    using namespace std;
    ostream_flags flags=saveflags(out);

    out << noshowpos;
    out << " [" << hex << setw(8) << setfill('0') << hit->id() << "] ";
    out << "|" << hex << setw(8) << setfill('0') << hit->getCellID0();
    out << "|" << hex << setw(8) << setfill('0') << hit->getCellID1();
    out << "|" << dec << setprecision(3) << scientific << showpos << hit->getEnergy(); 
    //out << "|" << dec << setprecision(3) << scientific << hit->getEnergyError() << "|"; 
    out << "|" << dec << setprecision(3) << scientific << "          "; 
    out << "|" << dec << setprecision(3) << scientific << showpos
        << hit->getPosition()[0] << ", " << hit->getPosition()[0] << ", " << hit->getPosition()[0]; 
    out << "|" << dec << setw(12) << setfill(' ') << hit->getNMCContributions()
        << endl;

    for(int k=0;k < hit->getNMCContributions();k++){
        try{
            out << "\t\t-> "
                 << setw(15) << left <<  setfill(' ') << k << ":"
                 << setw(10) << left <<  setfill(' ') << hit->getParticleCont(k)->getPDG() << "|"
                 << right << setprecision(3) << scientific << showpos << hit->getEnergyCont(k) << "|"
                 << setprecision(3) << scientific << setw(8) <<setfill(' ') << showpos<< hit->getTimeCont(k) << "|" 
                 << " " << hit->getPDGCont( k) << endl ;
        }catch(exception& e){
            out << e.what() << endl ;
        }
    }
    out << noshowpos;


    setflags(out, flags);
    return(out);
}

  std::ostream& operator<<( std::ostream& out, const EVENT::SimCalorimeterHit  &hit){
    //out<<lcio_long<EVENT::SimCalorimeterHit>(hit,NULL);
    out<<lcio_long(hit,NULL);
    return out;
  }

/*  std::ostream& operator<<( std::ostream& out, const LCIO::MCParticle*  part){
    out << endl
            <<  "[   id   ]index|      PDG |    px,     py,        pz    | energy  |gen|[simstat]| vertex x,     y   ,   z     | endpoint x,    y  ,   z     |    mass |  charge |  [parents] - [daughters] |"
        << endl
        << endl ;
      printf("[%8.8x]", part->id() );
      printf("%10d|" , part->getPDG() );
      printf("% 1.2e,% 1.2e,% 1.2e|" ,
         part->getMomentum()[0] ,
         part->getMomentum()[1] ,
         part->getMomentum()[2] );
      printf("% 1.2e|" , part->getEnergy() ) ;

      printf(" %1d |" , part->getGeneratorStatus()  );
      printf("% 1.2e,% 1.2e,% 1.2e|" ,
         part->getVertex()[0] ,
         part->getVertex()[1] ,
         part->getVertex()[2] );
      printf("% 1.2e,% 1.2e,% 1.2e|" ,
         part->getEndpoint()[0] ,
         part->getEndpoint()[1] ,
         part->getEndpoint()[2] );
      printf("% 1.2e|" , part->getMass() ) ;
      printf("% 1.2e|" , part->getCharge() ) ;

      cout << " [" ;

      for(unsigned int k=0;k<part->getParents().size();k++){
            if(k>0) cout << "," ;
            cout << p2i_map[ part->getParents()[k] ]  ;
      }
      cout << "] - [" ;
      for(unsigned int k=0;k<part->getDaughters().size();k++){
            if(k>0) cout << "," ;
            cout << p2i_map[ part->getDaughters()[k] ]  ;
      }
      cout << "] " << endl ;


    cout << endl
     << "-------------------------------------------------------------------------------- "
     << endl ;
}
*/


  const std::string& header(const EVENT::TrackerHit &){ //hauke
    static std::string _vtxh("    [id]    | position (x,y,z)            | EDep    | time    |type \n");
    return _vtxh;
  }
  
  const std::string& tail(const EVENT::TrackerHit &){ //hauke
    static std::string _vtxt("------------|-----------------------------|---------|---------|-----\n");
    return _vtxt;
  }

  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::TrackerHit>& sV){ //hauke
    const EVENT::TrackerHit* part = sV.obj;
    using namespace std;
    ostream_flags flags=saveflags(out);

    out << " [" << setfill('0') << setw(8) << hex<< part->id() << "] |" << dec;
    out << showpos << scientific << setprecision (2) << part->getPosition()[0] << ","<< part->getPosition()[1] << "," << part->getPosition()[2] << "|"; 
    out << part->getEDep() << "|";
    out << part->getTime() << "|";
    out << noshowpos << setw(4) << part->getType() << endl;

    setflags(out, flags);
    return out;
  }

  std::ostream& operator<<( std::ostream& out, const EVENT::TrackerHit &hit){ //hauke
    out << lcio_long(hit,NULL);
    return out;
  }

  const std::string& header(const EVENT::SimTrackerHit &){ //hauke
    static std::string _vtxh(" CellID  | position (x,y,z)            |  EDep   |  time   |PDG of MCParticle|   px    ,   py    ,   pz    |path length \n");
    return _vtxh;
  }
  
  const std::string& tail(const EVENT::SimTrackerHit &){ //hauke
    static std::string _vtxt("---------|-----------------------------|---------|---------|-----------------|-----------------------------|------------\n");
    return _vtxt;
  }

  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::SimTrackerHit>& sV){ //hauke
    const EVENT::SimTrackerHit* hit = sV.obj;
    using namespace std;
    ostream_flags flags=saveflags(out);

    out << noshowpos << " " << setw(8) << hex << hit->getCellID() << "|";
    out << showpos << scientific << setprecision (2) << setfill(' ') << hit->getPosition()[0]<<","<<hit->getPosition()[1]<<","<<hit->getPosition()[2]<<"|";
    out << hit->getEDep() << "|";
    out << hit->getTime() << "|";
    if(hit->getMCParticle()){
        out << setw(17) << hit->getMCParticle()->getPDG() << "|";
    }else{
        out << "                 |";
    }
    out << hit->getMomentum()[0] << "," << hit->getMomentum()[0] << "," << hit->getMomentum()[0] << "|";
    out << setw(11) << hit->getPathLength() << endl;
    out << noshowpos;


    out << setfill(' ') << setprecision(6)  <<  dec;
    return(out);
}

  std::ostream& operator<<( std::ostream& out, const EVENT::SimTrackerHit &hit){ //hauke
    out << lcio_long(hit,NULL);
    return(out);
  }


  const std::string& header(const EVENT::CalorimeterHit &){ //hauke
    static std::string _vtxh("    [id]    |cellId0 |cellId1 |  energy  |energyerr |        position (x,y,z)           \n");
    return _vtxh;
  }
  
  const std::string& tail(const EVENT::CalorimeterHit &){ //hauke
    static std::string _vtxt("------------|--------|--------|----------|----------|-----------------------------------\n");
    return _vtxt;
  }

  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::CalorimeterHit>& sV){ //hauke
    const EVENT::CalorimeterHit* hit = sV.obj;
    using namespace std;
    ostream_flags flags=saveflags(out);

    out << noshowpos;

    out << " [" << hex << setw(8) << setfill('0') << hit->id() << "] ";
    out << "|" << hex << setw(8) << setfill('0') << hit->getCellID0();
    out << "|" << hex << setw(8) << setfill('0') << hit->getCellID1();
    out << "|" << dec << setprecision(3) << scientific << showpos << hit->getEnergy(); 
    out << "|" << dec << setprecision(3) << scientific << hit->getEnergyError(); 
    out << "|" << dec << setprecision(3) << scientific << showpos
        << hit->getPosition()[0] << ", " << hit->getPosition()[0] << ", " << hit->getPosition()[0]; 
    out << endl << noshowpos;


    setflags(out, flags);
    return(out);
}

  std::ostream& operator<<( std::ostream& out, const EVENT::CalorimeterHit &hit){ //hauke
    out << lcio_long(hit,NULL);
    //out << lcio_long(a,NULL);

    return(out);
  }
/*
    using namespace std;
    stringstream tmp;
    out << noshowpos;

    out << setw(42) <<  std::setfill('-') << right << " CalorimeterHit " << setfill('-') << setw(28) << "-" << endl;
    //out << setw(30) << setfill(' ') << left << "ID" <<   right << setw(40) << hit->getId() << endl;
    out << setw(30) << setfill(' ') << left << "CellId0" <<   right << setw(40) << hex <<  hit->getCellID0() << endl;
    out << setw(30) << setfill(' ') << left << "CellId1" <<   right << setw(40) << hex << hit->getCellID1() << endl;
    out << setw(30) << setfill(' ') << left << "Energy [GeV]" <<   right << setw(40) << dec << hit->getEnergy() << endl;
    out << setw(30) << setfill(' ') << left << "Energy Error [GeV]" <<   right << setw(40) << hit->getEnergyError() << endl;
    tmp.str("");
    tmp  << hit->getPosition()[0] << ", " << hit->getPosition()[1]  << ", " << hit->getPosition()[2]; 
    out <<setw(30) << left << showpos << "Position (x,y,z) " << setfill(' ') << right <<setw(40) << tmp.str() << endl;
    out <<setw(30) << left << noshowpos << "Type " << setfill(' ') << right <<setw(40) << hit->getType() << endl;
    out << noshowpos;

    return(out);

    }
*/

  const std::string& header(const EVENT::MCParticle &){ //hauke
    static std::string _vtxh("    [id]    |      PDG |    px   ,    py   ,    pz   |  energy |gen| vertex x,    y    ,    z    |endpoint x,    y   ,    z    |  mass   | charge  |parents-daughters\n");
    return _vtxh;
  }
  
  const std::string& tail(const EVENT::MCParticle &){ //hauke
    static std::string _vtxt("------------|----------|-----------------------------|---------|---|-----------------------------|-----------------------------|---------|---------|------------------\n");
    return _vtxt;
  }

  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::MCParticle>& sV){ //hauke
    const EVENT::MCParticle* part = sV.obj;
    using namespace std;
    ostream_flags flags=saveflags(out);

    out << noshowpos <<  " [" << setfill('0') << setw(8) << hex<< part->id() << "] |" << dec;
    out << setfill(' ') << setw(10) << part->getPDG() << "|";
    out << showpos << scientific << setprecision (2) << part->getMomentum()[0] << ","<< part->getMomentum()[1] << "," << part->getMomentum()[2] << "|"; 
    out << part->getEnergy() << "|";
    out << " " << noshowpos << part->getGeneratorStatus() << showpos <<" |";
    out << part->getVertex()[0] << "," << part->getVertex()[1] << "," <<part->getVertex()[2] << "|";
    out << part->getEndpoint()[0] << "," << part->getEndpoint()[1] << "," <<part->getEndpoint()[2] << "|";
    out << part->getMass() << "|"; 
    out << part->getCharge() << "|"; 
    out << setw(6) << part->getParents().size() << " - " << setw(8) << part->getDaughters().size();
    out << endl << noshowpos;

    setflags(out, flags);
    return out;
  }


  std::ostream& operator<<( std::ostream& out, const EVENT::MCParticle &mcp){ //hauke
    out << lcio_long(mcp,NULL);
    return out;
  }

  const std::string& header(const EVENT::ReconstructedParticle &){ //hauke
    static std::string _vtxh("    [id]    |com|type|     momentum( px,py,pz)       | energy | mass   | charge |        position ( x,y,z)      |pidUsed\n");
    return _vtxh;
  }
  
  const std::string& tail(const EVENT::ReconstructedParticle &){ //hauke
    static std::string _vtxt("------------|---|----|-------------------------------|--------|--------|--------|-------------------------------|--------\n");
    return _vtxt;
  }

  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::ReconstructedParticle>& sV){ //hauke
    const EVENT::ReconstructedParticle * part = sV.obj;
    stringstream tmp;
    using namespace std;
    ostream_flags flags=saveflags(out);

    out << noshowpos <<  " [" << setfill('0') << setw(8) << hex << part->id() << "] |" << dec;
    out << setfill(' ') << setw(3) << part->isCompound() << "|";
    out << setfill(' ') << setw(4) << part->getType() << "|";

    tmp.str("");
    tmp << showpos << scientific << setprecision(2); 
    tmp << part->getMomentum()[0] << ", " << part->getMomentum()[1] << ", " << part->getMomentum()[2];
    out << tmp.str() << "|";


    out << noshowpos << scientific << setprecision(2); 
    out << part->getMass() << "|";
    out << part->getCharge() << "|";
    out << part->getEnergy() << "|";

    tmp.str("");
    tmp << part->getReferencePoint()[0] << ", " << part->getReferencePoint()[1] << ", " << part->getReferencePoint()[2];
    out << tmp.str() << "|";

    out << hex << setw(7) << setfill('0');
    if(part->getParticleIDUsed() != 0 ){
        out << part->getParticleIDUsed()->id();
    }else{ 
        out << 0;
    }
    out << endl;

    out << "    covariance( px,py,pz,E) : (" ;
    for(int l=0;l<10;l++){
        if(l==9){
            out << scientific << setprecision(2) << part->getCovMatrix()[l];
        }else{
            out << scientific << setprecision(2) << part->getCovMatrix()[l] << ", ";
        }



        //printf("%4.2e, ", part->getCovMatrix()[l] ) ;
    }
    out << ")" << endl ;

    out << "    particles ( [   id   ] ):" ;
    for(unsigned int l=0;l<part->getParticles().size();l++){
        if(l == part->getParticles().size()-1){
            out << hex <<  setfill('0') << "[" <<setw(8) << part->getParticles()[l]->id() << "]";
        }else{
            out << hex <<  setfill('0') << "[" <<setw(8) << part->getParticles()[l]->id() << "], ";
        }
         
        //printf("[%8.8x], ", part->getParticles()[l]->id() ) ;
    }
    out << endl ;

    out << "    tracks ( [   id   ] ): " ;
    for(unsigned int l=0;l<part->getTracks().size();l++){
        if(l == part->getTracks().size()-1){
            out << hex << setfill('0') << "[" << setw(8) << part->getTracks()[l]->id() << "]";
        }else{
            out << hex << setfill('0') << "[" << setw(8) << part->getTracks()[l]->id() << "], ";
        }

        //printf("[%8.8x], ",  part->getTracks()[l]->id() );
    }
    out << endl ;

    out << "    clusters ( [   id   ] ): " ;
    for(unsigned int l=0;l<part->getClusters().size();l++){
        if(l == part->getClusters().size()-1){
            out << hex <<  setfill('0') << "[" << setw(8) << part->getClusters()[l]->id() << "]";
        }else{
            out << hex <<  setfill('0') << "[" << setw(8) << part->getClusters()[l]->id() << "], ";
        }

        //printf("[%8.8x], ",  part->getClusters()[l]->id() );
    }
    out << endl ;
    out << "    particle ids ( [id], PDG, (type)): " ;
    for(unsigned int l=0;l<part->getParticleIDs().size();l++){
        ParticleID* pid = part->getParticleIDs()[l] ;
        out << hex <<  "[" << setw(8) <<pid->id() << "], " <<  dec << setw(6) << pid->getPDG() << ", " <<  "(" << setw(6) <<pid->getType() << ") ";
        //printf("[%8.8x], %6.6d, (%6.6d)  ",  pid->id() , pid->getPDG() , pid->getType() ) ;
    }
    out << endl ;

    setflags(out, flags);
    return out;
  }


  std::ostream& operator<<( std::ostream& out, const EVENT::ReconstructedParticle &part){ //hauke
    out << lcio_long(part,NULL);
    return(out);
  }

  const std::string& header(const EVENT::Track &){ //hauke
    static std::string _vtxh("    [id]    |    type   |   d0    |  phi    |  omega  |   z0    |tan lambd|   reference point(x,y,z)      |   dEdx  | dEdxErr |   chi2    \n");
    return _vtxh;
  }

  const std::string& tail(const EVENT::Track &){ //hauke
    static std::string _vtxt("------------|-----------|---------|---------|---------|---------|---------|-------------------------------|---------|---------|----------\n");
    return _vtxt;
  }

  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::Track>& sV){ //hauke
    const EVENT::Track * part = sV.obj;
    using namespace std;
    ostream_flags flags=saveflags(out);

    out << noshowpos <<  " [" << setfill('0') << setw(8) << hex<< part->id() << "] ";
    out << "|" << dec << setfill(' ') << setw(8) << part->getType();
    out << scientific << setprecision (2) << showpos;
    out << "|" << part->getD0(); 
    out << "|" << part->getPhi(); 
    out << "|" << part->getOmega();
    out << "|" << part->getZ0();
    out << "|" << part->getTanLambda();
    out << "|" << part->getReferencePoint()[0] << ", " << part->getReferencePoint()[1] << ", " <<part->getReferencePoint()[2];
    out << "|" << part->getdEdx();
    out << "|" << part->getdEdxError();
    out << "|" << part->getChi2();
    out << endl;
    
    out << "Errors: " ;
    unsigned int l;
    for(l=0;l<14;l++){
        out << part->getCovMatrix()[l] << ", ";
    }
    out << part->getCovMatrix()[l+1] << endl;


    out << "Tracks(id): " ;
    const TrackVec& tracks = part->getTracks() ;
    for(l=0;l<tracks.size();l++){
        if( tracks[l] != 0  )
            out << tracks[l]->id();
        else
            out << 0;
        if(l != tracks.size()-1){ out << ", ";}
    }

    out << endl << "Hits ->";
    const TrackerHitVec& hits= part->getTrackerHits() ;
    for(l=0;l<hits.size();l++){
        out << "[" << hits[l]->id() << "] ";
    }
    out << endl;

    out << "Radius of innermost hit " << part->getRadiusOfInnermostHit() << " / mm , " << " subdetector Hit numbers : " ;
    for(l=0 ; l< part->getSubdetectorHitNumbers().size()-1 ; l++) {
        out << part->getSubdetectorHitNumbers()[l] << ", " ;
    }
    out << part->getSubdetectorHitNumbers()[l+1] << endl;

    setflags(out, flags);
    return out;
  }


  std::ostream& operator<<( std::ostream& out, const EVENT::Track &part){ //hauke
    out << lcio_long(part,NULL);
    return(out);
  }
      


  const std::string& header(const EVENT::Cluster &){ //hauke
    static std::string _vtxh("    [id]    |type|  energy  |      position ( x,y,z)           |  itheta  |   iphi   \n");
    return _vtxh;
  }


  const std::string& tail(const EVENT::Cluster &){ //hauke
    static std::string _vtxt("------------|----|----------|----------------------------------|----------|----------\n");
    return _vtxt;
  }

  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::Cluster>& sV){ //hauke
    const EVENT::Cluster * clu = sV.obj;
    using namespace std;
    ostream_flags flags=saveflags(out);

    out << noshowpos <<  " [" << setfill('0') << setw(8) << hex << clu->id() << "] |" << dec;
    out << setfill(' ') << setw(4) << clu->getType() << "|";
    out << showpos << scientific << setprecision(3) << clu->getEnergy() << "|";
    out << clu->getPosition()[0] << ", " << clu->getPosition()[1] << ", " <<  clu->getPosition()[2] << "|";
    out << clu->getITheta() << "|";
    out << clu->getIPhi() <<  noshowpos << endl;

    setflags(out, flags);
    return out;
  }


  std::ostream& operator<<( std::ostream& out, const EVENT::Cluster &clu){ //hauke
    out << lcio_long(clu,NULL);
    return(out);
  }


//####################################################

  const std::string& header(const EVENT::Vertex &v){
    static std::string _vtxh(
      "\n    [id]    |pri|     alg. type     |    chi2   |    prob.  |       position ( x, y, z)       | [par] |  [idRecP]  \n");
    //_vtxh+=tail(v);
    return _vtxh;
  }
  
  const std::string& tail(const EVENT::Vertex &){
    static std::string _vtxt(
	"------------|---|-------------------|-----------|-----------|---------------------------------|-------|------------\n");
    return _vtxt;
  }

  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::Vertex>& sV){
    using namespace std;
    ostream_flags flags=saveflags(out);


    const EVENT::Vertex* v = sV.obj;
    out << noshowpos; 
    out << setfill('0');
    out << " [" << setw(8) << hex << v->id() << "] | " << v->isPrimary()<< " | ";
    out << setfill(' ') << setw(17) << left << v->getAlgorithmType() << " | ";
    out << showpos;
    
    out << setfill('0') << right << scientific << setprecision(2) << v->getChi2() << " | " << v->getProbability() << " | " <<
	v->getPosition()[0] << ", " << v->getPosition()[1] << ", " << v->getPosition()[2] << " | [" ;

    out << setw(3) << v->getParameters().size() << "] | [";
    out << setw(8) << hex << (v->getAssociatedParticle()!=NULL?v->getAssociatedParticle()->id():0) << "]\n";

    setflags(out, flags);
    return out;
  }


 
  std::ostream& operator<<( std::ostream& out, const EVENT::Vertex &v){
    out << lcio_long(v,NULL);
    return out;
  }

//#######################
  const std::string& header(const EVENT::LCRelation &v){
    static std::string _vtxh("| [  from  ] | [   to   ] | Weight |\n");
    //_vtxh+=tail(v);
    return _vtxh;
  }
  
  const std::string& tail(const EVENT::LCRelation &){
    static std::string _vtxt("|------------|------------|--------|\n");
    return _vtxt;
  }

  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::LCRelation>& sV){
    using namespace std;
    ostream_flags flags=saveflags(out);

    const EVENT::LCRelation* v = sV.obj;
    out << noshowpos; 
    out << "| [" << setw(8) << hex << (v->getFrom())->id() << "] |";
    out << " [" << setw(8) << hex << (v->getTo())->id() << "] |";
    out << " " << setw(8) << dec << v->getWeight() << "|\n";

    setflags(out, flags);
    return out;
  }


 
  std::ostream& operator<<( std::ostream& out, const EVENT::LCRelation &v){
    out << lcio_long(v,NULL);
    return out;
  }
//#######################
  const std::string& header(const EVENT::LCFloatVec &v){
    static std::string _vtxh("| [  id  ] |\n");
    //_vtxh+=tail(v);
    return _vtxh;
  }
  
  const std::string& tail(const EVENT::LCFloatVec &){
    static std::string _vtxt("|----------|\n");
    return _vtxt;
  }

  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::LCFloatVec>& sV){
    using namespace std;
    const EVENT::LCFloatVec* v = sV.obj;
    ostream_flags flags=saveflags(out);

    out << noshowpos; 
    out << "| [" << setw(8) << hex << v->id() << "] |\n";

    setflags(out, flags);
    return out;
  }


 
  std::ostream& operator<<( std::ostream& out, const EVENT::LCFloatVec &v){
    out << lcio_long(v,NULL);
    return out;
  }

//#######################
std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::CalorimeterHit> l) {
    const EVENT::CalorimeterHit *hit = l.object();
    const EVENT::LCCollection *col = l.collection();

    using namespace std;
    ostream_flags flags=saveflags(out);

    stringstream tmp;
    out << noshowpos;

    out << setw(42) <<  std::setfill('-') << right << " CalorimeterHit " << setfill('-') << setw(28) << "-" << endl;

    if(col != NULL){ 
        if(col->getTypeName() != LCIO::CALORIMETERHIT){
            out << "Warning: collection not of type " << LCIO::CALORIMETERHIT << endl ;
            setflags(out, flags);
            return(out);
        }
        tmp.str(""); 
        tmp << "0x" << hex << col->getFlag() << dec;
        out << setw(30) << setfill(' ') << left << "Collection Flag" << right << setw(40) <<  tmp.str() << endl;
        LCTOOLS::printParameters(col->getParameters()); //todo
        LCFlagImpl flag(col->getFlag());
        cout << "  -> LCIO::RCHBIT_LONG   : " << flag.bitSet( LCIO::RCHBIT_LONG ) << endl ;
        cout << "     LCIO::RCHBIT_BARREL : " << flag.bitSet( LCIO::RCHBIT_BARREL ) << endl ;
        cout << "     LCIO::RCHBIT_ID1    : " << flag.bitSet( LCIO::RCHBIT_ID1 ) << endl ;
        cout << "     LCIO::RCHBIT_TIME   : " << flag.bitSet( LCIO::RCHBIT_TIME ) << endl ;
        cout << "     LCIO::RCHBIT_NO_PTR : " << flag.bitSet( LCIO::RCHBIT_NO_PTR ) << endl ;
        cout << "     LCIO::RCHBIT_ENERGY_ERROR  : " << flag.bitSet( LCIO::RCHBIT_ENERGY_ERROR ) << endl ;
    }


    //out << setw(30) << setfill(' ') << left << "ID" <<   right << setw(40) << hit->getId() << endl;
    tmp.str("");
    tmp << "0x" << hex << hit->getCellID0() << dec;
    out << setw(30) << setfill(' ') << left << "CellId0" <<   right << setw(40) << tmp.str() << endl;
    tmp.str("");
    tmp << "0x" << hex << hit->getCellID1() << dec;

    out << setw(30) << setfill(' ') << left << "CellId1" <<   right << setw(40) << tmp.str() << endl;
    out << setw(30) << setfill(' ') << left << "Energy [GeV]" <<   right << setw(40) << dec << hit->getEnergy() << endl;
    out << setw(30) << setfill(' ') << left << "Energy Error [GeV]" <<   right << setw(40) << hit->getEnergyError() << endl;
    tmp.str("");
    tmp  << hit->getPosition()[0] << ", " << hit->getPosition()[1]  << ", " << hit->getPosition()[2]; 
    if(col != NULL){ 
        LCFlagImpl flag(col->getFlag());
        if(flag.bitSet(LCIO::CHBIT_LONG)){
            out <<setw(30) << left << showpos << "Position (x,y,z) " << setfill(' ') << right <<setw(40) << tmp.str() << endl;
        }
        else{
            out <<setw(30) << left << showpos << "Position (x,y,z) " << setfill(' ') << right <<setw(40) << "no position available" << endl;
        }
    }else{
        out <<setw(40) << left << showpos << "Position (x,y,z) [not verified]" << setfill(' ') << right <<setw(30) << tmp.str() << endl;
    }
    //out << setw(30) << left << noshowpos << "ID-Field" << right << setw(40) << idDecoder(hit).valuestring() << endl;
    out <<setw(30) << left << noshowpos << "Type " << setfill(' ') << right <<setw(40) << hit->getType() << endl;
    out << noshowpos;


    setflags(out, flags);
    return(out);
}

std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::Cluster> l) {
    const EVENT::Cluster *clu = l.object();
    const EVENT::LCCollection *col = l.collection();

    //out << "lcio_long: Cluster : lcio_long" << std::endl;

    using namespace std;
    ostream_flags flags=saveflags(out);

    stringstream tmp;
    out << noshowpos;
    out << setw(41) << setfill('-') << right << " Cluster " << setfill('-') << setw(29) << "-" << endl;

    if(col != NULL){ 
        if(col->getTypeName() != LCIO::CLUSTER){
            out << "Warning: collection not of type " << LCIO::CLUSTER << endl ;
            setflags(out, flags);
            return(out);

        }
        tmp.str(""); 
        tmp << "0x" << hex << col->getFlag() << dec;
        out << setw(30) << setfill(' ') << left << "Collection Flag" << right << setw(40) <<  tmp.str() << endl;
        LCTOOLS::printParameters(col->getParameters());
        LCFlagImpl flag( col->getFlag() ) ;
        cout << "     LCIO::CLBIT_HITS : " << flag.bitSet( LCIO::CLBIT_HITS ) << endl ;

    }


    tmp << "0x" << hex << clu->id();
    out << setw(30) << setfill(' ') << left << "Id" << right << setw(40) << tmp.str() << endl;
    out << setw(30) << setfill(' ') << left << "Type"<< setfill(' ') << right << setw(40) << clu->getType() << endl;
    out << setw(30) << left << "Energy [GeV]" << right << setw(40) << clu->getEnergy() << endl;
    tmp.str("");
    tmp  << dec << clu->getPosition()[0] << ", " << clu->getPosition()[1]  << ", " << clu->getPosition()[2]; 
    out << setw(30) << left << "Position [mm] (x,y,z) " << setfill(' ') << right <<setw(40) << tmp.str() << endl;
    out << setw(30) << left << "itheta" << setfill(' ') << right <<setw(40) << clu->getITheta() << endl;
    out << setw(30) << left << "iphi" << setfill(' ') << right <<setw(40) << clu->getIPhi() << endl;
    //to much output
/* 
    //raw code from LCTOOLS.cc
    out << " errors (6 pos)/( 3 dir): (" ;
      for(int l=0;l<6;l++){
    printf("%4.2e, ", clu->getPositionError()[l] ) ;
      }
      out << ")/("  ;
      for(int l=0;l<3;l++){
    printf("%4.2e, ", clu->getDirectionError()[l] ) ;
      }
      out << ")" << endl ;
      out << " clusters(e): " ;
     const ClusterVec& clusters = clu->getClusters() ;

      for(unsigned int l=0;l<clusters.size();l++){
        printf("%4.2e, ",  clusters[l]->getEnergy() ) ;
      }
      out << endl ;
      out <<" subdetector energies : " ;
      const FloatVec& sdE = clu->getSubdetectorEnergies() ;
      for(unsigned int l=0;l<sdE.size();l++){
        printf("%4.2e, ",  sdE[l] ) ;
      }
   const CalorimeterHitVec& hits= clu->getCalorimeterHits() ;
    const FloatVec& contr = clu->getHitContributions() ;
    for(unsigned int k=0;k<hits.size();k++){
      printf( " [%8.8x] (%4.3e), " , hits[k]->id(), contr[k] ) ;
      //      out << "0x" << hits[k]  << "(" << contr[k] << "), " ;
    }
    out << dec << endl  ;
*/


    setflags(out, flags);
    return(out);
}


std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::Track> ll){
    const EVENT::Track *part = ll.object();
    const EVENT::LCCollection *col = ll.collection();

    using namespace std;
    ostream_flags flags=saveflags(out);

    stringstream tmp;

    //out << scientific << setprecision (2) << showpos;
    out << noshowpos;
    out << setw(41) << setfill('-') << right << "-- Track ---" << setfill('-') << setw(29) << "-" << endl;

    if(col != NULL){ 
        if(col->getTypeName() != LCIO::TRACK){
            out << "Warning: collection not of type " << LCIO::TRACK << endl ;
            setflags(out, flags);
            return(out);

        }
        tmp.str(""); 
        tmp << "0x" << hex << col->getFlag() << dec;
        out << setw(30) << setfill(' ') << left << "Collection Flag" << right << setw(40) <<  tmp.str() << endl;
        LCTOOLS::printParameters(col->getParameters());
        LCFlagImpl flag( col->getFlag() ) ;
        cout << "     LCIO::TRBIT_HITS : " << flag.bitSet( LCIO::TRBIT_HITS ) << endl ;

    } 

    tmp << hex << setfill('0') << setw(8) << part->id();
    out << setw(30) << setfill(' ') << left << "Id" << right << setw(40) << tmp.str() << endl;
    out << setw(30) << setfill(' ') << left << "Type" << right << setw(40) << part->getType() << endl;
    out << setw(30) << setfill(' ') << left << "D0" << right << showpos << setw(40) << part->getD0() << endl;
    out << setw(30) << setfill(' ') << left << "Phi" << right << setw(40) << part->getPhi() << endl;
    out << setw(30) << setfill(' ') << left << "Omega" << right << setw(40) << part->getOmega() << endl;
    out << setw(30) << setfill(' ') << left << "Z0" << right << setw(40) << part->getZ0() << endl;
    out << setw(30) << setfill(' ') << left << "Tan Lambda" << right << setw(40) << part->getTanLambda() << endl;

    tmp.str("");
    tmp  << dec << part->getReferencePoint()[0] << ", " << part->getReferencePoint()[1]  << ", " << part->getReferencePoint()[2]; 
    out << setw(30) << setfill(' ') << left << "ReferencePoint" << right << setw(40) << tmp.str() << endl;
    out << setw(30) << setfill(' ') << left << "dEdx" << right << setw(40) << part->getdEdx() << endl;
    out << setw(30) << setfill(' ') << left << "dEdx Error" << right << setw(40) << part->getdEdxError() << endl;
    out << setw(30) << setfill(' ') << left << "Chi2" << right << setw(40) << part->getChi2() << endl;

    out << "Errors:     " << showpos;
    unsigned int l;
    for(l=0;l<14;l++){
        out << part->getCovMatrix()[l];
        if(! ((l+1)%5)){
            out << endl << "            ";
        } else{
            out << ", ";
        }
    }
    if(!((l+2)%5)){out << endl << "            ";}
    out << part->getCovMatrix()[l+1] << endl;



    out << "Tracks(id): " ;
    const TrackVec& tracks = part->getTracks() ;
    for(l=0;l<tracks.size();l++){
        if( tracks[l] != 0  )
            out << tracks[l]->id();
        else
            out << 0;


        if(!(l+1)%5){
            out << endl << "            ";
            continue;
        }
        if(l != tracks.size()-1){ out << ", ";}

    }

    out << endl << "Hits:       " << noshowpos;
    const TrackerHitVec& hits= part->getTrackerHits() ;
    for(l=0;l<hits.size();l++){
        out << "[" << hits[l]->id() << "] ";
        if(!((l+1)%7)){out << endl << "            ";}
    }
    out << endl;

    out << "Radius of innermost hit " << part->getRadiusOfInnermostHit() << " / mm , " << " subdetector Hit numbers : " ;
    out << endl << "            ";
    for(l=0 ; l< part->getSubdetectorHitNumbers().size()-1 ; l++) {
        out << part->getSubdetectorHitNumbers()[l] << ", " ;
        if(!((l+1)%20)){out << endl << "            ";}
    }
    if(!((l+2)%20)){out << endl << "            ";}
    out << part->getSubdetectorHitNumbers()[l+1] << endl;

    out << noshowpos;

    setflags(out, flags);
    return(out);
}


std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::ReconstructedParticle> ll){
    const EVENT::ReconstructedParticle *part = ll.object();
    const EVENT::LCCollection *col = ll.collection();

    using namespace std;
    ostream_flags flags=saveflags(out);

    stringstream tmp;

    out << setw(41) << setfill('-') << right << " ReconstructedParticle " << setfill('-') << setw(29) << "-" << endl;
    if(col != NULL){ 
        if(col->getTypeName() != LCIO::RECONSTRUCTEDPARTICLE){
            out << "Warning: collection not of type " << LCIO::RECONSTRUCTEDPARTICLE << endl ;
            setflags(out, flags);
            return(out);

        }
        tmp.str(""); 
        tmp << "0x" << hex << col->getFlag() << dec;
        out << setw(30) << setfill(' ') << left << "Collection Flag" << right << setw(40) <<  tmp.str() << endl;
        LCTOOLS::printParameters(col->getParameters());
    } 

    tmp.str("");
    tmp<<"0x" << hex << part->id() << dec;
    out << setw(30) << setfill(' ') << left << "Id" <<   right << setw(40) << tmp.str() << endl;
    out << dec << setw(30) << setfill(' ') << left << "Compound" <<   right << setw(40) << part->isCompound() << endl;
    out << setw(30) << setfill(' ') << left << "Type" <<   right << setw(40) << part->getType() << endl;

    tmp.str("");
    tmp << showpos << scientific << setprecision(2); 
    tmp << part->getMomentum()[0] << ", " << part->getMomentum()[1] << ", " << part->getMomentum()[2];
    out << setw(30) << setfill(' ') << left << "Momentum (x,y,z)" <<   right << setw(40) << tmp.str() << endl;

    out << noshowpos << scientific << setprecision(2); 
    out << setw(30) << setfill(' ') << left << "Mass [GeV]" <<   right << setw(40) << part->getMass() << endl;

    out << setw(30) << setfill(' ') << left << "Charge [e]" <<   right << setw(40) << part->getCharge() << endl;
    out << setw(30) << setfill(' ') << left << "Energy [GeV]" <<   right << setw(40) << part->getEnergy() << endl;

    tmp.str("");
    tmp << part->getReferencePoint()[0] << ", " << part->getReferencePoint()[1] << ", " << part->getReferencePoint()[2];

    out << setw(30) << setfill(' ') << left << "ReferencePoint" <<   right << setw(40) << tmp.str() << endl;
    out << setw(30) << setfill(' ') << left << "ParticleIDUsed";

    
    if(part->getParticleIDUsed() != 0 ){
        tmp.str("");
        tmp << "0x" << hex << part->getParticleIDUsed()->id() << dec;
        out << hex << right << setw(40) << tmp.str() << endl;
    }else{ 
        out << right << setw(40) << 0 << endl;
    }

    out <<         "Covariance p,E:  "; //??? 

    for(int l=0;l<10;l++){
        if(l==5) { out << endl << "                 ";}
        if(l==9){
            out << scientific << setprecision(2) << part->getCovMatrix()[l];
        }else{
            out << scientific << setprecision(2) << part->getCovMatrix()[l] << ", ";
        }
    }
    //out << ")" << endl ;

    out << endl << "Particles id:    ";
    for(unsigned int l=0;l<part->getParticles().size();l++){
        if(l == part->getParticles().size()-1){
            out << hex <<  setfill('0') << "[" <<setw(8) << part->getParticles()[l]->id() << "]";
        }else{
            out << hex <<  setfill('0') << "[" <<setw(8) << part->getParticles()[l]->id() << "], ";
        }
        if(! ((l+1)%4)) { out << endl <<"                 ";}
    }
    //out << endl ;

    out << endl << "Tracks id:    ";
    for(unsigned int l=0;l<part->getTracks().size();l++){
        if(! ((l+1)%5)){ out << endl << "                 ";}
        if(l == part->getTracks().size()-1){
            out << hex << setfill('0') << "[" << setw(8) << part->getTracks()[l]->id() << "]";
        }else{
            out << hex << setfill('0') << "[" << setw(8) << part->getTracks()[l]->id() << "], ";
        }

    }

    out << endl << "Clusters id:  ";
    for(unsigned int l=0;l<part->getClusters().size();l++){
        if(! ((l+1)%5)){ out << endl << "                 ";}

        if(l == part->getClusters().size()-1){
            out << hex <<  setfill('0') << "[" << setw(8) << part->getClusters()[l]->id() << "]";
        }else{
            out << hex <<  setfill('0') << "[" << setw(8) << part->getClusters()[l]->id() << "], ";
        }

    }
    out << endl << "Particle ids ( [id], PDG, (type)): " << endl;
    for(unsigned int l=0;l<part->getParticleIDs().size();l++){
        ParticleID* pid = part->getParticleIDs()[l] ;
        out << hex << "                 " <<  "[" << setw(8) <<pid->id() << "], " <<  dec << setw(6) << pid->getPDG() << ", " <<  "(" << setw(6) <<pid->getType() << ") " << endl;
    }
    out << endl ;

    setflags(out, flags);
    return(out);
}

std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::SimCalorimeterHit> ll){
    const EVENT::SimCalorimeterHit *hit = ll.object();
    const EVENT::LCCollection *col = ll.collection();

    using namespace std;
    ostream_flags flags=saveflags(out);

    stringstream tmp;
    //out << setprecision(5) << fixed;

    out << noshowpos;
    out << setw(46) <<  std::setfill('-') << right << " SimCalorimeterHit " << setfill('-') << setw(24) << "-" << endl;

    if(col != NULL){ 
        if(col->getTypeName() != LCIO::SIMCALORIMETERHIT){
            out << "Warning: collection not of type " << LCIO::SIMCALORIMETERHIT << endl ;
            setflags(out, flags);
            return(out);

        }
        tmp.str(""); 
        tmp << "0x" << hex << col->getFlag() << dec;
        out << setw(30) << setfill(' ') << left << "Collection Flag" << right << setw(40) <<  tmp.str() << endl;
        LCTOOLS::printParameters(col->getParameters());

        LCFlagImpl flag( col->getFlag() ) ;
        cout << "  -> LCIO::CHBIT_LONG   : " << flag.bitSet( LCIO::CHBIT_LONG ) << endl ;
        cout << "     LCIO::CHBIT_BARREL : " << flag.bitSet( LCIO::CHBIT_BARREL ) << endl ;
        cout << "     LCIO::CHBIT_ID1   :  " << flag.bitSet( LCIO::CHBIT_ID1 ) << endl ;
        cout << "     LCIO::CHBIT_PDG    : " << flag.bitSet( LCIO::CHBIT_PDG ) << endl ;
    } 
    tmp << "0x" << hex << hit->id() << dec;
    out << setw(30) << setfill(' ') << std::left << "Id" <<   right << setw(40) << tmp.str() << endl;

    out << setw(30) << left << "CellID0"<< setfill(' ') << right << setw(40) << hex << hit->getCellID0() << endl;
    out << setw(30) << left << "CellID1"<< setfill(' ') << right << setw(40) << hex << hit->getCellID1() << endl;
    out << setw(30) << left << "Energy [GeV]" << right << setw(40) << dec << hit->getEnergy() << std::endl;

    tmp.str("");
    tmp  << hit->getPosition()[0] << ", " << hit->getPosition()[1]  << ", " << hit->getPosition()[2]; 
    if(col != NULL){ 
        LCFlagImpl flag(col->getFlag());
        if(flag.bitSet(LCIO::CHBIT_LONG)){
            out <<setw(30) << left << showpos << "Position (x,y,z) " << setfill(' ') << right <<setw(40) << tmp.str() << endl;
        }
        else{
            out <<setw(30) << left << showpos << "Position (x,y,z) " << setfill(' ') << right <<setw(40) << "no position available" << endl;
        }
    }else{
        out <<setw(40) << left << showpos << "Position (x,y,z) [not verified]" << setfill(' ') << right <<setw(30) << tmp.str() << endl;
    }
    //out <<setw(30) << std::left << "Position [] (x,y,z) " << setfill(' ') << right <<setw(40) << tmp.str() << endl;
    

    out << setw(30) << std::left << "N MC Contributions: "<< setfill(' ') << right << setw(40) << hit->getNMCContributions() << endl; 
    for(int k=0;k < hit->getNMCContributions();k++){
        try{
            out << setw(30) << left << "Particle Number" << right << setw(40) << k << endl;
            out << setw(30) << left << "                Prim. PDG" << right << setw(40) << hit->getParticleCont( k)->getPDG() << endl;
            out << setw(30) << left << "                Energy [GeV]" << right << setw(40) << hit->getEnergyCont(k) << endl;
            out << setw(30) << left << "                Time" << right << setw(40) << hit->getTimeCont(k) << endl;
            out << setw(30) << left << "                Sec. PDG" << right << setw(40) << hit->getPDGCont(k) << endl;
        }catch(exception& e){
            out << e.what() << endl ;
        }
    }

    out << noshowpos;

    setflags(out, flags);
    return(out);
}

std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::TrackerHit> ll){
    const EVENT::TrackerHit *hit = ll.object();
    const EVENT::LCCollection *col = ll.collection();

    using namespace std;
    ostream_flags flags=saveflags(out);

    stringstream tmp;
    out << noshowpos;
    out << setw(41) <<  setfill('-') << right << " TrackerHit " << setfill('-') << setw(29) << "-" << endl;
    if(col != NULL){ 
        if(col->getTypeName() != LCIO::TRACKERHIT){
            out << "Warning: collection not of type " << LCIO::TRACKERHIT << endl ;
            setflags(out, flags);
            return(out);

        }
        tmp.str(""); 
        tmp << "0x" << hex << col->getFlag() << dec;
        out << setw(30) << setfill(' ') << left << "Collection Flag" << right << setw(40) <<  tmp.str() << endl;
        LCTOOLS::printParameters(col->getParameters());

        LCFlagImpl flag( col->getFlag() ) ;
        cout << "     LCIO::CHBIT_BARREL : " << flag.bitSet( LCIO::CHBIT_BARREL ) << endl ;
    } 
    tmp.str("");
    tmp << "0x" << hex << hit->id() << dec; 
    out << setw(30) << setfill(' ') << left << "Id" <<   right << setw(40) << tmp.str() << endl;
    tmp.str("");
    tmp << hit->getPosition()[0] << ", " << hit->getPosition()[1]  << ", " << hit->getPosition()[2]; 
    out << setw(30) << left << "Position (x,y,z) " << setfill(' ') << right <<setw(40) << tmp.str() << endl;
    out << setw(30) << left << "E_deposited " << setfill(' ') << right << setw(40) << hit->getEDep() << endl;
    out << setw(30) << left << "Time " << setfill(' ') << right << setw(40) << hit->getTime() << endl;
    out << setw(30) << left << "Type " << setfill(' ') << right << setw(40) << hit->getType() << endl;

    out << "Covariance of the position (x,y,z):" << endl ;
    out << "   cov(x,x) , cov(y,x) , cov(y,y) , cov(z,x) , cov(z,y) , cov(z,z)" << endl ;

    out << "   " << scientific << setprecision(2) ;
    for( unsigned int i=0 ; i < (hit->getCovMatrix().size() - 1) ; i++ ){
        out << hit->getCovMatrix()[i] << " , ";
    }
    out << hit->getCovMatrix()[ hit->getCovMatrix().size() ] << endl ;
 
    out << noshowpos;

    setflags(out, flags);
    return(out);
}


std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::SimTrackerHit> ll){
    const EVENT::SimTrackerHit *hit = ll.object();
    const EVENT::LCCollection *col = ll.collection();

    using namespace std;
    ostream_flags flags=saveflags(out);

    stringstream tmp;
    out << noshowpos;

    out << setw(42) <<  std::setfill('-') << right << " SimTrackerHit " << setfill('-') << setw(28) << "-" << endl;
    if(col != NULL){ 
        if(col->getTypeName() != LCIO::SIMTRACKERHIT){
            out << "Warning: collection not of type " << LCIO::SIMTRACKERHIT << endl ;
            setflags(out, flags);
            return(out);

        }
        tmp.str(""); 
        tmp << "0x" << hex << col->getFlag() << dec;
        out << setw(30) << setfill(' ') << left << "Collection Flag" << right << setw(40) <<  tmp.str() << endl;
        LCTOOLS::printParameters(col->getParameters());

        LCFlagImpl flag( col->getFlag() ) ;
        cout << "     LCIO::THBIT_BARREL : " << flag.bitSet( LCIO::THBIT_BARREL ) << endl ;
        cout << "     LCIO::THBIT_MOMENTUM : " << flag.bitSet( LCIO::THBIT_MOMENTUM ) << endl ;
    } 

    tmp.str("");
    tmp << "0x" << hex << hit->getCellID() << dec;
    out << setw(30) << setfill(' ') << std::left << "CellID" <<   right << setw(40) << tmp.str() << endl;
    tmp.str("");
    tmp  << dec << hit->getPosition()[0] << ", " << hit->getPosition()[1]  << ", " << hit->getPosition()[2]; 
    out <<setw(30) << std::left << "Position [mm] (x,y,z) " << dec << setfill(' ') << right <<setw(40) << tmp.str() << endl;
    out <<setw(30) << std::left << "dE/dx [GeV]" << setfill(' ') << right <<setw(40) << hit->getEDep() << endl;
    out <<setw(30) << std::left << "Time [ns]" << setfill(' ') << right <<setw(40) << hit->getTime() << endl;
    if(hit->getMCParticle() != NULL){
        out <<std::setw(30) << std::left << "PDG of MCParticle " << setfill(' ') << right <<setw(40) << hit->getMCParticle()->getPDG() << std::endl;
    }else{
        out <<std::setw(30) << std::left << "PDG of MCParticle " << setfill(' ') << right <<setw(40) << "[Empty]"  << std::endl;
    }


    tmp  << hit->getMomentum()[0] << ", " << hit->getMomentum()[1]  << ", " << hit->getMomentum()[2]; 
    if(col != NULL){ 
        LCFlagImpl flag(col->getFlag());
        if(flag.bitSet(LCIO::THBIT_MOMENTUM)){
            out <<setw(40) << left << "Momentum [GeV] (x,y,z) [not verified]" << setfill(' ') << right <<setw(30) << tmp.str() << endl;
        }else{
            out <<setw(30) << left << "Momentum [GeV] (x,y,z)" << setfill(' ') << right <<setw(40) << "not available"  << endl;
        }
    }else{
        out <<setw(40) << left << "Momentum [GeV] (x,y,z) [not verified]" << setfill(' ') << right <<setw(30) << tmp.str() << endl;
    }
    out <<setw(30) << left << "PathLength " << setfill(' ') << right <<setw(40) << hit->getPathLength() << endl;

    setflags(out, flags);
    return(out);
}


std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::MCParticle> ll){
    const EVENT::MCParticle *mcp = ll.object();
    const EVENT::LCCollection *col = ll.collection();

    using namespace std;
    ostream_flags flags=saveflags(out);

    stringstream tmp;
    out << noshowpos;

    //out << setprecision(5) << fixed;
    out << setw(41) <<  std::setfill('-') << right << " MCParticle " << setfill('-') << setw(29) << "-" << std::endl;
    if(col != NULL){ 
        if(col->getTypeName() != LCIO::MCPARTICLE){
            out << "Warning: collection not of type " << LCIO::MCPARTICLE << endl ;
            setflags(out, flags);
            return(out);

        }
        tmp.str(""); 
        tmp << "0x" << hex << col->getFlag() << dec;
        out << setw(30) << setfill(' ') << left << "Collection Flag" << right << setw(40) <<  tmp.str() << endl;
        LCTOOLS::printParameters(col->getParameters());
    } 

    tmp.str(""); 
    tmp << "0x" << hex << mcp->id() << dec;
    out << setw(30) << setfill(' ') << left << "Id" <<   right << setw(40) << tmp.str() << endl;
    out << setw(30) << left << "PDG"<< setfill(' ') << right << setw(40) << mcp->getPDG() << endl;
    out << setw(30) << std::left << "Energy [GeV]" << right << setw(40) << mcp->getEnergy() << endl;
    

    tmp.str("");
    tmp  << mcp->getMomentum()[0] << ", " << mcp->getMomentum()[1]  << ", " << mcp->getMomentum()[2]; 
    out << setw(30) << left << "Momentum [GeV] (x,y,z) " << setfill(' ') << right <<setw(40) << tmp.str() << endl;
    

    out << setw(30) << left <<"GeneratorStatus" << setfill(' ') << right << setw(40) << mcp->getGeneratorStatus() << endl; 
    //out << "SimulatorStatusString " << std::setw(26) << setfill(' ') << " " << mcp->getGeneratorStatusString() << std::endl; 


    tmp.str("");
    tmp << mcp->getVertex()[0] << ", " << mcp->getVertex()[1] << ", " << mcp->getVertex()[2];
    out << setw(30) << left << "Vertex [mm] (x,y,z) " << setfill(' ') << right << setw(40) << right << tmp.str() << endl; 

    tmp.str("");
    tmp  <<  mcp->getEndpoint()[0] << ", " << mcp->getEndpoint()[1] << ", " << mcp->getEndpoint()[2] ;
    out << setw(30) << left <<"Endpoint [mm] (x,y,z) " << setfill(' ') << setw(40) << right << tmp.str()  << endl; 
    out << setw(30) << left << "Mass [GeV] " << right << setw(40) << mcp->getMass() << endl; 
    out << setw(30) << left << "Charge " << right << setw(40) << mcp->getCharge() << endl; 
    out << setw(30) << left << "Parents " <<right << setw(40) << mcp->getParents().size() << endl; 
    out << setw(30) << left << "Daughters " << right << setw(40) << mcp->getDaughters().size() << endl; 

    //out<<LCTOOLS::getSimulatorStatusString(NULL) << LCTOOLS::getSimulatorStatusString(mcp);
    if(mcp->getSimulatorStatus() != 0){
        out<<LCTOOLS::getSimulatorStatusString() <<  LCTOOLS::getSimulatorStatusString((MCParticle *) mcp) << endl;
    }

    setflags(out, flags);
    return(out);
}

std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::Vertex> ll){
    const EVENT::Vertex *v = ll.object();
    const EVENT::LCCollection *col = ll.collection();

    using namespace std;
    ostream_flags flags=saveflags(out);

    stringstream tmp;
    out << noshowpos;

    out << setw(41) <<  std::setfill('-') << right << " Vertex " << setfill('-') << setw(29) << "-" << endl;
    if(col != NULL){ 
        if(col->getTypeName() != LCIO::VERTEX){
            out << "Warning: collection not of type " << LCIO::VERTEX << endl ;
            setflags(out, flags);
            return(out);
        }
    } 

    tmp.str("");
    tmp << "0x" << hex << v->id() << dec;

    out << setw(30) << setfill(' ') << left << "Id" <<   right << setw(40) << tmp.str() << endl;
    out << setw(30) << setfill(' ') << left << "Is Primary Vertex" <<   right << setw(40) << (v->isPrimary() ? "yes":"no") << endl;
    out << setw(30) << setfill(' ') << left << "Algorithm Type" <<   right << setw(40) << v->getAlgorithmType() << endl;
    out << scientific << setprecision(5);

    out << setw(30) << setfill(' ') << left << "Chi2" <<   right << setw(40) << v->getChi2() << endl;
    out << setw(30) << setfill(' ') << left << "Probability" <<   right << setw(40) << v->getProbability() << endl;
    tmp.str("");
    tmp<<setprecision(5);
    tmp << v->getPosition()[0] <<", "<<v->getPosition()[0] <<", "<<v->getPosition()[0]; 
    out << setw(30) << setfill(' ') << left << "Position (x,y,z)" <<   right << setw(40) << tmp.str() << endl;

    out << "Covariance Matrix          ";
    for(unsigned int i=0; i<v->getCovMatrix().size(); i++){
        out << v->getCovMatrix()[i] << (i<(v->getCovMatrix().size()-1)?", ":"\n");
        if((! ((i+1)%3)) && i < v->getCovMatrix().size()- 1){ cout << endl << "                           "; }
    }
    
    out << setw(30) << setfill(' ') << left << "Parameters";

    if(v->getParameters().size()==0){
      out <<   right << setw(40) << "[Empty]" << endl;
    } else { out << endl; }

    for(unsigned int i=0; i < v->getParameters().size(); i++){
      out << "   Parameter [";
      out << setw(3) << i << "]:\t";
      out << scientific << setprecision(5) << v->getParameters()[i] << endl;
    }
    
    out << setw(30) << setfill(' ') << left << "ARP ID" <<   right << setw(40) << (v->getAssociatedParticle()!=NULL?v->getAssociatedParticle()->id():0) << endl;

    setflags(out, flags);
    return out;
  }


std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::LCRelation> ll){
    const EVENT::LCRelation *v = ll.object();
    const EVENT::LCCollection *col = ll.collection();

    using namespace std;
    ostream_flags flags=saveflags(out);

    stringstream tmp;
    out << noshowpos;

    out << setw(41) <<  std::setfill('-') << right << " LCRelation " << setfill('-') << setw(29) << "-" << endl;
    if(col != NULL){ 
        if(col->getTypeName() != LCIO::LCRELATION){
            out << "Warning: collection not of type " << LCIO::LCRELATION << endl ;
            setflags(out, flags);
            return(out);
        }
    } 

    out << setw(30) << setfill(' ') << left << "From [id]" <<   right << hex << (v->getFrom())->id() << endl;
    out << setw(30) << setfill(' ') << left << "To [id]" <<   right << (v->getTo())->id()  << endl;
    out << setw(30) << setfill(' ') << left << "Weight" <<   right << v->getWeight()  << endl;

    setflags(out, flags);
    return out;
}


std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::LCFloatVec> ll){
    const EVENT::LCFloatVec *v = ll.object();
    const EVENT::LCCollection *col = ll.collection();

    using namespace std;
    ostream_flags flags=saveflags(out);

    stringstream tmp;
    out << noshowpos;

    out << setw(41) <<  std::setfill('-') << right << " LCFloatVec " << setfill('-') << setw(29) << "-" << endl;
    if(col != NULL){ 
        if(col->getTypeName() != LCIO::LCFLOATVEC){
            out << "Warning: collection not of type " << LCIO::LCFLOATVEC << endl ;
            setflags(out, flags);
            return(out);
        }
    } 
    tmp.str("");
    tmp << "0x" << hex << v->id() << dec;
    out << setw(30) << setfill(' ') << left << "Id" <<   right <<  tmp.str() << endl;

    setflags(out, flags);
    return out;
}


//deprecated
/*
  std::string brief(const EVENT::Vertex* v){
    std::string str;
    char buffer[256];
	  
    sprintf( buffer, " [%8.8x] | %1d | %4.2e | %4.2e | %5.3e, %5.3e, %5.3e | "
      , v->id()
      , v->isPrimary()
      , v->getChi2()
      , v->getProbability()
      , v->getPosition()[0]
      , v->getPosition()[1]
      , v->getPosition()[2]
    ) ;

    for(int i=0; i<VTXCOVMATRIX; i++)
      sprintf( buffer, "%s%5.3e%s", buffer, v->getCovMatrix()[i], (i<(VTXCOVMATRIX-1) ? ", ":" |") ) ;
    
    sprintf( buffer, "%s [%8.8x]\n", buffer, (v->getAssociatedParticle()!=NULL? v->getAssociatedParticle()->id(): 0) ) ;
    
    str = buffer;
    return str;
  }
*/

} // namespace
 
