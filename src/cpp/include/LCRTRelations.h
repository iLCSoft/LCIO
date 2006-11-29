#ifndef LCRTRelations_H
#define LCRTRelations_H 1

#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <sstream>
//#include <typeinfo>

using namespace std ;


typedef void (*DeleteFPtr)(void*) ;


struct SimplePtrInit{ static void* init() { return 0 ; } } ;

struct NoDelete{ static void clean(void *v) { /* no_op */ } } ;


template <class T>
struct CreationPtrInit{ static void* init() { return new T ; } } ;

template <class T>
struct DeletePtr{ static void clean(void *v) { delete (T*) v ; } } ;


template <class T>
struct DeleteElements{

  static void clean(void *v) { 
    
    T* vec = static_cast<T*>(v) ;
    
    for( typename T::iterator it = vec->begin() ;
	 it != vec->end() ; ++it ){
      delete *it ;
    }
    
    delete vec  ;    
  } 
} ;


typedef std::map< DeleteFPtr , void * > PtrMap ;

typedef std::vector< DeleteFPtr > DPtrVec ;
typedef std::vector< void * > PtrVec ;



template <class U, class T , class I=SimplePtrInit, class D=NoDelete >
struct LCBaseLinkTraits{

  typedef T type ;
  typedef T* ptr ;
  typedef T*& ext ; // return value of  ext<>()
  typedef T* cext ; // return value of  to<>() and from<>()
  typedef const T* cptr ;
  typedef T& ref ;
  typedef const T& cref ;
  typedef U tag ; // this ensures that a new class instance is created for every user extension


  typedef ptr value_type ;
  static const bool is_container=false ;

  static void clean(void *v) {
    D::clean( v ) ;
  }
  static ptr init() {
    return (ptr) I::init() ;
  }
  static DeleteFPtr deletePtr() { return  &clean ; }  ;
};



template <class U, class T , class I=CreationPtrInit<T>, class D=DeletePtr<T> >
struct LCBaseLinkContainerTraits{

  typedef T type ;
  typedef T* ptr ;
  typedef T*& ext ; // return value of  ext<>()
  typedef const T* cext ; // return value of  to<>() and from<>()
  typedef const T* cptr ;
  typedef T& ref ;
  typedef const T& cref ;
  typedef U tag ; // this ensures that a new class instance is created for every user extension

  typedef typename type::value_type value_type ;
  typedef typename type::iterator iterator ;
  typedef typename type::const_iterator const_iterator ;
  typedef cref container ;

  static const bool is_container=true ;

  static void clean(void *v) {
    D::clean( v ) ;
  }
  static ptr init() {
    return (ptr) I::init() ;
  }
  static DeleteFPtr deletePtr() { return  &clean ; }  ;
};



template <class U, class T> 
class LCLinkTraits : public LCBaseLinkTraits< U, T > {};


template <class U, class T> 
class LCOwnedLinkTraits : public LCBaseLinkTraits< U, T , SimplePtrInit , DeletePtr<T>  > {};


template <class U, class T> 
class LCOwnedLinkVectorTraits : 
  public LCBaseLinkContainerTraits< U, std::vector<T*>,
				    CreationPtrInit< std::vector<T*> > , 
				    DeleteElements< std::vector<T*> > > {};

template <class U, class T> 
class LCOwnedLinkListTraits : 
  public LCBaseLinkContainerTraits< U, std::list<T*>,
				    CreationPtrInit< std::list<T*> > , 
				    DeleteElements<  std::list<T*> > > {};

template <class U, class T> 
class LCLinkVectorTraits :
  public LCBaseLinkContainerTraits< U, std::vector<T*>,
				    CreationPtrInit< std::vector<T*> > , 
				    DeletePtr<std::vector<T*> > > {};

template <class U, class T> 
class LCLinkListTraits :
  public LCBaseLinkContainerTraits< U, std::list<T*>,
				    CreationPtrInit< std::list<T*> > , 
				    DeletePtr<std::list<T*> > > {};

template <class U>
struct FromRelation{} ;

template <class U>
struct ToRelation{} ;

template <class From, class To>
struct LCRelationTraits{
  typedef From from_traits  ;
  typedef To   to_traits  ;
} ;


template <class U, class From, class To>
struct LC1To1Relation : 
  public LCRelationTraits<LCLinkTraits<FromRelation<U>,From>,
			  LCLinkTraits<ToRelation<U>,To> > {} ; 


template <class U, class From, class To>
struct LC1ToNRelation : 
  public LCRelationTraits<LCLinkTraits<FromRelation<U>,From>,
			  LCLinkListTraits<ToRelation<U>,To> > {
} ; 

template <class U, class From, class To>
struct LCNToNRelation : 
  public LCRelationTraits<LCLinkListTraits<FromRelation<U>,From>,
			  LCLinkListTraits<ToRelation<U>,To> > {
} ; 


//--------------------------------------------------------------------
class LCRTRelations ;

