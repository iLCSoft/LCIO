// -*- C++ -*-
#ifndef IMPL_LCRefVec_H
#define IMPL_LCRefVec_H 1

#include "LCIOTypes.h"
#include "LCEventImpl.h"
#include <iostream>
//#include <iomanip>
#include <vector>

namespace IMPL {

  /** LCRef is used for replacing single pointers with object-indices 
   * 
   * @see LCIO
   * @author gaede 
   * @version Nov 14, 2009
   */
  template <class T>
  class LCRef{

    T _ptr;               //!
    EVENT::long64 _ref ;
    bool _havePtr ;       //!
    LCEventImpl* _evt ;   //!

  public:

    LCRef() : _ptr(0), 
	      _ref(0),
	      _havePtr( false ) {
      // -- need to get current event on loading.... 
      _evt = LCEventImpl::getCurrentEvent() ;
    }

    LCRef(T t) : _ptr(t), 
		 _ref(0),
		 _havePtr( t != 0 ) {
      // -- need to get current event on loading.... 
      _evt = LCEventImpl::getCurrentEvent() ;
    }
    
    const LCRef<T>& operator=(const T& t) {
      _ptr = t ;
      _ref = 0 ;
      _havePtr =  ( t != 0 )  ;
      return *this ;
    }

    void ptrToIndex() {
      _ref = _ptr->getIndex() ;
    }
    
    void indexToPtr() {
      _ptr = reinterpret_cast<T>( _evt->getObjectForIndex(_ref)  )  ;
    }


    // conversion to ptr
    operator T&() { 
      
//       std::cout << " operator T&() - this " << this 
//  		<< " _havePtr "  << _havePtr 
//  		<< " _ptr "  << _ptr 
//  		<< " _ref "  << std::hex << _ref << std::dec
//  		<< std::endl ;

      if( ! _havePtr ) {
	_havePtr = (_ptr!=0 && _ref==0 ) ;
	if( ! _havePtr ) {
	  indexToPtr() ;
	  _havePtr = true ; 
	}
      }
//       std::cout << " operator T&() - this " << this 
//  		<< " _havePtr "  << _havePtr 
//  		<< " _ptr "  << _ptr 
//  		<< " _ref "  << std::hex << _ref << std::dec
//  		<< std::endl ;

      return _ptr ;
    }

  };


  /** LCRefVec is used for replacing vectors of pointers with verctor of indices 
   * 
   * @see LCIO
   * @author gaede 
   * @version Nov 14, 2009
   */
  template <class T>
  class LCRefVec{

    std::vector<T> _vec;                 //! no RIO
    std::vector<EVENT::long64> _refVec ;
    LCEventImpl* _evt ;                  //! no RIO
    bool _havePtrs ;                     //! no RIO

  public:
    
    typedef typename std::vector<T>::iterator iterator ;
    typedef typename std::vector<T>::const_iterator const_iterator ;

  LCRefVec() : _havePtrs( false ) {
      // -- need to get current event on loading.... 
      _evt = LCEventImpl::getCurrentEvent() ;
    }
    
    void ptrToIndex() {


      unsigned n =  _vec.size() ;

      //std::cout << " ----------------ptrToIndex - n : " << n  << std::endl ;

      _refVec.clear() ;
      _refVec.resize( n ) ;
      for(unsigned i=0 ; i<n ; i++) {

	//std::cout << " ----------------ptrToIndex " << _vec[i]->getIndex() << std::endl ;

      	_refVec[ i ] = _vec[i]->getIndex() ;
      }
      //std::cout << " ----------------ptrToIndex - n index : " << _refVec.size() << std::endl ;
    }

    void indexToPtr() {

      unsigned n =  _refVec.size() ;
      //std::cout << " ----------------indexToPtr - n: " << n << "  _refVec.size()  " << _refVec.size() << std::endl ;

      _vec.clear() ;
      _vec.resize( n ) ;
      for(unsigned i=0 ; i<n ; i++) {

	//std::cout << " ----------------indexToPtr " << _refVec[i] << std::endl ;
 
      	_vec[i] = static_cast<T>( _evt->getObjectForIndex(  _refVec[i]  )  )  ;
      }
    }

    // forward some of std::vector's interface so that we can serve as a wrapper to vector ------------

    inline void push_back(T t){
      _vec.push_back(t) ;
    }
    inline void resize(size_t nS){   _vec.resize(nS) ; }

    inline T& operator[](size_t n){  return _vec[n] ; }

    inline const T& operator[](size_t n) const { return _vec[n] ; }

    inline T& at(size_t n){ return _vec.at(n)  ; }

    inline size_t size() const { return _vec.size()  ; }

    inline iterator begin(){  return _vec.begin() ; }
    inline iterator end(){ return _vec.end() ; }
    inline const_iterator begin() const {  return _vec.begin() ; }
    inline const_iterator end() const { return _vec.end() ; }
    
    // conversion to reference of std::vector
    operator std::vector<T>&() { 

      //std::cout << " --------------- operator std::vector<T>&() -  _vec.size()  " << _vec.size() << " refVec.size() " << _refVec.size() << std::endl ;

      if( ! _havePtrs ) {
 	_havePtrs = (_vec.size()>0 && _refVec.size()==0 ) ;
	if( ! _havePtrs ) {
	  indexToPtr() ;
	  _havePtrs = true ; 
	}
    }
      return _vec ;
    }

  };



} // namespace 
#endif 
