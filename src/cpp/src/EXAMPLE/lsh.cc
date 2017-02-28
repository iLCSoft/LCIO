/** 
 * == LCIO Shell ==
 * Simple tool to browse slcio files similar to 
 * directory trees.
 * 
 * (developed during DESY summerstudent programme 2007)
 * 
 * @author N. Chiapolini, DESY
 * @version $Id: lsh.cc,v 1.3 2010-04-13 10:58:09 engels Exp $
 */

#include <signal.h>
#include <string>
#include <cstring>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <iomanip>
#include "lcio.h"

// C headers needed for output redirection (paging)
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


#include "EVENT/LCCollection.h"
#include "EVENT/SimTrackerHit.h"
#include "EVENT/SimCalorimeterHit.h"

#include "IO/LCReader.h"
#include "UTIL/LCTOOLS.h"
#include "EVENT/LCRunHeader.h"
#include "UTIL/LCTime.h"

/*** <READLINE> ***/
extern "C" {
  #include <readline/readline.h>
  #include <readline/history.h>
}
 /*** </READLINE> ***/

using namespace std ;
using namespace lcio ;

// const int maxFilestring = 20;

typedef pair<int, string> level;

struct pagerInfo{
  char *filename;
  int fd_temp;
  int fd_old;
  bool save;
} ;

// Constants to identify postion in tree.
const int TOP = 0; // file information 
const int RUN = 1;
const int EVT = 2;
const int COL = 3;
vector<level> position;

map<int, string> mapRuns;
map<int, int> mapEventsInRun; 
int numEvents = 0;
bool withEvents = true;
bool interrupt = false; // flag for signal handling
bool pageOutput = false;

string pager = "less";
string egg   = "egg";
string prompt = "";

// memorie for objects
LCReader* lcReader;
LCRunHeader *runHdr ;
LCEvent *event;
LCCollection *gCol;


/** 
 * PROMPT
 */
const char * print_prompt() {
  
  prompt.clear() ;

  vector<level>::iterator levelIt;
  vector<level>::iterator levelItEnd = position.end();
  for( levelIt = position.begin(); levelIt != levelItEnd ; levelIt++ ){
    prompt += levelIt->second ;
  }
  prompt += "$ ";

  return prompt.c_str();
}

/** END PROMPT */




/** 
 * EXIT
 */

void leave(int ret) {
  cout << endl;
  try {
    lcReader->close() ;
  }
  catch (exception& e) {
  }
  delete lcReader;
//   
  exit(ret);
}

/** END EXIT */


/** 
 * SIGNAL HANDLERS
 */

void int_handler(int sig) {
  cout << "interrupting.." << sig  << endl;
  interrupt = true;
}

void term_handler(int sig) {
  cout << "leaving.. " << sig << endl;
  leave(1);
}

/** END SIGNAL HANDLERS */


/** 
 * PAGER HANDLING 
 */

void begin_paging(pagerInfo*file) {
  if (!(file->save)) {
    std::string s("./lcio_lsh.tmp" );
    file->filename =  &s[0] ;
  }

  // C style output redirection (changing file descriptors)
  file->fd_temp = open(file->filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR );
  file->fd_old = open(file->filename, O_RDONLY); // creating a place for the old stdout fd (shell)
  dup2(1, file->fd_old);  // backing up old fd pointed to by stdout (1 = stdout) 
  dup2(file->fd_temp, 1); // changing the fd pointed to by stdout 
  // stdout points now to the fd of our temp file
}


bool end_paging(pagerInfo*file) {
  dup2(file->fd_old, 1); // resetting the fd pointed to by stdout

  // stdout points to the shell's fd again
  close(file->fd_temp);
  close(file->fd_old); // removes the fd (the file stays open as stdout points to it too)

  string lessCommand = pager;
  lessCommand.append(" ");
  lessCommand.append(file->filename);

  int res = system(lessCommand.c_str());
  
  if (!(file->save)) {
    remove (file->filename);
  }
  
  return !res; // system returns 0 on success, but 0 is evaluated to false
}

/** END PAGER HANDLING */



/** 
 * PRINT FUNCITONS
 */

