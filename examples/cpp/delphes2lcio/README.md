# Delphes2LCIO
- author: F.Gaede, DESY
- date June, 2020
- status: EXPERIMENTAL


## Overview
Delphes2LCIO provides an example program for creating an LCIO file from a Delphes simulation.
The format of the LCIO file follows the new **mini-DST** format, that is currently established.
There is only one executable `DelphesSTDHEP2LCIO` that reads `stdhep` files as input.
Other formats can easily be implemented by the user, based on the original programs from Delphes.



-------


## Installation

### Prerequesites:
- installed **LCIO** ([https://github.com/iLCSoft/LCIO](https://github.com/iLCSoft/LCIO))
- installed **ROOT** ([https://root.cern.ch/](https://root.cern.ch/))
- installed **Delphes** ([https://github.com/delphes/delphes](https://github.com/delphes/delphes))

-------

### Getting the source code 

Delphes2LCIO is shipped with LCIO in `$LCIO/examples/cpp/delphes2lcio`. 
You can also directly download a recent copy of *delphes2lcio* via

```
svn co https://github.com/gaede/LCIO/branches/delphes2lcio/examples/cpp/delphes2lcio 
```

later this will be at:

```
svn co https://github.com/iLCSoft/LCIO/trunk/examples/cpp/delphes2lcio 
```
-------


### Build Process

Set the environment:
```
export LCIO=... path to LCIO...
export DELPHES_DIR= ... path to Delphes installation ...
source $ROOT_DIR/bin/thisroot.sh
```

then the build the source
```
cd delphes2lcio
mkdir build
cd build
cmake -D LCIO_DIR=$LCIO  ..

```

----------


## Examples

First updated the path environment variables:

```
export PATH=$PATH:$LCIO/bin:$DELPHES_DIR/bin:$PWD/bin
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$LCIO/lib:$DELPHES_DIR/lib
```


### Running Delphes
Then you can run Delphes with

```
DelphesSTDHEP2LCIO $DELPHES_DIR/cards/delphes_card_ILD.tcl output.slcio input.stdhep
```

This creates an LCIO files with the following default collections:


```
---------------------------------------------------------------------------
COLLECTION NAME               COLLECTION TYPE            DELPHES BRANCH
===========================================================================
Electrons                     ReconstructedParticle        Electron
Jets                          ReconstructedParticle        Jet
MCParticle                    MCParticle                   Particle
MCTruthRecoLink               LCRelation                     n.a.
Muons                         ReconstructedParticle        Muon
PFOs                          ReconstructedParticle        EFlowTrack,EFlowPhoton,EFlowNeutralHadron
Photons                       ReconstructedParticle        Photon
RecoMCTruthLink               LCRelation                     n.a.
---------------------------------------------------------------------------

```


This LCIO file can be analyzed like any other LCIO file with the usual tools,e.g.

- count the number of events in the file:

```
lcio_event_counter output.slcio 
```


- show alll the collections in the events in this file

```
anajob output.slcio | less
```

- detailed dump of the 42nd event:

```
 dumpevent output.slcio 42 | less

```

- a last event with EventSummaries collection is written to the file, it can be inspected with:

```
 dumpevent output.slcio -99 -99 | less

```



Or you can do a full analyis with **Marlin** etc. (see: [http://github.com/iLCSoft](http://github.com/iLCSoft)).


-----------------

### Root macros

Another way of quickly getting started with the analysis is to use ROOT macros.

----------

- first simple example:

```
cd ../examples/
root

root [0] .x ./fill_histos_lcio.C("../build/output.slcio")
```

this creates an output root file with the same base name and path, i.e.

```
root ../build/output.root 
root [3] hetotpfo->Draw()
```

------

- this example creates a higgs recoil mass peak (if run on a suitable stdhep file), e.g.:

```
cd ../examples/
root

root [0] .x higgs_recoil_plots.C("../build/E250-TDR_ws.Pe2e2h.Gwhizard-1_95.eR.pL.I106480.001.mini-DST.slcio")
```

-------

- and this is the same, using the EventSummaries collection for reading only events of interest:

```
root [0] .x higgs_recoil_plots_fast.C("../build/E250-TDR_ws.Pe2e2h.Gwhizard-1_95.eR.pL.I106480.001.mini-DST.slcio")
```
