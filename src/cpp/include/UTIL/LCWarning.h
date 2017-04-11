// -*- C++ -*-
#ifndef UTIL_LCWARNING_H
#define UTIL_LCWARNING_H 1

#include<map>
#include<string>
#include<iostream>

namespace UTIL {

/** Utility class to show warnings in LCIO.
 *  
 *  The maximum amount of warnings printed is configurable and per default set to 10.
 *
 *  Warnings are shown one last time when application ends.
 *
 * @author engels
 * @version
 */
class LCWarning {
public: 

    static LCWarning& getInstance(); // singleton class

    /** register a new warning
     *
     *  id = "a_unique_string_to_identify_the_warning"
     *  txt = "the world will end tomorrow, leave the room immediately and go get some fun ;)"
     *  max = the maximum amount of times to display the warning
     */
    void registerWarning( const std::string& id, const std::string& txt, int max=10 ) ;

    /** print the warning text associated to the given id */
    void printWarning( const std::string& id ) ;

private:
    // singleton settings
    LCWarning( std::ostream& outstream=std::cerr );
    ~LCWarning(); //{}
    LCWarning( const LCWarning& ) ;
    LCWarning & operator=(const LCWarning &);
    static LCWarning& instance ;


    struct _warning_cfg_struct{
      std::string txt{};
      int max{0};
      int counter{0};
    };

    std::map< std::string, _warning_cfg_struct > _warning_cfg{} ; // warning configurations
    std::map< std::string, _warning_cfg_struct >::iterator _warning_cfg_it{} ; // iterator
    std::ostream& _outstream ; // where warnings get printed

}; // class

} // namespace UTIL
#endif /* ifndef UTIL_LCWARNING_H */
