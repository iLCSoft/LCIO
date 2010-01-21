#ifndef LCRTRelations_H
#define LCRTRelations_H 1

#include <iostream>
#include <vector>
#include <list>
#include <map>

namespace lcrtrel_helper{

  //------------------ internal helper typdefs, function and classes ----------

  template <bool B>
  struct can_call_ext{};
  template <>
  struct can_call_ext<true>{ static void check(){/* no_op */ } ; };

  /** Function pointer for delete function */
  typedef void (*DeleteFPtr)(void*) ;

  /** Simple init function for simple pointers */
  struct SimplePtrInit{ static void* init() { return 0 ; } } ;

  /** Empty delete function for pointers w/o ownership */
  struct NoDelete{ static void clean(void *) { /* no_op */ } } ;

  /** Factory for objects of type  T*/
  template <class T>
  struct CreationPtrInit{ static void* init() { return new T ; } } ;

  /** Delete function for pointers w/ ownership.*/
  template <class T>
  struct DeletePtr{ static void clean(void *v) { delete (T*) v ; } } ;


  /** Delete function for containers of owned objects */
  template <class T>
  struct DeleteElements{

    static void clean(void *v) { 
      T* vec = static_cast<T*>(v) ;
      for( typename T::iterator it = vec->begin();it != vec->end(); ++it){
	delete *it ;
      }
      delete vec  ;    
    } 
  };


  /** Map of pointers to extension obbjects*/
  typedef std::map< unsigned , void * > PtrMap ;

  /** Vector of delete  functions */
  typedef std::vector< DeleteFPtr > DPtrVec ;

  /** Vector of pointers to extension obbjects*/
  //typedef std::vector< void * > PtrVec ;


  /** Base class for all extensions and relations */
  template <class U, class T , class I, class D, bool b>
  struct LCBaseTraits{
  
    typedef T*  ptr ;  /**<base pointer type  */
    typedef U tag ;    // this ensures that a new class instance is created for every user extension
  
    static const int allowed_to_call_ext = b ;
  
    static void clean(void *v) {
      D::clean( v ) ;
    }
    static ptr init() {
      return (ptr) I::init() ;
    }
    static DeleteFPtr deletePtr() { return  &clean ; }  ;
  };


  /** Base class for all extensions and relations of single objects */
  template <class U, class T , class I=SimplePtrInit, class D=NoDelete , bool b=1>
  struct LCBaseLinkTraits : public LCBaseTraits<U,T,I,D,b>{

    typedef LCBaseTraits<U,T,I,D,b> base ;

    typedef T*& ext_type ;                 // return value of  ext<>()
    typedef T*  rel_type ;                 // return value of  rel<>() 
    typedef typename base::ptr obj_ptr ;   // pointer to object

    static const bool is_container=false ;
  };

  /** Base class for all containers of extensions and relations, vectors, lists,... */
  template <class U, class T , class I=CreationPtrInit<T>, class D=DeletePtr<T> , bool b=1>
  struct LCBaseLinkContainerTraits : public LCBaseTraits<U,T,I,D,b>{

    typedef LCBaseTraits<U,T,I,D,b> base ;

    typedef       T*  ext_type ;              // return value of  ext<>()
    typedef const T*  rel_type ;              // return value of  rel<>() 
    typedef typename T::value_type obj_ptr ;  // pointer to object

    typedef typename T::iterator iterator ;
    typedef typename T::const_iterator const_iterator ;

    static const bool is_container=true ;
  };


  /** Helper class for relations */
  template <class U, class T> 
  struct RelationOneSide  : 
    public LCBaseLinkTraits<U,T,SimplePtrInit,NoDelete,false> {};
  
  
  /** Helper class for relations */
  template <class U, class T> 
  struct RelationManySide :
    public LCBaseLinkContainerTraits< U, std::list<T*>,
				      CreationPtrInit< std::list<T*> > , 
				      DeletePtr<std::list<T*> > ,false  > {};
  
  /** Helper class for relations */
  template <class U> struct FromRelation{} ;
  
  /** Helper class for relations */
  template <class U> struct ToRelation{} ;
  
  
  /** Helper class for biderectional relations provides the to and from type*/
  template <class From, class To>
  struct BiDirectional{
    typedef From from  ;
    typedef To   to  ;
  } ;
  
  
  /** Helper functions that treat single objects and containers */ 
  template <bool is_container>
  struct objorcont{

    template <class T, class S>
    inline static void add( T t, S s) { t->push_back( s )  ; }

