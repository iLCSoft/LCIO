#include "UTIL/ReconstructedParticleTools.h"
#include <algorithm>
#include "UTIL/TrackTools.h"
#include <cmath>

namespace UTIL{
    EVENT::Track* getLeadingTrack(const EVENT::ReconstructedParticle* particle){
        const EVENT::TrackVec& tracks = particle->getTracks();
        if ( tracks.empty() ) return nullptr;
        // compare momentum w/o using its common scale factor (which includes magnetic field)
        auto sortByMomentum = [](const EVENT::Track* a, const EVENT::Track* b) { return std::hypot(1., a->getTanLambda())/std::abs(a->getOmega()) < std::hypot(1., b->getTanLambda())/std::abs(b->getOmega()); };
        auto* leadingTrack = *(std::max_element(tracks.begin(), tracks.end(), sortByMomentum));

        return leadingTrack;
    }
}
