#include <SIO_testing.h>
#include <SIO_functions.h>

// -- std headers
#include <array>
#include <cmath>
#include <limits>

using UnitTest = sio::test::UnitTest;

int main() {

  UnitTest unitTest("test_functions");

  // Validate read/write operations
  std::array<unsigned char, sizeof(float)> buf;
  float write_fval = 12.365;
  float read_fval = 0;
  sio::functions::copy( (unsigned char*)&write_fval , buf.data(), sizeof(float) , 1 );
  sio::functions::copy( buf.data() , (unsigned char*)&read_fval, sizeof(float) , 1 );
  unitTest.test( "read/write op", fabs(write_fval - read_fval) < std::numeric_limits<float>::epsilon() );

  // Validate function validateName()
  unitTest.test( "validate name ok 1", sio::functions::validateName( "bibou" ) );
  unitTest.test( "validate name ok 2", sio::functions::validateName( "_bibou_" ) );
  unitTest.test( "validate name ok 3", sio::functions::validateName( "_bibou_054" ) );
  unitTest.test( "validate name wrong 1", not sio::functions::validateName( "-bibou_" ) );
  unitTest.test( "validate name wrong 2", not sio::functions::validateName( "054bibou" ) );
  unitTest.test( "validate name wrong 3", not sio::functions::validateName( "/054bibou" ) );
  unitTest.test( "validate name wrong 4", not sio::functions::validateName( "?054bibou" ) );
  unitTest.test( "validate name wrong 5", not sio::functions::validateName( "bibou-ere" ) );
  
  return 0;
}