//Prints important info for tracker and calorimeter hits (and number of entries for all collections)
void simplePrintCol(LCCollection* col) {
  int nElements;
  const std::string colType = col->getTypeName();
  
  SimTrackerHit *trackerHit;
  SimCalorimeterHit *calHit;
  
  
  cout << "elements:   " << (nElements = col->getNumberOfElements()) << endl;
  if (colType == LCIO::SIMTRACKERHIT ) {
    cout << " Hit \tCell"<< endl;
    for (int i=0; i<nElements; i++) {
      trackerHit =  dynamic_cast<SimTrackerHit*> ( col->getElementAt(i) );
      cout << setw(4) << i << "\t" << setw(10) << trackerHit->getCellID() << endl;
    }
  } else if (colType == LCIO::SIMCALORIMETERHIT ) {
    cout << " Hit \tCell      \tEnergy" << endl;
    for (int i=0; i<nElements; i++) {
      calHit =  dynamic_cast<SimCalorimeterHit*> ( col->getElementAt(i) );
      cout << setw(4) << i << "\t" << setw(10) << calHit->getCellID0() << "\t" << setw(6) << calHit->getEnergy() << endl;
    }
  }
}
// end simplePrintCol()


// prints a detailed listing of all the entries in the collection
void normalPrintCol(LCCollection *col) {
  if( col->getTypeName() == LCIO::MCPARTICLE ){
    LCTOOLS::printMCParticles( col ) ;

  }
  else if( col->getTypeName() == LCIO::SIMTRACKERHIT ){
	  
    LCTOOLS::printSimTrackerHits( col ) ;

  }
  else if( col->getTypeName() == LCIO::TPCHIT ){
	  
    LCTOOLS::printTPCHits( col ) ;

  }
  else if( col->getTypeName() == LCIO::TRACKERHIT ){
	  
    LCTOOLS::printTrackerHits( col ) ;

  }
  else if( col->getTypeName() == LCIO::SIMCALORIMETERHIT ){
	  
    LCTOOLS::printSimCalorimeterHits( col ) ;

  }
  else if( col->getTypeName() == LCIO::CALORIMETERHIT ){
	  
    LCTOOLS::printCalorimeterHits( col ) ;

  }
  else if( col->getTypeName() == LCIO::RAWCALORIMETERHIT ){
	  
    LCTOOLS::printRawCalorimeterHits( col ) ;

  }
  else if( col->getTypeName() == LCIO::LCFLOATVEC ){
	  
    LCTOOLS::printLCFloatVecs( col ) ;

  }
  else if( col->getTypeName() == LCIO::LCINTVEC ){
	  
    LCTOOLS::printLCIntVecs( col ) ;                               

  }
  else if( col->getTypeName() == LCIO::LCSTRVEC ){
	  
    LCTOOLS::printLCStrVecs( col ) ;                                 

  }
  else if( col->getTypeName() == LCIO::TRACK ){
	  
    LCTOOLS::printTracks( col ) ;

  }
  else if( col->getTypeName() == LCIO::CLUSTER ){
	  
    LCTOOLS::printClusters( col ) ;

  }
  else if( col->getTypeName() == LCIO::RECONSTRUCTEDPARTICLE ){
	  
    LCTOOLS::printReconstructedParticles( col ) ;

  }
  else if( col->getTypeName() == LCIO::VERTEX ){
	  
    LCTOOLS::printVertices( col ) ;

  }
  else if( col->getTypeName() == LCIO::LCGENERICOBJECT ){
	  
    LCTOOLS::printLCGenericObjects( col ) ;

  }
  else if( col->getTypeName() == LCIO::LCRELATION ){
	  
    LCTOOLS::printRelation( col ) ;
  }
  else if( col->getTypeName() == LCIO::TRACKERRAWDATA ){
	  
    LCTOOLS::printTrackerRawData( col ) ;
  }
  else if( col->getTypeName() == LCIO::TRACKERDATA ){
	  
    LCTOOLS::printTrackerData( col ) ;
  }
  else if( col->getTypeName() == LCIO::TRACKERPULSE ){
	  
    LCTOOLS::printTrackerPulse( col ) ;
  }
}
// end normalPrintCol()


// prints short summary on open file
void printTop() {
  if (withEvents) {
    cout << mapRuns.size() << " Runs - " << numEvents << " Events" << endl;
  } else {
    cout << mapRuns.size() << " Runs - ?? Events" << endl;
  }
}



