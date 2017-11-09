//////////////////////////////////////////////////////////
// Operators for convenient printing of LCIO objects
//////////////////////////////////////////////////////////

#include "UTIL/Operators.h"
#include "UTIL/LCObjectHandle.h"
#include "UTIL/LCTime.h"
#include "UTIL/CellIDDecoder.h"
#include "UTIL/PIDHandler.h"
#include "UTIL/LCTOOLS.h"
#include "UTIL/BitSet32.h"

#include "EVENT/TPCHit.h"
#include "EVENT/TrackerRawData.h"
#include "EVENT/TrackerData.h"
#include "EVENT/TrackerPulse.h"
#include "EVENT/LCIO.h"
#include "EVENT/MCParticle.h"
#include "EVENT/LCFloatVec.h"
#include "EVENT/LCIntVec.h"
#include "EVENT/Track.h"
#include "EVENT/TrackState.h"
#include "EVENT/Cluster.h"
#include "EVENT/ReconstructedParticle.h"
#include "EVENT/Vertex.h"
#include "EVENT/TrackerHit.h"
#include "EVENT/TrackerHitPlane.h"
#include "EVENT/TrackerHitZCylinder.h"
#include "EVENT/SimTrackerHit.h"
#include "EVENT/LCGenericObject.h"
#include "EVENT/LCRelation.h"
#include "EVENT/LCEvent.h"

//#include "IMPL/LCFlagImpl.h"
#include "LCIOSTLTypes.h"

#include <map>
#include <set>
#include <cstdio>
#include <sstream>

typedef std::vector<std::string> LCStrVec ;

using namespace std ;
using namespace EVENT ;
using namespace IMPL ;


namespace UTIL{

