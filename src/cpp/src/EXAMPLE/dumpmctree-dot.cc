#include "lcio.h"
#include "IO/LCReader.h"
#include "IMPL/LCTOOLS.h"
#include "EVENT/LCCollection.h"
#include "EVENT/MCParticle.h"

// #include "EVENT/LCRunHeader.h" 

// #include "EVENT/SimCalorimeterHit.h" 
// #include "EVENT/CalorimeterHit.h" 
// #include "EVENT/RawCalorimeterHit.h" 

// #include "UTIL/CellIDDecoder.h"

// #include <cstdlib>

#include <cmath>
#include <getopt.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
using namespace lcio ;

const static std::map<int, std::string> pdg2strMap ={
    {91, "Cluster"},
    {92, "Hadronization"},
    {94, "Start pythia"},

    // quarks
    {1, "d"},
    {-1, "-d"},
    {2, "u"},
    {-2, "-u"},
    {3, "s"},
    {-3, "-s"},
    {4, "c"},
    {-4, "-c"},
    {5, "b"},
    {-5, "-b"},
    {6, "t"},
    {-6, "-t"},
    {7, "b<SUP>'</SUP>"},
    {-7, "-b<SUP>'</SUP>"},
    {8, "t<SUP>'</SUP>"},
    {-8, "-t<SUP>'</SUP>"},
    {9, "g"},

    // leptons
    {11, "e<SUP>-</SUP>"},
    {-11, "e<SUP>+</SUP>"},
    {12, "&nu;<SUB>e</SUB>"},
    {-12, "&nu;<SUB>e</SUB>"},
    {13, "&mu;<SUP>-</SUP>"},
    {-13, "&mu;<SUP>+</SUP>"},
    {14, "&nu;<SUB>&mu;</SUB>"},
    {-14, "&nu;<SUB>&mu;</SUB>"},
    {15, "&tau;<SUP>-</SUP>"},
    {-15, "&tau;<SUP>+</SUP>"},
    {16, "&nu;<SUB>&tau;</SUB>"},
    {-16, "&nu;<SUB>&tau;</SUB>"},
    //Gauge and Higgs Boson
    {21, "g"},
    {22, "&gamma;"},
    {23, "Z<SUP>0</SUP>"},
    {24, "W<SUP>+</SUP>"},
    {-24, "W<SUP>-</SUP>"},
    {25, "H<SUP>0</SUP>"},
    // Higgs?
    // light mesons I=1
    {111, "&pi;<SUP>0</SUP>"},
    {-111, "&pi;<SUP>0</SUP>"},
    {211, "&pi;<SUP>+</SUP>"},
    {-211, "&pi;<SUP>-</SUP>"},
    {9000111, "a<SUB>0</SUB>(980)<SUP>0</SUP>"},
    {-9000111, "a<SUB>0</SUB>(980)<SUP>0</SUP>"},
    {9000211, "a<SUB>0</SUB>(980)<SUP>+</SUP>"},
    {-9000211, "a<SUB>0</SUB>(980)<SUP>-</SUP>"},
    {100111, "&pi;(1300)<SUP>0</SUP>"},
    {-100111, "&pi;(1300)<SUP>0</SUP>"},
    {100211, "&pi;(1300)<SUP>+</SUP>"},
    {-100211, "&pi;(1300)<SUP>-</SUP>"},
    {10111, "a<SUB>0</SUB>(1450)<SUP>0</SUP>"},
    {-10111, "a<SUB>0</SUB>(1450)<SUP>0</SUP>"},
    {10211, "a<SUB>0</SUB>(1450)<SUP>+</SUP>"},
    {-10211, "a<SUB>0</SUB>(1450)<SUP>-</SUP>"},
    {9010111, "&pi;(1800)<SUP>0</SUP>"},
    {-9010111, "&pi;(1800)<SUP>0</SUP>"},
    {9010211, "&pi;(1800)<SUP>+</SUP>"},
    {-9010211, "&pi;(1800)<SUP>-</SUP>"},
    {113, "&rho;(770)<SUP>0</SUP>"},
    {-113, "&rho;(770)<SUP>0</SUP>"},
    {213, "&rho;(770)<SUP>+</SUP>"},
    {-213, "&rho;(770)<SUP>-</SUP>"},
    {10113, "b<SUB>1</SUB>(1235)<SUP>0</SUP>"},
    {-10113, "b<SUB>1</SUB>(1235)<SUP>0</SUP>"},
    {10213, "b<SUB>1</SUB>(1235)<SUP>+</SUP>"},
    {-10213, "b<SUB>1</SUB>(1235)<SUP>-</SUP>"},
    {20113, "a<SUB>1</SUB>(1260)<SUP>0</SUP>"},
    {-20113, "a<SUB>1</SUB>(1260)<SUP>0</SUP>"},
    {20213, "a<SUB>1</SUB>(1260)<SUP>+</SUP>"},
    {-20213, "a<SUB>1</SUB>(1260)<SUP>-</SUP>"},
    {9000113, "&pi;<SUB>1</SUB>(1400)<SUP>0</SUP>"},
    {-9000113, "&pi;<SUB>1</SUB>(1400)<SUP>0</SUP>"},
    {9000213, "&pi;<SUB>1</SUB>(1400)<SUP>+</SUP>"},
    {-9000213, "&pi;<SUB>1</SUB>(1400)<SUP>-</SUP>"},
    {100113, "&rho;(1450)<SUP>0</SUP>"},
    {-100113, "&rho;(1450)<SUP>0</SUP>"},
    {100213, "&rho;(1450)<SUP>+</SUP>"},
    {-100213, "&rho;(1450)<SUP>-</SUP>"},
    {9010113, "&pi;<SUB>1</SUB>(1600)<SUP>0</SUP>"},
    {-9010113, "&pi;<SUB>1</SUB>(1600)<SUP>0</SUP>"},
    {9010213, "&pi;<SUB>1</SUB>(1600)<SUP>+</SUP>"},
    {-9010213, "&pi;<SUB>1</SUB>(1600)<SUP>-</SUP>"},
    {9020113, "a<SUB>1</SUB>(1640)<SUP>0</SUP>"},
    {-9020113, "a<SUB>1</SUB>(1640)<SUP>0</SUP>"},
    {9020213, "a<SUB>1</SUB>(1640)<SUP>+</SUP>"},
    {-9020213, "a<SUB>1</SUB>(1640)<SUP>-</SUP>"},
    {30113, "&rho;(1700)<SUP>0</SUP>"},
    {-30113, "&rho;(1700)<SUP>0</SUP>"},
    {30213, "&rho;(1700)<SUP>+</SUP>"},
    {-30213, "&rho;(1700)<SUP>-</SUP>"},
    {9030113, "&rho;(1900)<SUP>0</SUP>"},
    {-9030113, "&rho;(1900)<SUP>0</SUP>"},
    {9030213, "&rho;(1900)<SUP>+</SUP>"},
    {-9030213, "&rho;(1900)<SUP>-</SUP>"},
    {9040113, "&rho;(2150)<SUP>0</SUP>"},
    {-9040113, "&rho;(2150)<SUP>0</SUP>"},
    {9040213, "&rho;(2150)<SUP>+</SUP>"},
    {-9040213, "&rho;(2150)<SUP>-</SUP>"},
    {115, "a<SUB>2</SUB>(1320)<SUP>0</SUP>"},
    {-115, "a<SUB>2</SUB>(1320)<SUP>0</SUP>"},
    {215, "a<SUB>2</SUB>(1320)<SUP>+</SUP>"},
    {-215, "a<SUB>2</SUB>(1320)<SUP>-</SUP>"},
    {10115, "&pi;<SUB>2</SUB>(1670)<SUP>0</SUP>"},
    {-10115, "&pi;<SUB>2</SUB>(1670)<SUP>0</SUP>"},
    {10215, "&pi;<SUB>2</SUB>(1670)<SUP>+</SUP>"},
    {-10215, "&pi;<SUB>2</SUB>(1670)<SUP>-</SUP>"},
    {9000115, "a<SUB>2</SUB>(1700)<SUP>0</SUP>"},
    {-9000115, "a<SUB>2</SUB>(1700)<SUP>0</SUP>"},
    {9000215, "a<SUB>2</SUB>(1700)<SUP>+</SUP>"},
    {-9000215, "a<SUB>2</SUB>(1700)<SUP>-</SUP>"},
    {9010115, "&pi;<SUB>2</SUB>(2100)<SUP>0</SUP>"},
    {-9010115, "&pi;<SUB>2</SUB>(2100)<SUP>0</SUP>"},
    {9010215, "&pi;<SUB>2</SUB>(2100)<SUP>+</SUP>"},
    {-9010215, "&pi;<SUB>2</SUB>(2100)<SUP>-</SUP>"},
    {117, "&rho;<SUB>3</SUB>(1690)<SUP>0</SUP>"},
    {-117, "&rho;<SUB>3</SUB>(1690)<SUP>0</SUP>"},
    {217, "&rho;<SUB>3</SUB>(1690)<SUP>+</SUP>"},
    {-217, "&rho;<SUB>3</SUB>(1690)<SUP>-</SUP>"},
    {9000117, "&rho;<SUB>3</SUB>(1990)<SUP>0</SUP>"},
    {-9000117, "&rho;<SUB>3</SUB>(1990)<SUP>0</SUP>"},
    {9000217, "&rho;<SUB>3</SUB>(1990)<SUP>+</SUP>"},
    {-9000217, "&rho;<SUB>3</SUB>(1990)<SUP>-</SUP>"},
    {9010117, "&rho;<SUB>3</SUB>(2250)<SUP>0</SUP>"},
    {-9010117, "&rho;<SUB>3</SUB>(2250)<SUP>0</SUP>"},
    {9010217, "&rho;<SUB>3</SUB>(2250)<SUP>+</SUP>"},
    {-9010217, "&rho;<SUB>3</SUB>(2250)<SUP>-</SUP>"},
    {119, "a<SUB>4</SUB>(2040)<SUP>0</SUP>"},
    {-119, "a<SUB>4</SUB>(2040)<SUP>0</SUP>"},
    {219, "a<SUB>4</SUB>(2040)<SUP>+</SUP>"},
    {-219, "a<SUB>4</SUB>(2040)<SUP>-</SUP>"},


    // light mesons I=0
    {221, "&eta;"},
    {-221, "&eta;"},
    {331, "&eta;\'"},
    {-331, "&eta;\'"},
    {9000221, "f<SUB>0</SUB>(600)"},
    {-9000221, "f<SUB>0</SUB>(600)"},
    {9010221, "f<SUB>0</SUB>(980)"},
    {-9010221, "f<SUB>0</SUB>(980)"},
    {100221, "&eta;(1295)"},
    {-100221, "&eta;(1295)"},
    {10221, "f<SUB>0</SUB>(1370)"},
    {-10221, "f<SUB>0</SUB>(1370)"},
    {9020221, "&eta;(1405)"},
    {-9020221, "&eta;(1405)"},
    {100331, "&eta;(1475)"},
    {-100331, "&eta;(1475)"},
    {9030221, "f<SUB>0</SUB>(1500)"},
    {-9030221, "f<SUB>0</SUB>(1500)"},
    {10331, "f<SUB>0</SUB>(1710)"},
    {-10331, "f<SUB>0</SUB>(1710)"},
    {9040221, "&eta;(1760)"},
    {-9040221, "&eta;(1760)"},
    {9050221, "f<SUB>0</SUB>(2020)"},
    {-9050221, "f<SUB>0</SUB>(2020)"},
    {9060221, "f<SUB>0</SUB>(2100)"},
    {-9060221, "f<SUB>0</SUB>(2100)"},
    {9070221, "f<SUB>0</SUB>(2200)"},
    {-9070221, "f<SUB>0</SUB>(2200)"},
    {9080221, "&eta;(2225)"},
    {-9080221, "&eta;(2225)"},
    {223, "&omega;"},
    {-223, "&omega;"},
    {333, "&phi;"},
    {-333, "&phi;"},
    {10223, "h<SUB>1</SUB>(1170)"},
    {-10223, "h<SUB>1</SUB>(1170)"},
    {20223, "f<SUB>1</SUB>(1285)"},
    {-20223, "f<SUB>1</SUB>(1285)"},
    {10333, "h<SUB>1</SUB>(1380)"},
    {-10333, "h<SUB>1</SUB>(1380)"},
    {20333, "f<SUB>1</SUB>(1420)"},
    {-20333, "f<SUB>1</SUB>(1420)"},
    {100223, "&omega;(1420)"},
    {-100223, "&omega;(1420)"},
    {9000223, "f<SUB>1</SUB>(1510)"},
    {-9000223, "f<SUB>1</SUB>(1510)"},
    {9010223, "h<SUB>1</SUB>(1595)"},
    {-9010223, "h<SUB>1</SUB>(1595)"},
    {30223, "&omega;(1650)"},
    {-30223, "&omega;(1650)"},
    {100333, "&phi;(1680)"},
    {-100333, "&phi;(1680)"},
    {225, "f<SUB>2</SUB>(1270)"},
    {-225, "f<SUB>2</SUB>(1270)"},
    {9000225, "f<SUB>2</SUB>(1430)"},
    {-9000225, "f<SUB>2</SUB>(1430)"},
    {335, "f<SUB>2</SUB>\'(1525)"},
    {-335, "f<SUB>2</SUB>\'(1525)"},
    {9010225, "f<SUB>2</SUB>(1565)"},
    {-9010225, "f<SUB>2</SUB>(1565)"},
    {9020225, "f<SUB>2</SUB>(1640)"},
    {-9020225, "f<SUB>2</SUB>(1640)"},
    {10225, "&eta;<SUB>2</SUB>(1645)"},
    {-10225, "&eta;<SUB>2</SUB>(1645)"},
    {9030225, "f<SUB>2</SUB>(1810)"},
    {-9030225, "f<SUB>2</SUB>(1810)"},
    {10335, "&eta;<SUB>2</SUB>(1870)"},
    {-10335, "&eta;<SUB>2</SUB>(1870)"},
    {9040225, "f<SUB>2</SUB>(1910)"},
    {-9040225, "f<SUB>2</SUB>(1910)"},
    {9050225, "f<SUB>2</SUB>(1950)"},
    {-9050225, "f<SUB>2</SUB>(1950)"},
    {9060225, "f<SUB>2</SUB>(2010)"},
    {-9060225, "f<SUB>2</SUB>(2010)"},
    {9070225, "f<SUB>2</SUB>(2150)"},
    {-9070225, "f<SUB>2</SUB>(2150)"},
    {9080225, "f<SUB>2</SUB>(2300)"},
    {-9080225, "f<SUB>2</SUB>(2300)"},
    {9090225, "f<SUB>2</SUB>(2340)"},
    {-9090225, "f<SUB>2</SUB>(2340)"},
    {227, "&omega;<SUB>3</SUB>(1670)"},
    {-227, "&omega;<SUB>3</SUB>(1670)"},
    {337, "&phi;<SUB>3</SUB>(1850)"},
    {-337, "&phi;<SUB>3</SUB>(1850)"},
    {229, "f<SUB>4</SUB>(2050)"},
    {-229, "f<SUB>4</SUB>(2050)"},
    {9000229, "f<SUB>J</SUB>(2220)"},
    {-9000229, "f<SUB>J</SUB>(2220)"},
    {9010229, "f<SUB>4</SUB>(2300)"},
    {-9010229, "f<SUB>4</SUB>(2300)"},

    // strange mesons
    {130, "K<SUB>L</SUB><SUP>0</SUP>"},
    {-130, "K<SUB>L</SUB><SUP>0</SUP>"},
    {310, "K<SUB>S</SUB><SUP>0</SUP>"},
    {-310, "K<SUB>S</SUB><SUP>0</SUP>"},
    {311, "K<SUP>0</SUP>"},
    {-311, "K<SUP>0</SUP>"},
    {321, "K<SUP>+</SUP>"},
    {-321, "K<SUP>-</SUP>"},
    {9000311, "K<SUB>0</SUB><SUP>*</SUP>(800)<SUP>0</SUP>"},
    {-9000311, "K<SUB>0</SUB><SUP>*</SUP>(800)<SUP>0</SUP>"},
    {9000321, "K<SUB>0</SUB><SUP>*</SUP>(800)<SUP>+</SUP>"},
    {-9000321, "K<SUB>0</SUB><SUP>*</SUP>(800)<SUP>-</SUP>"},
    {10311, "K<SUB>0</SUB><SUP>*</SUP>(1430)<SUP>0</SUP>"},
    {-10311, "K<SUB>0</SUB><SUP>*</SUP>(1430)<SUP>0</SUP>"},
    {10321, "K<SUB>0</SUB><SUP>*</SUP>(1430)<SUP>+</SUP>"},
    {-10321, "K<SUB>0</SUB><SUP>*</SUP>(1430)<SUP>-</SUP>"},
    {100311, "K(1460)<SUP>0</SUP>"},
    {-100311, "K(1460)<SUP>0</SUP>"},
    {100321, "K(1460)<SUP>+</SUP>"},
    {-100321, "K(1460)<SUP>-</SUP>"},
    {9010311, "K(1830)<SUP>0</SUP>"},
    {-9010311, "K(1830)<SUP>0</SUP>"},
    {9010321, "K(1830)<SUP>+</SUP>"},
    {-9010321, "K(1830)<SUP>-</SUP>"},
    {9020311, "K<SUB>0</SUB><SUP>*</SUP>(1950)<SUP>0</SUP>"},
    {-9020311, "K<SUB>0</SUB><SUP>*</SUP>(1950)<SUP>0</SUP>"},
    {9020321, "K<SUB>0</SUB><SUP>*</SUP>(1950)<SUP>+</SUP>"},
    {-9020321, "K<SUB>0</SUB><SUP>*</SUP>(1950)<SUP>-</SUP>"},
    {313, "K<SUP>*</SUP>(892)<SUP>0</SUP>"},
    {-313, "K<SUP>*</SUP>(892)<SUP>0</SUP>"},
    {323, "K<SUP>*</SUP>(892)<SUP>+</SUP>"},
    {-323, "K<SUP>*</SUP>(892)<SUP>-</SUP>"},
    {10313, "K<SUB>1</SUB>(1270)<SUP>0</SUP>"},
    {-10313, "K<SUB>1</SUB>(1270)<SUP>0</SUP>"},
    {10323, "K<SUB>1</SUB>(1270)<SUP>+</SUP>"},
    {-10323, "K<SUB>1</SUB>(1270)<SUP>-</SUP>"},
    {20313, "K<SUB>1</SUB>(1400)<SUP>0</SUP>"},
    {-20313, "K<SUB>1</SUB>(1400)<SUP>0</SUP>"},
    {20323, "K<SUB>1</SUB>(1400)<SUP>+</SUP>"},
    {-20323, "K<SUB>1</SUB>(1400)<SUP>-</SUP>"},
    {100313, "K<SUP>*</SUP>(1410)<SUP>0</SUP>"},
    {-100313, "K<SUP>*</SUP>(1410)<SUP>0</SUP>"},
    {100323, "K<SUP>*</SUP>(1410)<SUP>+</SUP>"},
    {-100323, "K<SUP>*</SUP>(1410)<SUP>-</SUP>"},
    {9000313, "K<SUB>1</SUB>(1650)<SUP>0</SUP>"},
    {-9000313, "K<SUB>1</SUB>(1650)<SUP>0</SUP>"},
    {9000323, "K<SUB>1</SUB>(1650)<SUP>+</SUP>"},
    {-9000323, "K<SUB>1</SUB>(1650)<SUP>-</SUP>"},
    {30313, "K<SUP>*</SUP>(1680)<SUP>0</SUP>"},
    {-30313, "K<SUP>*</SUP>(1680)<SUP>0</SUP>"},
    {30323, "K<SUP>*</SUP>(1680)<SUP>+</SUP>"},
    {-30323, "K<SUP>*</SUP>(1680)<SUP>-</SUP>"},
    {315, "K<SUB>2</SUB><SUP>*</SUP>(1430)<SUP>0</SUP>"},
    {-315, "K<SUB>2</SUB><SUP>*</SUP>(1430)<SUP>0</SUP>"},
    {325, "K<SUB>2</SUB><SUP>*</SUP>(1430)<SUP>+</SUP>"},
    {-325, "K<SUB>2</SUB><SUP>*</SUP>(1430)<SUP>-</SUP>"},
    {9000315, "K<SUB>2</SUB>(1580)<SUP>0</SUP>"},
    {-9000315, "K<SUB>2</SUB>(1580)<SUP>0</SUP>"},
    {9000325, "K<SUB>2</SUB>(1580)<SUP>+</SUP>"},
    {-9000325, "K<SUB>2</SUB>(1580)<SUP>-</SUP>"},
    {10315, "K<SUB>2</SUB>(1770)<SUP>0</SUP>"},
    {-10315, "K<SUB>2</SUB>(1770)<SUP>0</SUP>"},
    {10325, "K<SUB>2</SUB>(1770)<SUP>+</SUP>"},
    {-10325, "K<SUB>2</SUB>(1770)<SUP>-</SUP>"},
    {20315, "K<SUB>2</SUB>(1820)<SUP>0</SUP>"},
    {-20315, "K<SUB>2</SUB>(1820)<SUP>0</SUP>"},
    {20325, "K<SUB>2</SUB>(1820)<SUP>+</SUP>"},
    {-20325, "K<SUB>2</SUB>(1820)<SUP>-</SUP>"},
    {9010315, "K<SUB>2</SUB><SUP>*</SUP>(1980)<SUP>0</SUP>"},
    {-9010315, "K<SUB>2</SUB><SUP>*</SUP>(1980)<SUP>0</SUP>"},
    {9010325, "K<SUB>2</SUB><SUP>*</SUP>(1980)<SUP>+</SUP>"},
    {-9010325, "K<SUB>2</SUB><SUP>*</SUP>(1980)<SUP>-</SUP>"},
    {9020315, "K<SUB>2</SUB>(2250)<SUP>0</SUP>"},
    {-9020315, "K<SUB>2</SUB>(2250)<SUP>0</SUP>"},
    {9020325, "K<SUB>2</SUB>(2250)<SUP>+</SUP>"},
    {-9020325, "K<SUB>2</SUB>(2250)<SUP>-</SUP>"},
    {317, "K<SUB>3</SUB><SUP>*</SUP>(1780)<SUP>0</SUP>"},
    {-317, "K<SUB>3</SUB><SUP>*</SUP>(1780)<SUP>0</SUP>"},
    {327, "K<SUB>3</SUB><SUP>*</SUP>(1780)<SUP>+</SUP>"},
    {-327, "K<SUB>3</SUB><SUP>*</SUP>(1780)<SUP>-</SUP>"},
    {9010317, "K<SUB>3</SUB>(2320)<SUP>0</SUP>"},
    {-9010317, "K<SUB>3</SUB>(2320)<SUP>0</SUP>"},
    {9010327, "K<SUB>3</SUB>(2320)<SUP>+</SUP>"},
    {-9010327, "K<SUB>3</SUB>(2320)<SUP>-</SUP>"},
    {319, "K<SUB>4</SUB><SUP>*</SUP>(2045)<SUP>0</SUP>"},
    {-319, "K<SUB>4</SUB><SUP>*</SUP>(2045)<SUP>0</SUP>"},
    {329, "K<SUB>4</SUB><SUP>*</SUP>(2045)<SUP>+</SUP>"},
    {-329, "K<SUB>4</SUB><SUP>*</SUP>(2045)<SUP>-</SUP>"},
    {9000319, "K<SUB>4</SUB>(2500)<SUP>0</SUP>"},
    {-9000319, "K<SUB>4</SUB>(2500)<SUP>0</SUP>"},
    {9000329, "K<SUB>4</SUB>(2500)<SUP>+</SUP>"},
    {-9000329, "K<SUB>4</SUB>(2500)<SUP>-</SUP>"},


    //charmed mesons
    {411, "D<SUP>+</SUP>"},
    {-411, "D<SUP>-</SUP>"},
    {421, "D<SUP>0</SUP>"},
    {-421, "D<SUP>0</SUP>"},
    {10411, "D<SUB>0</SUB><SUP>*</SUP>(2400)<SUP>+</SUP>"},
    {-10411, "D<SUB>0</SUB><SUP>*</SUP>(2400)<SUP>-</SUP>"},
    {10421, "D<SUB>0</SUB><SUP>*</SUP>(2400)<SUP>0</SUP>"},
    {-10421, "D<SUB>0</SUB><SUP>*</SUP>(2400)<SUP>0</SUP>"},
    {413, "D<SUP>*</SUP>(2010)<SUP>+</SUP>"},
    {-413, "D<SUP>*</SUP>(2010)<SUP>-</SUP>"},
    {423, "D<SUP>*</SUP>(2007)<SUP>0</SUP>"},
    {-423, "D<SUP>*</SUP>(2007)<SUP>0</SUP>"},
    {10413, "D<SUB>1</SUB>(2420)<SUP>+</SUP>"},
    {-10413, "D<SUB>1</SUB>(2420)<SUP>-</SUP>"},
    {10423, "D<SUB>1</SUB>(2420)<SUP>0</SUP>"},
    {-10423, "D<SUB>1</SUB>(2420)<SUP>0</SUP>"},
    {20413, "D<SUB>1</SUB>(H)<SUP>+</SUP>"},
    {-20413, "D<SUB>1</SUB>(H)<SUP>-</SUP>"},
    {20423, "D<SUB>1</SUB>(2430)<SUP>0</SUP>"},
    {-20423, "D<SUB>1</SUB>(2430)<SUP>0</SUP>"},
    {415, "D<SUB>2</SUB><SUP>*</SUP>(2460)<SUP>+</SUP>"},
    {-415, "D<SUB>2</SUB><SUP>*</SUP>(2460)<SUP>-</SUP>"},
    {425, "D<SUB>2</SUB><SUP>*</SUP>(2460)<SUP>0</SUP>"},
    {-425, "D<SUB>2</SUB><SUP>*</SUP>(2460)<SUP>0</SUP>"},
    {431, "D<SUB>s</SUB><SUP>+</SUP>"},
    {-431, "D<SUB>s</SUB><SUP>-</SUP>"},
    {10431, "D<SUB>s0</SUB><SUP>*</SUP>(2317)<SUP>+</SUP>"},
    {-10431, "D<SUB>s0</SUB><SUP>*</SUP>(2317)<SUP>-</SUP>"},
    {433, "D<SUB>s</SUB><SUP>*+</SUP>"},
    {-433, "D<SUB>s</SUB><SUP>*-</SUP>"},
    {10433, "D<SUB>s1</SUB>(2536)<SUP>+</SUP>"},
    {-10433, "D<SUB>s1</SUB>(2536)<SUP>-</SUP>"},
    {20433, "D<SUB>s1</SUB>(2460)<SUP>+</SUP>"},
    {-20433, "D<SUB>s1</SUB>(2460)<SUP>-</SUP>"},
    {435, "D<SUB>s2</SUB><SUP>*</SUP>(2573)<SUP>+</SUP>"},
    {-435, "D<SUB>s2</SUB><SUP>*</SUP>(2573)<SUP>-</SUP>"},

    //bottom mesons
    {511, "B<SUP>0</SUP>"},
    {-511, "B<SUP>0</SUP>"},
    {521, "B<SUP>+</SUP>"},
    {-521, "B<SUP>-</SUP>"},
    {10511, "B<SUB>0</SUB><SUP>*0</SUP>"},
    {-10511, "B<SUB>0</SUB><SUP>*0</SUP>"},
    {10521, "B<SUB>0</SUB><SUP>*+</SUP>"},
    {-10521, "B<SUB>0</SUB><SUP>*-</SUP>"},
    {513, "B<SUP>*0</SUP>"},
    {-513, "B<SUP>*0</SUP>"},
    {523, "B<SUP>*+</SUP>"},
    {-523, "B<SUP>*-</SUP>"},
    {10513, "B<SUB>1</SUB>(L)<SUP>0</SUP>"},
    {-10513, "B<SUB>1</SUB>(L)<SUP>0</SUP>"},
    {10523, "B<SUB>1</SUB>(L)<SUP>+</SUP>"},
    {-10523, "B<SUB>1</SUB>(L)<SUP>-</SUP>"},
    {20513, "B<SUB>1</SUB>(H)<SUP>0</SUP>"},
    {-20513, "B<SUB>1</SUB>(H)<SUP>0</SUP>"},
    {20523, "B<SUB>1</SUB>(H)<SUP>+</SUP>"},
    {-20523, "B<SUB>1</SUB>(H)<SUP>-</SUP>"},
    {515, "B<SUB>2</SUB><SUP>*0</SUP>"},
    {-515, "B<SUB>2</SUB><SUP>*0</SUP>"},
    {525, "B<SUB>2</SUB><SUP>*+</SUP>"},
    {-525, "B<SUB>2</SUB><SUP>*-</SUP>"},
    {531, "B<SUB>s</SUB><SUP>0</SUP>"},
    {-531, "B<SUB>s</SUB><SUP>0</SUP>"},
    {10531, "B<SUB>s0</SUB><SUP>*0</SUP>"},
    {-10531, "B<SUB>s0</SUB><SUP>*0</SUP>"},
    {533, "B<SUB>s</SUB><SUP>*0</SUP>"},
    {-533, "B<SUB>s</SUB><SUP>*0</SUP>"},
    {10533, "B<SUB>s1</SUB>(L)<SUP>0</SUP>"},
    {-10533, "B<SUB>s1</SUB>(L)<SUP>0</SUP>"},
    {20533, "B<SUB>s1</SUB>(H)<SUP>0</SUP>"},
    {-20533, "B<SUB>s1</SUB>(H)<SUP>0</SUP>"},
    {535, "B<SUB>s2</SUB><SUP>*0</SUP>"},
    {-535, "B<SUB>s2</SUB><SUP>*0</SUP>"},
    {541, "B<SUB>c</SUB><SUP>+</SUP>"},
    {-541, "B<SUB>c</SUB><SUP>-</SUP>"},
    {10541, "B<SUB>c0</SUB><SUP>*+</SUP>"},
    {-10541, "B<SUB>c0</SUB><SUP>*-</SUP>"},
    {543, "B<SUB>c</SUB><SUP>*+</SUP>"},
    {-543, "B<SUB>c</SUB><SUP>*-</SUP>"},
    {10543, "B<SUB>c1</SUB>(L)<SUP>+</SUP>"},
    {-10543, "B<SUB>c1</SUB>(L)<SUP>-</SUP>"},
    {20543, "B<SUB>c1</SUB>(H)<SUP>+</SUP>"},
    {-20543, "B<SUB>c1</SUB>(H)<SUP>-</SUP>"},
    {545, "B<SUB>c2</SUB><SUP>+</SUP>"},
    {-545, "B<SUB>c2</SUB><SUP>-</SUP>"},

    //cc mesons
    {441, "&eta;<SUB>c</SUB>(1S)"},
    {-441, "&eta;<SUB>c</SUB>(1S)"},
    {10441, "&chi;<SUB>c0</SUB>(1P)"},
    {-10441, "&chi;<SUB>c0</SUB>(1P)"},
    {100441, "&eta;<SUB>c</SUB>(2S)"},
    {-100441, "&eta;<SUB>c</SUB>(2S)"},
    {443, "J/&psi;(1S)"},
    {-443, "J/&psi;(1S)"},
    {10443, "h<SUB>c</SUB>(1P)"},
    {-10443, "h<SUB>c</SUB>(1P)"},
    {20443, "&chi;<SUB>c1</SUB>(1P)"},
    {-20443, "&chi;<SUB>c1</SUB>(1P)"},
    {100443, "&psi;(2S)"},
    {-100443, "&psi;(2S)"},
    {30443, "&psi;(3770)"},
    {-30443, "&psi;(3770)"},
    {9000443, "&psi;(4040)"},
    {-9000443, "&psi;(4040)"},
    {9010443, "&psi;(4160)"},
    {-9010443, "&psi;(4160)"},
    {9020443, "&psi;(4415)"},
    {-9020443, "&psi;(4415)"},
    {445, "&chi;<SUB>c2</SUB>(1P)"},
    {-445, "&chi;<SUB>c2</SUB>(1P)"},
    {100445, "&chi;<SUB>c2</SUB>(2P)"},
    {-100445, "&chi;<SUB>c2</SUB>(2P)"},

    //light baryons
    {2212, "p<SUP>+</SUP>"},
    {-2212, "p<SUP>-</SUP>"},
    {2112, "n"},
    {-2112, "n"},
    {2224, "&Delta;<SUP>++</SUP>"},
    {-2224, "&Delta;<SUP>--</SUP>"},
    {2214, "&Delta;<SUP>+</SUP>"},
    {-2214, "&Delta;<SUP>+</SUP>"},
    {2114, "&Delta;<SUP>0</SUP>"},
    {-2114, "&Delta;<SUP>0</SUP>"},
    {1114, "&Delta;<SUP>-</SUP>"},
    {-1114, "&Delta;<SUP>-</SUP>"},
    //strange baryons
    {3122, "&Lambda;"},
    {-3122, "&Lambda;"},
    {3222, "&Sigma;<SUP>+</SUP>"},
    {-3222, "&Sigma;<SUP>+</SUP>"},
    {3212, "&Sigma;<SUP>0</SUP>"},
    {-3212, "&Sigma;<SUP>0</SUP>"},
    {3112, "&Sigma;<SUP>-</SUP>"},
    {-3112, "&Sigma;<SUP>-</SUP>"},
    {3224, "&Sigma;<SUP>*+</SUP>"},
    {-3224, "&Sigma;<SUP>*+</SUP>"},
    {3214, "&Sigma;<SUP>*0</SUP>"},
    {-3214, "&Sigma;<SUP>*0</SUP>"},
    {3114, "&Sigma;<SUP>*-</SUP>"},
    {-3114, "&Sigma;<SUP>*-</SUP>"},
    {3322, "&Xi;<SUP>0</SUP>"},
    {-3322, "&Xi;<SUP>0</SUP>"},
    {3312, "&Xi;<SUP>-</SUP>"},
    {-3312, "&Xi;<SUP>-</SUP>"},
    {3324, "&Xi;<SUP>*0</SUP>"},
    {-3324, "&Xi;<SUP>*0</SUP>"},
    {3314, "&Xi;<SUP>*-</SUP>"},
    {-3314, "&Xi;<SUP>*-</SUP>"},
    {3334, "&Omega;<SUP>-</SUP>"},
    {-3334, "&Omega;<SUP>-</SUP>"},
    //charmed baryons
    {4122, "&Lambda;<SUB>c</SUB><SUP>+</SUP>"},
    {-4122, "&Lambda;<SUB>c</SUB><SUP>-</SUP>"},
    {4222, "&Sigma;<SUB>c</SUB><SUP>++</SUP>"},
    {-4222, "&Sigma;<SUB>c</SUB><SUP>--</SUP>"},
    {4212, "&Sigma;<SUB>c</SUB><SUP>+</SUP>"},
    {-4212, "&Sigma;<SUB>c</SUB><SUP>-</SUP>"},
    {4112, "&Sigma;<SUB>c</SUB><SUP>0</SUP>"},
    {-4112, "&Sigma;<SUB>c</SUB><SUP>0</SUP>"},
    {4224, "&Sigma;<SUB>c</SUB><SUP>*++</SUP>"},
    {-4224, "&Sigma;<SUB>c</SUB><SUP>*--</SUP>"},
    {4214, "&Sigma;<SUB>c</SUB><SUP>*+</SUP>"},
    {-4214, "&Sigma;<SUB>c</SUB><SUP>*-</SUP>"},
    {4114, "&Sigma;<SUB>c</SUB><SUP>*0</SUP>"},
    {-4114, "&Sigma;<SUB>c</SUB><SUP>*0</SUP>"},
    {4232, "&Xi;<SUB>c</SUB><SUP>+</SUP>"},
    {-4232, "&Xi;<SUB>c</SUB><SUP>-</SUP>"},
    {4132, "&Xi;<SUB>c</SUB><SUP>0</SUP>"},
    {-4132, "&Xi;<SUB>c</SUB><SUP>0</SUP>"},
    {4322, "&Xi;<SUB>c</SUB><SUP>\'+</SUP>"},
    {-4322, "&Xi;<SUB>c</SUB><SUP>\'-</SUP>"},
    {4312, "&Xi;<SUB>c</SUB><SUP>\'0</SUP>"},
    {-4312, "&Xi;<SUB>c</SUB><SUP>\'0</SUP>"},
    {4324, "&Xi;<SUB>c</SUB><SUP>*+</SUP>"},
    {-4324, "&Xi;<SUB>c</SUB><SUP>*-</SUP>"},
    {4314, "&Xi;<SUB>c</SUB><SUP>*0</SUP>"},
    {-4314, "&Xi;<SUB>c</SUB><SUP>*0</SUP>"},
    {4332, "&Omega;<SUB>c</SUB><SUP>0</SUP>"},
    {-4332, "&Omega;<SUB>c</SUB><SUP>0</SUP>"},
    {4334, "&Omega;<SUB>c</SUB><SUP>*0</SUP>"},
    {-4334, "&Omega;<SUB>c</SUB><SUP>*0</SUP>"},
    {4412, "&Xi;<SUB>cc</SUB><SUP>+</SUP>"},
    {-4412, "&Xi;<SUB>cc</SUB><SUP>-</SUP>"},
    {4422, "&Xi;<SUB>cc</SUB><SUP>++</SUP>"},
    {-4422, "&Xi;<SUB>cc</SUB><SUP>--</SUP>"},
    {4414, "&Xi;<SUB>cc</SUB><SUP>*+</SUP>"},
    {-4414, "&Xi;<SUB>cc</SUB><SUP>*-</SUP>"},
    {4424, "&Xi;<SUB>cc</SUB><SUP>*++</SUP>"},
    {-4424, "&Xi;<SUB>cc</SUB><SUP>*--</SUP>"},
    {4432, "&Omega;<SUB>cc</SUB><SUP>+</SUP>"},
    {-4432, "&Omega;<SUB>cc</SUB><SUP>-</SUP>"},
    {4434, "&Omega;<SUB>cc</SUB><SUP>*+</SUP>"},
    {-4434, "&Omega;<SUB>cc</SUB><SUP>*-</SUP>"},
    {4444, "&Omega;<SUB>ccc</SUB><SUP>++</SUP>"},
    {-4444, "&Omega;<SUB>ccc</SUB><SUP>--</SUP>"},
    //bottom baryons
    {5122, "&Lambda;<SUB>b</SUB><SUP>0</SUP>"},
    {-5122, "&Lambda;<SUB>b</SUB><SUP>0</SUP>"},
    {5112, "&Sigma;<SUB>b</SUB><SUP>-</SUP>"},
    {-5112, "&Sigma;<SUB>b</SUB><SUP>-</SUP>"},
    {5212, "&Sigma;<SUB>b</SUB><SUP>0</SUP>"},
    {-5212, "&Sigma;<SUB>b</SUB><SUP>0</SUP>"},
    {5222, "&Sigma;<SUB>b</SUB><SUP>+</SUP>"},
    {-5222, "&Sigma;<SUB>b</SUB><SUP>+</SUP>"},
    {5114, "&Sigma;<SUB>b</SUB><SUP>*-</SUP>"},
    {-5114, "&Sigma;<SUB>b</SUB><SUP>*-</SUP>"},
    {5214, "&Sigma;<SUB>b</SUB><SUP>*0</SUP>"},
    {-5214, "&Sigma;<SUB>b</SUB><SUP>*0</SUP>"},
    {5224, "&Sigma;<SUB>b</SUB><SUP>*+</SUP>"},
    {-5224, "&Sigma;<SUB>b</SUB><SUP>*+</SUP>"},
    {5132, "&Xi;<SUB>b</SUB><SUP>-</SUP>"},
    {-5132, "&Xi;<SUB>b</SUB><SUP>-</SUP>"},
    {5232, "&Xi;<SUB>b</SUB><SUP>0</SUP>"},
    {-5232, "&Xi;<SUB>b</SUB><SUP>0</SUP>"},
    {5312, "&Xi;<SUB>b</SUB><SUP>\'-</SUP>"},
    {-5312, "&Xi;<SUB>b</SUB><SUP>\'-</SUP>"},
    {5322, "&Xi;<SUB>b</SUB><SUP>\'0</SUP>"},
    {-5322, "&Xi;<SUB>b</SUB><SUP>\'0</SUP>"},
    {5314, "&Xi;<SUB>b</SUB><SUP>*-</SUP>"},
    {-5314, "&Xi;<SUB>b</SUB><SUP>*-</SUP>"},
    {5324, "&Xi;<SUB>b</SUB><SUP>*0</SUP>"},
    {-5324, "&Xi;<SUB>b</SUB><SUP>*0</SUP>"},
    {5332, "&Omega;<SUB>b</SUB><SUP>-</SUP>"},
    {-5332, "&Omega;<SUB>b</SUB><SUP>-</SUP>"},
    {5334, "&Omega;<SUB>b</SUB><SUP>*-</SUP>"},
    {-5334, "&Omega;<SUB>b</SUB><SUP>*-</SUP>"},
    {5142, "&Xi;<SUB>bc</SUB><SUP>0</SUP>"},
    {-5142, "&Xi;<SUB>bc</SUB><SUP>0</SUP>"},
    {5242, "&Xi;<SUB>bc</SUB><SUP>+</SUP>"},
    {-5242, "&Xi;<SUB>bc</SUB><SUP>-</SUP>"},
    {5412, "&Xi;<SUB>bc</SUB><SUP>\'0</SUP>"},
    {-5412, "&Xi;<SUB>bc</SUB><SUP>\'0</SUP>"},
    {5422, "&Xi;<SUB>bc</SUB><SUP>\'+</SUP>"},
    {-5422, "&Xi;<SUB>bc</SUB><SUP>\'-</SUP>"},
    {5414, "&Xi;<SUB>bc</SUB><SUP>*0</SUP>"},
    {-5414, "&Xi;<SUB>bc</SUB><SUP>*0</SUP>"},
    {5424, "&Xi;<SUB>bc</SUB><SUP>*+</SUP>"},
    {-5424, "&Xi;<SUB>bc</SUB><SUP>*-</SUP>"},
    {5342, "&Omega;<SUB>bc</SUB><SUP>0</SUP>"},
    {-5342, "&Omega;<SUB>bc</SUB><SUP>0</SUP>"},
    {5432, "&Omega;<SUB>bc</SUB><SUP>\'0</SUP>"},
    {-5432, "&Omega;<SUB>bc</SUB><SUP>\'0</SUP>"},
    {5434, "&Omega;<SUB>bc</SUB><SUP>*0</SUP>"},
    {-5434, "&Omega;<SUB>bc</SUB><SUP>*0</SUP>"},
    {5442, "&Omega;<SUB>bcc</SUB><SUP>+</SUP>"},
    {-5442, "&Omega;<SUB>bcc</SUB><SUP>-</SUP>"},
    {5444, "&Omega;<SUB>bcc</SUB><SUP>*+</SUP>"},
    {-5444, "&Omega;<SUB>bcc</SUB><SUP>*-</SUP>"},
    {5512, "&Xi;<SUB>bb</SUB><SUP>-</SUP>"},
    {-5512, "&Xi;<SUB>bb</SUB><SUP>+</SUP>"},
    {5522, "&Xi;<SUB>bb</SUB><SUP>0</SUP>"},
    {-5522, "&Xi;<SUB>bb</SUB><SUP>0</SUP>"},
    {5514, "&Xi;<SUB>bb</SUB><SUP>*-</SUP>"},
    {-5514, "&Xi;<SUB>bb</SUB><SUP>*+</SUP>"},
    {5524, "&Xi;<SUB>bb</SUB><SUP>*0</SUP>"},
    {-5524, "&Xi;<SUB>bb</SUB><SUP>*0</SUP>"},
    {5532, "&Omega;<SUB>bb</SUB><SUP>-</SUP>"},
    {-5532, "&Omega;<SUB>bb</SUB><SUP>+</SUP>"},
    {5534, "&Omega;<SUB>bb</SUB><SUP>*-</SUP>"},
    {-5534, "&Omega;<SUB>bb</SUB><SUP>*+</SUP>"},
    {5542, "&Omega;<SUB>bbc</SUB><SUP>0</SUP>"},
    {-5542, "&Omega;<SUB>bbc</SUB><SUP>0</SUP>"},
    {5544, "&Omega;<SUB>bbc</SUB><SUP>*0</SUP>"},
    {-5544, "&Omega;<SUB>bbc</SUB><SUP>*0</SUP>"},
    {5554, "&Omega;<SUB>bbb</SUB><SUP>-</SUP>"},
    {-5554, "&Omega;<SUB>bbb</SUB><SUP>+</SUP>"}
};

