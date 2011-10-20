{
//TString curr_dir( gSystem->Getenv( "PWD" ) );
//TString root_test_file( curr_dir + "/lcioEventTree.root" );
//cout << root_test_file << endl ;

// preload lcio libraries (need to be in LD_LIBRARY_PATH)
gSystem->Load( "liblcio");
gSystem->Load( "liblcioDict");

gROOT->ProcessLine(".x $LCIO/examples/cpp/rootDict/readEventTree.C( \"lcioEventTree.root\" )" );
}