// print details for collection (called: print | cat  and  ls | dummp within a collection)
void fun_print(string colNr, string flag) {
  if ((position.size() < 3)) {
    return;
  }
  unsigned int c;
  istringstream i(colNr);
  i >> c;
  lcReader->open( position[TOP].second ) ;
  if ((event = lcReader->readEvent( position[RUN].first, position[EVT].first )) != 0) {
    const vector<string> *colVec = event->getCollectionNames();
    if (c < colVec->size()) {
      gCol = event->getCollection((*colVec)[c]);
      cout << "name:     " << (*colVec)[c] << endl;
      cout << "type:     " << gCol->getTypeName() << endl;
  
      if (flag == "-s") {
        simplePrintCol(gCol);
      } else {
        normalPrintCol(gCol);
      }
    }
  }
  
  lcReader->close() ;
}
// end fun_print

/** END PRINT FUNCTIONS */




/** 
 * LS FUNCTIONS
 */

// list all runs in file
void lsRuns() {
  map<int, string>::iterator it;
  map<int, string>::iterator itEnd = mapRuns.end();
  for( it = mapRuns.begin() ; it != itEnd ; it++ ) {
    if (withEvents) {
      stringstream sstream;
      sstream << "(" << mapEventsInRun[it->first] << " events)";
      string tempEvents = sstream.str() ;
      cout << "[" << setw(2) << it->first << "] " << left << setw(40) << it->second << "  " << right << setw(13) << tempEvents << endl; 
    } else {
      cout << "[" << setw(2) << it->first << "] " << left << setw(40) << it->second << "  " << endl;
    }
  }
}
// end lsRuns()


// list all events in run
void lsEvents() {
  lcReader->open( position[TOP].second ) ;
  while ( (event = lcReader->readNextEvent() ) != 0  && !interrupt) {
    if (event->getRunNumber() == (position[RUN].first)) {
      cout << "[" << setw(2) << event->getEventNumber() << "] " << setw(3) << (event->getCollectionNames())->size() << " Collections" << endl;
    }
  }
  interrupt = false;
  lcReader->close() ;
}
//end lsEvents()


// list all collections in event
void lsCollections() {
  lcReader->open( position[TOP].second ) ;
  if ((event = lcReader->readEvent( position[RUN].first, position[EVT].first )) != 0) {
    const vector<string> *colVec = event->getCollectionNames();
    int i = 0;
    vector<string>::const_iterator it;
    vector<string>::const_iterator itEnd = colVec->end();
    for( it = colVec->begin(); it != itEnd ; it++ ){
      gCol = event->getCollection(*it);
      cout << "[" << setw(2) << i << "] " << left << setw(25) << *it << "  " << setw(20) << gCol->getTypeName() << "  " << right << setw(3) << gCol->getNumberOfElements () <<  endl;
      i++;
    }
  } else {
    cout << "this event does not exist" << endl;
  }
  lcReader->close() ;
}
// end lsCollections()

/** END LS FUNCTIONS */


/** 
 * COMMAND FUNCTIONS
 */

// ls 
void fun_ls() {
  switch( position.size()-1 ){
    case TOP:
      printTop();
      lsRuns();
      break;
    case RUN:
      lsEvents();
      break;
    case EVT:
      lsCollections();
      break;
    case COL:
      string s; 
      stringstream n;
      n << position[COL].first;
      n >> s;
      fun_print(s, "-s");
      break;
  }
}
// end ls


// cl (change level) | cd (change directory)
void fun_cd(string str) {
  unsigned n;
  
  if (str.size() < 1) {
    return;
  }
  
  char *cstr, *p;
  cstr = new char [str.size()+1];
  strcpy (cstr, str.c_str());
  p=strtok (cstr,"/");
  while (p!=NULL)
  {
    string next; 
    stringstream sstream;
    sstream << p;
    sstream >> next;
    // moving up 
    if (next == "..") {
      if (position.size() == 1) {
        leave(0);
      }
      position.pop_back();
    } else {
      // moving into next level
      istringstream i(next);
      i >> n;
      switch( position.size()-1 ){
        case TOP:
          position.push_back(level(n, "/run_"+next));
          break;
        case RUN:
          position.push_back(level(n, "/evt_"+next));
          break;
        case EVT:
          lcReader->open( position[TOP].second ) ;
          if ((event = lcReader->readEvent( position[RUN].first, position[EVT].first )) != 0) {
            const vector<string> *colVec = event->getCollectionNames();
            if (n < colVec->size()) {
              position.push_back(level(n, "/"+(*colVec)[n] ));
            } else {
              cout << "No collection with number " << n << endl;
            }
          }
          lcReader->close();
          break;
      }
    }
  
    p=strtok(NULL,"/");
  }

  delete[] cstr;  

}
// end cl | cd