bool isBeforeHadronisation(MCParticle* mc){
    if ( mc->getPDG() == 92 || mc->getPDG() == 91 || mc->getPDG() == 94 ) return true;
    for(auto daughter : mc->getDaughters() ){
        if ( isBeforeHadronisation(daughter) ) return true;
    }
    return false;
}

bool isAfterHadronization(EVENT::MCParticle* mc){
    if ( mc->getPDG() == 92 || mc->getPDG() == 91 || mc->getPDG() == 94 ) return true;
    const std::vector<MCParticle*> parents = mc->getParents();
    for(auto parent : parents){
        if ( isAfterHadronization(parent) ) return true;
    }
    return false;
}


void drawMcTree(LCEvent* event, bool drawSimulated, bool drawParton, bool drawOverlay){
    // initialize maps to store variables attached to the MC particles
    std::map<MCParticle*, int> p2idx;
    std::map<MCParticle*, bool> p2parton;

    const std::vector<std::string>* colNames = event->getCollectionNames();
    std::string mcColName;
    if ( std::find(colNames->begin(), colNames->end(), "MCParticle") != colNames->end() ){
        mcColName = "MCParticle";
    }
    else if ( std::find(colNames->begin(), colNames->end(), "MCParticlesSkimmed") != colNames->end() ){
        mcColName = "MCParticlesSkimmed";
    }
    else{
        std::cout<<"No MCParticles collection found"<<std::endl;
        return;
    }
    LCCollection* mcCol = event->getCollection(mcColName);

    //get interaction point
    MCParticle* mc0 = static_cast<MCParticle*> (mcCol->getElementAt(0));
    auto ip = mc0->getVertex();

    // Fill MCParticle pointer-to-variable maps
    for(int i=0; i< mcCol->getNumberOfElements(); ++i){
        MCParticle* mc = static_cast<MCParticle*> (mcCol->getElementAt(i));
        p2idx[mc] = i;
        p2parton[mc] = isBeforeHadronisation(mc);
    }

    //fill the stringstream content for the dot file
    std::stringstream nodes;
    std::stringstream labels;

    auto isRejected = [&](MCParticle* mc) ->  bool {
        bool rejected = false;
        if ( !drawSimulated && !drawOverlay && !drawParton ){
            //Ignore simulated and overlay particles or particles above hadronisation (excluding hadronisation string/cluster itself)
            rejected = mc->isCreatedInSimulation() || mc->isOverlay() || (p2parton[mc] && (!(mc->getPDG() == 91 || mc->getPDG() == 92)) );
        }
        else if( drawSimulated && !drawOverlay && !drawParton ){
            rejected = mc->isOverlay() || (p2parton[mc] && (!(mc->getPDG() == 91 || mc->getPDG() == 92)) );
        }
        else if( !drawSimulated && drawOverlay && !drawParton ){
            rejected = mc->isCreatedInSimulation() || (p2parton[mc] && (!(mc->getPDG() == 91 || mc->getPDG() == 92)) );
        }
        else if ( !drawSimulated && !drawOverlay && drawParton ){
            rejected = mc->isCreatedInSimulation() || mc->isOverlay();
        }
        else if ( drawSimulated && drawOverlay && !drawParton ){
            rejected = p2parton[mc] && (!(mc->getPDG() == 91 || mc->getPDG() == 92)) ;
        }
        else if ( drawSimulated && !drawOverlay && drawParton ){
            rejected = mc->isOverlay();
        }
        else if ( !drawSimulated && drawOverlay && drawParton ){
            rejected = mc->isCreatedInSimulation();
        }
        else{
            // identical to: if else ( drawSimulated && drawOverlay && drawParton ){
            rejected = false;
        }
        return rejected;
    };


    for(int i=0; i < mcCol->getNumberOfElements(); ++i){
        MCParticle* mc = static_cast<MCParticle*> (mcCol->getElementAt(i));
        if ( isRejected(mc) ) continue;

        //for each of this considered particles add daughters relations, excluding above-mentioned particles again
        for( auto daughter : mc->getDaughters() ){
            if ( isRejected(daughter) ) continue;
            nodes<<"    "<<p2idx[mc]<<"->"<<p2idx[daughter]<<";"<<std::endl;
        }

        int pdg = mc->getPDG();
        std::string label;
        if ( auto it = pdg2strMap.find(pdg); it != pdg2strMap.end() ){
            label = it->second;
        }
        else{
            label = std::to_string(pdg);
        }
        double dToIP = std::hypot( mc->getVertex()[0]-ip[0], mc->getVertex()[1]-ip[1], mc->getVertex()[2]-ip[2] );
        double pt = std::hypot( mc->getMomentum()[0], mc->getMomentum()[1] );
        double pz = mc->getMomentum()[2];
        labels<<p2idx[mc]<<"[label=<"<<label<<"<BR/>"<<std::fixed<<std::setprecision(2)<<dToIP<<" mm<BR/>"<<pt<<" | "<<pz<<" GeV"<<">";
        if ( p2parton[mc] ) labels<<" style=\"filled\" fillcolor=\"burlywood4\"";
        else if ( mc->isOverlay() && mc->isCreatedInSimulation() ) labels<<" style=\"filled\" fillcolor=\"darkorange4\"";
        else if ( mc->isOverlay() ) labels<<" style=\"filled\" fillcolor=\"dimgray\"";
        else if ( mc->isCreatedInSimulation() ) labels<<" style=\"filled\" fillcolor=\"darkorange\"";
        labels<<"];"<<std::endl;
    }

    //create dot file with a header
    std::ofstream outfile;
    outfile.open("dumpmctree_tmp.dot");
    outfile<<"digraph {"<<std::endl;
    outfile<<"    rankdir=TB;"<<std::endl;
    outfile<<nodes.str()<<std::endl;
    outfile<<labels.str()<<std::endl;
    outfile<<"}"<<std::endl;
}


