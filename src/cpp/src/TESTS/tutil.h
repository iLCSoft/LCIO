#include <iostream>
//#include <cstring>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>

class TEST{
  
public:
  
  TEST( const std::string& tname, std::ostream& stream=std::cout ): _testname(tname), _out(stream){
    _out << std::endl << "[" << _testname << "] ";
    _out << "TEST_BEGIN ******************************" << std::endl << std::endl;
  }
  
  ~TEST(){
    _out << std::endl << "[" << _testname << "] ";
    _out << "TEST_PASSED ******************************" << std::endl << std::endl;
  }
  
  void LOG( const std::string& msg ){
    _out << "[" << _testname << "] LOG: " << msg << std::endl;
  }
  
  template <class V1, class V2 >
  void operator()(const V1& v1, const V2& v2 , const std::string name ) {
    
    if ( ! (v1 == v2)  ) {
      
      std::stringstream sstr ;
      sstr << "  " << name<< " : [" << v1 << "] != [" << v2 <<"]" ;
      FAILED( sstr.str() ) ;
    } 
    return ;
  }

//   void operator()(bool cond, const std::string msg) {
//     if ( ! cond ) FAILED( msg ) ;
//     return ;
//   } 

    void FAILED( const std::string& msg ){
        
        std::stringstream errmsg;
        errmsg << std::endl;
        errmsg << "[" << _testname << "] TEST_FAILED ############################################" << std::endl;
        errmsg << "[" << _testname << "] TEST_ERROR: " << msg << std::endl;
        errmsg << "[" << _testname << "] TEST_FAILED ############################################" << std::endl;
        errmsg << std::endl;

        _out << errmsg.str();

        // also send error to stderr
        //std::cerr << errmsg.str();

        // abort program
        exit(1);
    }

private:
    std::string _testname;
    std::ostream& _out;
};