// dump data of active level
void fun_dump(string arg) {
  switch( position.size()-1 ){
    case RUN:
      // WHY IS THERE NO readRunHeader( int run ) ??
      lcReader->open( position[TOP].second ) ;
      while (( runHdr = lcReader->readNextRunHeader() ) != 0 && !interrupt) {
        int run = runHdr->getRunNumber();
        if (run == position[RUN].first) {
          interrupt = true;
          LCTOOLS::dumpRunHeader(runHdr);
        }
      }
      lcReader->close() ;
      interrupt = false;
      break;
    case EVT:
      lcReader->open( position[TOP].second ) ;
      if ((event = lcReader->readEvent( position[RUN].first, position[EVT].first )) != 0) {
        if(arg == "-d"){
          LCTOOLS::dumpEventDetailed(event);
        } else {
          LCTOOLS::dumpEvent(event);
        }
      }
      lcReader->close() ;
      break;
    case COL:
      string s; 
      stringstream n;
      n << position[COL].first;
      n >> s;
      fun_print(s, "-d");
      break;
  }
}
// end dump

void fun_egg() {
  for (int i = 0; i < 4; i++ ) {
    cout << "Glugg...." << endl;
    sleep(1);
  }
  cout << "Ahhh\n" << endl;
}



// open new file
void fun_open(string filename) {
  position.clear();
  mapRuns.clear();
  mapEventsInRun.clear();
  
  FILE * pFile;
  long size;

  // checking file size, as we do not want to wait for 
  // the shell to map all the events of a large file.
  pFile = fopen (filename.c_str(),"rb");
  if (pFile==NULL) perror ("Error opening file");
  else
  {
    fseek (pFile, 0, SEEK_END);
    size= ftell(pFile);
    fclose (pFile);
    if (size > 50000000) {
      cout << "Large file: not preparing map of events in runs!" << endl;
      cout << "(Initialising will still take some time though." << endl;
      cout << " You can skip it using [ctrl]-[c].)" << endl;
      withEvents = false;
    }
  }
  

  egg.replace(1,2,2,'e');
  position.push_back(level(0, filename));

  lcReader->open( position[TOP].second ) ;
  while (( runHdr = lcReader->readNextRunHeader() ) != 0 && !interrupt) {
    int run = runHdr->getRunNumber();
    mapRuns.insert ( pair<int, string>(run, runHdr->getDescription()) );
    mapEventsInRun.insert( pair<int, int>(run, 0));
  }
  interrupt = false;
  lcReader->close() ;
  
  if (withEvents) {
    lcReader->open( position[TOP].second ) ;
    while ( (event = lcReader->readNextEvent()) != 0  && !interrupt) {
      (mapEventsInRun[event->getRunNumber()])++;
      numEvents++;
    }
    interrupt = false;
    lcReader->close() ;
  }
}
// end open


// help output
void fun_help() {
  cout << "  COMMANDS:" << endl;
  cout << "    cd | cl <number of OBJECT|..>    change into OBJECT | leave object; multiple levels can be given at once" << endl;
  cout << "                                        e.g.:   file.slcio/run1$ cd ../3/1" << endl;
  cout << "    ls                               list elements on next level | list content of active collection" << endl;
  cout << "    dump [-d]                        dump data of active level; -d triggers detailed dump of event" << endl;
  cout << "    print | cat [-s] <collection nr> print content of collection given; -s triggers short print-out" << endl;
  cout << endl;
  cout << "    open <filename>                  open new file" << endl;
  cout << "    exit | quit                      exit program" << endl;
  cout << "    help                             print this help text" << endl;
  cout << "    pager <command>                  use pager <command> when paging output" << endl;
  cout << endl;
  cout << "  REDIRECTION:" <<endl;
  cout << "    If '|' or '>' is appended to the a command, the output will be redirected to a file and then"<<endl;
  cout << "    displayed with the pager (see above)." << endl;
  cout << "    If a string is given after the redirect token, it is used as filename and the file is stored." <<endl;
  cout << "    (existing files will get overwritten without prompt). If no filename is given, a temporary " << endl;
  cout << "    file will be used for paging." << endl;
  cout << "       e.g.:    file.slcio/run1/evt1$ dump -d > event1.txt "<<endl;
}
// end help output

/** END COMMAND FUNCTIONS */






/** 
 * MAIN
 */

