#ifndef UTIL_TrackTools_H
#define UTIL_TrackTools_H 1

#include "EVENT/Track.h"
#include <array>
#include <cmath>

namespace UTIL{
    /** Extract track momentum from its track parameters and magnetic field
    * @author Bohdan Dudar
    * @version August 2022
    */
    template <typename TrackLikeT>
    std::array<double, 3> getTrackMomentum(const TrackLikeT* track, double bz){
        double omega = track->getOmega();
        if (omega == 0.) return {0., 0., 0.};
        double phi = track->getPhi();
        double tanL = track->getTanLambda();
        double c_light = 299.792458; // mm/ns
        double pt = (1e-6 * c_light * bz) / std::abs(omega);
        return {pt*std::cos(phi), pt*std::sin(phi), pt*tanL};
    }
}

#endif
