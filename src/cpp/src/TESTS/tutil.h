#include <iostream>
//#include <cstring>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <array>
#include <limits>
#include <cmath>

template<typename T, size_t N>
std::ostream& operator<<(std::ostream& os, const std::array<T, N>& arr) {
  if constexpr (N == 0) {
    return os << "[]";
  }

  os << "[" << arr[0];
  for (size_t i = 1; i < N; ++i) {
    os << ", " << arr[i];
  }
  return os << "]";
}

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

  void operator()(bool cond, const std::string msg) {
    if ( ! cond ) FAILED( msg ) ;
    return ;
  }

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

bool approxEqual(double lhs, double rhs) {
  // Following a similar, but slightly simplified approach as Catch2::Approx here
  constexpr double epsilon = std::numeric_limits<float>::epsilon() * 100;
  const double margin = std::fabs(lhs) * epsilon;
  return (lhs + margin >= rhs) && (rhs + margin >= lhs);
}

template<typename T, size_t N, typename ApproxComp=decltype(approxEqual)>
bool approxEqArray(const std::array<T, N>& arr1, const std::array<T, N>& arr2, ApproxComp&& comp=approxEqual) {
  for (size_t i = 0; i < N; ++i) {
    if (!comp(arr1[i], arr2[i])) {
      return false;
    }
  }
  return true;
}