    /* 
    // EXP: INDEX MAP - UNDER DEVELOPMENT

    std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::Vertex, EVENT::LCCollection>& sV){
    const EVENT::Vertex* v = sV.obj;
    const EVENT::LCCollection* col = sV.cobj;

    //TODO: PROBLEM HERE!!!
    UTIL::IndexMap im(col,"AlgorithmNames","AlgorithmTypes");

    out << setfill('0');
    out << " [" << setw(8) << dec << v->id() << "] | " << v->isPrimary()<< " | ";
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




    //============================================================================
    //   # LCCollection
    //============================================================================

    const std::string& header(const EVENT::LCCollection *){ //hauke
        static std::string _h("| Number of elements | Type name |    Flag     | Is transient | Is default | Is subset |\n");
        return _h;
    }

    const std::string& tail(const EVENT::LCCollection *){ //hauke
        static std::string _t("|--------------------|-----------|-------------|--------------|------------|-----------|\n");
        return _t;
    }

    std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::LCCollection>& sV){ //hauke
        const EVENT::LCCollection* hit = sV.obj;
        out << noshowpos;
        out << "|" << dec << setw(20) << setfill(' ') << hit->getNumberOfElements();
        out << "|" << dec << setw(11) << setfill(' ') << hit->getTypeName();
        out << "|" << hex << setw(13) << setfill(' ') << hit->getFlag();
        out << "|" << dec << setw(14) << setfill(' ') << hit->isTransient();
        out << "|" << dec << setw(12) << setfill(' ') << hit->isDefault();
        out << "|" << dec << setw(11) << setfill(' ') << hit->isSubset() << "|" << endl;
        return out;
    }

    std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::LCCollection> l) { //hauke
        const EVENT::LCCollection *hit = l.object();
        const EVENT::LCCollection *col = l.collection();

        stringstream sstream;

        out << noshowpos;
        out << setw(41) << setfill('-') << right << " LCCollection " << setfill('-') << setw(29) << "-" << endl;

        if(col != NULL){
            if(col->getTypeName() != LCIO::LCCOLLECTION){
                out << "Warning: collection not of type " << LCIO::LCCOLLECTION << endl ;
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
        return out;
    }

    std::ostream& operator<<( std::ostream& out, const EVENT::LCCollection &hit){
        out<<lcio_long(hit,NULL);
        return out;
    }




    //============================================================================
    //   # LCEvent
    //============================================================================

    const std::string& header(const EVENT::LCEvent *){ //hauke
        static std::string _h("| Run number  |Event number |Detector name| Time stamp  |  Weight     |\n");
        return _h;
    }

    const std::string& tail(const EVENT::LCEvent *){ //hauke
        static std::string _t("|-------------|-------------|-------------|-------------|-------------|\n");
        return _t;
    }

    std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::LCEvent>& sV){ //hauke
        const EVENT::LCEvent* hit = sV.obj;
        out << noshowpos;
        out << "|" << dec << setw(13) << setfill(' ') << hit->getRunNumber();
        out << "|" << dec << setw(13) << setfill(' ') << hit->getEventNumber();
        out << "|" << dec << setw(13) << setfill(' ') << hit->getDetectorName();
        out << "|" << dec << setw(13) << setfill(' ') << hit->getTimeStamp();
        out << "|" << dec << setw(13) << setfill(' ') << hit->getWeight() << "|" << endl;
        return out;
    }


    std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::LCEvent> l) { //hauke
        const EVENT::LCEvent *hit = l.object();
        const EVENT::LCCollection *col = l.collection();

        stringstream sstream;

        out << noshowpos;
        out << setw(41) << setfill('-') << right << " LCEvent " << setfill('-') << setw(29) << "-" << endl;

        if(col != NULL){
            if(col->getTypeName() != LCIO::LCEVENT){
                out << "Warning: collection not of type " << LCIO::LCEVENT << endl ;
                return(out);

            }
            //print collection flags
        }

        //print object attributs
        //sstream << dec << hit->id();
        out << setw(30) << setfill(' ') << left << "Run number" << right << setw(40) << dec << hit->getRunNumber() << endl;
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
        return out;
    }

    std::ostream& operator<<( std::ostream& out, const EVENT::LCEvent  &hit){
        out<<lcio_long(hit,NULL);
        return out;
    }




    //============================================================================
    //   # LCFlag
    //============================================================================

    const std::string& header(const EVENT::LCFlag *){ //hauke
        static std::string _h("|    Flag    |");
        return _h;
    }

    const std::string& tail(const EVENT::LCFlag *){ //hauke
        static std::string _t("|------------|\n");
        return _t;
    }

    std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::LCFlag>& sV){ //hauke
        const EVENT::LCFlag* hit = sV.obj;
        out << noshowpos;
        out << "|" << hex << setw(8) << setfill(' ') << hit->getFlag() << "|" << endl;
        return out;
    }

    std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::LCFlag> l) { //hauke
        const EVENT::LCFlag *hit = l.object();
        const EVENT::LCCollection *col = l.collection();

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
        return out;
    }

    std::ostream& operator<<( std::ostream& out, const EVENT::LCFlag  &hit){
        out<<lcio_long(hit,NULL);
        return out;
    }




    //============================================================================
    //   # LCGenericObject
    //============================================================================

  const std::string& header(const EVENT::LCGenericObject* /*obj*/, const EVENT::LCCollection *col){ //hauke
        //const EVENT::LCCollection *col = &v;
        bool isFixedSize  =  BitSet32( col->getFlag() ).test( LCIO::GOBIT_FIXED);
        stringstream header;
        header.str("");

        header << " [   id   ] ";
        if(col != NULL){
            header << col->getParameters().getStringVal("DataDescription");
            header << " - isFixedSize: "   <<  ( isFixedSize ? "true" : "false" );
            header << endl;
        }else{
            header << " Data.... ";
            header << endl;
        }
        static std::string _t(header.str());
        return _t;
    }

    const std::string& tail(const EVENT::LCGenericObject *){ //hauke
        static std::string _t(" --------------------------------------------------------\n");
        return _t;
    }

    std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::LCGenericObject>& sV){ //hauke
        const EVENT::LCGenericObject* obj = sV.obj;
        out << noshowpos;
        out << " [" << dec << setw(8) << setfill('0') << obj->id() << "] ";
        out << setfill(' ') << dec;
        for(int j=0;j<obj->getNInt();j++){
            out << "i:" << obj->getIntVal( j ) << "; " ;
        }
        for(int j=0;j<obj->getNFloat();j++){
            out << "f:" << obj->getFloatVal( j ) << "; " ;
        }
        for(int j=0;j<obj->getNDouble();j++){
            out << "d:" << obj->getDoubleVal( j ) << "; " ;
        }
        return out;
    }


    std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::LCGenericObject> l) { //hauke
        const EVENT::LCGenericObject *hit = l.object();
        const EVENT::LCCollection *col = l.collection();

        stringstream sstream;

        out << noshowpos;
        out << setw(41) << setfill('-') << right << " LCGenericObject " << setfill('-') << setw(29) << "-" << endl;

        if(col != NULL){
            if(col->getTypeName() != LCIO::LCGENERICOBJECT){
                out << "Warning: collection not of type " << LCIO::LCGENERICOBJECT << endl ;
                return(out);

            }
            //print collection flags
        }

        //print object attributs
        sstream << dec << hit->id();
        out << setw(30) << setfill(' ') << left << "Id" << right << setw(40) << sstream.str() << endl;
        out << setw(30) << setfill(' ') << left << "Number of integer values"<< setfill(' ') << right << setw(40) << hit->getNInt() << endl;
        out << setw(30) << setfill(' ') << left << "Number of float values"<< setfill(' ') << right << setw(40) << hit->getNDouble() << endl;
        out << setw(30) << setfill(' ') << left << "Type name"<< setfill(' ') << right << setw(40) << hit->getTypeName() << endl;
        out << setw(30) << setfill(' ') << left << "Description"<< setfill(' ') << right << setw(40) << hit->getDataDescription() << endl;
        return out;
    }

    std::ostream& operator<<( std::ostream& out, const EVENT::LCGenericObject  &hit){
        out<<lcio_long(hit,NULL);
        return out;
    }




    //============================================================================
    //   # LCIntVec
    //============================================================================

    const std::string& header(const EVENT::LCIntVec *){ //hauke
        static std::string _h("| [   id   ] | val0, val1, ...\n");
        return _h;
    }

    const std::string& tail(const EVENT::LCIntVec *){ //hauke
        static std::string _t("|------------|----------------\n");
        return _t;
    }

    std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::LCIntVec>& sV){ //hauke
        const EVENT::LCIntVec* vec = sV.obj;
        out << noshowpos; 
        out << "| [" << setfill('0') << setw(8) << dec << vec->id() << "] |";
        for(unsigned int j=0;j< vec->size();j++){
            out << setfill(' ') << right << setw(8) << (*vec)[j];
            if( j<vec->size()-1){ 
                out << ", ";
            }
            if( ! ( (j+1) % 10)  ){ 
                out << endl << "     ";
            }
        }
        out << endl;

        return out;
    }

    std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::LCIntVec> l) { //hauke
        const EVENT::LCIntVec *hit = l.object();
        const EVENT::LCCollection *col = l.collection();

        stringstream sstream;

        out << noshowpos;
        out << setw(41) << setfill('-') << right << " LCIntVec " << setfill('-') << setw(29) << "-" << endl;

        if(col != NULL){
            if(col->getTypeName() != LCIO::LCINTVEC){
                out << "Warning: collection not of type " << LCIO::LCINTVEC << endl ;
                return(out);

            }
            //print collection flags
        }

        //print object attributs
        sstream << dec << hit->id();
        out << setw(30) << setfill(' ') << left << "Id" << right << setw(40) << sstream.str() << endl;
        return out;
    }

    std::ostream& operator<<( std::ostream& out, const EVENT::LCIntVec  &hit){
        out<<lcio_long(hit,NULL);
        return out;
    }




    //============================================================================
    //   # LCFloatVec
    //============================================================================

    const std::string& header(const EVENT::LCFloatVec *){
        static std::string _h(" [   id   ] | val0, val1, ...\n");
        //_h+=tail(v);
        return _h;
    }

    const std::string& tail(const EVENT::LCFloatVec *){
        static std::string _t("------------|------------------\n");
        return _t;
    }

    std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::LCFloatVec>& sV){
        const EVENT::LCFloatVec* vec = sV.obj;
        out << noshowpos; 
        out << " [" << setfill('0') << setw(8) << dec << vec->id() << "] |";
        for(unsigned int j=0;j< vec->size();j++){
            out << setfill(' ') << right << setw(8) << (*vec)[j];
            if( j<vec->size()-1){ 
                out << ", ";
            }
            if( ! ( (j+1) % 10)  ){ 
                out << endl << "     ";
            }
        }
        out << endl;

        return out;
    }

    std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::LCFloatVec> ll){
        const EVENT::LCFloatVec *v = ll.object();
        const EVENT::LCCollection *col = ll.collection();

        stringstream tmp;
        out << noshowpos;

        out << setw(41) <<  std::setfill('-') << right << " LCFloatVec " << setfill('-') << setw(29) << "-" << endl;
        if(col != NULL){ 
            if(col->getTypeName() != LCIO::LCFLOATVEC){
                out << "Warning: collection not of type " << LCIO::LCFLOATVEC << endl ;
                return(out);
            }
        } 
        tmp.str("");
        tmp << dec << v->id() << dec;
        out << setw(30) << setfill(' ') << left << "Id" <<   right <<  tmp.str() << endl;

        return out;
    }

    std::ostream& operator<<( std::ostream& out, const EVENT::LCFloatVec &v){
        out << lcio_long(v,NULL);
        return out;
    }




    //============================================================================
    //   # LCStrVec
    //============================================================================

    /*
       const std::string& header(const EVENT::LCStrVec &){ //hauke
       static std::string _h("");
       return _h;
       }

       const std::string& tail(const EVENT::LCStrVec &){ //hauke
       static std::string _t("\n");
       return _t;
       }

       std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::LCStrVec>& sV){ //hauke
       const EVENT::LCStrVec* hit = sV.obj;
       out << noshowpos;
       out << " [" << dec << setw(8) << setfill('0');// << hit->id() << "] ";
       }



       std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::LCStrVec> l) { //hauke
       const EVENT::LCStrVec *hit = l.object();
       const EVENT::LCCollection *col = l.collection();

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
    sstream << dec << hit->id();
    out << setw(30) << setfill(' ') << left << "Id" << right << setw(40) << sstream.str() << endl;
    out << setw(30) << setfill(' ') << left << "Type"<< setfill(' ') << right << setw(40) << hit->getType() << endl;
    out << setw(30) << left << "Energy [GeV]" << right << setw(40) << hit->getEnergy() << endl;
    }

    std::ostream& operator<<( std::ostream& out, const EVENT::LCStrVec  &hit){
    out<<lcio_long(hit,NULL);
    return out;
    }
     */




    //============================================================================
    //   # LCObject
    //============================================================================

    const std::string& header(const EVENT::LCObject *){ //hauke
        static std::string _h(" [  Id   ] \n");
        return _h;
    }

    const std::string& tail(const EVENT::LCObject *){ //hauke
        static std::string _t("-----------\n");
        return _t;
    }

    std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::LCObject>& sV){ //hauke
        const EVENT::LCObject* hit = sV.obj;
        out << noshowpos;
        out << " [" << dec << setw(8) << setfill('0') << hit->id() << "] ";
        return out;
    }

    std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::LCObject> l) { //hauke
        const EVENT::LCObject *hit = l.object();
        const EVENT::LCCollection *col = l.collection();

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
        sstream << dec << hit->id();
        out << setw(30) << setfill(' ') << left << "Id" << right << setw(40) << sstream.str() << endl;
        return out;
    }

    std::ostream& operator<<( std::ostream& out, const EVENT::LCObject  &hit){
        out<<lcio_long(hit,NULL);
        return out;
    }




    //============================================================================
    //   # LCParameters
    //============================================================================

    const std::string& header(const EVENT::LCParameters *){ //hauke
        static std::string _h("-----\n");
        return _h;
    }

    const std::string& tail(const EVENT::LCParameters *){ //hauke
        static std::string _t("-----\n");
        return _t;
    }

  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::LCParameters>& /*sV*/){ //hauke
        out << noshowpos;
        out << "| " << hex << setw(8) << setfill(' ')<< " |";
        return out;
    }

    std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::LCParameters> l) { //hauke
        //const EVENT::LCParameters *params = l.object();
        const EVENT::LCCollection *col = l.collection();

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

        //sstream << dec << hit->id();
        //out << setw(30) << setfill(' ') << left << "Id" << right << setw(40) << sstream.str() << endl;
        //out << setw(30) << setfill(' ') << left << "Type"<< setfill(' ') << right << setw(40) << hit->getType() << endl;
        //out << setw(30) << left << "Energy [GeV]" << right << setw(40) << hit->getEnergy() << endl;
        return out;

    }

    std::ostream& operator<<( std::ostream& out, const EVENT::LCParameters  &hit){
        out<<lcio_long(hit,NULL);
        return out;
    }




    //============================================================================
    //   # LCRelation
    //============================================================================

    const std::string& header(const EVENT::LCRelation *){
        static std::string _h("| [from_id ] | [ to_id  ] | Weight  |\n");
        return _h;
    }

    const std::string& tail(const EVENT::LCRelation *){
        static std::string _t("|------------|------------|---------|\n");
        return _t;
    }

    std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::LCRelation>& sV){
        const EVENT::LCRelation* v = sV.obj;
        out << noshowpos << setfill('0'); 
        out << "| [" << setw(8) << dec << ( v->getFrom() ?  v->getFrom()->id() : 0 ) << "] |";
        out << " [" << setw(8) << dec  << ( v->getTo() ?  v->getTo()->id() : 0 )  << "] |";
        out << " " << setw(8) << dec << v->getWeight() << "|";
        out << setfill(' ');
        out << endl;
        return out;
    }

    std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::LCRelation> ll){
        const EVENT::LCRelation *v = ll.object();
        const EVENT::LCCollection *col = ll.collection();

        stringstream tmp;
        out << noshowpos;

        out << setw(41) <<  std::setfill('-') << right << " LCRelation " << setfill('-') << setw(29) << "-" << endl;
        if(col != NULL){ 
            if(col->getTypeName() != LCIO::LCRELATION){
                out << "Warning: collection not of type " << LCIO::LCRELATION << endl ;
                return(out);
            }
        } 

        out << setw(30) << setfill(' ') << left << "From [id]" <<   right << dec << ( v->getFrom() ?  v->getFrom()->id() : 0 )  << endl;
        out << setw(30) << setfill(' ') << left << "To [id]" <<   right << ( v->getTo() ?  v->getTo()->id() : 0 )   << endl;
        out << setw(30) << setfill(' ') << left << "Weight" <<   right << v->getWeight()  << endl;

        return out;
    }

    std::ostream& operator<<( std::ostream& out, const EVENT::LCRelation &v){
        out << lcio_long(v,NULL);
        return out;
    }




    //============================================================================
    //   # LCRunHeader
    //============================================================================

    const std::string& header(const EVENT::LCRunHeader *){ //hauke
        static std::string _h(" [ Run number ] | Detector name | Description | Parameters |\n");
        return _h;
    }

    const std::string& tail(const EVENT::LCRunHeader *){ //hauke
        static std::string _t("----------------|---------------|-------------|------------|\n");
        return _t;
    }

    std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::LCRunHeader>& sV){ //hauke
        const EVENT::LCRunHeader* hit = sV.obj;
        out << noshowpos;
        out << " [" << dec << setw(8) << setfill('0') << hit->getRunNumber() << "] ";
        out << "|" << dec << setw(8) << setfill(' ') << hit->getDetectorName();
        out << "|" << dec << setw(8) << setfill(' ') << hit->getDescription();
        out << "|" << dec << setw(8) << setfill(' ') << hit->getParameters() << "|" << endl;
        return out;
    }

    std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::LCRunHeader> l) { //hauke
        const EVENT::LCRunHeader *hit = l.object();
        const EVENT::LCCollection *col = l.collection();

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
        sstream << dec << hit->id();
        out << setw(30) << setfill(' ') << left << "Run number"<< setfill(' ') << right << setw(40) << dec << hit->getRunNumber() <<dec << endl;
        out << setw(30) << setfill(' ') << left << "Detector name"<< setfill(' ') << right << setw(40) << dec << hit->getDetectorName() <<dec << endl;
        out << setw(30) << setfill(' ') << left << "Description"<< setfill(' ') << right << setw(40) << dec << hit->getDescription() <<dec << endl;
        out << setw(30) << setfill(' ') << left << "Parameters"<< setfill(' ') << right << setw(40) << dec << hit->getParameters() <<dec << endl;
        return out;
    }

    std::ostream& operator<<( std::ostream& out, const EVENT::LCRunHeader  &hit){
        out<<lcio_long(hit,NULL);
        return out;
    }




    //============================================================================
    //   # LCIO
    //============================================================================

    const std::string& header(const EVENT::LCIO *){ //hauke
        static std::string _h("-------------------");
        return _h;
    }

    const std::string& tail(const EVENT::LCIO *){ //hauke
        static std::string _t("-------------------\n");
        return _t;
    }

  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::LCIO>& /*sV*/){ //hauke
        out << noshowpos;
        out << " " << endl;
        return out;
    }

    std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::LCIO> l) { //hauke
        //const EVENT::LCIO *hit = l.object();
        const EVENT::LCCollection *col = l.collection();

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
        return out;
    }

    std::ostream& operator<<( std::ostream& out, const EVENT::LCIO  &hit){ //hauke
        out<<lcio_long(hit,NULL);
        return out;
    }




    //============================================================================
    //   # ParticleID
    //============================================================================

    const std::string& header(const EVENT::ParticleID *){ //hauke
        static std::string _h("|   Type   |    PDG   | Likelihood |Algorithm type|");
        return _h;
    }

    const std::string& tail(const EVENT::ParticleID *){ //hauke
        static std::string _t("\n");
        return _t;
    }

    std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::ParticleID>& sV){ //hauke
        const EVENT::ParticleID* hit = sV.obj;
        out << noshowpos;
        out << "|" << dec << setw(10) << setfill(' ') << hit->getType();
        out << "|" << dec << setw(10) << setfill(' ') << hit->getPDG();
        out << "|" << dec << setw(12) << setfill(' ') << hit->getLikelihood();
        out << "|" << dec << setw(14) << setfill(' ') << hit->getAlgorithmType();
        return out;
    }

    std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::ParticleID> l) { //hauke
        const EVENT::ParticleID *hit = l.object();
        const EVENT::LCCollection *col = l.collection();

        stringstream sstream;

        out << noshowpos;
        out << setw(41) << setfill('-') << right << " ParticleID " << setfill('-') << setw(29) << "-" << endl;

        if(col != NULL){
            if(col->getTypeName() != LCIO::PARTICLEID){
                out << "Warning: collection not of type " << LCIO::PARTICLEID << endl ;
                return(out);

            }
            //print collection flags
        }

        //print object attributs
        out << setw(30) << setfill(' ') << left << "Type"<< setfill(' ') << right << setw(40) << dec << hit->getType() << endl;
        out << setw(30) << setfill(' ') << left << "PDG"<< setfill(' ') << right << setw(40) << dec << hit->getPDG() << endl;
        out << setw(30) << setfill(' ') << left << "Likelihood  "<< setfill(' ') << right << setw(40) << dec << hit->getLikelihood() << endl;
        out << setw(30) << setfill(' ') << left << "Algorithm type"<< setfill(' ') << right << setw(40) << dec << hit->getAlgorithmType() << endl;
        return out;
    }

    std::ostream& operator<<( std::ostream& out, const EVENT::ParticleID  &hit){
        out<<lcio_long(hit,NULL);
        return out;
    }




    //============================================================================
    //   # RawCalorimeterHit
    //============================================================================

    const std::string& header(const EVENT::RawCalorimeterHit *){ //hauke
        static std::string _h(" [   id   ] |  cellId0 ( M, S, I, J, K) | cellId1  | amplitude |  time  \n"); 
        return _h;
    }

    const std::string& tail(const EVENT::RawCalorimeterHit *){ //hauke
        static std::string _t("------------|---------------------------|----------|-----------|-------\n");
        return _t;
    }

    std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::RawCalorimeterHit>& sV){ //hauke
        const EVENT::RawCalorimeterHit* hit = sV.obj;
        const EVENT::LCCollection *col = sV.col;

        out << noshowpos;
        out << " [" << setw(8) << setfill('0') << dec << hit->id();
        out << "| " << dec << setw(8) << setfill('0') << hit->getCellID0();
        out << "| " << dec << setw(8) << setfill('0') << hit->getCellID1();
        out << "|" << dec << setw(11) << setfill(' ') << hit->getAmplitude();
        out << "|" << dec << setw(11) << setfill(' ') << hit->getTimeStamp() << "|" << endl;

        if(col->getParameters().getStringVal(LCIO::CellIDEncoding) != ""){
            CellIDDecoder<RawCalorimeterHit> id(col);
            // out << endl << "        id-fields: ("<< id(const_cast<EVENT::RawCalorimeterHit*>(hit)).valueString() << ")" << std::endl ;
            out << endl << "        id-fields: ("<< id(hit).valueString() << ")" << std::endl ;

        } else{
            out << endl << "        id-fields: --- unknown/default ----   ";
        }

        return out;
    }

    std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::RawCalorimeterHit> l) { //hauke
        const EVENT::RawCalorimeterHit *hit = l.object();
        const EVENT::LCCollection *col = l.collection();

        stringstream sstream;

        out << noshowpos;
        out << setw(41) << setfill('-') << right << " RawCalorimeterHit " << setfill('-') << setw(29) << "-" << endl;

        if(col != NULL){
            if(col->getTypeName() != LCIO::RAWCALORIMETERHIT){
                out << "Warning: collection not of type " << LCIO::RAWCALORIMETERHIT << endl ;
                return(out);
            }
            BitSet32 flag( col->getFlag() ) ;
            out << "     LCIO::RCHBIT_ID1    : " << flag.test( LCIO::RCHBIT_ID1 ) << endl ;
            out << "     LCIO::RCHBIT_TIME   : " << flag.test( LCIO::RCHBIT_TIME ) << endl ;
            out << "     LCIO::RCHBIT_NO_PTR : " << flag.test( LCIO::RCHBIT_NO_PTR ) << endl ;
            //print collection flags
        }

        //print object attributs
        out << setw(30) << setfill(' ') << left << "CellID0"<< setfill(' ') << right << setw(40) << dec << hit->getCellID0() << endl;
        out << setw(30) << setfill(' ') << left << "CellID1"<< setfill(' ') << right << setw(40) << dec << hit->getCellID1() << endl;
        out << setw(30) << setfill(' ') << left << "Amplitude"<< setfill(' ') << right << setw(40) << dec << hit->getAmplitude() << endl;
        out << setw(30) << setfill(' ') << left << "TimeStamp"<< setfill(' ') << right << setw(40) << dec << hit->getTimeStamp() << endl;
        return out;
    }

    std::ostream& operator<<( std::ostream& out, const EVENT::RawCalorimeterHit  &hit){
        out<<lcio_long(hit,NULL);
        return out;
    }




    //============================================================================
    //   # TPCHit (deprecated)
    //============================================================================

    /*
       const std::string& header(const EVENT::TPCHit &){ //hauke
       static std::string _h("");
       return _h;
       }

       const std::string& tail(const EVENT::TPCHit &){ //hauke
       static std::string _t("\n");
       return _t;
       }

       std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::TPCHit>& sV){ //hauke
       const EVENT::TPCHit* hit = sV.obj;
       out << noshowpos;
       out << " [" << dec << setw(8) << setfill('0');// << hit->id() << "] ";
       }

       std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::TPCHit> l) {
       const EVENT::TPCHit *hit = l.object();
       const EVENT::LCCollection *col = l.collection();

       stringstream sstream;

       out << noshowpos;
       out << setw(41) << setfill('-') << right << " TPCHit " << setfill('-') << setw(29) << "-" << endl;

       if(col != NULL){
       if(col->getTypeName() != LCIO::TPCHIT){
       out << "Warning: collection not of type " << LCIO::TPCHIT << endl ;
       return(out);

       }
    //print collection flags
    }

    //print object attributs
     //sstream  << dec << hit->id();
    //out << setw(30) << setfill(' ') << left << "Id" << right << setw(40) << sstream.str() << endl;
    //out << setw(30) << setfill(' ') << left << "Type"<< setfill(' ') << right << setw(40) << hit->getType() << endl;
    //out << setw(30) << left << "Energy [GeV]" << right << setw(40) << hit->getEnergy() << endl;
    }

    std::ostream& operator<<( std::ostream& out, const EVENT::TPCHit  &hit){
    out<<lcio_long(hit,NULL);
    return out;
    }
     */




    //============================================================================
    //   # TrackerData
    //============================================================================

    const std::string& header(const EVENT::TrackerData *){ //hauke
        static std::string _h(" [   id   ] |  cellid0 |  cellid1 |   time   | cellid-fields  \n");
        return _h;
    }

    const std::string& tail(const EVENT::TrackerData *){ //hauke
        static std::string _t("------------|----------|----------|----------|----------------\n");
        return _t;
    }

    std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::TrackerData>& sV){ //hauke
        const EVENT::TrackerData* hit = sV.obj;
        const EVENT::LCCollection* col =  sV.col;

        out << noshowpos;
        out << " [" << dec << setw(8) << setfill('0') << hit->id() << "] |";
        out << " " << dec << setw(8) << setfill('0') << hit->getCellID0() << " |";
        out << " " << dec << setw(8) << setfill('0') << hit->getCellID1() << " |";
        out << " " << dec << setw(8) << setfill(' ') << hit->getTime() << " |"; 

        if(col->getParameters().getStringVal(  LCIO::CellIDEncoding ) != ""){
            CellIDDecoder<TrackerData> id(col);
            out<< " " << id(const_cast<EVENT::TrackerData*>(hit)).valueString();     
        } else{
            out << "    --- unknown/default ----   ";
        }

        out << "chargeADC : " ;
        const FloatVec& charge = hit->getChargeValues() ;

        for( unsigned j=0 ; j < charge.size() ; j++ ){
            out << charge[j] << "," ;
        }

        out << endl;
        return out;
    }

    std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::TrackerData> l) { //hauke
        const EVENT::TrackerData *hit = l.object();
        const EVENT::LCCollection *col = l.collection();

        stringstream sstream;

        out << noshowpos;
        out << setw(41) << setfill('-') << right << " TrackerData " << setfill('-') << setw(29) << "-" << endl;

        if(col != NULL){
            if(col->getTypeName() != LCIO::TRACKERDATA){
                out << "Warning: collection not of type " << LCIO::TRACKERDATA << endl ;
                return(out);

            }
            //print collection flags
        }

        //print object attributs
        out << setw(30) << setfill(' ') << left << "CellID0"<< setfill(' ') << right << setw(40) << dec << hit->getCellID0() << endl;
        out << setw(30) << setfill(' ') << left << "CellID1"<< setfill(' ') << right << setw(40) << dec << hit->getCellID1() << endl;
        out << setw(30) << setfill(' ') << left << "Time"<< setfill(' ') << right << setw(40) << dec << hit->getTime() << endl;
        return out;
    }

    std::ostream& operator<<( std::ostream& out, const EVENT::TrackerData  &hit){
        out<<lcio_long(hit,NULL);
        return out;
    }




    //============================================================================
    //   # TrackerPulse
    //============================================================================

    const std::string& header(const EVENT::TrackerPulse *){ //hauke
        static std::string _h(" [   id   ] | cellid0  | cellid1  | time | charge | quality  |corr.Data | cellid-fields: | cov(c,c), cov(t,c), cov(t,t) \n");
        return _h;
    }

    const std::string& tail(const EVENT::TrackerPulse *){ //hauke
        static std::string _t("------------|----------|----------|------|--------|----------|----------|----------------|------------------------------\n");
        return _t;
    }

    std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::TrackerPulse>& sV){ //hauke
        const EVENT::TrackerPulse* hit = sV.obj;
        const EVENT::LCCollection* col =  sV.col;

        out << noshowpos;
        out << " [" << dec << setw(8) << setfill('0') << hit->id() << "] |"; 
        out << " " << dec << setw(8) << setfill('0') << hit->getCellID0() << " |";
        out << " " << dec << setw(8) << setfill('0') << hit->getCellID1() << " |";
        out << dec << setw(6) << setfill(' ') << hit->getTime() << "|";
        out << dec << setw(8) << setfill(' ') << hit->getCharge() << "|";
        out << dec << setw(10) << setfill(' ') << hit->getQuality() << endl;
        TrackerData* corr =  hit->getTrackerData() ;
        if( corr != 0 ){
            out << "|" << dec << setfill('0') << setw(10)  << corr->id() << "]";
        }else{
            out << "|[" << dec << setfill('0') << setw(10)  << '0' << "]";
        }

        if(col->getParameters().getStringVal(  LCIO::CellIDEncoding ) != ""){
            CellIDDecoder<TrackerPulse> id(col);
            out<< "|" << id(const_cast<EVENT::TrackerPulse*>(hit)).valueString() << "|";     
        } else{
            out << "|    --- unknown/default ----   |";
        }



        for( unsigned int j=0; j < hit->getCovMatrix().size(); j++ ){
            out << hit->getCovMatrix()[j] << ", ";
        }

        out << endl;
        return out;
    }

    std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::TrackerPulse> l) {//hauke
        const EVENT::TrackerPulse *hit = l.object();
        const EVENT::LCCollection *col = l.collection();

        stringstream sstream;

        out << noshowpos;
        out << setw(41) << setfill('-') << right << " TrackerPulse " << setfill('-') << setw(29) << "-" << endl;

        if(col != NULL){
            if(col->getTypeName() != LCIO::TRACKERPULSE){
                out << "Warning: collection not of type " << LCIO::TRACKERPULSE << endl ;
                return(out);
            }
            //print collection flags
            BitSet32 flag( col->getFlag() ) ;
            out << "     LCIO::TRAWBIT_ID1    : " << flag.test( LCIO::TRAWBIT_ID1 ) << endl ;
        }

        //print object attributs
        out << setw(30) << setfill(' ') << left << "CellID0"<< setfill(' ') << right << setw(40) << dec << hit->getCellID0() << endl;
        out << setw(30) << setfill(' ') << left << "CellID1"<< setfill(' ') << right << setw(40) << dec << hit->getCellID1() << endl;
        out << setw(30) << setfill(' ') << left << "Time"<< setfill(' ') << right << setw(40) << dec << hit->getTime() << endl;
        out << setw(30) << setfill(' ') << left << "Charge"<< setfill(' ') << right << setw(40) << dec << hit->getCharge() << endl;
        out << setw(30) << setfill(' ') << left << "Quality"<< setfill(' ') << right << setw(40) << dec << hit->getQuality() << endl;
        return out;
    }

    std::ostream& operator<<( std::ostream& out, const EVENT::TrackerPulse  &hit){
        out<<lcio_long(hit,NULL);
        return out;
    }




    //============================================================================
    //   # TrackerRawData
    //============================================================================

    const std::string& header(const EVENT::TrackerRawData *){ //hauke
        static std::string _h(" [   id   ] |  cellid0 |  cellid1 |   time   | cellid-fields  \n");
        return _h;
    }

    const std::string& tail(const EVENT::TrackerRawData *){ //hauke
        static std::string _t("------------|----------|----------|----------|----------------\n");
        return _t;
    }

    std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::TrackerRawData>& sV){ //hauke
        const EVENT::TrackerRawData* hit = sV.obj;
        const EVENT::LCCollection* col =  sV.col;

        out << noshowpos;
        out << " [" << dec << setw(8) << setfill('0') << hit->id() << "] |";
        out << " " << dec << setw(8) << setfill('0') << hit->getCellID0() << " |";
        out << " " << dec << setw(8) << setfill('0') << hit->getCellID1() << " |";
        out << " " << dec << setw(8) << setfill(' ') << hit->getTime() << " |"; 

        if(col->getParameters().getStringVal(  LCIO::CellIDEncoding ) != ""){
            CellIDDecoder<TrackerRawData> id(col);
            out<< " " << id(const_cast<EVENT::TrackerRawData*>(hit)).valueString();     
        } else{
            out << "    --- unknown/default ----   ";
        }

        out << " adc values: " ;
        const ShortVec& charge = hit->getADCValues() ;
        for( unsigned j=0 ; j < charge.size() ; j++ ) {
            out << charge[j] << "," ;
        }

        out << endl;
        return out;
    }

    std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::TrackerRawData> l) { //hauke
        const EVENT::TrackerRawData *hit = l.object();
        const EVENT::LCCollection *col = l.collection();

        stringstream sstream;

        out << noshowpos;
        out << setw(41) << setfill('-') << right << " TrackerRawData " << setfill('-') << setw(29) << "-" << endl;

        if(col != NULL){
            if(col->getTypeName() != LCIO::TRACKERRAWDATA){
                out << "Warning: collection not of type " << LCIO::TRACKERRAWDATA << endl ;
                return(out);

            }
            //print collection flags
        }

        //print object attributs
        out << setw(30) << setfill(' ') << left << "CellID0"<< setfill(' ') << right << setw(40) << dec << hit->getCellID0() << endl;
        out << setw(30) << setfill(' ') << left << "CellID1"<< setfill(' ') << right << setw(40) << dec << hit->getCellID1() << endl;
        out << setw(30) << setfill(' ') << left << "Time"<< setfill(' ') << right << setw(40) << dec << hit->getTime() << endl;
        return out;
    }

    std::ostream& operator<<( std::ostream& out, const EVENT::TrackerRawData  &hit){
        out<<lcio_long(hit,NULL);
        return out;
    }





    //============================================================================
    //   # SimCalorimeterHit
    //============================================================================

    const std::string& header(const EVENT::SimCalorimeterHit *){ //hauke
        static std::string _h(" [   id   ] |cellId0 |cellId1 |  energy  |        position (x,y,z)          | nMCParticles \n           -> MC contribution: prim. PDG |  energy  |   time   | length  | sec. PDG | stepPosition (x,y,z) \n");
        return _h;
    }

    const std::string& tail(const EVENT::SimCalorimeterHit *){ //hauke
        static std::string _t("------------|--------|--------|----------|----------------------------------|--------------\n");
        return _t;
    }

    std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::SimCalorimeterHit>& sV){ //hauke
        const EVENT::SimCalorimeterHit* hit = sV.obj;
        const EVENT::LCCollection* col =  sV.col;
        BitSet32 flag(col->getFlag());

        out << noshowpos;
        out << " [" << dec << setw(8) << setfill('0') << hit->id() << "] ";
        out << "|" << dec << setw(8) << setfill('0') << hit->getCellID0();
        out << "|" << dec << setw(8) << setfill('0') << hit->getCellID1();
        out << "|" << dec << setprecision(3) << scientific << showpos << hit->getEnergy(); 
        if(flag.test( LCIO::CHBIT_LONG )){
            out << "|" << dec << setprecision(3) << scientific << showpos
                << hit->getPosition()[0] << ", " << hit->getPosition()[1] << ", " << hit->getPosition()[2]; 
        }else{
            out << "|      no position available       ";
        }
        out << "|" << dec << setw(12) << setfill(' ') << hit->getNMCContributions() << endl;


        if(col->getParameters().getStringVal(LCIO::CellIDEncoding) != ""){
            CellIDDecoder<SimCalorimeterHit> id(col);
            out << "        id-fields: ("<< id(const_cast<EVENT::SimCalorimeterHit*>(hit)).valueString() << ")" << std::endl ;
        } else{
            out << "        id-fields: --- unknown/default ----   ";
        }


        for(int k=0;k < hit->getNMCContributions();k++){
            try{
                out << "           ->" << setfill(' ') << right << "                 ";
                out << setw(11) << ( hit->getParticleCont(k) ? hit->getParticleCont(k)->getPDG()  : 0 ) << "|";
                out << setw(10) << hit->getEnergyCont(k)<< "|";
                out << setw(6) << hit->getTimeCont(k) << "|";

                if(flag.test(LCIO::CHBIT_STEP)){
                    out << hit->getLengthCont( k ) << "| (" ;
                    out << hit->getPDGCont( k ) << ", " ;
                    out << hit->getStepPosition( k )[0] << ", " ;
                    out << hit->getStepPosition( k )[1] << ", " ;
                    out << hit->getStepPosition( k )[2] << ")" ;
                }else{
                    out << " no PDG";
                }
            }catch(exception& e){
                out << e.what() << endl ;
            }
            if(k <  hit->getNMCContributions()-1 ){ out << endl; }
        }

        out << endl;
        out << noshowpos;

        return(out);
    }

    std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::SimCalorimeterHit> ll){
        const EVENT::SimCalorimeterHit *hit = ll.object();
        const EVENT::LCCollection *col = ll.collection();

        stringstream tmp;
        //out << setprecision(5) << fixed;

        out << noshowpos;
        out << setw(46) <<  std::setfill('-') << right << " SimCalorimeterHit " << setfill('-') << setw(24) << "-" << endl;

        if(col != NULL){ 
            if(col->getTypeName() != LCIO::SIMCALORIMETERHIT){
                out << "Warning: collection not of type " << LCIO::SIMCALORIMETERHIT << endl ;
                return(out);

            }
            tmp.str(""); 
            tmp << "0x" << hex << col->getFlag() << dec;
            out << setw(30) << setfill(' ') << left << "Collection Flag" << right << setw(40) <<  tmp.str() << endl;
            LCTOOLS::printParameters(col->getParameters());

            BitSet32 flag( col->getFlag() ) ;
            out << "  -> LCIO::CHBIT_LONG   : " << flag.test( LCIO::CHBIT_LONG ) << endl ;
            out << "     LCIO::CHBIT_BARREL : " << flag.test( LCIO::CHBIT_BARREL ) << endl ;
            out << "     LCIO::CHBIT_ID1    : " << flag.test( LCIO::CHBIT_ID1 ) << endl ;
            out << "     LCIO::CHBIT_STEP   : " << flag.test( LCIO::CHBIT_STEP ) << endl ;
        } 
        tmp << dec << hit->id() << dec;
        out << setw(30) << setfill(' ') << std::left << "Id" <<   right << setw(40) << tmp.str() << endl;

        //out << setw(30) << left << "CellID0"<< setfill(' ') << right << setw(40) << dec << hit->getCellID0() << endl;
        tmp.str("");
        tmp << dec << hit->getCellID0() << dec;
        out << setw(30) << setfill(' ') << std::left << "CellID0" <<   right << setw(40) << tmp.str() << endl;

        //out << setw(30) << left << "CellID1"<< setfill(' ') << right << setw(40) << dec << hit->getCellID1() << endl;
        tmp.str("");
        tmp << dec << hit->getCellID1() << dec;
        out << setw(30) << setfill(' ') << std::left << "CellID1" <<   right << setw(40) << tmp.str() << endl;

        out << setw(30) << left << "Energy [GeV]" << right << setw(40) << dec << hit->getEnergy() << std::endl;

        tmp.str("");
        tmp  << hit->getPosition()[0] << ", " << hit->getPosition()[1]  << ", " << hit->getPosition()[2]; 
        if(col != NULL){ 
            BitSet32 flag(col->getFlag());
            if(flag.test(LCIO::CHBIT_LONG)){
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
                out << setw(30) << left << "                Prim. PDG" << right << setw(40) << ( hit->getParticleCont(k) ? hit->getParticleCont(k)->getPDG()  : 0 ) << endl;
                out << setw(30) << left << "                Energy [GeV]" << right << setw(40) << hit->getEnergyCont(k) << endl;
                out << setw(30) << left << "                Time" << right << setw(40) << hit->getTimeCont(k) << endl;
                out << setw(30) << left << "                Length" << right << setw(40) << hit->getLengthCont(k) << endl;
                out << setw(30) << left << "                Sec. PDG" << right << setw(40) << hit->getPDGCont(k) << endl;
            }catch(exception& e){
                out << e.what() << endl ;
            }
        }

        out << noshowpos;
        return(out);
    }

    std::ostream& operator<<( std::ostream& out, const EVENT::SimCalorimeterHit  &hit){
        out<<lcio_long(hit,NULL);
        return out;
    }




    //============================================================================
    //   # TrackerHit
    //============================================================================

    const std::string& header(const EVENT::TrackerHit *){ //hauke
        static std::string _h(" [   id   ] |cellId0 |cellId1 | position (x,y,z)            | time    |[type]||[qual.]| EDep    |EDepError|  cov(x,x),  cov(y,x),  cov(y,y),  cov(z,x),  cov(z,y),  cov(z,z)\n");
        return _h;
    }

    const std::string& tail(const EVENT::TrackerHit *){ //hauke
        static std::string _t("------------|--------|--------|-----------------------------|---------|------|------|---------|---------|-----------------------------------------------------------------\n");
        return _t;
    }

    std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::TrackerHit>& sV){ //hauke
        const EVENT::TrackerHit* part = sV.obj;
        const EVENT::LCCollection* col =  sV.col;
        BitSet32 flag(col->getFlag());

        out << " [" << setfill('0') << setw(8) << dec << part->id() << "] ";
        out << "|" << setw(8) << setfill('0') << part->getCellID0();
        out << "|" << setw(8) << setfill('0') << part->getCellID1();
        out << "|" << showpos << scientific << setprecision (2) << part->getPosition()[0] << ","<< part->getPosition()[1] << "," << part->getPosition()[2] << "|"; 
        out << part->getTime() << "|";
        out << "[" << noshowpos << setw(4) << part->getType() << "]|";
        out << "[" << noshowpos << setw(4) << part->getQuality() << "]|";
        out << showpos << part->getEDep() << "|";
        out << part->getEDepError() << "|";
        unsigned int i;
        for(i=0;i<(part->getCovMatrix().size()-1);i++){
            out << " " << showpos << scientific << setprecision(2) << part->getCovMatrix()[i] << ",";
        }
        out << " " << showpos << scientific << setprecision(2) << part->getCovMatrix()[part->getCovMatrix().size()-1] << endl;

        const LCObjectVec& rawHits = part->getRawHits();
        if( ! rawHits.empty() ){
            out << "    rawHits ("<< rawHits.size() <<"): ";
        }
        try{
            for( i=0 ; i < rawHits.size() ; i++ ){
                if( rawHits[i] == 0 ) continue ;
                out << dec << "[" << rawHits[i]->id() << "], " <<  dec ;
            }
        }catch(std::exception& e){}

        out << dec << endl ;
        if(col->getParameters().getStringVal(LCIO::CellIDEncoding) != ""){
            CellIDDecoder<TrackerHit> id(col);
            out << "    id-fields: ("<< id(const_cast<EVENT::TrackerHit*>(part)).valueString() << ")" << std::endl ;
        } else{
            out << "    id-fields: --- unknown/default ----   ";
        }

        out << noshowpos << fixed;
        out << endl ;
        return out;
    }

    std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::TrackerHit> ll){
        const EVENT::TrackerHit *hit = ll.object();
        const EVENT::LCCollection *col = ll.collection();

        stringstream tmp;
        out << noshowpos;
        out << setw(41) <<  setfill('-') << right << " TrackerHit " << setfill('-') << setw(29) << "-" << endl;
        if(col != NULL){ 
            if(col->getTypeName() != LCIO::TRACKERHIT){
                out << "Warning: collection not of type " << LCIO::TRACKERHIT << endl ;
                return(out);

            }
            tmp.str(""); 
            tmp << "0x" << hex << col->getFlag() << dec;
            out << setw(30) << setfill(' ') << left << "Collection Flag" << right << setw(40) <<  tmp.str() << endl;
            LCTOOLS::printParameters(col->getParameters());

            BitSet32 flag( col->getFlag() ) ;
            out << "     LCIO::CHBIT_BARREL : " << flag.test( LCIO::CHBIT_BARREL ) << endl ;
        } 

        tmp.str("");
        tmp << dec << hit->id() << dec;
        out << setw(30) << setfill(' ') << left << "Id" <<   right << setw(40) << tmp.str() << endl;

        //out << setw(30) << left << "CellID0"<< setfill(' ') << right << setw(40) << dec << hit->getCellID0() << endl;
        tmp.str("");
        tmp  << dec << hit->getCellID0() << dec;
        out << setw(30) << setfill(' ') << std::left << "CellID0" <<   right << setw(40) << tmp.str() << endl;

        //out << setw(30) << left << "CellID1"<< setfill(' ') << right << setw(40) << dec << hit->getCellID1() << endl;
        tmp.str("");
        tmp  << dec << hit->getCellID1() << dec;
        out << setw(30) << setfill(' ') << std::left << "CellID1" <<   right << setw(40) << tmp.str() << endl;

        tmp.str("");
        tmp << hit->getPosition()[0] << ", " << hit->getPosition()[1]  << ", " << hit->getPosition()[2]; 
        out << setw(30) << left << "Position (x,y,z) " << setfill(' ') << right <<setw(40) << tmp.str() << endl;
        out << setw(30) << left << "E_deposited " << setfill(' ') << right << setw(40) << hit->getEDep() << endl;
        out << setw(30) << left << "Time " << setfill(' ') << right << setw(40) << hit->getTime() << endl;
        out << setw(30) << left << "Type    " << setfill(' ') << right << setw(40) << BitSet32( hit->getType() ) << endl;
        out << setw(30) << left << "Quality " << setfill(' ') << right << setw(40) << BitSet32( hit->getQuality() )<< endl;
        out << noshowpos;

        return(out);
    }

    std::ostream& operator<<( std::ostream& out, const EVENT::TrackerHit &hit){ //hauke
        out << lcio_long(hit,NULL);
        return out;
    }




    //============================================================================
    //   # TrackerHitPlane
    //============================================================================

    const std::string& header(const EVENT::TrackerHitPlane *){
        static std::string _h(" [   id   ] |cellId0 |cellId1 | position (x,y,z)            | time    |[type]|[qual.]| EDep    |EDepError|   du    |   dv    |q|  u (theta, phi)   |  v (theta, phi)\n");
        return _h;
    }

    const std::string& tail(const EVENT::TrackerHitPlane *){
        static std::string _t("------------|--------|--------|-----------------------------|---------|------|------|---------|---------|---------|---------|-|-------------------|-------------------|\n");
        return _t;
    }

    std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::TrackerHitPlane>& sV){
        const EVENT::TrackerHitPlane* part = sV.obj;
        const EVENT::LCCollection* col =  sV.col;
        BitSet32 flag(col->getFlag());

        out << " [" << setfill('0') << setw(8) << dec << part->id() << "] ";
        out << "|" << setw(8) << setfill('0') << part->getCellID0();
        out << "|" << setw(8) << setfill('0') << part->getCellID1();
        out << "|" << showpos << scientific << setprecision (2) << part->getPosition()[0] << ","<< part->getPosition()[1] << "," << part->getPosition()[2] << "|"; 
        out << part->getTime() << "|";
        out << "[" << noshowpos << setw(4) << part->getType() << "]|";
        out << "[" << noshowpos << setw(4) << part->getQuality() << "]|";
        out << showpos << part->getEDep() << "|";
        out << part->getEDepError() << "|";
        out << part->getdU() << "|";
        out << part->getdV() << "|";
        out << part->getQuality() << "|";

        out << part->getU()[0] << "," << part->getU()[1] << "|";
        out << part->getV()[0] << "," << part->getV()[1] << "|";

        unsigned int i;
        //for(i=0;i<(part->getCovMatrix().size()-1);i++){
        //    out << " " << showpos << scientific << setprecision(2) << part->getCovMatrix()[i] << ",";
        //}
        //out << " " << showpos << scientific << setprecision(2) << part->getCovMatrix()[part->getCovMatrix().size()-1] << endl;

        const LCObjectVec& rawHits = part->getRawHits();
        if( ! rawHits.empty() ){
            out << "    rawHits ("<< rawHits.size() <<"): ";
        }
        try{
            for( i=0 ; i < rawHits.size() ; i++ ){
                if( rawHits[i] == 0 ) continue ;
                out << dec << "[" << rawHits[i]->id() << "], " <<  dec ;
            }
        }catch(std::exception& e){}

        out << dec << endl ;
        if(col->getParameters().getStringVal(LCIO::CellIDEncoding) != ""){
            CellIDDecoder<TrackerHit> id(col);
            out << "    id-fields: ("<< id(const_cast<EVENT::TrackerHitPlane*>(part)).valueString() << ")" << std::endl ;
        } else{
            out << "    id-fields: --- unknown/default ----   ";
        }

        out << noshowpos << fixed;
        out << endl ;
        return out;
    }

    std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::TrackerHitPlane> ll){
        const EVENT::TrackerHitPlane *hit = ll.object();
        const EVENT::LCCollection *col = ll.collection();

        stringstream tmp;
        out << noshowpos;
        out << setw(41) <<  setfill('-') << right << " TrackerHitPlane " << setfill('-') << setw(29) << "-" << endl;
        if(col != NULL){ 
            if(col->getTypeName() != LCIO::TRACKERHITPLANE){
                out << "Warning: collection not of type " << LCIO::TRACKERHITPLANE << endl ;
                return(out);

            }
            tmp.str(""); 
            tmp << "0x" << hex << col->getFlag() << dec;
            out << setw(30) << setfill(' ') << left << "Collection Flag" << right << setw(40) <<  tmp.str() << endl;
            LCTOOLS::printParameters(col->getParameters());

            //BitSet32 flag( col->getFlag() ) ;
            //out << "     LCIO::CHBIT_BARREL : " << flag.test( LCIO::CHBIT_BARREL ) << endl ;
        } 

        tmp.str("");
        tmp  << dec << hit->id() << dec; 
        out << setw(30) << setfill(' ') << left << "Id" <<   right << setw(40) << tmp.str() << endl;

        //out << setw(30) << left << "CellID0"<< setfill(' ') << right << setw(40) << dec << hit->getCellID0() << endl;
        tmp.str("");
        tmp  << dec << hit->getCellID0() << dec;
        out << setw(30) << setfill(' ') << std::left << "CellID0" <<   right << setw(40) << tmp.str() << endl;

        //out << setw(30) << left << "CellID1"<< setfill(' ') << right << setw(40) << dec << hit->getCellID1() << endl;
        tmp.str("");
        tmp  << dec << hit->getCellID1() << dec;
        out << setw(30) << setfill(' ') << std::left << "CellID1" <<   right << setw(40) << tmp.str() << endl;

        tmp.str("");
        tmp << hit->getPosition()[0] << ", " << hit->getPosition()[1]  << ", " << hit->getPosition()[2]; 
        out << setw(30) << left << "Position (x,y,z) " << setfill(' ') << right <<setw(40) << tmp.str() << endl;
        out << setw(30) << left << "EDep " << setfill(' ') << right << setw(40) << hit->getEDep() << endl;
        out << setw(30) << left << "EDepError " << setfill(' ') << right << setw(40) << hit->getEDepError() << endl;
        out << setw(30) << left << "Time " << setfill(' ') << right << setw(40) << hit->getTime() << endl;
        out << setw(30) << left << "Type    " << setfill(' ') << right << setw(40) << BitSet32( hit->getType() ) << endl;
        out << setw(30) << left << "Quality " << setfill(' ') << right << setw(40) << BitSet32( hit->getQuality() )<< endl;
        out << setw(30) << left << "dU " << setfill(' ') << right << setw(40) << hit->getdU() << endl;
        out << setw(30) << left << "dV " << setfill(' ') << right << setw(40) << hit->getdV() << endl;
        out << setw(30) << left << "U " << setfill(' ') << right << setw(40) << hit->getU()[0] << "," << hit->getU()[1] << endl;
        out << setw(30) << left << "V " << setfill(' ') << right << setw(40) << hit->getV()[0] << "," << hit->getV()[1] << endl;
        out << setw(30) << left << "Quality " << setfill(' ') << right << setw(40) << hit->getQuality() << endl;
        out << noshowpos;

        return(out);
    }

    std::ostream& operator<<( std::ostream& out, const EVENT::TrackerHitPlane &hit){
        out << lcio_long(hit,NULL);
        return out;
    }



    //============================================================================
    //   # TrackerHitZCylinder
    //============================================================================

    const std::string& header(const EVENT::TrackerHitZCylinder *){
        //static std::string _h(" [   id   ] |cellId0 |cellId1 | position (x,y,z)            | time    |[type]| EDep    |EDepError|    r    |  dRPhi  |    dZ   |q|    center (x,y)   |\n");
        static std::string _h(" [   id   ] |cellId0 |cellId1 | position (x,y,z)            | time    |[type]|[qual.]| EDep    |EDepError|  dRPhi  |    dZ   |q|    center (x,y)   |\n");
        return _h;
    }

    const std::string& tail(const EVENT::TrackerHitZCylinder *){
        static std::string _t("------------|--------|--------|-----------------------------|---------|------|------|---------|---------|---------|---------|-|-------------------|\n");
        return _t;
    }

    std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::TrackerHitZCylinder>& sV){
        const EVENT::TrackerHitZCylinder* part = sV.obj;
        const EVENT::LCCollection* col =  sV.col;
        BitSet32 flag(col->getFlag());

        out << " [" << setfill('0') << setw(8) << dec << part->id() << "] ";
        out << "|" << setw(8) << setfill('0') << part->getCellID0();
        out << "|" << setw(8) << setfill('0') << part->getCellID1();
        out << "|" << showpos << scientific << setprecision (2) << part->getPosition()[0] << ","<< part->getPosition()[1] << "," << part->getPosition()[2] << "|"; 
        out << part->getTime() << "|";
        out << "[" << noshowpos << setw(4) << part->getType() << "]|";
        out << "[" << noshowpos << setw(4) << part->getQuality() << "]|";
        out << showpos << part->getEDep() << "|";
        out << part->getEDepError() << "|";
        //out << part->getR() << "|";
        out << part->getdRPhi() << "|";
        out << part->getdZ() << "|";
        out << part->getQuality() << "|";
        out << part->getCenter()[0] << "," << part->getCenter()[1] << "|";

        unsigned int i;
        //for(i=0;i<(part->getCovMatrix().size()-1);i++){
        //    out << " " << showpos << scientific << setprecision(2) << part->getCovMatrix()[i] << ",";
        //}
        //out << " " << showpos << scientific << setprecision(2) << part->getCovMatrix()[part->getCovMatrix().size()-1] << endl;

        const LCObjectVec& rawHits = part->getRawHits();
        if( ! rawHits.empty() ){
            out << "    rawHits ("<< rawHits.size() <<"): ";
        }
        try{
            for( i=0 ; i < rawHits.size() ; i++ ){
                if( rawHits[i] == 0 ) continue ;
                out << dec << "[" << rawHits[i]->id() << "], " <<  dec ;
            }
        }catch(std::exception& e){}

        out << dec << endl ;
        if(col->getParameters().getStringVal(LCIO::CellIDEncoding) != ""){
            CellIDDecoder<TrackerHit> id(col);
            out << "    id-fields: ("<< id(const_cast<EVENT::TrackerHitZCylinder*>(part)).valueString() << ")" << std::endl ;
        } else{
            out << "    id-fields: --- unknown/default ----   ";
        }

        out << noshowpos << fixed;
        out << endl ;
        return out;
    }

    std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::TrackerHitZCylinder> ll){
        const EVENT::TrackerHitZCylinder *hit = ll.object();
        const EVENT::LCCollection *col = ll.collection();

        stringstream tmp;
        out << noshowpos;
        out << setw(41) <<  setfill('-') << right << " TrackerHitZCylinder " << setfill('-') << setw(29) << "-" << endl;
        if(col != NULL){ 
            if(col->getTypeName() != LCIO::TRACKERHITZCYLINDER){
                out << "Warning: collection not of type " << LCIO::TRACKERHITZCYLINDER << endl ;
                return(out);

            }
            tmp.str(""); 
            tmp << "0x" << hex << col->getFlag() << dec;
            out << setw(30) << setfill(' ') << left << "Collection Flag" << right << setw(40) <<  tmp.str() << endl;
            LCTOOLS::printParameters(col->getParameters());

            //BitSet32 flag( col->getFlag() ) ;
            //out << "     LCIO::CHBIT_BARREL : " << flag.test( LCIO::CHBIT_BARREL ) << endl ;
        } 

        tmp.str("");
        tmp  << dec << hit->id() << dec; 
        out << setw(30) << setfill(' ') << left << "Id" <<   right << setw(40) << tmp.str() << endl;

        //out << setw(30) << left << "CellID0"<< setfill(' ') << right << setw(40) << dec << hit->getCellID0() << endl;
        tmp.str("");
        tmp  << dec << hit->getCellID0() << dec;
        out << setw(30) << setfill(' ') << std::left << "CellID0" <<   right << setw(40) << tmp.str() << endl;

        //out << setw(30) << left << "CellID1"<< setfill(' ') << right << setw(40) << dec << hit->getCellID1() << endl;
        tmp.str("");
        tmp  << dec << hit->getCellID1() << dec;
        out << setw(30) << setfill(' ') << std::left << "CellID1" <<   right << setw(40) << tmp.str() << endl;

        tmp.str("");
        tmp << hit->getPosition()[0] << ", " << hit->getPosition()[1]  << ", " << hit->getPosition()[2]; 
        out << setw(30) << left << "Position (x,y,z) " << setfill(' ') << right <<setw(40) << tmp.str() << endl;
        out << setw(30) << left << "EDep " << setfill(' ') << right << setw(40) << hit->getEDep() << endl;
        out << setw(30) << left << "EDepError " << setfill(' ') << right << setw(40) << hit->getEDepError() << endl;
        out << setw(30) << left << "Time " << setfill(' ') << right << setw(40) << hit->getTime() << endl;
        out << setw(30) << left << "Type    " << setfill(' ') << right << setw(40) << BitSet32( hit->getType() ) << endl;
        out << setw(30) << left << "Quality " << setfill(' ') << right << setw(40) << BitSet32( hit->getQuality() )<< endl;
        //out << setw(30) << left << "r " << setfill(' ') << right << setw(40) << hit->getR() << endl;
        out << setw(30) << left << "dRPhi " << setfill(' ') << right << setw(40) << hit->getdRPhi() << endl;
        out << setw(30) << left << "dZ " << setfill(' ') << right << setw(40) << hit->getdZ() << endl;
        out << setw(30) << left << "Center(x,y) " << setfill(' ') << right << setw(40) << hit->getCenter()[0] << "," << hit->getCenter()[1] << endl;
        out << setw(30) << left << "Quality " << setfill(' ') << right << setw(40) << hit->getQuality() << endl;
        out << noshowpos;

        return(out);
    }

    std::ostream& operator<<( std::ostream& out, const EVENT::TrackerHitZCylinder &hit){
        out << lcio_long(hit,NULL);
        return out;
    }




    //============================================================================
    //   # SimTrackerHit
    //============================================================================

    const std::string& header(const EVENT::SimTrackerHit *){ //hauke
        static std::string _h(" [   id   ] |cellId0 |cellId1 |  position (x,y,z)               |   EDep   |   time   |PDG of MCParticle|   (px,  py, pz)   | pathLength  | Quality \n");
        return _h;
    }

    const std::string& tail(const EVENT::SimTrackerHit *){ //hauke
        static std::string _t("------------|--------|--------|---------------------------------|----------|----------|-----------------|-------------------|-------------|---------\n");
        return _t;
    }

    std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::SimTrackerHit>& sV){ //hauke
        const EVENT::SimTrackerHit* hit =  sV.obj;
        const EVENT::LCCollection* col =  sV.col;

        BitSet32 flag(col->getFlag()) ;
        bool pStored = flag.test(LCIO::THBIT_MOMENTUM);

        int pdgid = 0;
        if(hit->getMCParticle()){
            pdgid = hit->getMCParticle()->getPDG() ;
        }

        out << " [" << setfill('0') << setw(8) << dec<< hit->id() << "] " << dec;
        out << "|" << dec << setw(8) << setfill('0') << hit->getCellID0();
        out << "|" << dec << setw(8) << setfill('0') << hit->getCellID1();



        //out << setw(8) << setfill('0') << dec << hit->getCellID() << setfill(' '); 

        //out << dec << "[";
        //if(col->getParameters().getStringVal(  LCIO::CellIDEncoding ) != ""){
        //    CellIDDecoder<SimTrackerHit> id(col);
        //    out<< id(const_cast<EVENT::SimTrackerHit*>(hit)).valueString();     
        //} else{
        //    out << "    --- unknown/default ----   ";
        //}
        //out <<  "]|(";
        out <<  "|(";

        out << dec << showpos << setprecision(2) << scientific<< hit->getPosition()[0] << ", ";
        out << hit->getPosition()[1] << ", ";
        out << hit->getPosition()[2] << ")| ";
        out << noshowpos;
        out << hit->getEDep () << " | ";
        out << hit->getTime () << " | ";
        out << setw(14) << pdgid << "  ";


        if( pStored ){
            out << " | (";
            out << hit->getMomentum()[0] << ", ";
            out << hit->getMomentum()[1] << ", ";
            out << hit->getMomentum()[2] << ") | ";
            out << std::setw(13) << hit->getPathLength() << "|";
        }else{
            out << "|   unknown         |";
	    out << "     n/a     |"; // path length
        }

        //qualityBits
        out << LCTOOLS::getQualityBits(hit);

        out << endl ;

        if(col->getParameters().getStringVal(LCIO::CellIDEncoding) != ""){
            CellIDDecoder<SimTrackerHit> id(col);
            out << "        id-fields: ("<< id(const_cast<EVENT::SimTrackerHit*>(hit)).valueString() << ")" << std::endl ;
        } else{
            out << "        id-fields: --- unknown/default ----   ";
        }

        out << endl ;


        /*
           out << noshowpos << " " << setw(8) << dec << hit->getCellID() << "|";
           out << showpos << scientific << setprecision (2) << setfill(' ') << hit->getPosition()[0]<<","<<hit->getPosition()[1]<<","<<hit->getPosition()[2]<<"|";
           out << hit->getEDep() << "|";
           out << hit->getTime() << "|";
           if(hit->getMCParticle()){
           out << setw(17) << hit->getMCParticle()->getPDG() << "|";
           }else{
           out << "                 |";
           }
           out << hit->getMomentum()[0] << "," << hit->getMomentum()[1] << "," << hit->getMomentum()[2] << "|";
           out << setw(11) << hit->getPathLength() << endl;
           out << noshowpos;

         */
        return(out);
    }

    std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::SimTrackerHit> ll){
        const EVENT::SimTrackerHit *hit = ll.object();
        const EVENT::LCCollection *col = ll.collection();

        stringstream tmp;
        out << noshowpos;

        out << setw(42) <<  std::setfill('-') << right << " SimTrackerHit " << setfill('-') << setw(28) << "-" << endl;
        if(col != NULL){ 
            if(col->getTypeName() != LCIO::SIMTRACKERHIT){
                out << "Warning: collection not of type " << LCIO::SIMTRACKERHIT << endl ;
                return(out);

            }
            tmp.str(""); 
            tmp << col->getFlag() << dec;
            out << setw(30) << setfill(' ') << left << "Collection Flag" << right << setw(40) <<  tmp.str() << endl;
            LCTOOLS::printParameters(col->getParameters());

            BitSet32 flag( col->getFlag() ) ;
            out << "     LCIO::THBIT_BARREL : " << flag.test( LCIO::THBIT_BARREL ) << endl ;
            out << "     LCIO::THBIT_MOMENTUM : " << flag.test( LCIO::THBIT_MOMENTUM ) << endl ;

            out << LCTOOLS::getQualityBits() << endl;
        } 

        //out << setw(30) << left << "CellID0"<< setfill(' ') << right << setw(40) << dec << hit->getCellID0() << endl;
        tmp.str("");
        tmp << hit->getCellID0() << dec;
        out << setw(30) << setfill(' ') << std::left << "CellID0" <<   right << setw(40) << tmp.str() << endl;

        //out << setw(30) << left << "CellID1"<< setfill(' ') << right << setw(40) << dec << hit->getCellID1() << endl;
        tmp.str("");
        tmp << hit->getCellID1() << dec;
        out << setw(30) << setfill(' ') << std::left << "CellID1" <<   right << setw(40) << tmp.str() << endl;

        tmp.str("");
        tmp  << dec << hit->getPosition()[0] << ", " << hit->getPosition()[1]  << ", " << hit->getPosition()[2]; 
        out <<setw(30) << std::left << "Position [mm] (x,y,z) " << dec << setfill(' ') << right <<setw(40) << tmp.str() << endl;
        out <<setw(30) << std::left << "dE/dx [GeV]" << setfill(' ') << right <<setw(40) << hit->getEDep() << endl;
        out <<setw(30) << std::left << "Time [ns]" << setfill(' ') << right <<setw(40) << hit->getTime() << endl;
        if(hit->getMCParticle() != NULL){
          out <<std::setw(30) << std::left << "PDG of MCParticle " << setfill(' ') << right <<setw(40) << hit->getMCParticle()->getPDG() << std::endl;
          out <<std::setw(30) << std::left << "ID  of MCParticle " << setfill(' ') << right <<setw(40) << hit->getMCParticle()->id() << std::endl;
        }else{
            out <<std::setw(30) << std::left << "PDG of MCParticle " << setfill(' ') << right <<setw(40) << "[Empty]"  << std::endl;
        }

        tmp.str("");
        tmp  << hit->getMomentum()[0] << ", " << hit->getMomentum()[1]  << ", " << hit->getMomentum()[2]; 
        if(col != NULL){ 
            BitSet32 flag(col->getFlag());
            if(flag.test(LCIO::THBIT_MOMENTUM)){
                out <<setw(40) << left << "Momentum [GeV] (x,y,z) [not verified]" << setfill(' ') << right <<setw(30) << tmp.str() << endl;
            }else{
                out <<setw(30) << left << "Momentum [GeV] (x,y,z)" << setfill(' ') << right <<setw(40) << "not available"  << endl;
            }
        }else{
            out <<setw(40) << left << "Momentum [GeV] (x,y,z) [not verified]" << setfill(' ') << right <<setw(30) << tmp.str() << endl;
        }
        out <<setw(30) << left << "PathLength " << setfill(' ') << right <<setw(40) << hit->getPathLength() << endl;

        out << noshowpos;

        return(out);
    }

    std::ostream& operator<<( std::ostream& out, const EVENT::SimTrackerHit &hit){ //hauke
        out << lcio_long(hit,NULL);
        return(out);
    }




    //============================================================================
    //   # CalorimeterHit
    //============================================================================

    const std::string& header(const EVENT::CalorimeterHit *){ //hauke
        static std::string _h(" [   id   ] |cellId0 |cellId1 |  energy  |energyerr |        position (x,y,z)           \n");
        return _h;
    }

    const std::string& tail(const EVENT::CalorimeterHit *){ //hauke
        static std::string _t("------------|--------|--------|----------|----------|-----------------------------------\n");
        return _t;
    }

    std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::CalorimeterHit>& sV){ //hauke
        const EVENT::CalorimeterHit* hit = sV.obj;
        const EVENT::LCCollection *col = sV.col;
        BitSet32 flag( col->getFlag() ) ;

        out << noshowpos;

        out << " [" << dec << setw(8) << setfill('0') << hit->id() << "] ";
        out << "|" << dec << setw(8) << setfill('0') << hit->getCellID0();
        out << "|" << dec << setw(8) << setfill('0') << hit->getCellID1();
        out << "|" << dec << setprecision(3) << scientific << showpos << hit->getEnergy(); 
        out << "|" << dec << setprecision(3) << scientific << hit->getEnergyError(); 
        if( flag.test( LCIO::CHBIT_LONG ) ){
            out << "|" << dec << setprecision(3) << scientific << showpos
                << hit->getPosition()[0] << ", " << hit->getPosition()[1] << ", " << hit->getPosition()[2]; 
        }else{
            out << "    no position available         ";
        }
        if(col->getParameters().getStringVal(LCIO::CellIDEncoding) != ""){
            CellIDDecoder<CalorimeterHit> id(col);
            out << endl << "        id-fields: ("<< id(const_cast<EVENT::CalorimeterHit*>(hit)).valueString() << ")" << std::endl ;
        } else{
            out << endl << "        id-fields: --- unknown/default ----   ";
        }

        out << endl << noshowpos;

        return(out);
    }

    std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::CalorimeterHit> l) {
        const EVENT::CalorimeterHit *hit = l.object();
        const EVENT::LCCollection *col = l.collection();

        stringstream tmp;
        out << noshowpos;

        out << setw(42) <<  std::setfill('-') << right << " CalorimeterHit " << setfill('-') << setw(28) << "-" << endl;

        if(col != NULL){ 
            if(col->getTypeName() != LCIO::CALORIMETERHIT){
                out << "Warning: collection not of type " << LCIO::CALORIMETERHIT << endl ;
                return(out);
            }
            tmp.str(""); 
            tmp << "0x" << hex << col->getFlag() << dec;
            out << setw(30) << setfill(' ') << left << "Collection Flag" << right << setw(40) <<  tmp.str() << endl;
            LCTOOLS::printParameters(col->getParameters()); //todo
            BitSet32 flag(col->getFlag());
            out << "  -> LCIO::RCHBIT_LONG   : " << flag.test( LCIO::RCHBIT_LONG ) << endl ;
            out << "     LCIO::RCHBIT_BARREL : " << flag.test( LCIO::RCHBIT_BARREL ) << endl ;
            out << "     LCIO::RCHBIT_ID1    : " << flag.test( LCIO::RCHBIT_ID1 ) << endl ;
            out << "     LCIO::RCHBIT_TIME   : " << flag.test( LCIO::RCHBIT_TIME ) << endl ;
            out << "     LCIO::RCHBIT_NO_PTR : " << flag.test( LCIO::RCHBIT_NO_PTR ) << endl ;
            out << "     LCIO::RCHBIT_ENERGY_ERROR  : " << flag.test( LCIO::RCHBIT_ENERGY_ERROR ) << endl ;
        }


        //out << setw(30) << setfill(' ') << left << "ID" <<   right << setw(40) << hit->getId() << endl;
        tmp.str("");
        tmp  << hit->getCellID0() << dec;
        out << setw(30) << setfill(' ') << left << "CellId0" <<   right << setw(40) << tmp.str() << endl;
        tmp.str("");
        tmp << hit->getCellID1() << dec;

        out << setw(30) << setfill(' ') << left << "CellId1" <<   right << setw(40) << tmp.str() << endl;
        out << setw(30) << setfill(' ') << left << "Energy [GeV]" <<   right << setw(40) << dec << hit->getEnergy() << endl;
        out << setw(30) << setfill(' ') << left << "Energy Error [GeV]" <<   right << setw(40) << hit->getEnergyError() << endl;
        tmp.str("");
        tmp  << hit->getPosition()[0] << ", " << hit->getPosition()[1]  << ", " << hit->getPosition()[2]; 
        if(col != NULL){ 
            BitSet32 flag(col->getFlag());
            if(flag.test(LCIO::CHBIT_LONG)){
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

        return(out);
    }


    std::ostream& operator<<( std::ostream& out, const EVENT::CalorimeterHit &hit){ //hauke
        out << lcio_long(hit,NULL);
        //out << lcio_long(a,NULL);

        return(out);
    }




    //============================================================================
    //   # MCParticle
    //============================================================================

    const std::string& header(const EVENT::MCParticle *){ //hauke
        static std::string _h(" [   id   ] | PDG |     px,     py,        pz     | px_ep ,  py_ep  ,  pz_ep      | energy |gen|[simstat ]|  vertex x,     y   ,   z      |  endpoint x,    y  ,   z      |    mass |  charge |  [parents] - [daughters] \n");
        return _h;
    }

    const std::string& tail(const EVENT::MCParticle *){ //hauke
        static std::string _t("------------|-----|-------------------------------|--------|---|----------|-------------------------------|-------------------------------|---------|---------|--------------------------\n");
        return _t;
    }

    std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::MCParticle>& sV){ //hauke
        const EVENT::MCParticle* part = sV.obj;

        out << " [" << dec << setfill('0') << setw(8) << part->id() << "] "<< dec ;
        //out << setfill (' ') << dec << setw(5) << index << "|";
        out << "|";
        out << setfill(' ')<< setw(5) << part->getPDG() << "|"; 
        out << scientific << showpos << setprecision(2)
            << part->getMomentum()[0] << ", " << part->getMomentum()[1] << ", " << part->getMomentum()[2] << "|";
        out << scientific << showpos << setprecision(2)
            << part->getMomentumAtEndpoint()[0] << ", " << part->getMomentumAtEndpoint()[1] << ", " << part->getMomentumAtEndpoint()[2] << "|";
        out << noshowpos;
        out << part->getEnergy() << "|";
        out << fixed << setw(3) << part->getGeneratorStatus() << "|";
        out << LCTOOLS::getSimulatorStatusString(const_cast<EVENT::MCParticle *>(part) ).c_str() << "|";
        out << scientific << showpos << setprecision(2) 
            << part->getVertex()[0] << ", " << part->getVertex()[1] << ", " << part->getVertex()[2] << "|"; 
        out << scientific << showpos << setprecision(2)
            << part->getEndpoint()[0] << ", " << part->getEndpoint()[1] << ", " << part->getEndpoint()[2] << "|"; 
        out << part->getMass() << "|"; 
        out << part->getCharge() << "|";
        out << " [" ;

        for(unsigned int k=0;k<part->getParents().size();k++){
            if(k>0) out << "," ;
            out << dec << setfill('0') << setw(8) << (part->getParents()[k])->id();
        }
        out << "] - [" ;
        for(unsigned int k=0;k<part->getDaughters().size();k++){
            if(k>0) out << "," ;
            out << dec << setfill('0') << setw(8)<< (part->getDaughters()[k])->id();
        }
        out << "] " << endl;

        out << dec << noshowpos << setfill(' ');
        return out;
    }

    std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::MCParticle> ll){
        const EVENT::MCParticle *mcp = ll.object();
        const EVENT::LCCollection *col = ll.collection();

        stringstream tmp;
        out << noshowpos;

        //out << setprecision(5) << fixed;
        out << setw(41) <<  std::setfill('-') << right << " MCParticle " << setfill('-') << setw(29) << "-" << std::endl;
        if(col != NULL){ 
            if(col->getTypeName() != LCIO::MCPARTICLE){
                out << "Warning: collection not of type " << LCIO::MCPARTICLE << endl ;
                return(out);

            }
            tmp.str(""); 
            tmp << "0x" << hex << col->getFlag() << dec;
            out << setw(30) << setfill(' ') << left << "Collection Flag" << right << setw(40) <<  tmp.str() << endl;
            LCTOOLS::printParameters(col->getParameters());
        } 

        tmp.str(""); 
        tmp << mcp->id() << dec;
        out << setw(30) << setfill(' ') << left << "Id" <<   right << setw(40) << tmp.str() << endl;
        out << setw(30) << left << "PDG"<< setfill(' ') << right << setw(40) << mcp->getPDG() << endl;
        out << setw(30) << std::left << "Energy [GeV]" << right << setw(40) << mcp->getEnergy() << endl;


        tmp.str("");
        tmp  << mcp->getMomentum()[0] << ", " << mcp->getMomentum()[1]  << ", " << mcp->getMomentum()[2]; 
        out << setw(30) << left << "Momentum [GeV] (x,y,z) " << setfill(' ') << right <<setw(40) << tmp.str() << endl;

        tmp.str("");
        tmp  << mcp->getMomentumAtEndpoint()[0] << ", " << mcp->getMomentumAtEndpoint()[1]  << ", " << mcp->getMomentumAtEndpoint()[2]; 
        out << setw(30) << left << "Momentum at Endpoint[GeV] (x,y,z) " << setfill(' ') << right <<setw(40) << tmp.str() << endl;


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

        out << noshowpos;

        return(out);
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

        out << " [" ;

        for(unsigned int k=0;k<part->getParents().size();k++){
        if(k>0) out << "," ;
        out << p2i_map[ part->getParents()[k] ]  ;
        }
        out << "] - [" ;
        for(unsigned int k=0;k<part->getDaughters().size();k++){
        if(k>0) out << "," ;
        out << p2i_map[ part->getDaughters()[k] ]  ;
        }
        out << "] " << endl ;


        out << endl
        << "-------------------------------------------------------------------------------- "
        << endl ;
        }
     */

    std::ostream& operator<<( std::ostream& out, const EVENT::MCParticle &mcp){ //hauke
        out << lcio_long(mcp,NULL);
        return out;
    }




    //============================================================================
    //   # ReconstructedParticle
    //============================================================================

    const std::string& header(const EVENT::ReconstructedParticle *){ //hauke
        static std::string _h(" [   id   ] |com|type|     momentum( px,py,pz)       | energy | mass   | charge |        position ( x,y,z)      |pidUsed|GoodnessOfPID|\n");
        return _h;
    }

    const std::string& tail(const EVENT::ReconstructedParticle *){ //hauke
        static std::string _t("------------|---|----|-------------------------------|--------|--------|--------|-------------------------------|-------|-------------|\n");
        return _t;
    }

    std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::ReconstructedParticle>& sV){ //hauke
        const EVENT::ReconstructedParticle * part = sV.obj;
        stringstream tmp;

        out << noshowpos <<  " [" << setfill('0') << setw(8) << dec << part->id() << "] |" << dec;
        out << setfill(' ') << setw(3) << part->isCompound() << "|";
        out << setfill(' ') << setw(4) << part->getType() << "|";

        tmp.str("");
        tmp << showpos << scientific << setprecision(2); 
        tmp << part->getMomentum()[0] << ", " << part->getMomentum()[1] << ", " << part->getMomentum()[2];
        out << tmp.str() << "|";


        out << noshowpos << scientific << setprecision(2); 
        out << part->getEnergy() << "|";
        out << part->getMass() << "|";
        out << part->getCharge() << "|";

        tmp.str("");
        tmp << part->getReferencePoint()[0] << ", " << part->getReferencePoint()[1] << ", " << part->getReferencePoint()[2];
        out << tmp.str() << "|";

        out << dec << setw(7) << setfill('0');
        if(part->getParticleIDUsed() != 0 ){
            out << part->getParticleIDUsed()->id();
        }else{ 
            out << 0;
        }
	out << "|" << setw(13) << setfill(' ') << scientific << part->getGoodnessOfPID() << "|" << endl;

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
                out << dec <<  setfill('0') << "[" <<setw(8) << part->getParticles()[l]->id() << "]";
            }else{
                out << dec <<  setfill('0') << "[" <<setw(8) << part->getParticles()[l]->id() << "], ";
            }

            //printf("[%8.8x], ", part->getParticles()[l]->id() ) ;
        }
        out << endl ;

        out << "    tracks ( [   id   ] ): " ;

        for(unsigned int l=0,N=part->getTracks().size(); l<N ;l++){
	  
	  out << dec << setfill('0') << "[" << setw(8) 
	      << ( part->getTracks()[l]  ?  part->getTracks()[l]->id()  : 0 ) << "]" ;
	  
	  if( l != N-1 ) out << ", ";
	}
        out << endl ;

        out << "    clusters ( [   id   ] ): " ;

	for(unsigned int l=0,N=part->getClusters().size(); l<N ;l++){

	  out << dec <<  setfill('0') << "[" << setw(8) 
	      <<  ( part->getClusters()[l] ? part->getClusters()[l]->id() : 0 ) << "]";
	  
	  if(l != N-1) out << ", " ;
	}
        out << endl ;
        out << "    particle ids ( [id], PDG, (type)): " << std::endl  ;
        for(unsigned int l=0;l<part->getParticleIDs().size();l++){
            ParticleID* pid = part->getParticleIDs()[l] ;
	    if(l==0) out << "         " << header( pid ) << std::endl ;
	    out << "         " << lcshort( pid ) << std::endl ;
	    //            out << dec <<  "[" << setw(8) <<pid->id() << "], " <<  dec << setw(6) << pid->getPDG() << ", " <<  "(" << setw(6) <<pid->getType() << ") ";
            //printf("[%8.8x], %6.6d, (%6.6d)  ",  pid->id() , pid->getPDG() , pid->getType() ) ;
        }
        out << endl ;

        Vertex* sv = dynamic_cast<Vertex*>(part->getStartVertex());
        Vertex* ev = dynamic_cast<Vertex*>(part->getEndVertex());
        ReconstructedParticle* svr=0;
        ReconstructedParticle* evr=0;

        if(sv!=0){
            svr = dynamic_cast<ReconstructedParticle*>(sv->getAssociatedParticle());
        }
        if(ev!=0){
            evr = dynamic_cast<ReconstructedParticle*>(ev->getAssociatedParticle());
        }

        out << setfill('0') << noshowpos << dec;
        out << "    vertices: startVertex( id:[ " << setw(8) << (sv !=0? sv->id(): 0);
        out << "id_aRP: "<< setw(8) << ((sv != 0 && svr != 0) ? svr->id() : 0 );
        out <<  "]  endVertex( id:[" << setw(8) << ( ev != 0 ? ev->id() : 0 );
        out << "], id_aRP:[" << setw(8) << ((ev != 0 && evr != 0) ? evr->id() : 0 ) << "]";

        out << noshowpos << setfill(' ') << dec; 
        out << endl;

        return out;
    }

    std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::ReconstructedParticle> ll){
        const EVENT::ReconstructedParticle *part = ll.object();
        const EVENT::LCCollection *col = ll.collection();

        stringstream tmp;

        out << setw(41) << setfill('-') << right << " ReconstructedParticle " << setfill('-') << setw(29) << "-" << endl;
        if(col != NULL){ 
            if(col->getTypeName() != LCIO::RECONSTRUCTEDPARTICLE){
                out << "Warning: collection not of type " << LCIO::RECONSTRUCTEDPARTICLE << endl ;
                return(out);

            }
            tmp.str(""); 
            tmp << "0x" << hex << col->getFlag() << dec;
            out << setw(30) << setfill(' ') << left << "Collection Flag" << right << setw(40) <<  tmp.str() << endl;
            LCTOOLS::printParameters(col->getParameters());
        } 

        tmp.str("");
        tmp<<"0x" << dec << part->id() << dec;
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
            tmp  << dec << part->getParticleIDUsed()->id() << dec;
            out << dec << right << setw(40) << tmp.str() << endl;
        }else{ 
            out << right << setw(40) << 0 << endl;
        }

	out << setw(30) << setfill(' ') << left << "GoodnessOfPID"  <<   right << setw(40) << part->getGoodnessOfPID() << endl;
	
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
                out << dec <<  setfill('0') << "[" <<setw(8) << part->getParticles()[l]->id() << "]";
            }else{
                out << dec <<  setfill('0') << "[" <<setw(8) << part->getParticles()[l]->id() << "], ";
            }
            if(! ((l+1)%4)) { out << endl <<"                 ";}
        }
        //out << endl ;

        out << endl << "Tracks id:    ";

        for(unsigned int l=0,N=part->getTracks().size(); l<N ;l++){
	  if(! ((l+1)%5)){ out << endl << "                 ";}
	  out << dec << setfill('0') << "[" << setw(8) 
	      << ( part->getTracks()[l]  ?  part->getTracks()[l]->id()  : 0 ) << "]" ;
	  
	  if( l != N-1 ) out << ", ";
	}

        out << endl << "Clusters id:  ";
	for(unsigned int l=0,N=part->getClusters().size(); l<N ;l++){

	  if(! ((l+1)%5)){ out << endl << "                 ";}
	  out << dec <<  setfill('0') << "[" << setw(8) 
	      <<  ( part->getClusters()[l] ? part->getClusters()[l]->id() : 0 ) << "]";
	  
	  if(l != N-1) out << ", " ;
	}

        out << endl << "Particle ids ( [id], PDG, (type)): " << endl;
        for(unsigned int l=0;l<part->getParticleIDs().size();l++){
            ParticleID* pid = part->getParticleIDs()[l] ;
            out << dec << "                 " <<  "[" << setw(8) <<pid->id() << "], " <<  dec << setw(6) << pid->getPDG() << ", " <<  "(" << setw(6) <<pid->getType() << ") " << endl;
        }
        out << endl ;

        out << noshowpos;
        return(out);
    }

    std::ostream& operator<<( std::ostream& out, const EVENT::ReconstructedParticle &part){ //hauke
        out << lcio_long(part,NULL);
        return(out);
    }




    //============================================================================
    //   # TrackState
    //============================================================================

    const std::string& header(const EVENT::TrackState *){
        static std::string _h(" [   id   ] |    d0    |  phi     | omega    |    z0     | tan lambda|   reference point(x,y,z)        \n");
        return _h;
    }

    const std::string& tail(const EVENT::TrackState *){
        static std::string _t("------------|----------|----------|----------|-----------|-----------|-------------------------------- \n");
        return _t;
    }

    std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::TrackState>& sV){
        const EVENT::TrackState *trk = sV.obj;

        out << noshowpos <<  " [" << setfill('0') << setw(8) << dec<< trk->id() << "] ";
        out << scientific << setprecision(6) << showpos << dec << setfill(' ');
        out << " |" << trk->getD0(); 
        out << " |" << trk->getPhi(); 
        out << " |" << trk->getOmega();
        out << " |" << setprecision (3) << trk->getZ0();
        out << " |" << trk->getTanLambda();
        out << " |(" << setprecision(2) << trk->getReferencePoint()[0] << ", " << trk->getReferencePoint()[1] << ", " <<trk->getReferencePoint()[2];
        out << endl;

        out << " cov matrix: " << showpos << scientific << setprecision(6) << setw(15) << setfill(' ')  ;

	// print cov matrix as lower triangle matrix  
        for( unsigned l=0 , N=trk->getCovMatrix().size(), ncolumns = 1 , nele =1 ; l <N ; ++l , ++nele) {
	  out << trk->getCovMatrix()[l];
	  if(! ( (nele) % ncolumns ) ){ 
	    nele = 0 ;
	    ++ncolumns ;
	    out << " | " ; // separator for row
	  } else {
	    out << ", ";
	  } 
        }
	out << endl ;


        out << endl;
        return out;
    }

    std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::TrackState> ll){
        const EVENT::TrackState *part = ll.object();
        const EVENT::LCCollection *col = ll.collection();

        stringstream tmp;

        //out << scientific << setprecision (2) << showpos;
        out << noshowpos;
        out << setw(41) << setfill('-') << right << "-- TrackState ---" << setfill('-') << setw(29) << "-" << endl;
        if(col != NULL){ 
            if(col->getTypeName() != LCIO::TRACKSTATE){
                out << "Warning: collection not of type " << LCIO::TRACKSTATE << endl ;
                return(out);

            }
            tmp.str(""); 
            tmp << "0x" << hex << col->getFlag() << dec;
            out << setw(30) << setfill(' ') << left << "Collection Flag" << right << setw(40) <<  tmp.str() << endl;
            LCTOOLS::printParameters(col->getParameters());
        } 

	tmp.str("") ;
	switch( part->getLocation() ){
	case EVENT::TrackState::AtOther         :   tmp <<  "AtOther"        ;   break ;
	case EVENT::TrackState::AtIP            :   tmp <<  "AtIP"           ;   break ;
	case EVENT::TrackState::AtFirstHit      :   tmp <<  "AtFirstHit"     ;   break ;
	case EVENT::TrackState::AtLastHit       :   tmp <<  "AtLastHit"      ;   break ;
	case EVENT::TrackState::AtCalorimeter   :   tmp <<  "AtCalorimeter " ;   break ;
	case EVENT::TrackState::AtVertex        :   tmp <<  "AtVertex"       ;   break ;
	}
	out << setw(30) << setfill(' ') << left << "Location" << right << setw(40) << tmp.str() << endl;
	tmp.str("") ;
        tmp << dec << setfill('0') << setw(8) << part->id();
	out << scientific << setprecision(6) ;
        out << setw(30) << setfill(' ') << left << "Id"          << right << setw(40) << tmp.str() << endl;
        out << setw(30) << setfill(' ') << left << "D0"          << right << setw(40) << part->getD0() << endl;
        out << setw(30) << setfill(' ') << left << "Phi"         << right << setw(40) << part->getPhi() << endl;
        out << setw(30) << setfill(' ') << left << "Omega"       << right << setw(40) << part->getOmega() << endl;
        out << setw(30) << setfill(' ') << left << "Z0"          << right << setw(40) << part->getZ0() << endl;
        out << setw(30) << setfill(' ') << left << "Tan Lambda"  << right << setw(40) << part->getTanLambda() << endl;

        tmp.str("");
        tmp  << dec << part->getReferencePoint()[0] << ", " << part->getReferencePoint()[1]  << ", " << part->getReferencePoint()[2]; 
        out << setw(30) << setfill(' ') << left << "ReferencePoint" << right << setw(40) << tmp.str() << endl;

        out << "Cov matrix:" << showpos << scientific << setprecision(6) << setw(15) << setfill(' ')  ;


	// print cov matrix as lower triangle matrix 
        for( unsigned l=0 , N=part->getCovMatrix().size(), ncolumns = 1 , nele =1 ; l <N ; ++l , ++nele) {

	  out << part->getCovMatrix()[l];

	  if(! ( (nele) % ncolumns ) ){ 
	    nele = 0 ;
	    ++ncolumns ;
	    out << endl << "             " ;
	  } else {
	    out << ", ";
	  } 
        }

        out << noshowpos;
        return(out);
    }

    std::ostream& operator<<( std::ostream& out, const EVENT::TrackState &part){
        out << lcio_long(part,NULL);
        return(out);
    }




    //============================================================================
    //   # Track
    //============================================================================

    const std::string& header(const EVENT::Track *){ //hauke
        static std::string _h(" [   id   ] |   type   |    d0    |  phi     | omega    |    z0     | tan lambda|   reference point(x,y,z)        |    dEdx  |  dEdxErr |   chi2   |  ndf   \n");
        return _h;
    }

    const std::string& tail(const EVENT::Track *){ //hauke
        static std::string _t("------------|----------|----------|----------|----------|-----------|-----------|---------------------------------|----------|----------|-------- \n");
        return _t;
    }

    std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::Track>& sV){ //hauke
        const EVENT::Track *trk = sV.obj;
        BitSet32 flag(0) ;

        if(sV.col != NULL){
            const EVENT::LCCollection *col = sV.col;
            flag=BitSet32(col->getFlag());
        }

        out << noshowpos <<  " [" << setfill('0') << setw(8) << dec<< trk->id() << "] ";
        out << "| " << dec << setw(8) << trk->getType();
        out << scientific << setprecision (2) << showpos << dec << setfill(' ');
        out << " |" << trk->getD0(); 
        out << " |" << trk->getPhi(); 
        out << " |" << trk->getOmega();
        out << " |" <<setprecision (3) << trk->getZ0();
        out << " |" << trk->getTanLambda();
        out << " |(" << setprecision(2) << trk->getReferencePoint()[0] << ", " << trk->getReferencePoint()[1] << ", " <<trk->getReferencePoint()[2];
        out << ")|" << trk->getdEdx();
        out << " |" << trk->getdEdxError();
        out << " |" << trk->getChi2();
        out << " |" << noshowpos << setw(5) << trk->getNdf() ; 
        out << endl;

        out << " errors: "  << showpos << scientific << setprecision(6) << setw(15) << setfill(' ')  ; 

	// print cov matrix as lower triangle matrix  
        for( unsigned l=0 , N=trk->getCovMatrix().size(), ncolumns = 1 , nele =1 ; l <N ; ++l , ++nele) {
	  out << trk->getCovMatrix()[l];
	  if(! ( (nele) % ncolumns ) ){ 
	    nele = 0 ;
	    ++ncolumns ;
	    out << " | " ; // separator for row
	  } else {
	    out << ", ";
	  } 
        }
	out << endl ;

        out << " tracks(id): " ;
        const TrackVec& tracks = trk->getTracks() ;

        for(unsigned int l=0,N=tracks.size(); l<N ;l++){
	  
	  out << dec << setfill('0') << "[" << setw(8)  << dec 
	      << (  tracks[l]  ?   tracks[l]->id()  : 0 ) << "]" ;
	  
	  if( l != N-1 ) out << ", ";
	}

        out << endl;
        if(sV.col != NULL){
            if(flag.test(LCIO::TRBIT_HITS)){
                out << " hits ->";
                const TrackerHitVec& hits= trk->getTrackerHits() ;
                for(unsigned l=0;l<hits.size();l++){
                    out << "[" << setfill('0') << setw(8) <<  ( hits[l] ? hits[l]->id() : 0 ) << "] ";
                }
                out << endl;
            }
        }

        out << " radius of innermost hit " << trk->getRadiusOfInnermostHit() << " / mm , " << " subdetector Hit numbers : " 
	    << setfill(' ') << dec << fixed ;
        for(unsigned l=0 ; l< trk->getSubdetectorHitNumbers().size(); l++) {
	  out << trk->getSubdetectorHitNumbers()[l];
	  if(l < trk->getSubdetectorHitNumbers().size()-1){
	    out << ", ";
	  }
        }
        out << endl;

        return out;
    }

    std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::Track> ll){
        const EVENT::Track *part = ll.object();
        const EVENT::LCCollection *col = ll.collection();

        stringstream tmp;

        //out << scientific << setprecision (2) << showpos;
        out << noshowpos;
        out << setw(41) << setfill('-') << right << "-- Track ---" << setfill('-') << setw(29) << "-" << endl;

        if(col != NULL){ 
            if(col->getTypeName() != LCIO::TRACK){
                out << "Warning: collection not of type " << LCIO::TRACK << endl ;
                return(out);

            }
            tmp.str(""); 
            tmp << "0x" << hex << col->getFlag() << dec;
            out << setw(30) << setfill(' ') << left << "Collection Flag" << right << setw(40) <<  tmp.str() << endl;
            LCTOOLS::printParameters(col->getParameters());
            BitSet32 flag( col->getFlag() ) ;
            out << "     LCIO::TRBIT_HITS : " << flag.test( LCIO::TRBIT_HITS ) << endl ;

        } 

        tmp << dec << setfill('0') << setw(8) << part->id();
        out << setw(30) << setfill(' ') << left << "Id" << right << setw(40) << tmp.str() << endl;
        out << setw(30) << setfill(' ') << left << "Type" << right << setw(40) << part->getType() << endl;
        // out << setw(30) << setfill(' ') << left << "D0" << right << showpos << setw(40) << part->getD0() << endl;
        // out << setw(30) << setfill(' ') << left << "Phi" << right << setw(40) << part->getPhi() << endl;
        // out << setw(30) << setfill(' ') << left << "Omega" << right << setw(40) << part->getOmega() << endl;
        // out << setw(30) << setfill(' ') << left << "Z0" << right << setw(40) << part->getZ0() << endl;
        // out << setw(30) << setfill(' ') << left << "Tan Lambda" << right << setw(40) << part->getTanLambda() << endl;
        // tmp.str("");
        // tmp  << dec << part->getReferencePoint()[0] << ", " << part->getReferencePoint()[1]  << ", " << part->getReferencePoint()[2]; 
        // out << setw(30) << setfill(' ') << left << "ReferencePoint" << right << setw(40) << tmp.str() << endl;
        out << setw(30) << setfill(' ') << left << "dEdx" << right << setw(40) << part->getdEdx() << endl;
        out << setw(30) << setfill(' ') << left << "dEdx Error" << right << setw(40) << part->getdEdxError() << endl;
        out << setw(30) << setfill(' ') << left << "Chi2" << right << setw(40) << part->getChi2() << endl;
        out << setw(30) << setfill(' ') << left << "Ndf" << right << setw(40) << noshowpos << part->getNdf() << endl;

        // out << "Errors:     " << showpos;
        // unsigned int l;
        // for(l=0;l<14;l++){
        //     out << part->getCovMatrix()[l];
        //     if(! ((l+1)%5)){
        //         out << endl << "            ";
        //     } else{
        //         out << ", ";
        //     }
        // }
        // if(!((l+2)%5)){out << endl << "            ";}
        // out << part->getCovMatrix()[l+1] << endl;

	for(unsigned i=0,N=part->getTrackStates().size() ; i<N ; ++i ){

	  out << *part->getTrackStates()[i] ;
	}


        out << "Tracks(id): " ;
        const TrackVec& tracks = part->getTracks() ;

        for(unsigned l=0;l<tracks.size();l++){
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
        for(unsigned l=0;l<hits.size();l++){
            out << "[" << ( hits[l] ? hits[l]->id() : 0 ) << "] ";
            if(!((l+1)%7)){out << endl << "            ";}
        }
        out << endl;

        out << "Radius of innermost hit " << part->getRadiusOfInnermostHit() << " / mm , " << " subdetector Hit numbers : " ;
	out << endl << "            ";
        // for(l=0 ; l< part->getSubdetectorHitNumbers().size()-1 ; l++) {
        //     out << part->getSubdetectorHitNumbers()[l] << ", " ;
        //     if(!((l+1)%20)){out << endl << "            ";}
        // }
        // if(!((l+2)%20)){out << endl << "            ";}
        // out << part->getSubdetectorHitNumbers()[l+1] << endl;
        for(unsigned l=0 ; l< part->getSubdetectorHitNumbers().size(); l++) {
	  out << part->getSubdetectorHitNumbers()[l];
	  if(l < part->getSubdetectorHitNumbers().size()-1){
	    out << ", ";
	  }
        }
        out << endl;

        out << noshowpos;
        return(out);
    }

    std::ostream& operator<<( std::ostream& out, const EVENT::Track &part){ //hauke
        out << lcio_long(part,NULL);
        return(out);
    }




    //============================================================================
    //   # Cluster
    //============================================================================

    const std::string& header(const EVENT::Cluster *){ //hauke
        static std::string _h(" [   id   ] |type|  energy  |energyerr |      position ( x,y,z)           |  itheta  |   iphi   \n");
        return _h;
    }

    const std::string& tail(const EVENT::Cluster *){ //hauke
        static std::string _t("------------|----|----------|----------|----------------------------------|----------|----------\n");
        return _t;
    }

    std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::Cluster>& sV){ //hauke
        const EVENT::Cluster * clu = sV.obj;
        const EVENT::LCCollection *col = sV.col;

        BitSet32 flag( col->getFlag() ) ;

        out << noshowpos <<  " [" << setfill('0') << setw(8) << dec << clu->id() << "] |" << dec;
        out << setfill(' ') << setw(4) << clu->getType() << "|";
        out << showpos << scientific << setprecision(3) << clu->getEnergy() << "|" << clu->getEnergyError() << "|";
        out << clu->getPosition()[0] << ", " << clu->getPosition()[1] << ", " <<  clu->getPosition()[2] << "|";
        out << clu->getITheta() << "|";
        out << clu->getIPhi();

        out << " errors (6 pos)/( 3 dir): (" ;
        for(int l=0;l<6;l++){
            out << setw(4) << clu->getPositionError()[l] << ", ";
        }
        out << ")/(";
        for(int l=0;l<3;l++){
            out << setw(4) << clu->getDirectionError()[l];
        }
        out << ")" << endl ;
        out << " clusters(e): " ;
        const ClusterVec& clusters = clu->getClusters() ;
        for(unsigned int l=0;l<clusters.size();l++){
            out << setw(4) << ( clusters[l] ? clusters[l]->getEnergy() : 0. )  << ", ";
        }
        out << endl ;
        out <<" subdetector energies : " ;
        const FloatVec& sdE = clu->getSubdetectorEnergies() ;
        for(unsigned int l=0;l<sdE.size();l++){
            out << setw(4) <<  sdE[l]  << ", ";
        }
        out << endl ;


        if( flag.test( LCIO::CLBIT_HITS ) ) {
            out << " hits ->";
            const CalorimeterHitVec& hits= clu->getCalorimeterHits() ;
            const FloatVec& contr = clu->getHitContributions() ;
            for(unsigned int k=0;k<hits.size();k++){
                out << "[" << setfill('0' ) << dec << setw(8) <<  ( hits[k] ? hits[k]->id() : 0 ) << "]" 
                    << "(" << scientific << setprecision (2) << showpos << dec << setfill(' ') << contr[k] << ") " ;
                if( ! ( (k+1) % 10 ) ) out << std::endl ;
            }
        }

        out   << endl;

        out << dec <<  noshowpos;
        return out;
    }

    std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::Cluster> l) {
        const EVENT::Cluster *clu = l.object();
        const EVENT::LCCollection *col = l.collection();

        //out << "lcio_long: Cluster : lcio_long" << std::endl;

        stringstream tmp;
        out << noshowpos;
        out << setw(41) << setfill('-') << right << " Cluster " << setfill('-') << setw(29) << "-" << endl;

        if(col != NULL){ 
            if(col->getTypeName() != LCIO::CLUSTER){
                out << "Warning: collection not of type " << LCIO::CLUSTER << endl ;
                return(out);

            }
            tmp.str(""); 
            tmp << "0x" << hex << col->getFlag() << dec;
            out << setw(30) << setfill(' ') << left << "Collection Flag" << right << setw(40) <<  tmp.str() << endl;
            LCTOOLS::printParameters(col->getParameters());
            BitSet32 flag( col->getFlag() ) ;
            out << "     LCIO::CLBIT_HITS : " << flag.test( LCIO::CLBIT_HITS ) << endl ;

        }


        tmp  << dec << clu->id();
        out << setw(30) << setfill(' ') << left << "Id" << right << setw(40) << tmp.str() << endl;
        out << setw(30) << setfill(' ') << left << "Type"<< setfill(' ') << right << setw(40) << clu->getType() << endl;
        out << setw(30) << left << "Energy [GeV]" << right << setw(40) << clu->getEnergy() << endl;
        out << setw(30) << left << "EnergyError [GeV]" << right << setw(40) << clu->getEnergyError() << endl;
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

        return(out);
    }

    std::ostream& operator<<( std::ostream& out, const EVENT::Cluster &clu){ //hauke
        out << lcio_long(clu,NULL);
        return(out);
    }




    //============================================================================
    //   # Vertex
    //============================================================================

    const std::string& header(const EVENT::Vertex *){
        static std::string _h( "\n[   id   ] |pri|     alg. type     |    chi2   |    prob.  |       position ( x, y, z)       | [par] |  [idRecP]  \n");
        return _h;
    }

    const std::string& tail(const EVENT::Vertex *){
        static std::string _t(  "------------|---|-------------------|-----------|-----------|---------------------------------|-------|------------\n");
        return _t;
    }

    std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::Vertex>& sV){

        const EVENT::Vertex* v = sV.obj;
        out << noshowpos; 
        out << setfill('0');
        out << " [" << setw(8) << dec << v->id() << "] | " << v->isPrimary()<< " | ";
        out << setfill(' ') << setw(17) << left << v->getAlgorithmType() << " | ";
        out << showpos;

        out << setfill('0') << right << scientific << setprecision(2) << v->getChi2() << " | " << v->getProbability() << " | " <<
            v->getPosition()[0] << ", " << v->getPosition()[1] << ", " << v->getPosition()[2] << " | [" ;

        out << setw(3) << v->getParameters().size() << "] | [";
        out << setw(8) << dec << (v->getAssociatedParticle()!=NULL?v->getAssociatedParticle()->id():0) << "]\n";

        out << noshowpos; 
        return out;
    }

    std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::Vertex> ll){
        const EVENT::Vertex *v = ll.object();
        const EVENT::LCCollection *col = ll.collection();

        stringstream tmp;
        out << noshowpos;

        out << setw(41) <<  std::setfill('-') << right << " Vertex " << setfill('-') << setw(29) << "-" << endl;
        if(col != NULL){ 
            if(col->getTypeName() != LCIO::VERTEX){
                out << "Warning: collection not of type " << LCIO::VERTEX << endl ;
                return(out);
            }
        } 

        tmp.str("");
        tmp  << dec << v->id() << dec;

        out << setw(30) << setfill(' ') << left << "Id" <<   right << setw(40) << tmp.str() << endl;
        out << setw(30) << setfill(' ') << left << "Is Primary Vertex" <<   right << setw(40) << (v->isPrimary() ? "yes":"no") << endl;
        out << setw(30) << setfill(' ') << left << "Algorithm Type" <<   right << setw(40) << v->getAlgorithmType() << endl;
        out << scientific << setprecision(5);

        out << setw(30) << setfill(' ') << left << "Chi2" <<   right << setw(40) << v->getChi2() << endl;
        out << setw(30) << setfill(' ') << left << "Probability" <<   right << setw(40) << v->getProbability() << endl;
        tmp.str("");
        tmp<<setprecision(5);
        tmp << v->getPosition()[0] <<", "<<v->getPosition()[1] <<", "<<v->getPosition()[2]; 
        out << setw(30) << setfill(' ') << left << "Position (x,y,z)" <<   right << setw(40) << tmp.str() << endl;

        out << "Covariance Matrix          ";
        for(unsigned int i=0; i<v->getCovMatrix().size(); i++){
            out << v->getCovMatrix()[i] << (i<(v->getCovMatrix().size()-1)?", ":"\n");
            if((! ((i+1)%3)) && i < v->getCovMatrix().size()- 1){ out << endl << "                           "; }
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

        return out;
    }

    std::ostream& operator<<( std::ostream& out, const EVENT::Vertex &v){
        out << lcio_long(v,NULL);
        return out;
    }


} // namespace

