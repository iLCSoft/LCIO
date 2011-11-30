/**
    Tool for checking number of hits from a given collection
    
    For usage help call:
        
        ./lcio_check_colhits -h
    
    TODO: verbosity

    author: Jan Engels, Desy - IT
 */

// getopt includes
// http://www.ibm.com/developerworks/aix/library/au-unix-getopt.html#listing3
//#include <unistd.h> // for getopt
#include <getopt.h> // for getopt long

// std includes
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>

// lcio includes
#include "lcio.h"
#include "IO/LCReader.h"
#include "IMPL/LCTOOLS.h"
#include "IMPL/LCCollectionVec.h"
//#include "EVENT/LCRunHeader.h" 

using namespace std ;
using namespace lcio ;


// struct for storing global options
struct opts_t {
    const char* colName;    // COLLECTION_NAME
    char** inputFiles;      // INPUT_FILE(S)
    int inputFilesNum;      // # of INPUT_FILE(S)
    int verbosity;          // -v option
    bool pedantic;          // -p option
    int startevent;         // --startevent option
    int maxevents;          // --maxevents option
    int exphits;            // --exphits option
    int minhits;            // --minhits option
    int maxhits;            // --maxhits option
    int abshiterror;        // --abshiterror option
    int absevterror;        // --absevterror option 
    float relhiterror;      // --relhiterror option
    float relevterror;      // --relevterror option 
    // counters
    int hitsTotal;          // total number of hits
    int eventsTotal;        // total number of events
    int eventsFailed;       // total number of events where hit checking has failed
    int eventsSkipped;      // total number of events skipped (i.e. events which contain no hits)
} opts;

// number of command line arguments (not including options)
static const int min_args = 2 ;

// getopt string
// ':' after an option means that option requires an argument
static const char *optString = "s:n:x:m:M:h?" ;

// getopt long options declarations
static const struct option longOpts[] = {
    { "verbose", no_argument, NULL, 'v' },
    { "pedantic", no_argument, NULL, 'p' },
    { "startevent", required_argument, NULL, 's' },
    { "maxevents", required_argument, NULL, 'n' },
    { "exphits", required_argument, NULL, 'x' },
    { "minhits", required_argument, NULL, 'm' },
    { "maxhits", required_argument, NULL, 'M' },
    { "abshiterror", required_argument, NULL, 0 },
    { "absevterror", required_argument, NULL, 0 },
    { "relhiterror", required_argument, NULL, 0 },
    { "relevterror", required_argument, NULL, 0 },
    { "help", no_argument, NULL, 'h' },
    { NULL, no_argument, NULL, 0 }
};

/* Display program usage, and exit.
 */