    template <class T, class S>
    inline static void remove( T t, S s) { t->remove( s )  ; }
  };


  /** Helper functions specialization for single objects*/ 
  template <>
  struct objorcont<false>{

    template <class T, class S> 
    inline static void add( T& t, S s) { t = s ; }

    template <class T, class S>
    inline static void remove( T& t, S s) { t = 0 ; }
  };

  //-----------end of  internal helper typdefs, function and classes ----------


} // end namespace lcrtrel_helper

namespace lcrtrel{

  using namespace lcrtrel_helper ;

  /** Simple Extension -  pointer to an object of type T. 
   *  The class U needs to be the subclass  type, e.g.<br>
   *  struct MyAttributes : public LCExtension<MyAttributes,Attributes> {} ; <br>
   */
  template <class U, typename T> 
  struct LCExtension : public LCBaseLinkTraits< U, T > {};

  /** Simple Extension -  pointer to an object of type T where the ownership is taken over
   *  by the object holding the extension, i.e. it deletes the object when itself is deleted. 
   *  The class U needs to be the subclass  type, e.g.<br>
   *  struct MyAttributes : public LCExtension<MyAttributes,Attributes> {} ; <br>
   */
  template <class U, typename T> 
  class LCOwnedExtension : public LCBaseLinkTraits< U, T , SimplePtrInit , DeletePtr<T> > {};


  /** Extension vector holding pointers to objects of type T - no ownership of the objects is taken. */
  template <class U, class T> 
  class LCExtensionVector :
    public LCBaseLinkContainerTraits< U, std::vector<T*>,
				      CreationPtrInit< std::vector<T*> > , 
				      DeletePtr<std::vector<T*> > > {};


  /** Extension vector holding pointers to objects of type T - ownership of the objects is taken, i.e.
   *  all objects pointed to in the vector are deleted when the object itself is deleted.
   */
  template <class U, class T> 
  class LCOwnedExtensionVector : 
    public LCBaseLinkContainerTraits< U, std::vector<T*>,
				      CreationPtrInit< std::vector<T*> > , 
				      DeleteElements< std::vector<T*> > > {};


  /** Extension list holding pointers to objects of type T - no ownership of the objects is taken. */
  template <class U, class T> 
  class LCExtensionList :
    public LCBaseLinkContainerTraits< U, std::list<T*>,
				      CreationPtrInit< std::list<T*> > , 
				      DeletePtr<std::list<T*> > > {};

  /** Extension list holding pointers to objects of type T - ownership of the objects is taken, i.e.
   *  all objects pointed to in the vector are deleted when the object itself is deleted.
   */
  template <class U, class T> 
  class LCOwnedExtensionList : 
    public LCBaseLinkContainerTraits< U, std::list<T*>,
				      CreationPtrInit< std::list<T*> > , 
				      DeleteElements<  std::list<T*> > > {};


  /** One to one relation between two objects of type From and To */
  template <class U, class From, class To>
  struct LC1To1Relation : 
    public BiDirectional<RelationOneSide<FromRelation<U>,From>,
			 RelationOneSide<ToRelation<U>,To> > {

  } ; 

  /** One to many relation between one object of type From to many objects of type To */
  template <class U, class From, class To>
  struct LC1ToNRelation : 
    public BiDirectional<RelationOneSide<FromRelation<U>,From>,
			 RelationManySide<ToRelation<U>,To> > {
  } ; 

  /** Many to many relation between objects of type From to objects of type To */
  template <class U, class From, class To>
  struct LCNToNRelation : 
    public BiDirectional<RelationManySide<FromRelation<U>,From>,
			 RelationManySide<ToRelation<U>,To> > {
  } ; 


  /** Special Extension that allows to write int extensions directly (not through a pointer !). */
  template <class U >
  struct LCIntExtension{  // FIXME: need to check on 64 bit architecture...
    
    typedef int ptr ;  // base pointer type 
    
    typedef U tag ;     // this ensures that a new class instance is created for every user extension
    
    static const int allowed_to_call_ext = 1 ;
    
    static void clean(void *v) { }

    static ptr init() { 
      return 0 ; 
    }
    static DeleteFPtr deletePtr() { return  &clean ; }  ;

    typedef int& ext_type ;                
  };


  /** Special Extension that allows to write float extensions directly (not through a pointer !). */
  template <class U >

  struct LCFloatExtension{// FIXME: need to check on 64 bit architecture...

