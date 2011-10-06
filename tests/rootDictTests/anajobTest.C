{
TString lcio_dir( gSystem->Getenv( "LCIO" ) );
TString lcio_test_file( lcio_dir + "/examples/cpp/rootDict/DST01-06_ppr004_bbcsdu.slcio" );
//cout << lcio_test_file << endl ;

// preload lcio libraries - this is needed because the lcio libraries might
// not yet been installed with 'make install' to the $LCIO/lib installation directory
// and this is the location where the example macros look for, so we need to preload the
// ones from the build directory in order to call 'make test' before 'make install'
gSystem->Load( "../lib/liblcio.so");
gSystem->Load( "../lib/liblcioDict.so");

// doesn't work in batch mode
//.x $LCIO/examples/cpp/rootDict/anajob.C( lcio_test_file );

gROOT->ProcessLine(".x $LCIO/examples/cpp/rootDict/anajob.C( lcio_test_file )" );
}