void display_usage( const char* progName, const std::string& errmsg = "" )
{
    stringstream msg;
    if( errmsg != "" )
    {
        msg << endl << "error: " << errmsg << endl ;

    }

    msg << endl
        << "lcio tool for checking an expected number of hits in lcio file(s) from a given collection name"
        << endl << endl
        << "  use: " << progName << " [OPTIONS] COLLECTION_NAME INPUT_FILE(S)"
        << endl << endl
        << "  OPTIONS:"
        << endl << endl
        << "    -v, --verbose  : increase verbosity (use -vvv for debugging)"
        << endl
        << "    -p, --pedantic : pedantic mode (events which contain no hits are counted as failed events)"
        << endl
        << "    -s, --startevent  : start event number (default = 0)"
        << endl
        << "    -n, --maxevents  : maximum number of events (default = all events)"
        << endl
        << "    -m, --minhits  : if set this value is used as the absolute minimum number of hits (*)"
        << endl
        << "    -M, --maxhits  : if set this value is used as the absolute maximum number of hits (*)"
        << endl
        << "    -x, --exphits  : expected number of hits +- error , if no error is given the exact value is expected!"
        << endl
        << "    --abshiterror : absolute error for hits, e.g. if exphits = 100 and abshiterror = 3, valid range will be [97-103]"
        << endl
        << "    --relhiterror : relative error for hits, e.g. if exphits = 100 and relhiterror = 0.1, valid range will be [90 - 110]"
        << endl
        << "    --absevterror : absolute error for events, e.g. if absevterror = 3 means hit checking may fail at most in 3 events"
        << endl
        << "    --relevterror : relative error for events, e.g. if relevterror = 0.1 means hit checking may fail for at most 10% of the total events where hits were found"
        << endl << endl << endl
        << "  NOTES:"
        << endl << endl
        << "    --minhits and --maxhits options always have priority over other options!"
        << endl << endl << endl
        << "  EXAMPLES:"
        << endl << endl
        << "    display stats for MCParticle collections found in simjobs.slcio:"
        << endl
        << "    " << progName << " MCParticle simjob.slcio"
        << endl << endl
        << "    check that MCParticle collections in simjob.slcio contain exactly 101 hits in all events:"
        << endl
        << "    " << progName << " --exphits 101 MCParticle simjob.slcio"
        << endl << endl
        << "    check that MCParticle collections in simjob.slcio contain 100 +- 1 hits in all events:"
        << endl
        << "    " << progName << " --exphits 100 --abshiterror 1 MCParticle simjob.slcio"
        << endl << endl
        << "    check that MCParticle collections in simjob.slcio contain 100 +- 3% hits in all events --> [97-103]:"
        << endl
        << "    " << progName << " --exphits 100 --relhiterror .03 MCParticle simjob.slcio"
        << endl << endl
        << "    check that MCParticle collections in simjob.slcio and recjob.slcio contain 100 +- 1 hit where 10 events may fail the check:"
        << endl
        << "    " << progName << " --exphits 100 --abshiterror 1 --absevterror 10 MCParticle simjob.slcio recjobs.slcio"
        << endl << endl
        << "    check that MCParticle collections in simjob.slcio and recjob.slcio contain 100 +- 1 hit where half the events may fail the check:"
        << endl
        << "    " << progName << " --exphits 100 --abshiterror 1 --relevterror .5 MCParticle simjob.slcio recjobs.slcio"
        << endl << endl
    ;
    cerr << msg.str() << endl << endl;
    exit( EXIT_FAILURE );
}

void showopts( void )
{
    //cout << "COLLECTION_NAME: " << opts.colName << endl ;
    cout << "pedantic: " << opts.pedantic << endl ;
    cout << "startevent: " << opts.startevent << endl ;
    cout << "maxevents: " << opts.maxevents << endl ;
    cout << "minhits: " << opts.minhits << endl ;
    cout << "maxhits: " << opts.maxhits << endl ;
    cout << "exphits: " << opts.exphits << endl ;
    cout << "abshiterror: " << opts.abshiterror << endl ;
    cout << "relhiterror: " << opts.relhiterror << endl ;
    cout << "absevterror: " << opts.absevterror << endl ;
    cout << "relevterror: " << opts.relevterror << endl ;
    cout << "eventsFailed: " << opts.eventsFailed << endl ;
}

//enum loglevels { DEBUG, INFO, WARNING, ERROR };



