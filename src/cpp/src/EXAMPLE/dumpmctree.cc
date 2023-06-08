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

//UTILITY functions
std::map<int, std::string> getPdgNamesMap(){
    std::map<int, std::string> pdg2str;

    pdg2str[91] = "Cluster";
    pdg2str[92] = "Hadronization";
    pdg2str[94] = "Start pythia";

    // quarks
    pdg2str[1] = "d";
    pdg2str[-1] = "-d";
    pdg2str[2] = "u";
    pdg2str[-2] = "-u";
    pdg2str[3] = "s";
    pdg2str[-3] = "-s";
    pdg2str[4] = "c";
    pdg2str[-4] = "-c";
    pdg2str[5] = "b";
    pdg2str[-5] = "-b";
    pdg2str[6] = "t";
    pdg2str[-6] = "-t";
    pdg2str[7] = "b<SUP>'</SUP>";
    pdg2str[-7] = "-b<SUP>'</SUP>";
    pdg2str[8] = "t<SUP>'</SUP>";
    pdg2str[-8] = "-t<SUP>'</SUP>";
    pdg2str[9] = "g";

    // leptons
    pdg2str[11] = "e<SUP>-</SUP>";
    pdg2str[-11] = "e<SUP>+</SUP>";
    pdg2str[12] = "&nu;<SUB>e</SUB>";
    pdg2str[-12] = "&nu;<SUB>e</SUB>";
    pdg2str[13] = "&mu;<SUP>-</SUP>";
    pdg2str[-13] = "&mu;<SUP>+</SUP>";
    pdg2str[14] = "&nu;<SUB>&mu;</SUB>";
    pdg2str[-14] = "&nu;<SUB>&mu;</SUB>";
    pdg2str[15] = "&tau;<SUP>-</SUP>";
    pdg2str[-15] = "&tau;<SUP>+</SUP>";
    pdg2str[16] = "&nu;<SUB>&tau;</SUB>";
    pdg2str[-16] = "&nu;<SUB>&tau;</SUB>";
    //Gauge and Higgs Boson
    pdg2str[21] = "g";
    pdg2str[22] = "&gamma;";
    pdg2str[23] = "Z<SUP>0</SUP>";
    pdg2str[24] = "W<SUP>+</SUP>";
    pdg2str[-24] = "W<SUP>-</SUP>";
    pdg2str[25] = "H<SUP>0</SUP>";
    // Higgs?
    // light mesons I=1
    pdg2str[111] = "&pi;<SUP>0</SUP>";
    pdg2str[-111] = "&pi;<SUP>0</SUP>";
    pdg2str[211] = "&pi;<SUP>+</SUP>";
    pdg2str[-211] = "&pi;<SUP>-</SUP>";
    pdg2str[9000111] = "a<SUB>0</SUB>(980)<SUP>0</SUP>";
    pdg2str[-9000111] = "a<SUB>0</SUB>(980)<SUP>0</SUP>";
    pdg2str[9000211] = "a<SUB>0</SUB>(980)<SUP>+</SUP>";
    pdg2str[-9000211] = "a<SUB>0</SUB>(980)<SUP>-</SUP>";
    pdg2str[100111] = "&pi;(1300)<SUP>0</SUP>";
    pdg2str[-100111] = "&pi;(1300)<SUP>0</SUP>";
    pdg2str[100211] = "&pi;(1300)<SUP>+</SUP>";
    pdg2str[-100211] = "&pi;(1300)<SUP>-</SUP>";
    pdg2str[10111] = "a<SUB>0</SUB>(1450)<SUP>0</SUP>";
    pdg2str[-10111] = "a<SUB>0</SUB>(1450)<SUP>0</SUP>";
    pdg2str[10211] = "a<SUB>0</SUB>(1450)<SUP>+</SUP>";
    pdg2str[-10211] = "a<SUB>0</SUB>(1450)<SUP>-</SUP>";
    pdg2str[9010111] = "&pi;(1800)<SUP>0</SUP>";
    pdg2str[-9010111] = "&pi;(1800)<SUP>0</SUP>";
    pdg2str[9010211] = "&pi;(1800)<SUP>+</SUP>";
    pdg2str[-9010211] = "&pi;(1800)<SUP>-</SUP>";
    pdg2str[113] = "&rho;(770)<SUP>0</SUP>";
    pdg2str[-113] = "&rho;(770)<SUP>0</SUP>";
    pdg2str[213] = "&rho;(770)<SUP>+</SUP>";
    pdg2str[-213] = "&rho;(770)<SUP>-</SUP>";
    pdg2str[10113] = "b<SUB>1</SUB>(1235)<SUP>0</SUP>";
    pdg2str[-10113] = "b<SUB>1</SUB>(1235)<SUP>0</SUP>";
    pdg2str[10213] = "b<SUB>1</SUB>(1235)<SUP>+</SUP>";
    pdg2str[-10213] = "b<SUB>1</SUB>(1235)<SUP>-</SUP>";
    pdg2str[20113] = "a<SUB>1</SUB>(1260)<SUP>0</SUP>";
    pdg2str[-20113] = "a<SUB>1</SUB>(1260)<SUP>0</SUP>";
    pdg2str[20213] = "a<SUB>1</SUB>(1260)<SUP>+</SUP>";
    pdg2str[-20213] = "a<SUB>1</SUB>(1260)<SUP>-</SUP>";
    pdg2str[9000113] = "&pi;<SUB>1</SUB>(1400)<SUP>0</SUP>";
    pdg2str[-9000113] = "&pi;<SUB>1</SUB>(1400)<SUP>0</SUP>";
    pdg2str[9000213] = "&pi;<SUB>1</SUB>(1400)<SUP>+</SUP>";
    pdg2str[-9000213] = "&pi;<SUB>1</SUB>(1400)<SUP>-</SUP>";
    pdg2str[100113] = "&rho;(1450)<SUP>0</SUP>";
    pdg2str[-100113] = "&rho;(1450)<SUP>0</SUP>";
    pdg2str[100213] = "&rho;(1450)<SUP>+</SUP>";
    pdg2str[-100213] = "&rho;(1450)<SUP>-</SUP>";
    pdg2str[9010113] = "&pi;<SUB>1</SUB>(1600)<SUP>0</SUP>";
    pdg2str[-9010113] = "&pi;<SUB>1</SUB>(1600)<SUP>0</SUP>";
    pdg2str[9010213] = "&pi;<SUB>1</SUB>(1600)<SUP>+</SUP>";
    pdg2str[-9010213] = "&pi;<SUB>1</SUB>(1600)<SUP>-</SUP>";
    pdg2str[9020113] = "a<SUB>1</SUB>(1640)<SUP>0</SUP>";
    pdg2str[-9020113] = "a<SUB>1</SUB>(1640)<SUP>0</SUP>";
    pdg2str[9020213] = "a<SUB>1</SUB>(1640)<SUP>+</SUP>";
    pdg2str[-9020213] = "a<SUB>1</SUB>(1640)<SUP>-</SUP>";
    pdg2str[30113] = "&rho;(1700)<SUP>0</SUP>";
    pdg2str[-30113] = "&rho;(1700)<SUP>0</SUP>";
    pdg2str[30213] = "&rho;(1700)<SUP>+</SUP>";
    pdg2str[-30213] = "&rho;(1700)<SUP>-</SUP>";
    pdg2str[9030113] = "&rho;(1900)<SUP>0</SUP>";
    pdg2str[-9030113] = "&rho;(1900)<SUP>0</SUP>";
    pdg2str[9030213] = "&rho;(1900)<SUP>+</SUP>";
    pdg2str[-9030213] = "&rho;(1900)<SUP>-</SUP>";
    pdg2str[9040113] = "&rho;(2150)<SUP>0</SUP>";
    pdg2str[-9040113] = "&rho;(2150)<SUP>0</SUP>";
    pdg2str[9040213] = "&rho;(2150)<SUP>+</SUP>";
    pdg2str[-9040213] = "&rho;(2150)<SUP>-</SUP>";
    pdg2str[115] = "a<SUB>2</SUB>(1320)<SUP>0</SUP>";
    pdg2str[-115] = "a<SUB>2</SUB>(1320)<SUP>0</SUP>";
    pdg2str[215] = "a<SUB>2</SUB>(1320)<SUP>+</SUP>";
    pdg2str[-215] = "a<SUB>2</SUB>(1320)<SUP>-</SUP>";
    pdg2str[10115] = "&pi;<SUB>2</SUB>(1670)<SUP>0</SUP>";
    pdg2str[-10115] = "&pi;<SUB>2</SUB>(1670)<SUP>0</SUP>";
    pdg2str[10215] = "&pi;<SUB>2</SUB>(1670)<SUP>+</SUP>";
    pdg2str[-10215] = "&pi;<SUB>2</SUB>(1670)<SUP>-</SUP>";
    pdg2str[9000115] = "a<SUB>2</SUB>(1700)<SUP>0</SUP>";
    pdg2str[-9000115] = "a<SUB>2</SUB>(1700)<SUP>0</SUP>";
    pdg2str[9000215] = "a<SUB>2</SUB>(1700)<SUP>+</SUP>";
    pdg2str[-9000215] = "a<SUB>2</SUB>(1700)<SUP>-</SUP>";
    pdg2str[9010115] = "&pi;<SUB>2</SUB>(2100)<SUP>0</SUP>";
    pdg2str[-9010115] = "&pi;<SUB>2</SUB>(2100)<SUP>0</SUP>";
    pdg2str[9010215] = "&pi;<SUB>2</SUB>(2100)<SUP>+</SUP>";
    pdg2str[-9010215] = "&pi;<SUB>2</SUB>(2100)<SUP>-</SUP>";
    pdg2str[117] = "&rho;<SUB>3</SUB>(1690)<SUP>0</SUP>";
    pdg2str[-117] = "&rho;<SUB>3</SUB>(1690)<SUP>0</SUP>";
    pdg2str[217] = "&rho;<SUB>3</SUB>(1690)<SUP>+</SUP>";
    pdg2str[-217] = "&rho;<SUB>3</SUB>(1690)<SUP>-</SUP>";
    pdg2str[9000117] = "&rho;<SUB>3</SUB>(1990)<SUP>0</SUP>";
    pdg2str[-9000117] = "&rho;<SUB>3</SUB>(1990)<SUP>0</SUP>";
    pdg2str[9000217] = "&rho;<SUB>3</SUB>(1990)<SUP>+</SUP>";
    pdg2str[-9000217] = "&rho;<SUB>3</SUB>(1990)<SUP>-</SUP>";
    pdg2str[9010117] = "&rho;<SUB>3</SUB>(2250)<SUP>0</SUP>";
    pdg2str[-9010117] = "&rho;<SUB>3</SUB>(2250)<SUP>0</SUP>";
    pdg2str[9010217] = "&rho;<SUB>3</SUB>(2250)<SUP>+</SUP>";
    pdg2str[-9010217] = "&rho;<SUB>3</SUB>(2250)<SUP>-</SUP>";
    pdg2str[119] = "a<SUB>4</SUB>(2040)<SUP>0</SUP>";
    pdg2str[-119] = "a<SUB>4</SUB>(2040)<SUP>0</SUP>";
    pdg2str[219] = "a<SUB>4</SUB>(2040)<SUP>+</SUP>";
    pdg2str[-219] = "a<SUB>4</SUB>(2040)<SUP>-</SUP>";


    // light mesons I=0
    pdg2str[221] = "&eta;";
    pdg2str[-221] = "&eta;";
    pdg2str[331] = "&eta;\'";
    pdg2str[-331] = "&eta;\'";
    pdg2str[9000221] = "f<SUB>0</SUB>(600)";
    pdg2str[-9000221] = "f<SUB>0</SUB>(600)";
    pdg2str[9010221] = "f<SUB>0</SUB>(980)";
    pdg2str[-9010221] = "f<SUB>0</SUB>(980)";
    pdg2str[100221] = "&eta;(1295)";
    pdg2str[-100221] = "&eta;(1295)";
    pdg2str[10221] = "f<SUB>0</SUB>(1370)";
    pdg2str[-10221] = "f<SUB>0</SUB>(1370)";
    pdg2str[9020221] = "&eta;(1405)";
    pdg2str[-9020221] = "&eta;(1405)";
    pdg2str[100331] = "&eta;(1475)";
    pdg2str[-100331] = "&eta;(1475)";
    pdg2str[9030221] = "f<SUB>0</SUB>(1500)";
    pdg2str[-9030221] = "f<SUB>0</SUB>(1500)";
    pdg2str[10331] = "f<SUB>0</SUB>(1710)";
    pdg2str[-10331] = "f<SUB>0</SUB>(1710)";
    pdg2str[9040221] = "&eta;(1760)";
    pdg2str[-9040221] = "&eta;(1760)";
    pdg2str[9050221] = "f<SUB>0</SUB>(2020)";
    pdg2str[-9050221] = "f<SUB>0</SUB>(2020)";
    pdg2str[9060221] = "f<SUB>0</SUB>(2100)";
    pdg2str[-9060221] = "f<SUB>0</SUB>(2100)";
    pdg2str[9070221] = "f<SUB>0</SUB>(2200)";
    pdg2str[-9070221] = "f<SUB>0</SUB>(2200)";
    pdg2str[9080221] = "&eta;(2225)";
    pdg2str[-9080221] = "&eta;(2225)";
    pdg2str[223] = "&omega;";
    pdg2str[-223] = "&omega;";
    pdg2str[333] = "&phi;";
    pdg2str[-333] = "&phi;";
    pdg2str[10223] = "h<SUB>1</SUB>(1170)";
    pdg2str[-10223] = "h<SUB>1</SUB>(1170)";
    pdg2str[20223] = "f<SUB>1</SUB>(1285)";
    pdg2str[-20223] = "f<SUB>1</SUB>(1285)";
    pdg2str[10333] = "h<SUB>1</SUB>(1380)";
    pdg2str[-10333] = "h<SUB>1</SUB>(1380)";
    pdg2str[20333] = "f<SUB>1</SUB>(1420)";
    pdg2str[-20333] = "f<SUB>1</SUB>(1420)";
    pdg2str[100223] = "&omega;(1420)";
    pdg2str[-100223] = "&omega;(1420)";
    pdg2str[9000223] = "f<SUB>1</SUB>(1510)";
    pdg2str[-9000223] = "f<SUB>1</SUB>(1510)";
    pdg2str[9010223] = "h<SUB>1</SUB>(1595)";
    pdg2str[-9010223] = "h<SUB>1</SUB>(1595)";
    pdg2str[30223] = "&omega;(1650)";
    pdg2str[-30223] = "&omega;(1650)";
    pdg2str[100333] = "&phi;(1680)";
    pdg2str[-100333] = "&phi;(1680)";
    pdg2str[225] = "f<SUB>2</SUB>(1270)";
    pdg2str[-225] = "f<SUB>2</SUB>(1270)";
    pdg2str[9000225] = "f<SUB>2</SUB>(1430)";
    pdg2str[-9000225] = "f<SUB>2</SUB>(1430)";
    pdg2str[335] = "f<SUB>2</SUB>\'(1525)";
    pdg2str[-335] = "f<SUB>2</SUB>\'(1525)";
    pdg2str[9010225] = "f<SUB>2</SUB>(1565)";
    pdg2str[-9010225] = "f<SUB>2</SUB>(1565)";
    pdg2str[9020225] = "f<SUB>2</SUB>(1640)";
    pdg2str[-9020225] = "f<SUB>2</SUB>(1640)";
    pdg2str[10225] = "&eta;<SUB>2</SUB>(1645)";
    pdg2str[-10225] = "&eta;<SUB>2</SUB>(1645)";
    pdg2str[9030225] = "f<SUB>2</SUB>(1810)";
    pdg2str[-9030225] = "f<SUB>2</SUB>(1810)";
    pdg2str[10335] = "&eta;<SUB>2</SUB>(1870)";
    pdg2str[-10335] = "&eta;<SUB>2</SUB>(1870)";
    pdg2str[9040225] = "f<SUB>2</SUB>(1910)";
    pdg2str[-9040225] = "f<SUB>2</SUB>(1910)";
    pdg2str[9050225] = "f<SUB>2</SUB>(1950)";
    pdg2str[-9050225] = "f<SUB>2</SUB>(1950)";
    pdg2str[9060225] = "f<SUB>2</SUB>(2010)";
    pdg2str[-9060225] = "f<SUB>2</SUB>(2010)";
    pdg2str[9070225] = "f<SUB>2</SUB>(2150)";
    pdg2str[-9070225] = "f<SUB>2</SUB>(2150)";
    pdg2str[9080225] = "f<SUB>2</SUB>(2300)";
    pdg2str[-9080225] = "f<SUB>2</SUB>(2300)";
    pdg2str[9090225] = "f<SUB>2</SUB>(2340)";
    pdg2str[-9090225] = "f<SUB>2</SUB>(2340)";
    pdg2str[227] = "&omega;<SUB>3</SUB>(1670)";
    pdg2str[-227] = "&omega;<SUB>3</SUB>(1670)";
    pdg2str[337] = "&phi;<SUB>3</SUB>(1850)";
    pdg2str[-337] = "&phi;<SUB>3</SUB>(1850)";
    pdg2str[229] = "f<SUB>4</SUB>(2050)";
    pdg2str[-229] = "f<SUB>4</SUB>(2050)";
    pdg2str[9000229] = "f<SUB>J</SUB>(2220)";
    pdg2str[-9000229] = "f<SUB>J</SUB>(2220)";
    pdg2str[9010229] = "f<SUB>4</SUB>(2300)";
    pdg2str[-9010229] = "f<SUB>4</SUB>(2300)";

    // strange mesons
    pdg2str[130] = "K<SUB>L</SUB><SUP>0</SUP>";
    pdg2str[-130] = "K<SUB>L</SUB><SUP>0</SUP>";
    pdg2str[310] = "K<SUB>S</SUB><SUP>0</SUP>";
    pdg2str[-310] = "K<SUB>S</SUB><SUP>0</SUP>";
    pdg2str[311] = "K<SUP>0</SUP>";
    pdg2str[-311] = "K<SUP>0</SUP>";
    pdg2str[321] = "K<SUP>+</SUP>";
    pdg2str[-321] = "K<SUP>-</SUP>";
    pdg2str[9000311] = "K<SUB>0</SUB><SUP>*</SUP>(800)<SUP>0</SUP>";
    pdg2str[-9000311] = "K<SUB>0</SUB><SUP>*</SUP>(800)<SUP>0</SUP>";
    pdg2str[9000321] = "K<SUB>0</SUB><SUP>*</SUP>(800)<SUP>+</SUP>";
    pdg2str[-9000321] = "K<SUB>0</SUB><SUP>*</SUP>(800)<SUP>-</SUP>";
    pdg2str[10311] = "K<SUB>0</SUB><SUP>*</SUP>(1430)<SUP>0</SUP>";
    pdg2str[-10311] = "K<SUB>0</SUB><SUP>*</SUP>(1430)<SUP>0</SUP>";
    pdg2str[10321] = "K<SUB>0</SUB><SUP>*</SUP>(1430)<SUP>+</SUP>";
    pdg2str[-10321] = "K<SUB>0</SUB><SUP>*</SUP>(1430)<SUP>-</SUP>";
    pdg2str[100311] = "K(1460)<SUP>0</SUP>";
    pdg2str[-100311] = "K(1460)<SUP>0</SUP>";
    pdg2str[100321] = "K(1460)<SUP>+</SUP>";
    pdg2str[-100321] = "K(1460)<SUP>-</SUP>";
    pdg2str[9010311] = "K(1830)<SUP>0</SUP>";
    pdg2str[-9010311] = "K(1830)<SUP>0</SUP>";
    pdg2str[9010321] = "K(1830)<SUP>+</SUP>";
    pdg2str[-9010321] = "K(1830)<SUP>-</SUP>";
    pdg2str[9020311] = "K<SUB>0</SUB><SUP>*</SUP>(1950)<SUP>0</SUP>";
    pdg2str[-9020311] = "K<SUB>0</SUB><SUP>*</SUP>(1950)<SUP>0</SUP>";
    pdg2str[9020321] = "K<SUB>0</SUB><SUP>*</SUP>(1950)<SUP>+</SUP>";
    pdg2str[-9020321] = "K<SUB>0</SUB><SUP>*</SUP>(1950)<SUP>-</SUP>";
    pdg2str[313] = "K<SUP>*</SUP>(892)<SUP>0</SUP>";
    pdg2str[-313] = "K<SUP>*</SUP>(892)<SUP>0</SUP>";
    pdg2str[323] = "K<SUP>*</SUP>(892)<SUP>+</SUP>";
    pdg2str[-323] = "K<SUP>*</SUP>(892)<SUP>-</SUP>";
    pdg2str[10313] = "K<SUB>1</SUB>(1270)<SUP>0</SUP>";
    pdg2str[-10313] = "K<SUB>1</SUB>(1270)<SUP>0</SUP>";
    pdg2str[10323] = "K<SUB>1</SUB>(1270)<SUP>+</SUP>";
    pdg2str[-10323] = "K<SUB>1</SUB>(1270)<SUP>-</SUP>";
    pdg2str[20313] = "K<SUB>1</SUB>(1400)<SUP>0</SUP>";
    pdg2str[-20313] = "K<SUB>1</SUB>(1400)<SUP>0</SUP>";
    pdg2str[20323] = "K<SUB>1</SUB>(1400)<SUP>+</SUP>";
    pdg2str[-20323] = "K<SUB>1</SUB>(1400)<SUP>-</SUP>";
    pdg2str[100313] = "K<SUP>*</SUP>(1410)<SUP>0</SUP>";
    pdg2str[-100313] = "K<SUP>*</SUP>(1410)<SUP>0</SUP>";
    pdg2str[100323] = "K<SUP>*</SUP>(1410)<SUP>+</SUP>";
    pdg2str[-100323] = "K<SUP>*</SUP>(1410)<SUP>-</SUP>";
    pdg2str[9000313] = "K<SUB>1</SUB>(1650)<SUP>0</SUP>";
    pdg2str[-9000313] = "K<SUB>1</SUB>(1650)<SUP>0</SUP>";
    pdg2str[9000323] = "K<SUB>1</SUB>(1650)<SUP>+</SUP>";
    pdg2str[-9000323] = "K<SUB>1</SUB>(1650)<SUP>-</SUP>";
    pdg2str[30313] = "K<SUP>*</SUP>(1680)<SUP>0</SUP>";
    pdg2str[-30313] = "K<SUP>*</SUP>(1680)<SUP>0</SUP>";
    pdg2str[30323] = "K<SUP>*</SUP>(1680)<SUP>+</SUP>";
    pdg2str[-30323] = "K<SUP>*</SUP>(1680)<SUP>-</SUP>";
    pdg2str[315] = "K<SUB>2</SUB><SUP>*</SUP>(1430)<SUP>0</SUP>";
    pdg2str[-315] = "K<SUB>2</SUB><SUP>*</SUP>(1430)<SUP>0</SUP>";
    pdg2str[325] = "K<SUB>2</SUB><SUP>*</SUP>(1430)<SUP>+</SUP>";
    pdg2str[-325] = "K<SUB>2</SUB><SUP>*</SUP>(1430)<SUP>-</SUP>";
    pdg2str[9000315] = "K<SUB>2</SUB>(1580)<SUP>0</SUP>";
    pdg2str[-9000315] = "K<SUB>2</SUB>(1580)<SUP>0</SUP>";
    pdg2str[9000325] = "K<SUB>2</SUB>(1580)<SUP>+</SUP>";
    pdg2str[-9000325] = "K<SUB>2</SUB>(1580)<SUP>-</SUP>";
    pdg2str[10315] = "K<SUB>2</SUB>(1770)<SUP>0</SUP>";
    pdg2str[-10315] = "K<SUB>2</SUB>(1770)<SUP>0</SUP>";
    pdg2str[10325] = "K<SUB>2</SUB>(1770)<SUP>+</SUP>";
    pdg2str[-10325] = "K<SUB>2</SUB>(1770)<SUP>-</SUP>";
    pdg2str[20315] = "K<SUB>2</SUB>(1820)<SUP>0</SUP>";
    pdg2str[-20315] = "K<SUB>2</SUB>(1820)<SUP>0</SUP>";
    pdg2str[20325] = "K<SUB>2</SUB>(1820)<SUP>+</SUP>";
    pdg2str[-20325] = "K<SUB>2</SUB>(1820)<SUP>-</SUP>";
    pdg2str[9010315] = "K<SUB>2</SUB><SUP>*</SUP>(1980)<SUP>0</SUP>";
    pdg2str[-9010315] = "K<SUB>2</SUB><SUP>*</SUP>(1980)<SUP>0</SUP>";
    pdg2str[9010325] = "K<SUB>2</SUB><SUP>*</SUP>(1980)<SUP>+</SUP>";
    pdg2str[-9010325] = "K<SUB>2</SUB><SUP>*</SUP>(1980)<SUP>-</SUP>";
    pdg2str[9020315] = "K<SUB>2</SUB>(2250)<SUP>0</SUP>";
    pdg2str[-9020315] = "K<SUB>2</SUB>(2250)<SUP>0</SUP>";
    pdg2str[9020325] = "K<SUB>2</SUB>(2250)<SUP>+</SUP>";
    pdg2str[-9020325] = "K<SUB>2</SUB>(2250)<SUP>-</SUP>";
    pdg2str[317] = "K<SUB>3</SUB><SUP>*</SUP>(1780)<SUP>0</SUP>";
    pdg2str[-317] = "K<SUB>3</SUB><SUP>*</SUP>(1780)<SUP>0</SUP>";
    pdg2str[327] = "K<SUB>3</SUB><SUP>*</SUP>(1780)<SUP>+</SUP>";
    pdg2str[-327] = "K<SUB>3</SUB><SUP>*</SUP>(1780)<SUP>-</SUP>";
    pdg2str[9010317] = "K<SUB>3</SUB>(2320)<SUP>0</SUP>";
    pdg2str[-9010317] = "K<SUB>3</SUB>(2320)<SUP>0</SUP>";
    pdg2str[9010327] = "K<SUB>3</SUB>(2320)<SUP>+</SUP>";
    pdg2str[-9010327] = "K<SUB>3</SUB>(2320)<SUP>-</SUP>";
    pdg2str[319] = "K<SUB>4</SUB><SUP>*</SUP>(2045)<SUP>0</SUP>";
    pdg2str[-319] = "K<SUB>4</SUB><SUP>*</SUP>(2045)<SUP>0</SUP>";
    pdg2str[329] = "K<SUB>4</SUB><SUP>*</SUP>(2045)<SUP>+</SUP>";
    pdg2str[-329] = "K<SUB>4</SUB><SUP>*</SUP>(2045)<SUP>-</SUP>";
    pdg2str[9000319] = "K<SUB>4</SUB>(2500)<SUP>0</SUP>";
    pdg2str[-9000319] = "K<SUB>4</SUB>(2500)<SUP>0</SUP>";
    pdg2str[9000329] = "K<SUB>4</SUB>(2500)<SUP>+</SUP>";
    pdg2str[-9000329] = "K<SUB>4</SUB>(2500)<SUP>-</SUP>";


    //charmed mesons
    pdg2str[411] = "D<SUP>+</SUP>";
    pdg2str[-411] = "D<SUP>-</SUP>";
    pdg2str[421] = "D<SUP>0</SUP>";
    pdg2str[-421] = "D<SUP>0</SUP>";
    pdg2str[10411] = "D<SUB>0</SUB><SUP>*</SUP>(2400)<SUP>+</SUP>";
    pdg2str[-10411] = "D<SUB>0</SUB><SUP>*</SUP>(2400)<SUP>-</SUP>";
    pdg2str[10421] = "D<SUB>0</SUB><SUP>*</SUP>(2400)<SUP>0</SUP>";
    pdg2str[-10421] = "D<SUB>0</SUB><SUP>*</SUP>(2400)<SUP>0</SUP>";
    pdg2str[413] = "D<SUP>*</SUP>(2010)<SUP>+</SUP>";
    pdg2str[-413] = "D<SUP>*</SUP>(2010)<SUP>-</SUP>";
    pdg2str[423] = "D<SUP>*</SUP>(2007)<SUP>0</SUP>";
    pdg2str[-423] = "D<SUP>*</SUP>(2007)<SUP>0</SUP>";
    pdg2str[10413] = "D<SUB>1</SUB>(2420)<SUP>+</SUP>";
    pdg2str[-10413] = "D<SUB>1</SUB>(2420)<SUP>-</SUP>";
    pdg2str[10423] = "D<SUB>1</SUB>(2420)<SUP>0</SUP>";
    pdg2str[-10423] = "D<SUB>1</SUB>(2420)<SUP>0</SUP>";
    pdg2str[20413] = "D<SUB>1</SUB>(H)<SUP>+</SUP>";
    pdg2str[-20413] = "D<SUB>1</SUB>(H)<SUP>-</SUP>";
    pdg2str[20423] = "D<SUB>1</SUB>(2430)<SUP>0</SUP>";
    pdg2str[-20423] = "D<SUB>1</SUB>(2430)<SUP>0</SUP>";
    pdg2str[415] = "D<SUB>2</SUB><SUP>*</SUP>(2460)<SUP>+</SUP>";
    pdg2str[-415] = "D<SUB>2</SUB><SUP>*</SUP>(2460)<SUP>-</SUP>";
    pdg2str[425] = "D<SUB>2</SUB><SUP>*</SUP>(2460)<SUP>0</SUP>";
    pdg2str[-425] = "D<SUB>2</SUB><SUP>*</SUP>(2460)<SUP>0</SUP>";
    pdg2str[431] = "D<SUB>s</SUB><SUP>+</SUP>";
    pdg2str[-431] = "D<SUB>s</SUB><SUP>-</SUP>";
    pdg2str[10431] = "D<SUB>s0</SUB><SUP>*</SUP>(2317)<SUP>+</SUP>";
    pdg2str[-10431] = "D<SUB>s0</SUB><SUP>*</SUP>(2317)<SUP>-</SUP>";
    pdg2str[433] = "D<SUB>s</SUB><SUP>*+</SUP>";
    pdg2str[-433] = "D<SUB>s</SUB><SUP>*-</SUP>";
    pdg2str[10433] = "D<SUB>s1</SUB>(2536)<SUP>+</SUP>";
    pdg2str[-10433] = "D<SUB>s1</SUB>(2536)<SUP>-</SUP>";
    pdg2str[20433] = "D<SUB>s1</SUB>(2460)<SUP>+</SUP>";
    pdg2str[-20433] = "D<SUB>s1</SUB>(2460)<SUP>-</SUP>";
    pdg2str[435] = "D<SUB>s2</SUB><SUP>*</SUP>(2573)<SUP>+</SUP>";
    pdg2str[-435] = "D<SUB>s2</SUB><SUP>*</SUP>(2573)<SUP>-</SUP>";

    //bottom mesons
    pdg2str[511] = "B<SUP>0</SUP>";
    pdg2str[-511] = "B<SUP>0</SUP>";
    pdg2str[521] = "B<SUP>+</SUP>";
    pdg2str[-521] = "B<SUP>-</SUP>";
    pdg2str[10511] = "B<SUB>0</SUB><SUP>*0</SUP>";
    pdg2str[-10511] = "B<SUB>0</SUB><SUP>*0</SUP>";
    pdg2str[10521] = "B<SUB>0</SUB><SUP>*+</SUP>";
    pdg2str[-10521] = "B<SUB>0</SUB><SUP>*-</SUP>";
    pdg2str[513] = "B<SUP>*0</SUP>";
    pdg2str[-513] = "B<SUP>*0</SUP>";
    pdg2str[523] = "B<SUP>*+</SUP>";
    pdg2str[-523] = "B<SUP>*-</SUP>";
    pdg2str[10513] = "B<SUB>1</SUB>(L)<SUP>0</SUP>";
    pdg2str[-10513] = "B<SUB>1</SUB>(L)<SUP>0</SUP>";
    pdg2str[10523] = "B<SUB>1</SUB>(L)<SUP>+</SUP>";
    pdg2str[-10523] = "B<SUB>1</SUB>(L)<SUP>-</SUP>";
    pdg2str[20513] = "B<SUB>1</SUB>(H)<SUP>0</SUP>";
    pdg2str[-20513] = "B<SUB>1</SUB>(H)<SUP>0</SUP>";
    pdg2str[20523] = "B<SUB>1</SUB>(H)<SUP>+</SUP>";
    pdg2str[-20523] = "B<SUB>1</SUB>(H)<SUP>-</SUP>";
    pdg2str[515] = "B<SUB>2</SUB><SUP>*0</SUP>";
    pdg2str[-515] = "B<SUB>2</SUB><SUP>*0</SUP>";
    pdg2str[525] = "B<SUB>2</SUB><SUP>*+</SUP>";
    pdg2str[-525] = "B<SUB>2</SUB><SUP>*-</SUP>";
    pdg2str[531] = "B<SUB>s</SUB><SUP>0</SUP>";
    pdg2str[-531] = "B<SUB>s</SUB><SUP>0</SUP>";
    pdg2str[10531] = "B<SUB>s0</SUB><SUP>*0</SUP>";
    pdg2str[-10531] = "B<SUB>s0</SUB><SUP>*0</SUP>";
    pdg2str[533] = "B<SUB>s</SUB><SUP>*0</SUP>";
    pdg2str[-533] = "B<SUB>s</SUB><SUP>*0</SUP>";
    pdg2str[10533] = "B<SUB>s1</SUB>(L)<SUP>0</SUP>";
    pdg2str[-10533] = "B<SUB>s1</SUB>(L)<SUP>0</SUP>";
    pdg2str[20533] = "B<SUB>s1</SUB>(H)<SUP>0</SUP>";
    pdg2str[-20533] = "B<SUB>s1</SUB>(H)<SUP>0</SUP>";
    pdg2str[535] = "B<SUB>s2</SUB><SUP>*0</SUP>";
    pdg2str[-535] = "B<SUB>s2</SUB><SUP>*0</SUP>";
    pdg2str[541] = "B<SUB>c</SUB><SUP>+</SUP>";
    pdg2str[-541] = "B<SUB>c</SUB><SUP>-</SUP>";
    pdg2str[10541] = "B<SUB>c0</SUB><SUP>*+</SUP>";
    pdg2str[-10541] = "B<SUB>c0</SUB><SUP>*-</SUP>";
    pdg2str[543] = "B<SUB>c</SUB><SUP>*+</SUP>";
    pdg2str[-543] = "B<SUB>c</SUB><SUP>*-</SUP>";
    pdg2str[10543] = "B<SUB>c1</SUB>(L)<SUP>+</SUP>";
    pdg2str[-10543] = "B<SUB>c1</SUB>(L)<SUP>-</SUP>";
    pdg2str[20543] = "B<SUB>c1</SUB>(H)<SUP>+</SUP>";
    pdg2str[-20543] = "B<SUB>c1</SUB>(H)<SUP>-</SUP>";
    pdg2str[545] = "B<SUB>c2</SUB><SUP>+</SUP>";
    pdg2str[-545] = "B<SUB>c2</SUB><SUP>-</SUP>";

    //cc mesons
    pdg2str[441] = "&eta;<SUB>c</SUB>(1S)";
    pdg2str[-441] = "&eta;<SUB>c</SUB>(1S)";
    pdg2str[10441] = "&chi;<SUB>c0</SUB>(1P)";
    pdg2str[-10441] = "&chi;<SUB>c0</SUB>(1P)";
    pdg2str[100441] = "&eta;<SUB>c</SUB>(2S)";
    pdg2str[-100441] = "&eta;<SUB>c</SUB>(2S)";
    pdg2str[443] = "J/&psi;(1S)";
    pdg2str[-443] = "J/&psi;(1S)";
    pdg2str[10443] = "h<SUB>c</SUB>(1P)";
    pdg2str[-10443] = "h<SUB>c</SUB>(1P)";
    pdg2str[20443] = "&chi;<SUB>c1</SUB>(1P)";
    pdg2str[-20443] = "&chi;<SUB>c1</SUB>(1P)";
    pdg2str[100443] = "&psi;(2S)";
    pdg2str[-100443] = "&psi;(2S)";
    pdg2str[30443] = "&psi;(3770)";
    pdg2str[-30443] = "&psi;(3770)";
    pdg2str[9000443] = "&psi;(4040)";
    pdg2str[-9000443] = "&psi;(4040)";
    pdg2str[9010443] = "&psi;(4160)";
    pdg2str[-9010443] = "&psi;(4160)";
    pdg2str[9020443] = "&psi;(4415)";
    pdg2str[-9020443] = "&psi;(4415)";
    pdg2str[445] = "&chi;<SUB>c2</SUB>(1P)";
    pdg2str[-445] = "&chi;<SUB>c2</SUB>(1P)";
    pdg2str[100445] = "&chi;<SUB>c2</SUB>(2P)";
    pdg2str[-100445] = "&chi;<SUB>c2</SUB>(2P)";

    //light baryons
    pdg2str[2212] = "p<SUP>+</SUP>";
    pdg2str[-2212] = "p<SUP>-</SUP>";
    pdg2str[2112] = "n";
    pdg2str[-2112] = "n";
    pdg2str[2224] = "&Delta;<SUP>++</SUP>";
    pdg2str[-2224] = "&Delta;<SUP>--</SUP>";
    pdg2str[2214] = "&Delta;<SUP>+</SUP>";
    pdg2str[-2214] = "&Delta;<SUP>+</SUP>";
    pdg2str[2114] = "&Delta;<SUP>0</SUP>";
    pdg2str[-2114] = "&Delta;<SUP>0</SUP>";
    pdg2str[1114] = "&Delta;<SUP>-</SUP>";
    pdg2str[-1114] = "&Delta;<SUP>-</SUP>";
    //strange baryons
    pdg2str[3122] = "&Lambda;";
    pdg2str[-3122] = "&Lambda;";
    pdg2str[3222] = "&Sigma;<SUP>+</SUP>";
    pdg2str[-3222] = "&Sigma;<SUP>+</SUP>";
    pdg2str[3212] = "&Sigma;<SUP>0</SUP>";
    pdg2str[-3212] = "&Sigma;<SUP>0</SUP>";
    pdg2str[3112] = "&Sigma;<SUP>-</SUP>";
    pdg2str[-3112] = "&Sigma;<SUP>-</SUP>";
    pdg2str[3224] = "&Sigma;<SUP>*+</SUP>";
    pdg2str[-3224] = "&Sigma;<SUP>*+</SUP>";
    pdg2str[3214] = "&Sigma;<SUP>*0</SUP>";
    pdg2str[-3214] = "&Sigma;<SUP>*0</SUP>";
    pdg2str[3114] = "&Sigma;<SUP>*-</SUP>";
    pdg2str[-3114] = "&Sigma;<SUP>*-</SUP>";
    pdg2str[3322] = "&Xi;<SUP>0</SUP>";
    pdg2str[-3322] = "&Xi;<SUP>0</SUP>";
    pdg2str[3312] = "&Xi;<SUP>-</SUP>";
    pdg2str[-3312] = "&Xi;<SUP>-</SUP>";
    pdg2str[3324] = "&Xi;<SUP>*0</SUP>";
    pdg2str[-3324] = "&Xi;<SUP>*0</SUP>";
    pdg2str[3314] = "&Xi;<SUP>*-</SUP>";
    pdg2str[-3314] = "&Xi;<SUP>*-</SUP>";
    pdg2str[3334] = "&Omega;<SUP>-</SUP>";
    pdg2str[-3334] = "&Omega;<SUP>-</SUP>";
    //charmed baryons
    pdg2str[4122] = "&Lambda;<SUB>c</SUB><SUP>+</SUP>";
    pdg2str[-4122] = "&Lambda;<SUB>c</SUB><SUP>-</SUP>";
    pdg2str[4222] = "&Sigma;<SUB>c</SUB><SUP>++</SUP>";
    pdg2str[-4222] = "&Sigma;<SUB>c</SUB><SUP>--</SUP>";
    pdg2str[4212] = "&Sigma;<SUB>c</SUB><SUP>+</SUP>";
    pdg2str[-4212] = "&Sigma;<SUB>c</SUB><SUP>-</SUP>";
    pdg2str[4112] = "&Sigma;<SUB>c</SUB><SUP>0</SUP>";
    pdg2str[-4112] = "&Sigma;<SUB>c</SUB><SUP>0</SUP>";
    pdg2str[4224] = "&Sigma;<SUB>c</SUB><SUP>*++</SUP>";
    pdg2str[-4224] = "&Sigma;<SUB>c</SUB><SUP>*--</SUP>";
    pdg2str[4214] = "&Sigma;<SUB>c</SUB><SUP>*+</SUP>";
    pdg2str[-4214] = "&Sigma;<SUB>c</SUB><SUP>*-</SUP>";
    pdg2str[4114] = "&Sigma;<SUB>c</SUB><SUP>*0</SUP>";
    pdg2str[-4114] = "&Sigma;<SUB>c</SUB><SUP>*0</SUP>";
    pdg2str[4232] = "&Xi;<SUB>c</SUB><SUP>+</SUP>";
    pdg2str[-4232] = "&Xi;<SUB>c</SUB><SUP>-</SUP>";
    pdg2str[4132] = "&Xi;<SUB>c</SUB><SUP>0</SUP>";
    pdg2str[-4132] = "&Xi;<SUB>c</SUB><SUP>0</SUP>";
    pdg2str[4322] = "&Xi;<SUB>c</SUB><SUP>\'+</SUP>";
    pdg2str[-4322] = "&Xi;<SUB>c</SUB><SUP>\'-</SUP>";
    pdg2str[4312] = "&Xi;<SUB>c</SUB><SUP>\'0</SUP>";
    pdg2str[-4312] = "&Xi;<SUB>c</SUB><SUP>\'0</SUP>";
    pdg2str[4324] = "&Xi;<SUB>c</SUB><SUP>*+</SUP>";
    pdg2str[-4324] = "&Xi;<SUB>c</SUB><SUP>*-</SUP>";
    pdg2str[4314] = "&Xi;<SUB>c</SUB><SUP>*0</SUP>";
    pdg2str[-4314] = "&Xi;<SUB>c</SUB><SUP>*0</SUP>";
    pdg2str[4332] = "&Omega;<SUB>c</SUB><SUP>0</SUP>";
    pdg2str[-4332] = "&Omega;<SUB>c</SUB><SUP>0</SUP>";
    pdg2str[4334] = "&Omega;<SUB>c</SUB><SUP>*0</SUP>";
    pdg2str[-4334] = "&Omega;<SUB>c</SUB><SUP>*0</SUP>";
    pdg2str[4412] = "&Xi;<SUB>cc</SUB><SUP>+</SUP>";
    pdg2str[-4412] = "&Xi;<SUB>cc</SUB><SUP>-</SUP>";
    pdg2str[4422] = "&Xi;<SUB>cc</SUB><SUP>++</SUP>";
    pdg2str[-4422] = "&Xi;<SUB>cc</SUB><SUP>--</SUP>";
    pdg2str[4414] = "&Xi;<SUB>cc</SUB><SUP>*+</SUP>";
    pdg2str[-4414] = "&Xi;<SUB>cc</SUB><SUP>*-</SUP>";
    pdg2str[4424] = "&Xi;<SUB>cc</SUB><SUP>*++</SUP>";
    pdg2str[-4424] = "&Xi;<SUB>cc</SUB><SUP>*--</SUP>";
    pdg2str[4432] = "&Omega;<SUB>cc</SUB><SUP>+</SUP>";
    pdg2str[-4432] = "&Omega;<SUB>cc</SUB><SUP>-</SUP>";
    pdg2str[4434] = "&Omega;<SUB>cc</SUB><SUP>*+</SUP>";
    pdg2str[-4434] = "&Omega;<SUB>cc</SUB><SUP>*-</SUP>";
    pdg2str[4444] = "&Omega;<SUB>ccc</SUB><SUP>++</SUP>";
    pdg2str[-4444] = "&Omega;<SUB>ccc</SUB><SUP>--</SUP>";
    //bottom baryons
    pdg2str[5122] = "&Lambda;<SUB>b</SUB><SUP>0</SUP>";
    pdg2str[-5122] = "&Lambda;<SUB>b</SUB><SUP>0</SUP>";
    pdg2str[5112] = "&Sigma;<SUB>b</SUB><SUP>-</SUP>";
    pdg2str[-5112] = "&Sigma;<SUB>b</SUB><SUP>-</SUP>";
    pdg2str[5212] = "&Sigma;<SUB>b</SUB><SUP>0</SUP>";
    pdg2str[-5212] = "&Sigma;<SUB>b</SUB><SUP>0</SUP>";
    pdg2str[5222] = "&Sigma;<SUB>b</SUB><SUP>+</SUP>";
    pdg2str[-5222] = "&Sigma;<SUB>b</SUB><SUP>+</SUP>";
    pdg2str[5114] = "&Sigma;<SUB>b</SUB><SUP>*-</SUP>";
    pdg2str[-5114] = "&Sigma;<SUB>b</SUB><SUP>*-</SUP>";
    pdg2str[5214] = "&Sigma;<SUB>b</SUB><SUP>*0</SUP>";
    pdg2str[-5214] = "&Sigma;<SUB>b</SUB><SUP>*0</SUP>";
    pdg2str[5224] = "&Sigma;<SUB>b</SUB><SUP>*+</SUP>";
    pdg2str[-5224] = "&Sigma;<SUB>b</SUB><SUP>*+</SUP>";
    pdg2str[5132] = "&Xi;<SUB>b</SUB><SUP>-</SUP>";
    pdg2str[-5132] = "&Xi;<SUB>b</SUB><SUP>-</SUP>";
    pdg2str[5232] = "&Xi;<SUB>b</SUB><SUP>0</SUP>";
    pdg2str[-5232] = "&Xi;<SUB>b</SUB><SUP>0</SUP>";
    pdg2str[5312] = "&Xi;<SUB>b</SUB><SUP>\'-</SUP>";
    pdg2str[-5312] = "&Xi;<SUB>b</SUB><SUP>\'-</SUP>";
    pdg2str[5322] = "&Xi;<SUB>b</SUB><SUP>\'0</SUP>";
    pdg2str[-5322] = "&Xi;<SUB>b</SUB><SUP>\'0</SUP>";
    pdg2str[5314] = "&Xi;<SUB>b</SUB><SUP>*-</SUP>";
    pdg2str[-5314] = "&Xi;<SUB>b</SUB><SUP>*-</SUP>";
    pdg2str[5324] = "&Xi;<SUB>b</SUB><SUP>*0</SUP>";
    pdg2str[-5324] = "&Xi;<SUB>b</SUB><SUP>*0</SUP>";
    pdg2str[5332] = "&Omega;<SUB>b</SUB><SUP>-</SUP>";
    pdg2str[-5332] = "&Omega;<SUB>b</SUB><SUP>-</SUP>";
    pdg2str[5334] = "&Omega;<SUB>b</SUB><SUP>*-</SUP>";
    pdg2str[-5334] = "&Omega;<SUB>b</SUB><SUP>*-</SUP>";
    pdg2str[5142] = "&Xi;<SUB>bc</SUB><SUP>0</SUP>";
    pdg2str[-5142] = "&Xi;<SUB>bc</SUB><SUP>0</SUP>";
    pdg2str[5242] = "&Xi;<SUB>bc</SUB><SUP>+</SUP>";
    pdg2str[-5242] = "&Xi;<SUB>bc</SUB><SUP>-</SUP>";
    pdg2str[5412] = "&Xi;<SUB>bc</SUB><SUP>\'0</SUP>";
    pdg2str[-5412] = "&Xi;<SUB>bc</SUB><SUP>\'0</SUP>";
    pdg2str[5422] = "&Xi;<SUB>bc</SUB><SUP>\'+</SUP>";
    pdg2str[-5422] = "&Xi;<SUB>bc</SUB><SUP>\'-</SUP>";
    pdg2str[5414] = "&Xi;<SUB>bc</SUB><SUP>*0</SUP>";
    pdg2str[-5414] = "&Xi;<SUB>bc</SUB><SUP>*0</SUP>";
    pdg2str[5424] = "&Xi;<SUB>bc</SUB><SUP>*+</SUP>";
    pdg2str[-5424] = "&Xi;<SUB>bc</SUB><SUP>*-</SUP>";
    pdg2str[5342] = "&Omega;<SUB>bc</SUB><SUP>0</SUP>";
    pdg2str[-5342] = "&Omega;<SUB>bc</SUB><SUP>0</SUP>";
    pdg2str[5432] = "&Omega;<SUB>bc</SUB><SUP>\'0</SUP>";
    pdg2str[-5432] = "&Omega;<SUB>bc</SUB><SUP>\'0</SUP>";
    pdg2str[5434] = "&Omega;<SUB>bc</SUB><SUP>*0</SUP>";
    pdg2str[-5434] = "&Omega;<SUB>bc</SUB><SUP>*0</SUP>";
    pdg2str[5442] = "&Omega;<SUB>bcc</SUB><SUP>+</SUP>";
    pdg2str[-5442] = "&Omega;<SUB>bcc</SUB><SUP>-</SUP>";
    pdg2str[5444] = "&Omega;<SUB>bcc</SUB><SUP>*+</SUP>";
    pdg2str[-5444] = "&Omega;<SUB>bcc</SUB><SUP>*-</SUP>";
    pdg2str[5512] = "&Xi;<SUB>bb</SUB><SUP>-</SUP>";
    pdg2str[-5512] = "&Xi;<SUB>bb</SUB><SUP>+</SUP>";
    pdg2str[5522] = "&Xi;<SUB>bb</SUB><SUP>0</SUP>";
    pdg2str[-5522] = "&Xi;<SUB>bb</SUB><SUP>0</SUP>";
    pdg2str[5514] = "&Xi;<SUB>bb</SUB><SUP>*-</SUP>";
    pdg2str[-5514] = "&Xi;<SUB>bb</SUB><SUP>*+</SUP>";
    pdg2str[5524] = "&Xi;<SUB>bb</SUB><SUP>*0</SUP>";
    pdg2str[-5524] = "&Xi;<SUB>bb</SUB><SUP>*0</SUP>";
    pdg2str[5532] = "&Omega;<SUB>bb</SUB><SUP>-</SUP>";
    pdg2str[-5532] = "&Omega;<SUB>bb</SUB><SUP>+</SUP>";
    pdg2str[5534] = "&Omega;<SUB>bb</SUB><SUP>*-</SUP>";
    pdg2str[-5534] = "&Omega;<SUB>bb</SUB><SUP>*+</SUP>";
    pdg2str[5542] = "&Omega;<SUB>bbc</SUB><SUP>0</SUP>";
    pdg2str[-5542] = "&Omega;<SUB>bbc</SUB><SUP>0</SUP>";
    pdg2str[5544] = "&Omega;<SUB>bbc</SUB><SUP>*0</SUP>";
    pdg2str[-5544] = "&Omega;<SUB>bbc</SUB><SUP>*0</SUP>";
    pdg2str[5554] = "&Omega;<SUB>bbb</SUB><SUP>-</SUP>";
    pdg2str[-5554] = "&Omega;<SUB>bbb</SUB><SUP>+</SUP>";

    return pdg2str;
}

