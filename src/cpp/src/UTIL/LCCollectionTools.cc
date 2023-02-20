#include "UTIL/LCCollectionTools.h"

namespace UTIL{
    int getElementIndex(const EVENT::LCObject* item, EVENT::LCCollection* collection){
        for(int i=0; i < collection->getNumberOfElements(); ++i){
            if ( item == collection->getElementAt(i) ) return i;
        }
        return -1;
    }
}
