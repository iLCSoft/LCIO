#include "UTIL/Operators.h"

#include "EVENT/ReconstructedParticle.h"

////////////////////////////////////////////////////////////////////////////////
// EXP: UNDER DEVELOPMENT!!!
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

// #ifdef USE_CLHEP
// #include "UTIL/LCFourVector.h"
// #endif
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
    static std::string _vtxh("    [id]    | position (x,y,z)            | dEdx    | time    |type \n");
    return _vtxh;
  }
  
  const std::string& tail(const EVENT::TrackerHit &){ //hauke
    static std::string _vtxt("------------|-----------------------------|---------|---------|-----\n");
    return _vtxt;
  }

  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::TrackerHit>& sV){ //hauke
    const EVENT::TrackerHit* part = sV.obj;
    using namespace std;
    out << " [" << setfill('0') << setw(8) << hex<< part->id() << "] |" << dec;
    out << showpos << scientific << setprecision (2) << part->getPosition()[0] << ","<< part->getPosition()[1] << "," << part->getPosition()[2] << "|"; 
    out << part->getdEdx() << "|";
    out << part->getTime() << "|";
    out << noshowpos << setw(4) << part->getType() << endl;
    return out;
  }

  std::ostream& operator<<( std::ostream& out, const EVENT::TrackerHit &hit){ //hauke
    out << lcio_long(hit,NULL);
    return out;
  }

  const std::string& header(const EVENT::SimTrackerHit &){ //hauke
    static std::string _vtxh(" CellID  | position (x,y,z)            |  dEdx   |  time   |PDG of MCParticle|   px    ,   py    ,   pz    |path length \n");
    return _vtxh;
  }
  
  const std::string& tail(const EVENT::SimTrackerHit &){ //hauke
    static std::string _vtxt("---------|-----------------------------|---------|---------|-----------------|-----------------------------|------------\n");
    return _vtxt;
  }

  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::SimTrackerHit>& sV){ //hauke
    const EVENT::SimTrackerHit* hit = sV.obj;
    using namespace std;
    out << noshowpos << " " << setw(8) << hex << hit->getCellID() << "|";
    out << showpos << scientific << setprecision (2) << setfill(' ') << hit->getPosition()[0]<<","<<hit->getPosition()[1]<<","<<hit->getPosition()[2]<<"|";
    out << hit->getdEdx() << "|";
    out << hit->getTime() << "|";
    if(hit->getMCParticle()){
        out << setw(17) << hit->getMCParticle()->getPDG() << "|";
    }else{
        out << "                 |";
    }
    out << hit->getMomentum()[0] << "," << hit->getMomentum()[0] << "," << hit->getMomentum()[0] << "|";
    out << setw(11) << hit->getPathLength() << endl;
    out << noshowpos;

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
    out << noshowpos;

    out << " [" << hex << setw(8) << setfill('0') << hit->id() << "] ";
    out << "|" << hex << setw(8) << setfill('0') << hit->getCellID0();
    out << "|" << hex << setw(8) << setfill('0') << hit->getCellID1();
    out << "|" << dec << setprecision(3) << scientific << showpos << hit->getEnergy(); 
    out << "|" << dec << setprecision(3) << scientific << hit->getEnergyError(); 
    out << "|" << dec << setprecision(3) << scientific << showpos
        << hit->getPosition()[0] << ", " << hit->getPosition()[0] << ", " << hit->getPosition()[0]; 
    out << endl << noshowpos;

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
    out << noshowpos <<  " [" << setfill('0') << setw(8) << hex << clu->id() << "] |" << dec;
    out << setfill(' ') << setw(4) << clu->getType() << "|";
    out << showpos << scientific << setprecision(3) << clu->getEnergy() << "|";
    out << clu->getPosition()[0] << ", " << clu->getPosition()[1] << ", " <<  clu->getPosition()[2] << "|";
    out << clu->getITheta() << "|";
    out << clu->getIPhi() <<  noshowpos << endl;
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

    out << noshowpos; 
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
    const EVENT::LCRelation* v = sV.obj;
    out << noshowpos; 
    out << "| [" << setw(8) << hex << (v->getFrom())->id() << "] |";
    out << " [" << setw(8) << hex << (v->getTo())->id() << "] |";
    out << " " << setw(8) << dec << v->getWeight() << "|\n";
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
    out << noshowpos; 
    out << "| [" << setw(8) << hex << v->id() << "] |\n";
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

    return(out);
}