bool isBeforeHadronisation(MCParticle* mc){
    if ( mc->getPDG() == 92 || mc->getPDG() == 91 ) return true;
    for(auto daughter : mc->getDaughters() ){
        if ( isBeforeHadronisation(daughter) ) return true;
    }
    return false;
}

bool isAfterHadronization(EVENT::MCParticle* mc){
    if ( mc->getPDG() == 92 || mc->getPDG() == 91 ) return true;
    const std::vector<MCParticle*> parents = mc->getParents();
    for(auto parent : parents){
        if ( isAfterHadronization(parent) ) return true;
    }
    return false;
}


void drawMcTree(LCEvent* event, bool drawSimulated, bool drawParton, bool drawOverlay){
    // initialize maps to store variables attached to the MC particles
    std::map<MCParticle*, int> p2idx;
    std::map<MCParticle*, bool> p2hadronization;
    std::map<MCParticle*, bool> p2parton;
    std::map<MCParticle*, double> p2distance;
    std::map<MCParticle*, double> p2pt;
    std::map<MCParticle*, double> p2pz;
    std::map<int, std::string> pdg2str = getPdgNamesMap();

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
        p2distance[mc] = std::hypot( mc->getVertex()[0]-ip[0], mc->getVertex()[1]-ip[1], mc->getVertex()[2]-ip[2] );
        p2pt[mc] = std::hypot( mc->getMomentum()[0], mc->getMomentum()[1] );
        p2pz[mc] = mc->getMomentum()[2];
        p2parton[mc] = isBeforeHadronisation(mc);
        p2hadronization[mc] = isAfterHadronization(mc);
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
        bool hasName = pdg2str.find(pdg) != pdg2str.end();
        std::string label;
        if (hasName) label = pdg2str[pdg];
        else label = std::to_string(pdg);

        labels<<p2idx[mc]<<"[label=<"<<label<<"<BR/>"<<std::fixed<<std::setprecision(2)<<p2distance[mc]<<" mm<BR/>"<<p2pt[mc]<<" | "<<p2pz[mc]<<" GeV"<<">";
        if ( p2parton[mc] ) labels<<" style=\"filled\" fillcolor=\"burlywood4\"";
        else if ( mc->isOverlay() && mc->isCreatedInSimulation() ) labels<<" style=\"filled\" fillcolor=\"darkorange4\"";
        else if ( mc->isOverlay() ) labels<<" style=\"filled\" fillcolor=\"dimgray\"";
        else if ( mc->isCreatedInSimulation() ) labels<<" style=\"filled\" fillcolor=\"darkorange\"";
        labels<<"];"<<std::endl;
    }

    //create dot file with a header
    std::ofstream outfile;
    outfile.open("test.dot");
    outfile<<"digraph {"<<std::endl;
    outfile<<"    rankdir=TB;"<<std::endl;
    outfile<<nodes.str()<<std::endl;
    outfile<<labels.str()<<std::endl;

    outfile<<"}"<<std::endl;
    system("rm -f test.svg && dot -Tsvg test.dot > test.svg");
    system("xdg-open test.svg");
}



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
        std::cout << " usage: dumpmctree -[s] -[p] -[o] filename eventNumber " << std::endl << std::endl ;
        std::cout << " OPTIONS: "<< std::endl;
        std::cout << std::left << std::setw(20) << " -s" << std::setw(20) << "Draw particles created in simulation (material interaction with the detector). Highlighted in orange"<< std::endl;
        std::cout << std::left << std::setw(20) << " -p" << std::setw(20) << "Draw parton shower particles above pythia hadronisation string/cluster (pdg = 92/91). E.g. (Z/W/H/quarks/gluons/etc). Highlighted in burlywood "<< std::endl;
        std::cout << std::left << std::setw(20) << " -o" << std::setw(20) << "Draw overlay particles. Highlighted in gray" <<std::endl<<std::endl;
        std::cout << " INPUT: "<< std::endl;
        std::cout << std::left << std::setw(20) << " filename" << std::setw(20) << "slcio file to draw the event from." <<std::endl;
        std::cout << std::left << std::setw(20) << " eventNumber" << std::setw(20) << "event number to draw." <<std::endl;
        exit(1);
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
                    std::cout << " usage: dumpmctree -[s] -[p] -[o] filename eventNumber " << std::endl << std::endl ;
                    std::cout << " OPTIONS: "<< std::endl;
                    std::cout << std::left << std::setw(20) << " -s" << std::setw(20) << "Draw particles created in simulation (material interaction with the detector)"<< std::endl;
                    std::cout << std::left << std::setw(20) << " -p" << std::setw(20) << "Draw parton shower particles above pythia hadronisation string/cluster (pdg = 92/91). E.g. (Z/W/H/quarks/gluons/etc) "<< std::endl;
                    std::cout << std::left << std::setw(20) << " -o" << std::setw(20) << "Draw overlay particles." <<std::endl<<std::endl;
                    std::cout << " INPUT: "<< std::endl;
                    std::cout << std::left << std::setw(20) << " filename" << std::setw(20) << "slcio file to draw the event from." <<std::endl;
                    std::cout << std::left << std::setw(20) << " eventNumber" << std::setw(20) << "event number to draw." <<std::endl;
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
        std::cout << " usage: event number must be a positive integer!" << std::endl ;        
        exit(1) ;
    }

    LCReader* lcReader = LCFactory::getInstance()->createLCReader() ;
    lcReader->setReadCollectionNames({"MCParticle", "MCParticlesSkimmed"});

    LCEvent* event = nullptr;
    try{
        lcReader->open(filename);
        lcReader->skipNEvents(eventNumber - 1);
        event = lcReader->readNextEvent(); 
        if(!event){
            std::cout << " File has less events than required" << eventNumber << std::endl;
            exit(1);
        }

        // Do actuall drawing here
        drawMcTree(event, drawSimulated, drawParton, drawOverlay);
        lcReader->close();
    }
    catch( IOException& e) {
        std::cout << e.what() << std::endl;
        exit(1);
    }
    return 0;
}




