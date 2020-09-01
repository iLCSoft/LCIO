# Delphes2LCIO
- author: F.Gaede, DESY
- date June, 2020



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
make -j 4 install
```

----------


## Examples

First updated the path environment variables:

```
export PATH=$PATH:$LCIO/bin:$DELPHES_DIR/bin:$PWD/bin
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$LCIO/lib:$DELPHES_DIR/lib
```


### Running Delphes
Then you can run Delphes, e.g. with

```
DelphesSTDHEP2LCIO $DELPHES_DIR/cards/delphes_card_ILC.tcl output.slcio input.stdhep
```

**This is requires a recent version of Delphes that has the new ILC card.
This generic ILC Delphes card can also be downloaded from
[https://github.com/iLCSoft/ILCDelphes](https://github.com/iLCSoft/ILCDelphes)**


This creates an LCIO file with the following default collections:


```
---------------------------------------------------------------------------
COLLECTION NAME               COLLECTION TYPE            DELPHES BRANCH
===========================================================================
IsolatedElectrons             ReconstructedParticle        Electron
Jets                          ReconstructedParticle        Jet
MCParticles                   MCParticle                   Particle
MCTruthRecoLink               LCRelation                     n.a.
IsolatedMuons                 ReconstructedParticle        Muon
PFOs                          ReconstructedParticle        EFlowTrack,EFlowPhoton,EFlowNeutralHadron
IsolatedPhotons               ReconstructedParticle        Photon
RecoMCTruthLink               LCRelation                     n.a.
Durham2Jets                   ReconstructedParticle        Durham2Jets
...                           ...                          ...
Durham6Jets                   ReconstructedParticle        Durham6Jets
---------------------------------------------------------------------------

```

See section **Advanced Topics** below how the default collection and branch names can be changed.

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


-----


## Advanced Topics

### Configuration file

The mapping of Delphes branches to LCIO collection by default works for the new ILC delphes card from
[https://github.com/iLCSoft/ILCDelphes](https://github.com/iLCSoft/ILCDelphes).


If needed, the configuration can be changed, for example if your delphes card produces different output
branches such as different jet collections.

For an example configuration see [./examples/delphes2lcio.cfg](./examples/delphes2lcio.cfg).

You can copy and edit this file and the add the following lines to your delphes card


```
#################
# Delphes2LCIO
#################

set Delphes2LCIO_ConfigFileName "../examples/delphes2lcio.cfg"

```

where the path to the config file has to be either relative from your working directory or preferably
an absolute path name.

The following maps are required:

- *MCParticleMap, PFOMap, JetMap, MuonMap, ElectronMap, PhotonMap*.

These maps for extra jet collections are optional (comment out from the configuration file, if not present for your delphes card) :

- *ExtraJetMap2, ExtraJetMap3, ExtraJetMap4, ExtraJetMap5, ExtraJetMap6*.

Additional jet collections can be added as long as their name contains the string `"ExtraJet"` and is different from
all other names. For jet collections the parameter `useDelphes4Vec` defines wether the 4-vector is taken from the delphes
jet (!=0) or wether it is computed from the jet constituent PFOs (default).


Additional PFO maps can also be added. Their names have to start with `"ExtraPFO"`. For example
this will create an extra PFO collection called `BCalPFOs` from the Delphes branch `BCalPhoton`
and assign the photon PDG code:

```
ExtraPFOMapBCal
branchName  BCalPhoton
isCharged  0
lcioName  BCalPFOs
mass  0.
pdg  22

```



#### Event Meta data

Event meta data, such as cross sections or polarization data can also be added to the
configuration file and will then be added to every event as `LCParameters`.

For example add sth. like this at the end of the file:

```
# --------------------------------
EventParametersFloat
CrossSection_fb  1.23456
beamPol1  1.
beamPol2 -1.
Energy  250.
# --------------------------------

```