int main(int argc, char** argv ){

    int opt = 0;
    int longIndex = 0;

    /* Initialize opts before we get to work. */
    opts.verbosity = 0;
    opts.colName = NULL;      // COLLECTION_NAME
    opts.inputFiles = NULL;   // INPUT_FILE(S)
    opts.inputFilesNum = 0;   // # of INPUT_FILE(S)
    opts.pedantic = false;
    opts.startevent = 0;      // 0 == not set
    opts.maxevents = 0;       // 0 == not set
    opts.minhits = 0;         // 0 == not set
    opts.maxhits = 0;         // 0 == not set
    opts.exphits = 0;         // 0 == not set
    opts.abshiterror = 0;     // 0 == not set
    opts.absevterror = 0;     // 0 == not set
    opts.relhiterror = 0;     // 0 == not set
    opts.relevterror = 0;     // 0 == not set
    //initialize counters
    opts.hitsTotal = 0 ;
    opts.eventsTotal = 0 ;
    opts.eventsFailed = 0 ;
    opts.eventsSkipped = 0 ;

    // process the arguments with getopt_long(), then populate opts. 
    opt = getopt_long( argc, argv, optString, longOpts, &longIndex );
    while( opt != -1 )
    {
        switch( opt )
        {
            case 'h':   /* fall-through is intentional */
            case '?':
                display_usage( argv[0] );
                break;

            case 'p':
                opts.pedantic = true;
                break;

            case 'v':
                opts.verbosity++;
                break;

            case 's':
                opts.startevent = atoi(optarg);

                if( opts.startevent < 0 )
                {
                    display_usage( argv[0], "invalid option: startevent must be >= 0" );
                }
                break;

            case 'n':
                opts.maxevents = atoi(optarg);

                if( opts.maxevents < 1 )
                {
                    display_usage( argv[0], "invalid option: maxevents must be >= 1" );
                }
                break;

            case 'x':
                opts.exphits = atoi(optarg);

                if( opts.exphits < 1 )
                {
                    display_usage( argv[0], "invalid option: exphits must be > 1" );
                }
                break;

            case 'm':
                opts.minhits = atoi(optarg);

                if( opts.minhits < 1 )
                {
                    display_usage( argv[0], "invalid option: minhits must be > 1" );
                }
                break;

            case 'M':
                opts.maxhits = atoi(optarg);
            
                if( opts.maxhits < 1 )
                {
                    display_usage( argv[0], "invalid option: maxhits must be > 1" );
                }
                break;

            
            // long options without a short arg
            case 0:

                if( strcmp( "abshiterror", longOpts[longIndex].name ) == 0 )
                {
                    opts.abshiterror = atoi(optarg);
                
                    if( opts.abshiterror < 1 )
                    {
                        display_usage( argv[0], "invalid option: abshiterror must be > 1" );
                    }
                }

                if( strcmp( "absevterror", longOpts[longIndex].name ) == 0 )
                {
                    opts.absevterror = atoi(optarg);
                
                    if( opts.absevterror < 1 )
                    {
                        display_usage( argv[0], "invalid option: absevterror must be > 1" );
                    }
                }

                if( strcmp( "relhiterror", longOpts[longIndex].name ) == 0 )
                {
                    opts.relhiterror = atof(optarg);
                
                    if( opts.relhiterror <= 0 || opts.relhiterror >= 1)
                    {
                        display_usage( argv[0], "invalid option: 0 < relhiterror < 1 , (e.g.: 0.3)" );
                    }
                }

                if( strcmp( "relevterror", longOpts[longIndex].name ) == 0 )
                {
                    opts.relevterror = atof(optarg);
                
                    if( opts.relevterror <= 0 || opts.relevterror >= 1)
                    {
                        display_usage( argv[0], "invalid option: 0 < relevterror < 1 , (e.g.: 0.3)" );
                    }
                }

                break;


            default:
                // should never get here!
                break;
        }

        opt = getopt_long( argc, argv, optString, longOpts, &longIndex );
    }

    // argc also includes command line options
    // optind is set to the index of the first positional command line argument
    // so (argc - optind) == total number of command line arguments (without options)
    if( (argc - optind) < min_args )
    {
        display_usage( argv[0], "wrong number of arguments" );
    }

    //if( opts.minhits == 0 && opts.maxhits == 0 && opts.exphits == 0 )
    //{
    //    display_usage( argv[0], "one of the following options must be set: minhits, maxhits or exphits" );
    //}

    if( opts.abshiterror > 0 && opts.relhiterror > 0 )
    {
        display_usage( argv[0], "abshiterror and relhiterror options are mutually exclusive" );
    }

    if( opts.absevterror > 0 && opts.relevterror > 0 )
    {
        display_usage( argv[0], "absevterror and relevterror options are mutually exclusive" );
    }


    // check if the expected hits option is set
    if( opts.exphits > 0 )
    {
        // look if any error options are used and set min and max hits appropriately
        if( (opts.abshiterror > 0) || (opts.relhiterror > 0) )
        {
            if( opts.relhiterror > 0 )
            {
                opts.abshiterror = (int)(opts.exphits*opts.relhiterror) ;
                // in case the expression above rounds to 0 we need to set it to 1
                // to ensure minhits and maxhits are set appropriately
                if( opts.abshiterror == 0 )
                {
                    opts.abshiterror = 1 ;
                }
            }

            if( opts.abshiterror > 0 )
            {
                // if minhits option is set it always overwrites other options
                if( opts.minhits == 0 )
                {
                    opts.minhits = (opts.exphits - opts.abshiterror) ;

                    // make sure minhits is always >= 0
                    if( opts.minhits < 0 )
                    {
                        opts.minhits = 0;
                    }
                }

                // if maxhits option is set it always overwrites other options
                if( opts.maxhits == 0 )
                {
                    opts.maxhits = (opts.exphits + opts.abshiterror) ;
                }
            }
        }
        else // if no error options are set, the exact number of hits is expected
        {

            // if minhits option is set it always overwrites other options
            if( opts.minhits == 0 )
            {
                opts.minhits = opts.exphits ;
            }

            // if maxhits option is set it always overwrites other options
            if( opts.maxhits == 0 )
            {
                opts.maxhits = opts.exphits ;
            }
        }
    }

    // make sure maxhits is always greater than minhits
    if( opts.maxhits > 0 && ( opts.maxhits < opts.minhits ) )
    {
        display_usage( argv[0], "maxhits must be > minhits" );
    }

    // collection name
    opts.colName = argv[optind] ;
    optind++; // increment optind to the next argument

    // and finally the remaining list of lcio files
    opts.inputFiles    = argv + optind;
    opts.inputFilesNum = argc - optind;

    //showopts();


    // ---------------------------------------------------------------------
    // end of command line argument parsing
    // ---------------------------------------------------------------------


    vector<string> inputFilesVec ;

    for( int i=0 ; i < opts.inputFilesNum ; i++ ){
        //cout << opts.inputFiles[ i ] << endl;
        inputFilesVec.push_back( opts.inputFiles[ i ] );
    }

    LCReader* lcReader = LCFactory::getInstance()->createLCReader() ;

    lcReader->open( inputFilesVec ) ;

    if( opts.startevent > 0 ){
        lcReader->skipNEvents( opts.startevent );
    }


    LCEvent* evt ;
    LCCollection* hits = NULL;

    //----------- the event loop -----------
    try{
        while( (evt = lcReader->readNextEvent()) != 0 )
        {
            hits = evt->getCollection( opts.colName );
            if( hits )
            {
                int nHits = hits->getNumberOfElements() ;
                // cout << opts.colName << " : " << nHits << endl;

                if( nHits < opts.minhits )
                {
                    //cerr << "error in event [" << opts.eventsTotal << "]: minHits condition failed: nHits [" << nHits << "] < minhits [" << opts.minhits << "]" << endl;
                    opts.eventsFailed++ ;
                }

                if( opts.maxhits > 0 )
                {
                    if( nHits > opts.maxhits )
                    {
                        //cerr << "error in event [" << opts.eventsTotal << "]: maxHits condition failed: nHits [" << nHits << "] > maxhits [" << opts.maxhits << "]" << endl;
                        opts.eventsFailed++ ;
                    }
                }
                opts.hitsTotal += nHits ;
            }
            else // event which contains no hits from the given collection
            {
                opts.eventsSkipped++ ;

                if( opts.pedantic )
                {
                    opts.eventsFailed++ ; // in pedantic mode events containing no hits are marked as failed
                }
            }
            //LCTOOLS::dumpEvent( evt ) ;
            opts.eventsTotal++ ;

            if( opts.maxevents && opts.eventsTotal >= opts.maxevents )
            {
                break ;
            }
        }
    }
    catch( IOException& e )
    {
        cerr << "io error when reading data : " << e.what() << endl ;
    }
    // -------- end of event loop -----------

    if( opts.eventsTotal > 0 )
    {
        cout << "found " << opts.hitsTotal << " " << opts.colName << " hits in " << opts.eventsTotal << " events" << endl ;
        cout << "avg number of " << opts.colName << " hits per event: " << (float)opts.hitsTotal/opts.eventsTotal << endl ;

        if( opts.eventsSkipped )
        {
            cout << "found " << opts.eventsSkipped << " events containg no hits from " << opts.colName << endl ;
            cout << "avg number of " << opts.colName << " hits per event (only events including hits): " << (float)opts.hitsTotal/(opts.eventsTotal-opts.eventsSkipped) << endl ;
        }
    }
    else
    {
        cerr << "no events were found!" << endl ;
    }
    if( opts.eventsFailed > 0 )
    {
        cout << "found " << opts.eventsFailed  << " events were hit checking has failed" << endl ;
    }


    lcReader->close();
    delete lcReader ;

    if( opts.relevterror > 0 )
    {
        opts.absevterror = (int)(opts.eventsTotal*opts.relevterror) ;
    }

    if( opts.eventsFailed > opts.absevterror )
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

