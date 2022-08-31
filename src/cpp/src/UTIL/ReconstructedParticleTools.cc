#include "UTIL/ReconstructedParticleTools.h"
#include <algorithm>
#include "UTIL/TrackTools.h"
// #include <vector>

namespace UTIL{
    EVENT::Track* getLeadingTrack(const EVENT::ReconstructedParticle* particle, double bz){
        const EVENT::TrackVec& tracks = particle->getTracks();
        if ( tracks.empty() ) return nullptr;
        auto sortByMomentum = [bz](const EVENT::Track* a, const EVENT::Track* b) { return getTrackMomentum(a, bz) < getTrackMomentum(b, bz); };
        auto* leadingTrack = *(std::max_element(tracks.begin(), tracks.end(), sortByMomentum));

        return leadingTrack;
    }
}
