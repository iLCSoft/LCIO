/**Fortran interface - define the length of pointers
 * this has to made machine independent ...
 * 
 * @author Vogt
 * @version Sep 30, 2003
 */

typedef int PTRTYPE ;
#define CFORTRANPNTR INT
#define CFORTRANPNTRV INTV


/** Template for casting from integers to LCIO classes 
 * First template is the resulting type, second argument is the original 
 * type that was used when the pointer was passed to fortran.
 */

template<class T, class O> T* f2c_pointer(PTRTYPE ptr){

  if(! (ptr) ) return 0  ; 

  O*  o_ptr = reinterpret_cast<O*>( ptr ) ;

  return dynamic_cast<T*>( o_ptr ) ; 
}

#define C2F_POINTER(T,ptr) reinterpret_cast<int>( dynamic_cast<T>( (ptr) ) ) ;
