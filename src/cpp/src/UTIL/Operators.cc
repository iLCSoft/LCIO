#include "UTIL/Operators.h"

////////////////////////////////////////////////////////////////////////////////
// EXP: UNDER DEVELOPMENT!!!
////////////////////////////////////////////////////////////////////////////////

using namespace std;

namespace UTIL{

  std::ostream& operator<<( std::ostream& out, const UTIL::lcio_short<EVENT::Vertex>& sV){
    const EVENT::Vertex* v=sV.obj;
    
    out << setfill('0');
    out << " [" << setw(8) << hex << v->id() << "] | " << v->isPrimary()<< " | ";
    out << scientific << setprecision(3) << v->getChi2() << " | " << v->getProbability() << " | " <<
	v->getPosition()[0] << "," <<
        v->getPosition()[1] << "," <<
        v->getPosition()[2] << " | " ;
                                                                                                                                                             
    for(int i=0;i<VTXCOVMATRIX;i++)
      out << v->getCovMatrix()[i] << (i<(VTXCOVMATRIX-1)?",":" | [");
    out << setw(3) << v->getParameters().size() << "] | [";
    out << setw(8) << hex << (v->getAssociatedParticle()!=NULL?v->getAssociatedParticle()->id():0) << "]\n";
    
    return out;
                                                                                                                                                             
  }
 
  std::ostream& operator<<( std::ostream& out, const EVENT::Vertex* v){
    
    out << setfill('0');
    out << "Vertex ID:\t\t[" << setw(8) << hex << v->id() << "]" << endl;
    out << "Is Primary Vertex:\t" << (v->isPrimary() ? "true":"false") << endl;
    out << scientific << setprecision(5);
    out << "Chi2:\t\t\t" << v->getChi2() << endl;
    out << "Probability:\t\t" << v->getProbability() << endl;
    out << "Position (x, y, z):\t" <<
	v->getPosition()[0] <<", "<<
	v->getPosition()[1] <<", "<<
	v->getPosition()[2] << endl;
    
    out << "Covariance Matrix:\t";
    for(int i=0; i<VTXCOVMATRIX; i++)
      out << v->getCovMatrix()[i] << (i<(VTXCOVMATRIX-1)?", ":"\n");
    
    out << "Parameters:";
    if(v->getParameters().size()==0){
      out << "\t\t[Empty]" << endl;
    }
    else { out << endl; }
    for(unsigned int i=0; i < v->getParameters().size(); i++){
      out << "   Parameter [";
      out << setw(3) << i << "]:\t";
      out << scientific << setprecision(5) << v->getParameters()[i] << endl;
    }
    
    out << "Associated Reconstructed Particle ID:\t["; 
    out << setw(8) << hex << (v->getAssociatedParticle()!=NULL?v->getAssociatedParticle()->id():0) <<"]\n\n";
   
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

  const std::string& header(const EVENT::Vertex* v){
    
    static std::string _vtxh(
      "\n    [id]    |pri|    chi2   |    prob.  |      position ( x, y, z)      |"
	"                 covariance matrix (px, py, pz)              | [par] |  [idRecP]  \n");
    _vtxh+=tail(new IMPL::VertexImpl());
    return _vtxh;
  }
  
  const std::string& tail(const EVENT::Vertex* v){
    
    static std::string _vtxt(
	"------------|---|-----------|-----------|-------------------------------|"
	"-------------------------------------------------------------|-------|------------\n");
    return _vtxt;
  }

} // namespace
 
