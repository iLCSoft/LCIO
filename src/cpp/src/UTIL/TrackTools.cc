#include "UTIL/TrackTools.h"
#include <cmath>

namespace UTIL{
    std::array<double, 3> getTrackMomentum(const EVENT::Track* track, double bz){
        std::array<double, 3> mom;
        double omega = track->getOmega();
        double phi = track->getPhi();
        double tanL = track->getTanLambda();
        double c_light = 299.792458; // mm/ns
        double pt = (1e-6 * c_light * bz) / std::abs(omega);
        mom[0] = pt*std::cos(phi);
        mom[1] = pt*std::sin(phi);    
        mom[2] = pt*tanL;
        return mom;
    }
}
