{
TString lcio_dir( gSystem->Getenv( "LCIO" ) );
TString lcio_test_file( lcio_dir + "/examples/cpp/rootDict/DST01-06_ppr004_bbcsdu.slcio" );

// preload lcio libraries (need to be in LD_LIBRARY_PATH)
gSystem->Load( "liblcio");
gSystem->Load( "liblcioDict");

gROOT->ProcessLine(".x $LCIO/examples/cpp/rootDict/writeTree.C( lcio_test_file )" );
}