int main(int argc, char** argv ) {
  string temp = egg;
  
  // Signal handlers
  signal(SIGINT, int_handler);
  signal(SIGTERM, term_handler);
  temp.push_back('r');
  
  
  // checking command and printing help if necessary
  if (argc != 2) {
    cout << "usage: lsh <file name>|-h" << endl;
    exit(1);
  }
  temp.insert(1,"b");
  if (!strcmp(argv[1], "-h")) {
    fun_help();
    exit(0);
  }
  temp.erase(0,1);
  
  
  // initialisation
  cout << setprecision(3) << fixed;
  lcReader = LCFactory::getInstance()->createLCReader( IO::LCReader::directAccess ) ;
  egg = temp;
  
  fun_open(argv[1]);         // opening new file and preparing general information
  printTop();


  
  /** MAIN SHELL LOOP */
  do {
//     string commandBuf;
    vector<string> commandVec;
    commandVec.reserve( 1024 ) ;

    /** read input */
//    /** ohne readline **/
//     if (!getline(cin, commandBuf)) {
//       leave(0);
//     }    
//     if (!commandBuf.size()) {
//       continue;
//     }
//     
//     string buf;
//     stringstream sstream(commandBuf);
//     while (sstream >> buf) {
//       commandVec.push_back(buf);
//     }
    
    
    char *line = readline (print_prompt());

    // exit with CTRL+D
    if( *line == '\0' ) {
        free(line);
        cout << "exit" << endl;
        leave(0);
    }

    if (!(line && *line)) {
      free(line);
      cout << endl;
      continue;
    }
    add_history(line);
    
    string buf;
    stringstream sstream(line);
    while (sstream >> buf) {
      commandVec.push_back(buf);
    }
    free(line);

    /** process input */
    // check if output should be paged
    pagerInfo data;
    data.save = false;
    
    int cvSize = commandVec.size();
    if (cvSize > 1) {
      // if redirect symbol end command, redirect to tmp-file
      if ((commandVec[cvSize-1] == "|") || (commandVec[cvSize-1] == ">")) {
        pageOutput = true;
        commandVec.pop_back();
      } else 
        // if redirect symbol is followed by a string, redirect to 
        // a file named according to this string 
        //(careful, file will be truncated without warning)
        if ((commandVec[cvSize-2] == "|") || (commandVec[cvSize-2] == ">")) {
        pageOutput = true; 
        data.save = true;
	//        strcpy(data.filename, commandVec[cvSize-1].c_str());
	//fg: cannot use strcpy w/o memory allocation - assign filename directly from vector 
	//    as a workaround (works as long as commandVec does not change...)
        data.filename = &commandVec[cvSize-1][0] ;
	}  
    }
    
    
    // if the output is paged, but paging failes, we want to 
    // run the command again, without paging 
    // in addition a message should be printed
    int run = 0;
    bool outputSuccess; 
    do {
      if (pageOutput) {
        begin_paging(&data);
      }


      // select action based on command given
      // -> improvement: move handling of parameters into functions and
      //                 pass pointer to commandVec instead of values
      if ((commandVec[0] == "exit") || (commandVec[0] == "quit")) { 
        leave(0); 
      }
      if (commandVec[0] == "ls") {
        fun_ls();
      }
      if ((commandVec[0] == "cl") || (commandVec[0] == "cd")) {
        fun_cd(commandVec[1]);
      }
      if ((commandVec[0] == "print") || (commandVec[0] == "cat")) {
        // add default output flag, if non was given
        if (commandVec.size() == 2) { 
          vector<string>::iterator it;
          it = ++(commandVec.begin());
          commandVec.insert ( it , "-d" );
        }
        fun_print(commandVec[2], commandVec[1]);
      }
      if ((commandVec[0] == "dump")) {
        if (commandVec.size() < 2) {
          commandVec.push_back("-s");
        }
        fun_dump(commandVec[1]);
      }
      if ((commandVec[0] == "open") || (commandVec[0] == "file")) {
        fun_open(commandVec[1]);
      }
      if ((commandVec[0] == egg)) {
        fun_egg(); 
      }
      if ((commandVec[0] == "help")) {
        fun_help();
      }
      if ((commandVec[0] == "pager")) {
        pager = commandVec[1];
      }
      if (!cin) { 
        leave(0);
      }

      if (pageOutput) {
        outputSuccess = end_paging(&data);
      } else {
        outputSuccess = true;
      } 
      pageOutput = false;
      ++run;
    } while (!outputSuccess); // repeat until output succesfully printed (max two runs)
    if (run > 2) {
      cout << "  paging failed - active pager: " << pager << " run:  " << run << endl;
      cout << "  You can set another pager with:  pager <command>" << endl;
    }
    
  } while (1 == 1);

}
