#ifndef UTIL_TrackTools_H
#define UTIL_TrackTools_H 1

#include "EVENT/Track.h"
#include <array>

namespace UTIL{
    /** Extract track momentum from its track parameters and magnetic field
    * @author Bohdan Dudar
    * @version August 2022
    */
    std::array<double, 3> getTrackMomentum(const EVENT::Track* track, double bz);
}

#endif