    typedef float  ptr ;  // base pointer type 
    typedef U tag ;     // this ensures that a new class instance is created for every user extension
    typedef float& ext_type ;     // return value of  ext<>()
  
    static const int allowed_to_call_ext = 1 ;
    static void clean(void *v) { }
    static ptr init() { return 0 ; }
    static DeleteFPtr deletePtr() { return  &clean ; }  ;
  };


  //--------------------------------------------------------------------



  class LCRTRelations ;

  // exclude from dictionary (template lookup does not work)
#ifndef __CINT__

  /** Set the 1-to-1 relation between two objects - prexisting inconsistent relations 
      involving the two objects are deleted to enforce a consistent set of from-to relations. */
  template <class R> 
  void set_relation( typename R::from::obj_ptr f, 
		     typename R::to::obj_ptr t) ;

  /** Unset the 1-to-1 relation from f */
  template <class R> 
  void unset_relation(typename R::from::obj_ptr f );


  /** Add a link from f to t to an N-to-N relation ship  */
  template <class R> 
  void add_relation( typename R::from::obj_ptr f, 
		     typename R::to::obj_ptr t) ;


  /** Remove the link from from f to t from the N-to-N relation ship  */
  template <class R> 
  void remove_relation( typename R::from::obj_ptr f, 
			typename R::to::obj_ptr t) ;


  /** Removes all relations from the given object */
  template <class R> 
  void remove_relations(typename R::from::obj_ptr f );


  /** Merge the relations from f2 to f1 - after this call f1 will hold all 
   *  the relations and f2 will be empty. 
   */
  template <class R> 
  void merge_relations( typename R::from::obj_ptr f1, 
			typename R::from::obj_ptr f2) ;



  /** Base class that provides run time (user) extensions and relation between objects. 
   * Every subclass object of LCRTRelations (and thus LCObbject) will automatically have 
   * the following functionality:<br>
   * <ul>
   * <li>extension of the object with arbitrary (even non-LCObject) classes</li>
   * <li>extension of single objects or vectors, lists of objects</li>
   * <li>optionally ownership is taken for extension objects (memory management)</li>
   * <li>relations to other subclasses of LCRTRelations:
   *  <ul>
   *  <li>one to one</li>
   *  <li>one to many</li>
   *  <li>many to many</li>
   *  </li></ul>
   * </ul>
   * 
   * The described functionality is provided through the two templated member functions:<br>
   * ext<class V>() <br>
   * rel<class V>() <br>
   * the class V is a user defined so called traits class, that uniquely tags the 
   * extension/relationship and defines the types of the objects involved. <br>
   * For extensions users have to subclass one of the following classes:<br>
   * LCExtension, LCOwnedExtension, LCIntExtension, LCFloatExtension,<br>
   * LCExtensionVector, LCExtensionList, LCOwnedExtensionVector, LCOwnedExtensionList. <br>
   * For example 
   * the following defines a user extension of a vector of strings that are owned by the 
   * object (i.e. deleted when the object is deleted):<br>
   * <p><b>
   * struct ParticleIDs : LCOwnedExtensionVector<ParticleIDs, std::string> {};<br>
   * </b>
   * &nbsp;&nbsp;&nbsp;(note: the first template parameter has to be the class itself !)
   * <p>
   * This extension can then be used  anywhere in the following code for all LCObjects, e.g.:<br>
   * <p><b>
   *   MCParticle*  mcp = dynamic_cast<MCParticle*>( mcpcol->getElementAt(i) ) ;<br>
   * 	mcp->ext<ParticleIDs>()->push_back( new std::string("charged")  )  ;<br>
   * 	mcp->ext<ParticleIDs>()->push_back( new std::string("hadron")  )  ;<br>
   * 	mcp->ext<ParticleIDs>()->push_back( new std::string("pion")  )  ;<br>
   * </b><p>
   * and be read out again: <br>
   * <p><b>
   *	ParticleIDs::ext_type pidv =  mcp->ext<ParticleIDs>() ;<br>
   *	for( ParticleIDs::const_iterator ipid = pidv->begin() ; ipid != pidv->end(); ++ipid){<br>
   *	 &nbsp;&nbsp; std::cout << **ipid << ", " ;<br>
   *	}<br>
   *</b><p>
   * <p>
   * Similarily the following defines a one to many relationship between Tracks and Clusters:<br>
   * <p><b>
   * struct TrkCluLink : LC1ToNRelation<TrkCluLink,Track,Cluster> {} ; <br>
   * </b><p>
   * Relations are allways biderectional, i.e. there is a <b>from</b> and a <b>to</b> side.
   * They can then be set and modified with the following functions:<br>
   * set_relation(), unset_relation(), add_relation(), <br>
   * remove_relation(), remove_relations(),
   * merge_relations(). <br>
   * For example:<br>
   * <p>
   * <b>
   *	Track*   trk = dynamic_cast<Track*>   ( trkcol->getElementAt(j) ) ; <br>
   *    //... <br>
   *	Cluster* clu = dynamic_cast<Cluster*> ( clucol->getElementAt(k) ) ; <br>
   *	add_relation<TrkCluLink>( trk ,clu ); <br>
   *</b><p>
   * The many side can then  be read out with a const_iterator and the one side with a 
   * normal pointer:<br>
   * <p><b>
   *	Track* trk =  clu->rel<TrkCluLink::from>() ; <br>
   *    //...<br>
	TrkCluLink::to::rel_type clulist =  trk->rel<TrkCluLink::to>() ; <br>
	for( TrkCluLink::to::const_iterator iclu = clulist->begin() ; iclu != clulist->end() ; ++iclu ){ <br>
	  &nbsp;&nbsp;Cluster* clu = *iclu ;  // iterator is of type pointer to container element <br>
	  &nbsp;&nbsp;std::cout <<  "   assigned cluster with E = " << clu->getEnergy() << std::endl ;  <br>
	}
   *</b>
   * <p>
   * More examples can be found in <b>$LCIO/src/cpp/src/EXAMPLES/lcrtrelations.cc</b>.
   */

