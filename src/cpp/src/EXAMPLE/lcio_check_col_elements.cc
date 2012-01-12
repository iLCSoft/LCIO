/**
    Tool for checking number of elements from a given collection
    
    For usage help call:
        
        ./lcio_check_colelements -h
    
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
#include <cmath>

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
    bool average;           // -a option
    int startevent;         // --startevent option
    int maxevents;          // --maxevents option
    int expelements;            // --expelements option
    int minelements;            // --minelements option
    int maxelements;            // --maxelements option
    int abselementerror;        // --abselementerror option
    int abseventerror;        // --abseventerror option 
    float relelementerror;      // --relelementerror option
    float releventerror;      // --releventerror option 
    // counters
    int elementsTotal;          // total number of elements
    int eventsTotal;        // total number of events
    int eventsFailed;       // total number of events where element checking has failed
    int eventsSkipped;      // total number of events skipped (i.e. events which contain no elements)
} opts;

// number of command line arguments (not including options)
static const int min_args = 2 ;

// getopt string
// ':' after an option means that option requires an argument
static const char *optString = "s:n:x:m:M:pavh?" ;

// getopt long options declarations
static const struct option longOpts[] = {
    { "verbose", no_argument, NULL, 'v' },
    { "pedantic", no_argument, NULL, 'p' },
    { "average", no_argument, NULL, 'a' },
    { "startevent", required_argument, NULL, 's' },
    { "maxevents", required_argument, NULL, 'n' },
    { "expelements", required_argument, NULL, 'x' },
    { "minelements", required_argument, NULL, 'm' },
    { "maxelements", required_argument, NULL, 'M' },
    { "abselementerror", required_argument, NULL, 0 },
    { "abseventerror", required_argument, NULL, 0 },
    { "relelementerror", required_argument, NULL, 0 },
    { "releventerror", required_argument, NULL, 0 },
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
        << "lcio tool for checking an expected number of elements in lcio file(s) from a given collection name"
        << endl << endl
        << "  use: " << progName << " [OPTIONS] COLLECTION_NAME INPUT_FILE(S)"
        << endl << endl
        << "  OPTIONS:"
        << endl << endl
        << "    -v, --verbose  : increase verbosity (use -vvv for debugging)"
        << endl
        << "    -p, --pedantic : events which contain no elements are counted as failed events and"
        << endl
        << "                     not ignored for calculating the average number of elements"
        << endl
        << "    -a, --average : use this option to check the average number of elements instead of treating events individually"
        << endl
        << "    -s, --startevent  : start event number (default = 0)"
        << endl
        << "    -n, --maxevents  : maximum number of events (default = all events)"
        << endl
        << "    -m, --minelements  : if set this value is used as the absolute minimum number of elements (*)"
        << endl
        << "    -M, --maxelements  : if set this value is used as the absolute maximum number of elements (*)"
        << endl
        << "    -x, --expelements  : expected number of elements +- error , if no error is given the exact value is expected!"
        << endl
        << "    --abselementerror : absolute error for elements, e.g. if expelements = 100 and abselementerror = 3, valid range will be [97-103]"
        << endl
        << "    --relelementerror : relative error for elements, e.g. if expelements = 100 and relelementerror = 0.1, valid range will be [90 - 110]"
        << endl
        << "    --abseventerror : absolute error for events, e.g. abseventerror = 3 means that at most 3 events may fail (**)"
        << endl
        << "    --releventerror : relative error for events, e.g. releventerror = 0.1 means that at most 10% of the total events may fail (**)"
        << endl << endl
        << "    (*)  --minelements and --maxelements options always have priority over other options!"
        << endl << endl
        << "    (**) --abseventerror and --releventerror options are both influenced by --pedantic option, i.e."
        << endl
        << "           not only the events which fail the checking of elements are counted as failed, "
        << endl
        << "           but also the ones which do not contain any elements."
        << endl << endl << endl
        << "  EXAMPLES:"
        << endl << endl
        << "    display stats for MCParticle collections found in simjobs.slcio:"
        << endl
        << "    " << progName << " MCParticle simjob.slcio"
        << endl << endl
        << "    check that MCParticle collections in simjob.slcio contain exactly 101 elements in all events:"
        << endl
        << "    " << progName << " --expelements 101 MCParticle simjob.slcio"
        << endl << endl
        << "    check that the average number of elements from the MCParticle collections in simjob.slcio lies between [99-101]"
        << endl
        << "    # note that --abselementerror 1 can be left out as checking for the exact average doesn't really make much sense.."
        << endl
        << "    " << progName << " -a --expelements 100 --abselementerror 1 MCParticle simjob.slcio"
        << endl << endl
        << "    check that MCParticle collections in simjob.slcio contain 100 +- 3% elements in all events --> [97-103]:"
        << endl
        << "    " << progName << " --expelements 100 --relelementerror .03 MCParticle simjob.slcio"
        << endl << endl
        << "    check that MCParticle collections in simjob.slcio and recjob.slcio contain 100 +- 1 element where 10 events may fail the check:"
        << endl
        << "    " << progName << " --expelements 100 --abselementerror 1 --abseventerror 10 MCParticle simjob.slcio recjobs.slcio"
        << endl << endl
        << "    check that MCParticle collections in simjob.slcio and recjob.slcio contain 100 +- 1 element where half the events may fail the check:"
        << endl
        << "    " << progName << " --expelements 100 --abselementerror 1 --releventerror .5 MCParticle simjob.slcio recjobs.slcio"
        << endl << endl
    ;
    cerr << msg.str() << endl << endl;
    exit( EXIT_FAILURE );
}

void showopts( void )
{
    cout << endl;
    cout << "-----------------------------------" << endl ;
    cout << "debug infos:" << endl ;
    cout << "-----------------------------------" << endl ;
    cout << "collectionName: " << opts.colName << endl ;
    cout << "pedantic: " << opts.pedantic << endl ;
    cout << "average: " << opts.average << endl ;
    cout << "startevent: " << opts.startevent << endl ;
    cout << "maxevents: " << opts.maxevents << endl ;
    cout << "minelements: " << opts.minelements << endl ;
    cout << "maxelements: " << opts.maxelements << endl ;
    cout << "expelements: " << opts.expelements << endl ;
    cout << "abselementerror: " << opts.abselementerror << endl ;
    cout << "relelementerror: " << opts.relelementerror << endl ;
    cout << "abseventerror: " << opts.abseventerror << endl ;
    cout << "releventerror: " << opts.releventerror << endl ;
    cout << "elementsTotal: " << opts.elementsTotal << endl ;
    cout << "eventsTotal: " << opts.eventsTotal << endl ;
    cout << "eventsFailed: " << opts.eventsFailed << endl ;
    cout << "eventsSkipped: " << opts.eventsSkipped << endl ;
    cout << "-----------------------------------" << endl ;
    cout << endl << endl ;
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
    opts.average = false;
    opts.startevent = 0;      // 0 == not set
    opts.maxevents = 0;       // 0 == not set
    opts.minelements = 0;         // 0 == not set
    opts.maxelements = 0;         // 0 == not set
    opts.expelements = 0;         // 0 == not set
    opts.abselementerror = 0;     // 0 == not set
    opts.abseventerror = 0;     // 0 == not set
    opts.relelementerror = 0;     // 0 == not set
    opts.releventerror = 0;     // 0 == not set
    //initialize counters
    opts.elementsTotal = 0 ;
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

            case 'a':
                opts.average = true;
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
                opts.expelements = atoi(optarg);

                if( opts.expelements < 1 )
                {
                    display_usage( argv[0], "invalid option: expelements must be > 1" );
                }
                break;

            case 'm':
                opts.minelements = atoi(optarg);

                if( opts.minelements < 1 )
                {
                    display_usage( argv[0], "invalid option: minelements must be > 1" );
                }
                break;

            case 'M':
                opts.maxelements = atoi(optarg);
            
                if( opts.maxelements < 1 )
                {
                    display_usage( argv[0], "invalid option: maxelements must be > 1" );
                }
                break;

            
            // long options without a short arg
            case 0:

                if( strcmp( "abselementerror", longOpts[longIndex].name ) == 0 )
                {
                    opts.abselementerror = atoi(optarg);
                
                    if( opts.abselementerror < 1 )
                    {
                        display_usage( argv[0], "invalid option: abselementerror must be > 1" );
                    }
                }

                if( strcmp( "abseventerror", longOpts[longIndex].name ) == 0 )
                {
                    opts.abseventerror = atoi(optarg);
                
                    if( opts.abseventerror < 1 )
                    {
                        display_usage( argv[0], "invalid option: abseventerror must be > 1" );
                    }
                }

                if( strcmp( "relelementerror", longOpts[longIndex].name ) == 0 )
                {
                    opts.relelementerror = atof(optarg);
                
                    if( opts.relelementerror <= 0 || opts.relelementerror >= 1)
                    {
                        display_usage( argv[0], "invalid option: 0 < relelementerror < 1 , (e.g.: 0.3)" );
                    }
                }

                if( strcmp( "releventerror", longOpts[longIndex].name ) == 0 )
                {
                    opts.releventerror = atof(optarg);
                
                    if( opts.releventerror <= 0 || opts.releventerror >= 1)
                    {
                        display_usage( argv[0], "invalid option: 0 < releventerror < 1 , (e.g.: 0.3)" );
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

    //if( opts.minelements == 0 && opts.maxelements == 0 && opts.expelements == 0 )
    //{
    //    display_usage( argv[0], "one of the following options must be set: minelements, maxelements or expelements" );
    //}

    if( opts.abselementerror > 0 && opts.relelementerror > 0 )
    {
        display_usage( argv[0], "abselementerror and relelementerror options are mutually exclusive" );
    }

    if( opts.abseventerror > 0 && opts.releventerror > 0 )
    {
        display_usage( argv[0], "abseventerror and releventerror options are mutually exclusive" );
    }

    if( (opts.abseventerror > 0 || opts.releventerror > 0 ) && opts.average )
    {
        display_usage( argv[0], "average option cannot be used in conjunction with abseventerror or releventerror options" );
    }



    // check if the expected elements option is set
    if( opts.expelements > 0 )
    {
        // look if any error options are used and set min and max elements appropriately
        if( (opts.abselementerror > 0) || (opts.relelementerror > 0) )
        {
            if( opts.relelementerror > 0 )
            {
                opts.abselementerror = (int)round(opts.expelements*opts.relelementerror) ;
                // in case the expression above rounds to 0 we need to set it to 1
                // to ensure minelements and maxelements are set appropriately
                if( opts.abselementerror == 0 )
                {
                    opts.abselementerror = 1 ;
                }
            }

            if( opts.abselementerror > 0 )
            {
                // if minelements option is set it always overwrites other options
                if( opts.minelements == 0 )
                {
                    opts.minelements = (opts.expelements - opts.abselementerror) ;

                    // make sure minelements is always >= 0
                    if( opts.minelements < 0 )
                    {
                        opts.minelements = 0;
                    }
                }

                // if maxelements option is set it always overwrites other options
                if( opts.maxelements == 0 )
                {
                    opts.maxelements = (opts.expelements + opts.abselementerror) ;
                }
            }
        }
        else // if no error options are set, the exact number of elements is expected
        {

            // if minelements option is set it always overwrites other options
            if( opts.minelements == 0 )
            {
                opts.minelements = opts.expelements ;
                if( opts.average )
                {
                    // if average option is used we need to add a +-1 range
                    // to ensure that the avg float number lies between minelements and maxelements
                    opts.minelements-- ;
                }
            }

            // if maxelements option is set it always overwrites other options
            if( opts.maxelements == 0 )
            {
                opts.maxelements = opts.expelements ;
                if( opts.average )
                {
                    // if average option is used we need to add a +-1 range
                    // to ensure that the avg float number lies between minelements and maxelements
                    opts.maxelements++ ;
                }
            }
        }
    }
    else // expected elements option not set
    {
        if( opts.abselementerror > 0 || opts.relelementerror > 0 )
        {
            display_usage( argv[0], "expelements option must be set if using abselementerror or relelementerror" );
        }
    }

    // make sure maxelements is always greater than minelements
    if( opts.maxelements > 0 && ( opts.maxelements < opts.minelements ) )
    {
        display_usage( argv[0], "maxelements must be > minelements" );
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


    LCEvent* event = NULL ;
    LCCollection* elements = NULL ;

    //----------- the event loop -----------
    try{
        while( (event = lcReader->readNextEvent()) != 0 )
        {
            try
            {
                elements = event->getCollection( opts.colName );
            }
            catch( lcio::DataNotAvailableException& e )
            {
                //cerr << "error in event [" << opts.eventsTotal << "] no elements were found" << endl;
                //cerr << e.what() << endl ;
                elements = NULL ;
            }

            if( elements != NULL )
            {
                int nelements = elements->getNumberOfElements() ;
                // cout << opts.colName << " : " << nelements << endl;

                if( nelements < opts.minelements )
                {
                    //cerr << "error in event [" << opts.eventsTotal << "]: minelements condition failed: nelements [" << nelements << "] < minelements [" << opts.minelements << "]" << endl;
                    opts.eventsFailed++ ;
                }

                if( opts.maxelements > 0 )
                {
                    if( nelements > opts.maxelements )
                    {
                        //cerr << "error in event [" << opts.eventsTotal << "]: maxelements condition failed: nelements [" << nelements << "] > maxelements [" << opts.maxelements << "]" << endl;
                        opts.eventsFailed++ ;
                    }
                }
                opts.elementsTotal += nelements ;
            }
            else // event which contains no elements from the given collection
            {
                opts.eventsSkipped++ ;

                if( opts.pedantic )
                {
                    opts.eventsFailed++ ; // in pedantic mode events containing no elements are marked as failed
                }
            }
            //LCTOOLS::dumpEvent( event ) ;
            opts.eventsTotal++ ;

            if( opts.maxevents && opts.eventsTotal >= opts.maxevents )
            {
                break ;
            }
        }
    }
    catch( lcio::IOException& e )
    //catch( ... )
    {
        cerr << "io error when reading data : " << e.what() << endl ;
    }
    // -------- end of event loop -----------

    // free lcReader
    lcReader->close();
    delete lcReader ;


    // --------------- show some stats -------------------------------------------------------------------------

    float avg = 0 ;

    if( opts.eventsTotal > 0 )
    {

        if( opts.pedantic )
        {
            avg = (float)opts.elementsTotal / opts.eventsTotal ;
        }
        else
        {
            if( opts.eventsTotal != opts.eventsSkipped )
            {
                avg = (float)opts.elementsTotal / (opts.eventsTotal - opts.eventsSkipped) ;
            }
        }

        if( opts.releventerror > 0 )
        {
            opts.abseventerror = (int)round(opts.eventsTotal*opts.releventerror) ;
        }

        cout << endl ;
        cout << "-----------------------------------------------------------" << endl ;
        cout << endl ;
        cout << "found " << opts.elementsTotal << " " << opts.colName << " element(s) in " << opts.eventsTotal << " event(s)" << endl ;
        cout << "avg number of " << opts.colName << " elements per event: " << avg << endl ;

        if( opts.eventsSkipped )
        {
            cout << opts.eventsSkipped << " event(s) contained no elements from " << opts.colName << endl ;
        }

        if( opts.eventsFailed > 0 )
        {
            cout << opts.eventsFailed  << " event" << (opts.eventsFailed==1?" has":"s have") << " failed matching the expected number of elements" << endl ;
        }

        if( opts.eventsTotal == opts.eventsSkipped )
        {
            cerr << "no " << opts.colName << " collection was found!" << endl ;
            return EXIT_FAILURE ;
        }
    }
    else
    {
        cerr << "no events were found!" << endl ;
        return EXIT_FAILURE ;
    }

    showopts() ;

    cout << "-----------------------------------------------------------" << endl ;

    if( opts.average )
    {
        if( avg < opts.minelements )
        {
            return EXIT_FAILURE ;
        }

        if( (opts.maxelements > 0) && (avg > opts.maxelements) )
        {
            return EXIT_FAILURE ;
        }
    }
    else
    {
        if( opts.eventsFailed > opts.abseventerror )
        {
            return EXIT_FAILURE ;
        }
    }

    return EXIT_SUCCESS;
}

