#include "UTIL/LCWarning.h"

#include <iostream>
#include "Exceptions.h"

using namespace std;

namespace UTIL{


///// add new warnings here ===================================================
LCWarning::LCWarning( ostream& outstream ) : _outstream(outstream) {


    // ----- Warnings for SimTrackerHit ---------------------------------------
    registerWarning(
        "SIMTRACKERHIT_DEPRECATED_GETDEDX",
        "used DEPRECATED method SimTrackerHitImpl::getdEdx()\n"
        "please change your code to use SimTrackerHitImpl::getEDep() instead"
    );

    registerWarning(
        "SIMTRACKERHIT_DEPRECATED_SETDEDX",
        "used DEPRECATED method SimTrackerHitImpl::setdEdx()\n"
        "please change your code to use SimTrackerHitImpl::setEDep() instead"
    );

    registerWarning(
        "SIMTRACKERHIT_DEPRECATED_GETCELLID",
        "used DEPRECATED method SimTrackerHitImpl::getCellID()\n"
        "please change your code to use SimTrackerHitImpl::getCellID0() instead"
    );

    registerWarning(
        "SIMTRACKERHIT_DEPRECATED_SETCELLID",
        "used DEPRECATED method SimTrackerHitImpl::setCellID()\n"
        "please change your code to use SimTrackerHitImpl::setCellID0() instead"
    );

    // ------------------------------------------------------------------------



    // ----- Warnings for TrackerHit ------------------------------------------
    registerWarning(
        "TRACKERHIT_DEPRECATED_GETDEDX",
        "used DEPRECATED method TrackerHitImpl::getdEdx()\n"
        "please change your code to use TrackerHitImpl::getEDep()"
    );

    registerWarning(
        "TRACKERHIT_DEPRECATED_SETDEDX",
        "used DEPRECATED method TrackerHitImpl::setdEdx()\n"
        "please change your code to use TrackerHitImpl::setEDep() instead."
    );
    // ------------------------------------------------------------------------



    // ----- Warnings for SimCalorimeterHit ---------------------------------------
    registerWarning(
        "SIMCALORIMETERHIT_DEPRECATED_GETNMCPARTICLES",
        "used DEPRECATED method SimCalorimeterHitImpl::getNMCParticles()\n"
        "please change your code to use SimCalorimeterHitImpl::getNMCContributions() instead"
    );
    // ------------------------------------------------------------------------



    // ----- Warnings for MCParticle ------------------------------------------
    registerWarning(
        "MCPARTICLE_DEPRECATED_GETNUMBEROFPARENTS",
        "used DEPRECATED method MCParticleImpl::getNumberOfParents()\n"
        "please change your code to use MCParticleImpl::getParents().size() instead."
    );

    registerWarning(
        "MCPARTICLE_DEPRECATED_GETPARENT",
        "used DEPRECATED method MCParticleImpl::getParent(i)\n"
        "please change your code to use MCParticleImpl::getParents()[i] instead."
    );

    registerWarning(
        "MCPARTICLE_DEPRECATED_GETNUMBEROFDAUGHTERS",
        "used DEPRECATED method MCParticleImpl::getNumberOfDaughters()\n"
        "please change your code to use MCParticleImpl::getDaughters().size() instead."
    );

    registerWarning(
        "MCPARTICLE_DEPRECATED_GETDAUGHTER",
        "used DEPRECATED method MCParticleImpl::getDaughter(i)\n"
        "please change your code to use MCParticleImpl::getDaughters()[i] instead."
    );
    // ------------------------------------------------------------------------


}
///// =========================================================================



void LCWarning::registerWarning( const string& id, const string& txt, int max ) {

    if( _warning_cfg.find( id ) != _warning_cfg.end() ){
        throw EVENT::Exception( std::string( "Warning [ "+id+" ] was already registered") ); 
    }

    _warning_cfg[ id ].txt = txt ;
    _warning_cfg[ id ].max = max ;
    _warning_cfg[ id ].counter = max ;

    // _outstream << "registered warning " << id << "with max limit: " << max << endl;
}



void LCWarning::printWarning( const string& id ) {

    // if max limit has been reached for displaying the warning, just return and do nothing.
    if( _warning_cfg[ id ].counter <= 0 ){ return ; }

    _outstream << endl << endl
         << endl << "=========== WARNING ========================================================="
         << endl << _warning_cfg[ id ].txt;

    if(  _warning_cfg[ id ].counter > 1 ){
        _outstream << endl << "*** this warning will be printed (at most) "
            << _warning_cfg[ id ].max << " time(s). [ "
            << _warning_cfg[ id ].counter-1 << " time(s) left ]" ;
    }
    // max warnings have been reached.
    else{
        _outstream << endl << "*** the maximum amount of times for printing this warning has been reached."
            << endl << "*** this warning will be printed one last time before the application exits." ;

    }

    _outstream << endl << "============================================================================="
         << endl << endl ;

    // decrease counter
    _warning_cfg[ id ].counter-- ;
}



// destructor shows warnings one very last time before program ends
LCWarning::~LCWarning(){

    bool first_warning = true;

    for( _warning_cfg_it = _warning_cfg.begin(); _warning_cfg_it != _warning_cfg.end(); _warning_cfg_it++ ){
        if( (*_warning_cfg_it).second.max != (*_warning_cfg_it).second.counter ){
            if( first_warning ){
                _outstream << endl << endl
                    << endl << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
                    << endl << "+++ FOLLOWING WARNINGS WERE FOUND:"
                    << endl << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
                ;
                first_warning = false ;
            }
            _outstream << endl << endl << _warning_cfg[ (*_warning_cfg_it).first ].txt ;
        }
    }
    _outstream << endl << endl;
}



// singleton
LCWarning& LCWarning::getInstance()
{
  static LCWarning instance;
  return instance;
}



} // namespace UTIL