  class LCRTRelations{
  
    // declare functions for relation handling as friends
    template <class R> 
    friend void set_relation( typename R::from::obj_ptr f, 
			      typename R::to::obj_ptr t);
    template <class R> 
    friend void unset_relation(typename R::from::obj_ptr f );

    template <class R> 
    friend void add_relation( typename R::from::obj_ptr f, 
			      typename R::to::obj_ptr t) ;
    template <class R> 
    friend void remove_relation( typename R::from::obj_ptr f, 
				 typename R::to::obj_ptr t) ;
    template <class R> 
    friend void remove_relations(typename R::from::obj_ptr f );

    template <class R> 
    friend void merge_relations( typename R::from::obj_ptr f1, 
				 typename R::from::obj_ptr f2) ;
  
  public:
  
    /** Provides access to an extension object - the type and ownership is defined 
     *  by the class V which should be a subtype of LCExtension, LCOwnedExtension,
     *  LCExtensionVector, LCExtensionList,...
     */
    template <class V>
    inline typename V::ext_type  ext() { 

      //      static char check[ V::allowed_to_call_ext] ;
      can_call_ext<V::allowed_to_call_ext>::check() ;

      return  ptr<V>() ;
    }
  

    /** Provides read access to relations - the object types and their connectivity 
     *  are defined by the class V which has to be a subtype of either 
     *  LC1To1Relation, LC1ToNRelation or LCNToNRelation.
     */
    template <class V>
    inline typename V::rel_type rel() {
    
      return  ptr<V>() ;
    }
  
  

    //   LCRTRelations() { 
    //     _vec = new PtrVec( 32  ) ; // initialize to prevent from to many resizes
    //   }
    //   ~LCRTRelations() {
    //    for( unsigned i=0 ; i< cleaners().size() ; ++i){
    //      cleaners()[i]( _vec->operator[](i)  ) ;  // call the delete function
    //    }
    //    delete _vec ;
    //   }



    ~LCRTRelations() {
      for( PtrMap::iterator it = _map.begin() ; it != _map.end() ; ++it ){
	cleaners()[ it->first ] ( it->second ) ;  // call the delete function
      }
    }
  
    //   void print(){
    //     std::cout << " ---- LCRTRelations -- : " << std::endl ;
    //     typedef std::map< void * , void*  > MyPtrMap ;
    //     MyPtrMap& map = *(MyPtrMap*) &_map ;
    //     for( MyPtrMap::iterator it = map.begin() ;
    // 	 it != map.end() ; ++it ){
    //       std::cout << "      ----   key : " << &(it->first) << " value " 
    // 		<<  (void*)it->second  << std::endl ;
    //     }
    //   }


  protected:

    //   /** Returns the reference to the pointer to the extension/relation object */
    //   template <class V>
    //   inline typename V::ptr & ptr() {
    
