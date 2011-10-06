{
TString lcio_dir( gSystem->Getenv( "LCIO" ) );
TString lcio_test_file( lcio_dir + "/examples/cpp/rootDict/DST01-06_ppr004_bbcsdu.slcio" );

// preload lcio libraries - this is needed because the lcio libraries might
// not yet been installed with 'make install' to the $LCIO/lib installation directory
// and this is the location where the example macros look for, so we need to preload the
// ones from the build directory in order to call 'make test' before 'make install'
res = gSystem->Load( "../lib/liblcio.so");
res = gSystem->Load( "../lib/liblcioDict.so");

gROOT->ProcessLine(".x $LCIO/examples/cpp/rootDict/writeEventTree.C( lcio_test_file )" );
}
