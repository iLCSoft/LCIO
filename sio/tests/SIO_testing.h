#ifndef SIO_TESTING_H
#define SIO_TESTING_H

// -- std headers
#include <iostream>
#include <string>
#include <sstream>

namespace sio {

  namespace test {

    class UnitTest {
    public:
      UnitTest(const std::string &name, bool abortOnFail = true) :
      m_name(name),
      m_abortOnFail(abortOnFail) {
        std::cout << "==================================" << std::endl;
        std::cout << "== Starting unit test '" << m_name << "'" << std::endl;
        std::cout << "==================================" << std::endl;
      }
      
      ~UnitTest() {
        printSummary();
      }
      
      template <typename U, typename V>
      void test(const std::string& t, const U& lhs, const V& rhs) {
        std::stringstream sstr;
        sstr << "[TEST:" << t << "] ";  
        if( lhs == rhs ) {
          sstr << "'" << lhs << "' == '" << rhs << "'";
          pass( sstr.str() );
        }
        else {
          sstr << "'" << lhs << "' != '" << rhs << "'";
          error( sstr.str() );
        }
      }
      
      template <typename C>
      void test(const std::string& t, const C& condition) {
        std::stringstream sstr;
        sstr << "[TEST:" << t << "] ";  
        if( (condition) ) {
          sstr << "'" << condition << "'";
          pass( sstr.str() );
        }
        else {
          sstr << "'" << condition << "'";
          error( sstr.str() );
        }
      }
      
      void error(const std::string& message) {
        m_failed++;
        std::cerr << message << std::endl;
        if( m_abortOnFail ) {
          std::cerr << "Abort on fail !" << std::endl;
          printSummary();
          exit(1);
        }
      }
      
      void pass(const std::string& message) {
        m_passed++;
        std::cout << message << std::endl;
      }
      
      std::string status() {
        return ( m_failed > 0 ? "TEST_FAILED" : "TEST_PASSED");
      }
      
      void printSummary() {
        std::cout << "==================================" << std::endl;
        std::cout << "[" << m_name << "]: " << m_passed << " PASSED, " << m_failed << " FAILED" << std::endl;
        std::cout <<  status() << std::endl;
        std::cout << "==================================" << std::endl;
      }
      
    private:
      std::string          m_name = {""};
      bool                 m_abortOnFail = {true};
      unsigned int         m_passed = {0};
      unsigned int         m_failed = {0};
    };

  }
  
}

#endif //  SIO_TESTING_H