    //     typedef std::vector< typename V::ptr  > MyPtrVec ;
    //     MyPtrVec* vec = (MyPtrVec*) _vec ;

    //     unsigned id =  typeID<V>()  ;

    //     if( ! (vec->size() > id )  ) {
    //       vec->resize( id + 1 ) ;
    //     }

    //     typename V::ptr& p =  vec->operator[](id) ;

    //     if( p == 0 ) 
    //       p = V::init() ;

    //     return  p ;
    //   }
  


    /** Returns the reference to the pointer to the extension/relation object */
    template <class V>
    typename V::ptr & ptr() {
    
      typedef std::map< unsigned , typename V::ptr  > MyPtrMap ;
    
      MyPtrMap* map = (MyPtrMap*) &_map ;
    
      typename MyPtrMap::iterator it = map->find( typeID<V>() ) ;
    
      if( it == map->end() )
	it = map->insert( map->begin(), 
			  std::make_pair(  typeID<V>() , V::init() )) ;
    
      return  it->second  ;
    }

  private:
  
    static DPtrVec& cleaners(){
      static DPtrVec v ;
      return v ;
    }
  
    inline unsigned nextID(DeleteFPtr cp){
      static unsigned id(0) ;

      //     std::cout << " ---- nextID " << id+1  << " - delete Ptr  " 
      // <<  cp << std::endl ;
      
      cleaners().push_back( cp ) ;

      return id++ ;
    }
  
    template <class T>
    inline unsigned typeID(){
      static const unsigned uid  = nextID( T::deletePtr() ) ;

      return uid ;
    } 
  
  
    //   PtrVec* _vec ; 
    PtrMap _map ;
  
  } ;
  

  //----------------------- relation function definitions -----------------------------------


  template <class R> 
  void unset_relation(typename R::from::obj_ptr f){
  
    if( f != 0 ){
    
      LCRTRelations* t = f->LCRTRelations::rel<typename R::to>() ;
    
      if( t != 0 ) 
	t->LCRTRelations::ptr<typename R::from>() = 0 ;
    
      f->LCRTRelations::ptr<typename R::to>() = 0 ;
    }
  }

  template <class R> 
  void set_relation(typename R::from::obj_ptr f, typename R::to::obj_ptr t){
  
    // clear old relations first
    unset_relation<R>( f ) ;
    unset_relation<R>(t->LCRTRelations::rel<typename R::from>() ) ; 
  
    f->LCRTRelations::ptr<typename R::to>() =  t ;
    t->LCRTRelations::ptr<typename R::from>() =  f ;
  }




  template <class R> 
  void add_relation(  typename R::from::obj_ptr f, 
		      typename R::to::obj_ptr t){

    f->LCRTRelations::ptr<typename R::to>()->push_back( t ) ;

    //   std::cout << " ask to assign " << f << " to " << t << std::endl ;
    objorcont<R::from::is_container>::add( t->LCRTRelations::ptr<typename R::from>() , f ) ; 
  }




  template <class R> 
  void remove_relation( typename R::from::obj_ptr f, 
			typename R::to::obj_ptr t ) {
  
    f->LCRTRelations::ptr<typename R::to>()->remove( t ) ;

    objorcont<R::from::is_container>::remove( t->LCRTRelations::ptr<typename R::from>() , f ) ; 
  }


  template <class R> 
  void remove_relations( typename R::from::obj_ptr f ) {
  
    typename R::to::ptr cl = f->LCRTRelations::ptr<typename R::to>() ;
  
    for( typename R::to::iterator it = cl->begin(); it!=cl->end(); ++it){
    
      objorcont<R::from::is_container>::remove((*it)->LCRTRelations::ptr<typename R::from>(), f ) ; 

    }
    cl->clear() ;
  }

  template <class R> 
  void merge_relations(typename R::from::obj_ptr f1, 
		       typename R::from::obj_ptr f2 ) {
  
    typename R::to::ptr  lt2 = f2->LCRTRelations::ptr<typename R::to>() ;
  
    for( typename R::to::iterator it = lt2->begin() ;it !=  lt2->end() ; it++ ){
    
      objorcont<R::from::is_container>::remove( (*it)->LCRTRelations::ptr<typename R::from>(), f2 ) ; 
      objorcont<R::from::is_container>::add( (*it)->LCRTRelations::ptr<typename R::from>(), f1 ) ; 
    }

    f1->LCRTRelations::ptr<typename R::to>()->merge( *lt2 ) ;
  }
#endif // __CINT__

} // end namespace

#endif