constexpr static auto usage = R"USAGE(usage: dumpmctree [-s] [-p] [-o] filename eventNumber)USAGE";
constexpr static auto help = R"HELP(

OPTIONS:
    -s:             Draw particles created in simulation highlighted in orange

    -p:             Draw parton shower particles above pythia hadronisation string/cluster (pdg = 92/91).
                    E.g. (Z/W/H/quarks/gluons/etc). Highlighted in burlywood

    -o:             Draw overlay particles. Highlighted in gray


INPUT:
    filename:       input slcio file with an event to draw

    eventNumber:    event number to draw

)HELP";


/** dump MC table of the event from the slcio file in the neat graphviz tree representation.
*/
int main(int argc, char** argv ){
    //options (optional)
    bool drawSimulated = false;
    bool drawParton = false;
    bool drawOverlay = false;

    //inputs (required)
    char* filename ;
    int eventNumber{};

    if ( (argc < 3) ||
         (argv[argc-1] == NULL) || (argv[argc-2] == NULL) ||
         (argv[argc-1][0] == '-') || (argv[argc-2][0] == '-') ) {
        // there is NO input
        std::cerr << "Input error" << std::endl;
        std::cerr << usage << std::endl;
        std::cerr << help << std::endl;
        return 1;
    }
    else {
        // there is an input
        filename = argv[argc-2];
        eventNumber = atoi( argv[argc-1] );
    }

    // Retrieve the options:
    while ( true ){
        switch ( getopt(argc, argv, "spoh") ) {
            case 's':
                drawSimulated = true;
                continue;
            case 'p':
                drawParton = true;
                continue;
            case 'o':
                drawOverlay = true;
                continue;
            case '?':
                    std::cout << " Unknown option, please check your flags" << std::endl ;
                    [[fallthrough]];
            case 'h':
            default:
                std::cerr << usage << std::endl;
                std::cerr << help << std::endl;
                break;
            case -1:
                break;
        }
        break;
    }

    //print debug info
    std::cout << "Reading event " << eventNumber<< " from file " << std::endl << filename << std::endl;
    std::cout << "draw simulated: " << drawSimulated<<std::endl;
    std::cout << "draw parton: " << drawParton<<std::endl;
    std::cout << "draw overlay: " << drawOverlay<<std::endl;


    if( eventNumber < 1 ){
        std::cerr << " usage: event number must be a positive integer!" << std::endl ;        
        return 1;
    }

    LCReader* lcReader = LCFactory::getInstance()->createLCReader() ;
    lcReader->setReadCollectionNames({"MCParticle", "MCParticlesSkimmed"});

    LCEvent* event = nullptr;
    try{
        lcReader->open(filename);
        lcReader->skipNEvents(eventNumber - 1);
        event = lcReader->readNextEvent(); 
        if(!event){
            std::cerr << " File has less events than required " << eventNumber << std::endl;
            return 1;
        }

        // Do actuall drawing here
        drawMcTree(event, drawSimulated, drawParton, drawOverlay);
        lcReader->close();
    }
    catch( IOException& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}