/** Set the 1-to-1 relation between two objects - prexisting inconsistent relations 
    involving the two objects are deleted to enforce a coinsistent set of from-to relations. */
template <class R> 
void set_relation( typename R::from_traits::value_type f, 
		   typename R::to_traits::value_type t) ;

/** Unset the 1-to-1 relation from f */
template <class R> 
void unset_relation(typename R::from_traits::value_type f );


/** Add a link from f to t to an N-to-N relation ship  */
template <class R> 
void add_relation( typename R::from_traits::value_type f, 
		   typename R::to_traits::value_type t) ;


/** Remove the link from from f to t from the N-to-N relation ship  */
template <class R> 
void remove_relation( typename R::from_traits::value_type f, 
		      typename R::to_traits::value_type t) ;


/** Removes all relations from the given object */
template <class R> 
void remove_relations(typename R::from_traits::value_type f );


/** Merge the relations from f2 to f1 - after this call f1 will hold all 
 *  the relations and f2 will be empty. 
 */
template <class R> 
void merge_relations( typename R::from_traits::value_type f1, 
		      typename R::from_traits::value_type f2) ;



/*External helper function with partial specialization */
template <class V, bool take_reference>
struct ext_helper{
  inline static typename V::ext ext( typename V::ptr& p ) { return p ;}
};

/*External helper function with partial specialization */
template <class V>
struct ext_helper<V,true>{
  inline static typename V::ext ext( typename V::ptr& p ) { return *p ;}
};



/** Base class for run time extensions to and relation between objects */

class LCRTRelations{
  
  template <class R> 
  friend void set_relation( typename R::from_traits::value_type f, 
			    typename R::to_traits::value_type t);

  template <class R> 
  friend void unset_relation(typename R::from_traits::value_type f );

  template <class R> 
  friend void add_relation( typename R::from_traits::value_type f, 
		     typename R::to_traits::value_type t) ;
  template <class R> 
  friend void remove_relation( typename R::from_traits::value_type f, 
			       typename R::to_traits::value_type t) ;
  template <class R> 
  friend void remove_relations(typename R::from_traits::value_type f );

  template <class R> 
  friend void merge_relations( typename R::from_traits::value_type f1, 
			       typename R::from_traits::value_type f2) ;
  
public:
  
  template <class V>
  typename V::ext  ext() { 
//     return ext_helper<V,V::is_container>::ext(  ptr<V>()  ) ;
    return  ptr<V>() ;
  }
  
  template <class V>
  typename V::to_traits::cext to() {
    return  ext<typename V::to_traits>() ;
  }
  
  template <class V>
  typename V::from_traits::cext from() {
    return  ext<typename V::from_traits>() ;
  }
  
  LCRTRelations() { 
    _vec = new PtrVec( 32  ) ; // initialize to prevent from to many resizes
  }

  ~LCRTRelations() {


//     std::cout << "  --- in ~LCRTRelations() - cleaners.size: " 
// 	      <<  cleaners().size() << std::endl ;

   for( unsigned i=0 ; i< cleaners().size() ; ++i){

//      std::cout << " cleaners()[i] : " <<  cleaners()[i] << std::endl ;
//      if( cleaners()[i] != 0 ) 
     cleaners()[i]( _vec->operator[](i)  ) ;  // call the delete function
     
   }
   delete _vec ;
  }

//   ~LCRTRelations() {
    
//     for( PtrMap::iterator it = _map.begin() ;
// 	 it != _map.end() ; ++it ){
      
//       it->first( it->second ) ;  // call the delete function
//     }
//   }





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

  /** provides write access to a relation - only for friends */
  template <class V>
  typename V::to_traits::ext access_to() {
    return  ext<typename V::to_traits>() ;
  }

  /** provides write access to a relation - only for friends */
  template <class V>
  typename V::from_traits::ext access_from() {
    return  ext<typename V::from_traits>() ;
  }

  /** Returns the reference to the pointer to the extension/relation object */
  template <class V>
  typename V::ptr & ptr() {
    
    typedef std::vector< typename V::ptr  > MyPtrVec ;
    MyPtrVec* vec = (MyPtrVec*) _vec ;

    unsigned id =  typeID<V>()  ;

    if( ! (vec->size() > id )  ) {
//     std::cout << "   -  need to resize vec from " << vec->size() << " to  " 
// 	      << id +  1 << std::endl ;
      vec->resize( id + 1 ) ;
    }

    typename V::ptr& p =  vec->operator[](id) ;

//     std::cout << " ----- p : " << p << " -- type: " 
// 	      << typeid(typename V::ptr).name() 
// 	      << "   size: " << vec->size()
// 	      << "   typid: : " << id 
// 	      << std::endl ;

    if( p == 0 ) 
      p = V::init() ;

    return  p ;
  }
  

//   /** Returns the reference to the pointer to the extension/relation object */
//   template <class V>
//   typename V::ptr & ptr() {
    
//     typedef std::map< DeleteFPtr , typename V::ptr  > MyPtrMap ;
    
//     MyPtrMap& map = *(MyPtrMap*) &_map ;
    
//     typename MyPtrMap::iterator it = map.find( V::deletePtr() ) ;
    
//     if( it == map.end() )
//       it = map.insert( map.begin(), 
// 		       std::make_pair( V::deletePtr(), V::init() )) ;
    
//     return  it->second  ;
//   }

private:
  
