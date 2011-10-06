{
//TString curr_dir( gSystem->Getenv( "PWD" ) );
//TString root_test_file( curr_dir + "/lcioEventTree.root" );
//cout << root_test_file << endl ;

// preload lcio libraries - this is needed because the lcio libraries might
// not yet been installed with 'make install' to the $LCIO/lib installation directory
// and this is the location where the example macros look for, so we need to preload the
// ones from the build directory in order to call 'make test' before 'make install'
gSystem->Load( "../lib/liblcio.so");
gSystem->Load( "../lib/liblcioDict.so");

gROOT->ProcessLine(".x $LCIO/examples/cpp/rootDict/readEventTree.C( \"lcioEventTree.root\" )" );
}
