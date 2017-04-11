#ifndef LCIO_INDEXMAP_H
#define LCIO_INDEXMAP_H 1

#include "EVENT/LCCollection.h"
#include "EVENT/LCParameters.h"

#include "LCIOSTLTypes.h"

#include <string>

namespace UTIL{
  
/** utility class to manage indices according to Collection Parameters
 * 
 *  EXP: UNDER DEVELOPMENT!!! - Don't use this class yet
 *
 * @author engels
 * @version $Id: IndexMap.h,v 1.2 2006-09-21 06:10:38 gaede Exp $
*/

  class IndexMap{
  
  public:

    /** Constructor: takes as arguments the collection where the parameters are stored and the parameter names to manage
     *  Before you loop over a collection create an IndexMap object with the keys to manage. Example:
     *	
     *	LCCollection* col = evt->getCollection( "Vertices" ) ;
     * 
     *  IndexMap imvtx( col, "AlgorithmNames", "AlgorithmTypes" ) ;
     *
     *  Loop over Collection....
     *
     *    Vertex* v = dynamic_cast<Vertex*>( col->getElementAt( i ) ) ;
     *
     *	  cout << "vertex " << i << has been created with " << imvtx.decode( v->getAlgorithmType() ) << endl;
     *
     *	End Loop
     *
     *	---------------------------------
     *
     *	For using the encoding use similar process:
     *
     *	IndexMap imvtx( col, "AlgorithmNames", "AlgorithmTypes" ) ;
     *
     *	Loop for creating vertices...
     *	  
     *	  VertexImpl* v = new VertexImpl ;
     *
     *	  v->setAlgorithmType( imvtx.encode( "ZvTop" ) );
     *
     *	  // If the parameter "ZvTop" or even the collection parameters "AlgorithmNames" and "AlgorithmTypes" do not exist
     *    // they are automatically added and ZvTop gets a corresponding new AlgorithmType which is also automatically added
     *
     *	End Loop
     *  
     */
    IndexMap(const EVENT::LCCollection* col, const std::string& key1, const std::string& key2);

    /// no copy constructor
    IndexMap(const IndexMap&) = delete ;
    /// no assignment operator
    IndexMap& operator=(const IndexMap&) = delete ;

    //destructor
    ~IndexMap();

    /** encode string to index (e.g. vertex->setAlgorithmType( imvtx.enconde( "ZvTop" ) ); */
    int encode(const std::string name);

    /** decode int to string (e.g. cout << "vertex has been created with " << imvtx.decode( vtx->getAlgorithmType() ) << endl;*/
    const std::string& decode(int type) const;
  
  protected:
    const EVENT::LCCollection* _col;
    const std::string _key1;
    const std::string _key2;
    EVENT::StringVec _strvec{};
    EVENT::IntVec _intvec{};
  }; // class

}//namespace

#endif /* ifndef LCIO_INDEXMAP_H */
