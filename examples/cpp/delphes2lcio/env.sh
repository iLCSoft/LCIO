#
# set the environment variables for delphes2lcio
#


# adapt pathes as needed:
export LCIO=/data/ilcsoft/HEAD/lcio/HEAD 
export DELPHES_DIR=~/podio/delphes/install
source /data/ilcsoft/root/6.18.04/bin/thisroot.sh


#=====================================================
export PATH=$PATH:$LCIO/bin:$DELPHES_DIR/bin:$PWD/bin
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$LCIO/lib:$DELPHES_DIR/lib