std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::Cluster> l) {
    const EVENT::Cluster *clu = l.object();
    const EVENT::LCCollection *col = l.collection();

    //out << "lcio_long: Cluster : lcio_long" << std::endl;

    using namespace std;
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

    return(out);
}


std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::Track> ll){
    const EVENT::Track *part = ll.object();
    const EVENT::LCCollection *col = ll.collection();

    using namespace std;
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
    return(out);
}


std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::ReconstructedParticle> ll){
    const EVENT::ReconstructedParticle *part = ll.object();
    const EVENT::LCCollection *col = ll.collection();

    using namespace std;
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

    out << noshowpos;
    return(out);
}

std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::SimCalorimeterHit> ll){
    const EVENT::SimCalorimeterHit *hit = ll.object();
    const EVENT::LCCollection *col = ll.collection();

    using namespace std;
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
    return(out);
}

std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::TrackerHit> ll){
    const EVENT::TrackerHit *hit = ll.object();
    const EVENT::LCCollection *col = ll.collection();

    using namespace std;
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

        LCFlagImpl flag( col->getFlag() ) ;
        cout << "     LCIO::CHBIT_BARREL : " << flag.bitSet( LCIO::CHBIT_BARREL ) << endl ;
    } 
    tmp.str("");
    tmp << "0x" << hex << hit->id() << dec; 
    out << setw(30) << setfill(' ') << left << "Id" <<   right << setw(40) << tmp.str() << endl;
    tmp.str("");
    tmp << hit->getPosition()[0] << ", " << hit->getPosition()[1]  << ", " << hit->getPosition()[2]; 
    out << setw(30) << left << "Position (x,y,z) " << setfill(' ') << right <<setw(40) << tmp.str() << endl;
    out << setw(30) << left << "dE/dx " << setfill(' ') << right << setw(40) << hit->getdEdx() << endl;
    out << setw(30) << left << "Time " << setfill(' ') << right << setw(40) << hit->getTime() << endl;
    out << setw(30) << left << "Type " << setfill(' ') << right << setw(40) << hit->getType() << endl;
    out << noshowpos;

    return(out);
}


std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::SimTrackerHit> ll){
    const EVENT::SimTrackerHit *hit = ll.object();
    const EVENT::LCCollection *col = ll.collection();

    using namespace std;
    stringstream tmp;
    out << noshowpos;

    out << setw(42) <<  std::setfill('-') << right << " SimTrackerHit " << setfill('-') << setw(28) << "-" << endl;
    if(col != NULL){ 
        if(col->getTypeName() != LCIO::SIMTRACKERHIT){
            out << "Warning: collection not of type " << LCIO::SIMTRACKERHIT << endl ;
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
    out <<setw(30) << std::left << "dE/dx [GeV]" << setfill(' ') << right <<setw(40) << hit->getdEdx() << endl;
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

    out << noshowpos;

    return(out);
}


std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::MCParticle> ll){
    const EVENT::MCParticle *mcp = ll.object();
    const EVENT::LCCollection *col = ll.collection();

    using namespace std;
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

    out << noshowpos;

    return(out);
}

std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::Vertex> ll){
    const EVENT::Vertex *v = ll.object();
    const EVENT::LCCollection *col = ll.collection();

    using namespace std;
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

    return out;
  }


std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::LCRelation> ll){
    const EVENT::LCRelation *v = ll.object();
    const EVENT::LCCollection *col = ll.collection();

    using namespace std;
    stringstream tmp;
    out << noshowpos;

    out << setw(41) <<  std::setfill('-') << right << " LCRelation " << setfill('-') << setw(29) << "-" << endl;
    if(col != NULL){ 
        if(col->getTypeName() != LCIO::LCRELATION){
            out << "Warning: collection not of type " << LCIO::LCRELATION << endl ;
            return(out);
        }
    } 

    out << setw(30) << setfill(' ') << left << "From [id]" <<   right << hex << (v->getFrom())->id() << endl;
    out << setw(30) << setfill(' ') << left << "To [id]" <<   right << (v->getTo())->id()  << endl;
    out << setw(30) << setfill(' ') << left << "Weight" <<   right << v->getWeight()  << endl;

    return out;
}


std::ostream& operator<<( std::ostream& out, const LCIO_LONG<EVENT::LCFloatVec> ll){
    const EVENT::LCFloatVec *v = ll.object();
    const EVENT::LCCollection *col = ll.collection();

    using namespace std;
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
    tmp << "0x" << hex << v->id() << dec;
    out << setw(30) << setfill(' ') << left << "Id" <<   right <<  tmp.str() << endl;

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
 
