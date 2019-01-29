#ifndef SIO_TESTING_COMMON_H
#define SIO_TESTING_COMMON_H

#include "SIO_definitions.h"
#include "SIO_functions.h"
#include "SIO_block.h"
#include "SIO_stream.h"

#include <vector>

namespace sio {
  
  namespace test {
    
    class SimpleBlock : public SIO_block {
    public:
      struct Data {
        int       m_integer{0};
        double    m_double{0.};
        float     m_float{0.f};
      };
    public:
      SimpleBlock() :
        SIO_block("SimpleBlock") {
        /* nop */
      }
      unsigned int xfer( SIO_stream* stream, SIO_operation, unsigned int) {
        unsigned int status ; // needed by the SIO_DATA macro
        SIO_DATA( stream , &m_data.m_integer, 1 ) ;
        SIO_DATA( stream , &m_data.m_double, 1 ) ;
        SIO_DATA( stream , &m_data.m_float, 1 ) ;
        return SIO_BLOCK_SUCCESS;
      }
      unsigned int version() const {
        return SIO_VERSION_ENCODE( 1, 0 ) ;
      }
      const Data &data() const {
        return m_data;
      }
      Data &data() {
        return m_data;
      }
    private:
      Data      m_data{};
    };
    
    
    class BlockWithPtr : public SIO_block {
    public:
      struct Data {
        int       m_integer{0};
        double    m_double{0.};
        float     m_float{0.f};
      };
    public:
      BlockWithPtr() :
        SIO_block("BlockWithPtr") {
        /* nop */
      }
      ~BlockWithPtr() {
        clear();
      }
      unsigned int xfer( SIO_stream* stream, SIO_operation op, unsigned int) {
        unsigned int status ; // needed by the SIO_DATA macro
        if(op == SIO_OP_READ) {
          clear();
          int size;
          SIO_DATA( stream , &size, 1 ) ;
          m_data.resize(size);
          for(int i=0 ; i<size ; i++) {
            m_data[i] = new Data();
            SIO_PTAG( stream , m_data[i] ) ;
            SIO_DATA( stream , &(m_data[i]->m_integer), 1 ) ;
            SIO_DATA( stream , &(m_data[i]->m_double), 1 ) ;
            SIO_DATA( stream , &(m_data[i]->m_float), 1 ) ;
          }
          SIO_DATA( stream , &size, 1 ) ;
          m_ptrs.resize(size);
          for(int i=0 ; i<size ; i++) {
            SIO_PNTR( stream , &(m_ptrs[i]) ) ;
          }
        }
        else {
          int size = m_data.size(); 
          SIO_DATA( stream , &size, 1 ) ;
          for(auto d : m_data) {
            SIO_PTAG( stream , d ) ;
            SIO_DATA( stream , &d->m_integer, 1 ) ;
            SIO_DATA( stream , &d->m_double, 1 ) ;
            SIO_DATA( stream , &d->m_float, 1 ) ;
          }
          size = m_ptrs.size(); 
          SIO_DATA( stream , &size, 1 ) ;
          for(auto d : m_ptrs) {
            SIO_PNTR( stream , &d ) ;
          }
        }
        return SIO_BLOCK_SUCCESS;
      }
      unsigned int version() const {
        return SIO_VERSION_ENCODE( 1, 0 ) ;
      }
      void clear() {
        for(auto d : m_data) {
          delete d;
        }
        m_data.clear();
        m_ptrs.clear();
      }
      void addData( int i, double d, float f ) {
        auto data = new Data();
        data->m_integer = i;
        data->m_float = f;
        data->m_double = d;
        m_data.push_back(data);
        m_ptrs.push_back(data);
        m_ptrs.push_back(data);
      }
      const std::vector<Data*> &data() const {
        return m_data;
      }
      const std::vector<Data*> &ptrs() const {
        return m_ptrs;
      }
      bool validateData() const {
        if(m_data.size()*2 != m_ptrs.size()) {
          std::cout << "Sizes: " << m_data.size() << ", " << m_ptrs.size() << std::endl;
          return false;
        }
        for(unsigned int i=0 ; i<m_data.size() ; i++) {
          std::cout << "Counters " << i << ", " << i*2 << ", " << i*2+1 << std::endl;
          if( m_data.at(i) != m_ptrs.at(i*2) ) {
            std::cout << "data is " << m_data.at(i) << std::endl;
            std::cout << "ptr even is " << m_ptrs.at(i*2) << std::endl;
            return false;
          }
          if( m_data.at(i) != m_ptrs.at(i*2+1) ) {
            std::cout << "data is " << m_data.at(i) << std::endl;
            std::cout << "ptr odd is " << m_ptrs.at(i*2+1) << std::endl;
            return false;
          }
        }
        return true;
      }
      
    private:
      // Owns the data
      std::vector<Data*>      m_data{};
      // Reference the owned data
      std::vector<Data*>      m_ptrs{};
    };
    
  }
  
}

#endif  //  SIO_TESTING_COMMON_H