  static DPtrVec& cleaners(){
    static DPtrVec v ;
    return v ;
  }
  
  unsigned nextID(DeleteFPtr cp){
    static unsigned id(0) ;

//     std::cout << " ---- nextID " << id+1  << " - delete Ptr  " 
    // <<  cp << std::endl ;
      
    cleaners().push_back( cp ) ;

    return id++ ;
  }
  
  template <class T>
  unsigned typeID(){
    const static unsigned uid  = nextID( T::deletePtr() ) ;

    return uid ;
  } ;
  
  //   PtrMap _map ;
  
  
  PtrVec* _vec ; 
  
} ;
  


/** Unset the 1-to-1 relation from this object if it exists*/
template <class R> 
void unset_relation(typename R::from_traits::value_type f){

  if( f != 0 ){
    
    LCRTRelations* t = f->LCRTRelations::to<R>() ;

    if( t != 0 ) 
      t->LCRTRelations::access_from<R>() = 0 ;

    f->LCRTRelations::access_to<R>() = 0 ;
  }
}

template <class R> 
void set_relation(typename R::from_traits::value_type f, typename R::to_traits::value_type t){
  
  // clear old relations first
  unset_relation<R>( f ) ;
  unset_relation<R>(t->LCRTRelations::from<R>() ) ; 
  
  f->LCRTRelations::access_to<R>() =  t ;
  t->LCRTRelations::access_from<R>() =  f ;
}



template <bool is_container>
struct helper{

  template <class T, class S>
  inline static void add( T t, S s) { t->push_back( s )  ; }

  template <class T, class S>
  inline static void remove( T t, S s) { t->remove( s )  ; }
};

template <>
struct helper<false>{

  template <class T, class S> 
  inline static void add( T& t, S s) { 

    t = s ; 
//     std::cout << " assigning " << s << " to " << t << std::endl ;
  }

  template <class T, class S>
  inline static void remove( T& t, S s) { t = 0 ; }
};

template <class R> 
void add_relation(  typename R::from_traits::value_type f, 
		    typename R::to_traits::value_type t){

  f->LCRTRelations::access_to<R>()->push_back( t ) ;

//   std::cout << " ask to assign " << f << " to " << t << std::endl ;
  helper<R::from_traits::is_container>::add( t->LCRTRelations::access_from<R>() , f ) ; 
}




template <class R> 
void remove_relation( typename R::from_traits::value_type f, 
		      typename R::to_traits::value_type t ) {
  
  f->LCRTRelations::access_to<R>()->remove( t ) ;

  helper<R::from_traits::is_container>::remove( t->LCRTRelations::access_from <R>() , f ) ; 
}


template <class R> 
void remove_relations( typename R::from_traits::value_type f ) {
  
  typename R::to_traits::ref  cl = *f->LCRTRelations::access_to<R>() ;
  
  for( typename R::to_traits::iterator it = cl.begin(); it!=cl.end(); ++it){
    

//     (*it)->LCRTRelations::access_from<R>().remove( f ) ;

    helper<R::from_traits::is_container>::remove((*it)->LCRTRelations::access_from <R>() , f ) ; 

  }
  cl.clear() ;
}

template <class R> 
void merge_relations(typename R::from_traits::value_type f1, 
		     typename R::from_traits::value_type f2 ) {
  
  typename R::to_traits::ref  lt2 = *f2->LCRTRelations::access_to<R>() ;
  
  for( typename R::to_traits::iterator it = lt2.begin() ;it !=  lt2.end() ; it++ ){
    
//     typename R::from_traits::value_type  lf2 = (*it)->LCRTRelations::access_from<R>() ;

    helper<R::from_traits::is_container>::remove( (*it)->LCRTRelations::access_from<R>(), f2 ) ; 
    helper<R::from_traits::is_container>::add( (*it)->LCRTRelations::access_from<R>(), f1 ) ; 

//   for( typename R::to_traits::iterator it = lt2.begin() ;it !=  lt2.end() ; it++ ){
    
// //     typename R::from_traits::value_type  lf2 = (*it)->LCRTRelations::access_from<R>() ;
// //     lf2.remove( f2 )  ;
//     helper<R::from_traits::is_container>::remove( lf2, f2 ) ; 
// //     lf2.push_back( f1 ) ;
//     helper<R::from_traits::is_container>::add( lf2, f1 ) ; 

  }

  typename R::to_traits::ref  lt1 = *f1->LCRTRelations::access_to<R>() ;
  
  lt1.merge( lt2 ) ;
}


#endif
