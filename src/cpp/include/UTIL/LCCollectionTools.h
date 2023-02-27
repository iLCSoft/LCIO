#ifndef UTIL_LCCollectionTools_H
#define UTIL_LCCollectionTools_H 1

#include "EVENT/LCObject.h"
#include "EVENT/LCCollection.h"

namespace UTIL{
    /** Extract object index inside a given LCCollection. If object is not found, return -1.
    * @author Bohdan Dudar
    * @version August 2022
    */
    int getElementIndex(const EVENT::LCObject* item, EVENT::LCCollection* collection);
}

#endif
