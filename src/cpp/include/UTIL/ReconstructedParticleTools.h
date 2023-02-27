#ifndef UTIL_ReconstructedParticleTools_H
#define UTIL_ReconstructedParticleTools_H 1

#include "EVENT/ReconstructedParticle.h"
#include "EVENT/Track.h"

namespace UTIL{
    /** Extract the leading track (sorted by momentum) in case of multiple tracks attached to a single ReconstructedParticle.
    * @author Bohdan Dudar
    * @version August 2022
    */
    EVENT::Track* getLeadingTrack(const EVENT::ReconstructedParticle* particle);
}

#endif
