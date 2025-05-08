# v02-22-06

* 2025-03-28 jmcarcell ([PR#205](https://github.com/ilCSoft/LCIO/pull/205))
  - Add LANGUAGES CXX in the top level CMakeLists.txt to prevent checking for a C compiler.

* 2025-03-24 Bohdan Dudar ([PR#204](https://github.com/ilCSoft/LCIO/pull/204))
  - Make `getRelatedToMaxWeightAndObject()`, `getRelatedToMaxWeightObject()`, `getRelatedFromMaxWeightObject()` return non-const `LCObject` pointer.

# v02-22-05

* 2025-02-10 Nazar Bartosik ([PR#202](https://github.com/iLCSoft/LCIO/pull/202))
  - Merge the remaining changes that were made to the muon collider fork
    - Make the `anajob.py` example python3 compatible (and remove python2 support) and install it as `anajob.py` executable if `BUILD_ROOTDICT` is enabled.

# v02-22-04

* 2025-02-03 Thomas Madlener ([PR#201](https://github.com/iLCSoft/LCIO/pull/201))
  - Make it possible to create empty subset collections during patching
    - Appending a **`*`** (star) to the collection type name (no spaces) will make this collection an empty subset collection if it is not already present.
    - Fixes [#199 ](https://github.com/iLCSoft/LCIO/issues/199)

# v02-22-03

* 2024-10-20 Valentin Volkl ([PR#197](https://github.com/iLCSoft/LCIO/pull/197))
  - Fix compilation on MacOS by adding missing include in dumpmctree-dot.cc

# v02-22-02

* 2024-09-05 tmadlener ([PR#193](https://github.com/iLCSoft/LCIO/pull/193))
  - Add functionality to `CheckCollections` that makes it possible to add missing ParticleID algorithms to ReconstructedParticle collections
    - This makes it possible to make very consistent event contents that are necessary for conversion to EDM4hep

* 2024-08-01 tmadlener ([PR#196](https://github.com/iLCSoft/LCIO/pull/196))
  - Run Key4hep CI workflows on OSs that are still supported

* 2024-08-01 tmadlener ([PR#195](https://github.com/iLCSoft/LCIO/pull/195))
  - Add a basic `.gitignore` file to avoid accidentally comitting configured / generated files

* 2024-08-01 Thomas Madlener ([PR#194](https://github.com/iLCSoft/LCIO/pull/194))
  - Make sure to require a version of SIO that is consistent with what we would use to build an internal version.

# v02-22-01

* 2024-06-24 jmcarcell ([PR#192](https://github.com/iLCSoft/LCIO/pull/192))
  - Fix possibly wrong behavior with `std::remove_if` with a `erase - remove` idiom

* 2024-06-24 Wouter Deconinck ([PR#191](https://github.com/iLCSoft/LCIO/pull/191))
  - fix: parentheses in SIOTrack.java

# v02-22

* 2024-06-07 tmadlener ([PR#189](https://github.com/iLCSoft/LCIO/pull/189))
  - Remove mentions of the removed F77 API (see [#161](https://github.com/iLCSoft/LCIO/pull/161)) from the documentation

* 2024-06-06 tmadlener ([PR#190](https://github.com/iLCSoft/LCIO/pull/190))
  - Remove the no longer used settings for the 32bit compatibility mode

* 2024-06-06 Nazar Bartosik ([PR#147](https://github.com/iLCSoft/LCIO/pull/147))
  - Add `Nholes` and and `subdetectorHoleNumbers` to the `Track` for keeping track of missing hits in a Track.

* 2024-05-08 Bohdan Dudar ([PR#170](https://github.com/iLCSoft/LCIO/pull/170))
  - Added new utility `dumpmctree` to draw the MC table of the event stored in the slcio file as the graphviz tree diagram, which represents parent-daughter relations visually in a easier way.
    - `dumpmctree` is a small wrapper script around the actual `dumpmctree-dot` executable. The latter produces a `.dot` file which is then transformed into an `.svg` file via the wrapper script and the `dot` executable.
    - The script relies on `dot` & `xdg-open` to be available on your system.

* 2024-04-15 tmadlener ([PR#188](https://github.com/iLCSoft/LCIO/pull/188))
  - Make the `PIDHandler` usable as `const` object by marking getters that do not mutate internal state as `const`

# v02-21

* 2024-01-15 Thomas Madlener ([PR#187](https://github.com/iLCSoft/LCIO/pull/187))
  - Switch to c++20 for the key4hep nightlies based CI

* 2024-01-11 tmadlener ([PR#184](https://github.com/iLCSoft/LCIO/pull/184))
  - Make sure that LCIO installations can be used in downstream projects if they are built with builtin SIO. Fixes https://github.com/iLCSoft/LCIO/issues/183
    - Add a test to make sure that this works as intended
  - Bump the minimum required version for SIO to v00-01 in order to have CMake targets available.

* 2023-12-15 jmcarcell ([PR#175](https://github.com/iLCSoft/LCIO/pull/175))
  - Bump the minimum CMake version to 3.14

* 2023-12-13 jmcarcell ([PR#178](https://github.com/iLCSoft/LCIO/pull/178))
  - Bump the SIO version to v00-02
  - Fix two compiler warnings with clang

* 2023-12-06 Andre Rummler ([PR#181](https://github.com/iLCSoft/LCIO/pull/181))
  - Replace the copy of SIO that is used for builtin SIO support with the necessary cmake configuration to fetch it on the fly via CMake's `FetchContent` as this simplifies the maintenance of the vendored version of SIO greatly. **This is a transparent change for users, as long as internet connection to fetch the SIO sources during building is available**

# v02-20-02

* 2023-12-06 tmadlener ([PR#182](https://github.com/iLCSoft/LCIO/pull/182))
  - Also put pre-generated headers into include directories of LCIO target to make in-source builds (e.g. via. CMake's FetchContent) possible.

* 2023-12-01 tmadlener ([PR#180](https://github.com/iLCSoft/LCIO/pull/180))
  - Populate the `target_include_directories` of the `LCIO::lcio` target to make it properly usable downstream.

# v02-20-01

* 2023-11-14 jmcarcell ([PR#177](https://github.com/iLCSoft/LCIO/pull/177))
  - Export targets and make sure they can be used by other CMake projects

* 2023-09-07 jmcarcell ([PR#176](https://github.com/iLCSoft/LCIO/pull/176))
  - Make an alias to `SIO::sio` when building SIO within LCIO so that it has the same target as the standalone SIO, that has the target `SIO::sio`

* 2023-08-15 jmcarcell ([PR#174](https://github.com/iLCSoft/LCIO/pull/174))
  - Make cmake respect CMAKE_INSTALL_<dir>

* 2023-07-17 jmcarcell ([PR#173](https://github.com/iLCSoft/LCIO/pull/173))
  - Drop FindCLHEP.cmake

* 2023-07-17 jmcarcell ([PR#172](https://github.com/iLCSoft/LCIO/pull/172))
  - Don't make the uninstall target if it already exists

* 2023-06-13 Leonhard Reichenbach ([PR#171](https://github.com/iLCSoft/LCIO/pull/171))
  - LCRelationNavigator: add methods `getRelatedToMaxWeightAndObject` and `getRelatedFromMaxWeightAndObject` returning both the object and the weight in a tuple

# v02-20

* 2023-05-30 Andre Sailer ([PR#168](https://github.com/iLCSoft/LCIO/pull/168))
  - Pregenerated Headers: remove self-include from some headers (breaks include-what-you-use)
  - LCIterator, LCRTRelations: remove template syntax causing errors in gcc13/c++20
  - RunEvent, LCObject, TrackStateImpl: added default copy and move constructor and assignment operator to avoid error about "'definition of implicit copy constructor for 'LCObject' is deprecated because it has a user-declared destructor'"

* 2023-05-12 tmadlener ([PR#167](https://github.com/iLCSoft/LCIO/pull/167))
  - Fix checking of collection types to make sure patching works correctly

* 2023-05-03 Thomas Madlener ([PR#166](https://github.com/iLCSoft/LCIO/pull/166))
  - Upgrade `python-lint` workflow to run on `ubuntu-lates` since `ubuntu-18.04` runners have been removed.

* 2023-05-03 Finn Johannsen ([PR#165](https://github.com/iLCSoft/LCIO/pull/165))
  - Fixes to the on the fly collection patching that are necessary for the LCIO to EDM4hep standalone conversion.
    - Make `CheckCollections` check the `FromType` and `ToType` collection parameters to figure out the involved types for `LCRelations`. Add them to the output of `CheckCollections::print`
    - Make the `CheckCollectoins::patchCollections` parse these strings back for `LCRelation` collections and set them as collection parameters for collections it creates on the fly.
    - Add a `--minimal` flag to `check_missing_cols` in order to make it possible to produce outputs that can be more easily consumbed by other programs.

* 2023-05-03 Thomas Madlener ([PR#164](https://github.com/iLCSoft/LCIO/pull/164))
  - Add Key4hep release based CI workflow
  - Fix remaining warnings to enable `-Werror`
  - Update *checkout* action to v3, since v2 is deprecated. 
  - **CLHEP >= 2.0** is now required for building the examples that use CLHEP functionality (`test_fourvector`).

* 2023-02-10 jmcarcell ([PR#162](https://github.com/iLCSoft/LCIO/pull/162))
  - Add test dependencies so that tests can run in parallel

* 2023-02-10 jmcarcell ([PR#161](https://github.com/iLCSoft/LCIO/pull/161))
  - Remove the deprecated C API and fortran bindings (c.f. [#137](https://github.com/iLCSoft/LCIO/pull/137) and [#151](https://github.com/iLCSoft/LCIO/issues/151))

# v02-19-01

* 2023-02-06 Bohdan Dudar ([PR#163](https://github.com/iLCSoft/LCIO/pull/163))
  - `getRelatedTo(From)MaxWeightObject()` and `getRelatedTo(From)MaxWeight()` now accept generic decode function of `float(float)` signature as a second argument, which specifies how to decode the weight. Default option is identity function (just compares weights as they are).
  - Helper functions to decode and encode "track"/"cluster" specific weights from PFO-MCParticle LCRelation collection are added to MarlinUtil in [MarlinUtil#36](https://github.com/iLCSoft/MarlinUtil/pull/36).

* 2023-02-03 jmcarcell ([PR#160](https://github.com/iLCSoft/LCIO/pull/160))
  - Fix a compiler warning about `strncpy` usage

# v02-19

* 2022-12-11 Frank Gaede ([PR#158](https://github.com/iLCSoft/LCIO/pull/158))
  - add utility class `CheckCollections` that allows to parse lcio files for collections that are not present in every event and to patch such events with empty collections of the given (Name,Type) for further processing
  - add example tool `check_missing_cols` that checks for collections that are not in every event in a set of files and prints a summary to stdout:
       - `usage:  check_missing_cols <input-file1> [[input-file2],...]`
  - add example tool `patch_missing_cols` that creates a copy of the input file with the same set of collections in all events:
      -  ` usage:  patch_missing_cols <input-file> <output-file> ` 
  - these tools are needed in cases where code expects consistent sets of collections in every event, as for example in a conversion to `EDM4hep`

# v02-18

* 2022-11-08 Thomas Madlener ([PR#155](https://github.com/iLCSoft/LCIO/pull/155))
  - Add a previously missed function declaration to the .aid file to fix the java bindings. Fixes #154

* 2022-10-19 Thomas Madlener ([PR#153](https://github.com/iLCSoft/LCIO/pull/153))
  - Make c++17 the default and minimum c++ version for building LCIO. All "major builds" of LCIO have been using c++17 for at least a few years now, so this should not be a big issue.

* 2022-10-19 Thomas Madlener ([PR#152](https://github.com/iLCSoft/LCIO/pull/152))
  - Remove macOS workflow since github hosted runners no longer support all necessary features. See also: https://github.com/AIDASoft/run-lcg-view/pull/3
  - Update used github actions to latest available version for better caching behavior and cleaner log outputs
  - Switch to newer LCG releases for the build environments to target more recent compilers and python versions.
    - Keep one build environment that is close to the one used for the iLCSoft v02-02 to avoid accidental breaks.

* 2022-10-19 Bohdan Dudar ([PR#150](https://github.com/iLCSoft/LCIO/pull/150))
  - Added a utility function to calculate Track momentum based on its track parameters and magnetic field
  - Added methods to the LCRelationNavigator that extract the highest weight with an option to indicate weight encoding type ("track"/"cluster").
  - Added a utility function to get an index of a provided object inside a given LCCollection
  - Added a utility function to return a leading track from ReconstructedParticle in case it has multiple tracks attached.

# v02-17-01

* 2022-05-09 Thomas Madlener ([PR#146](https://github.com/iLCSoft/LCIO/pull/146))
  - Install all the necessary headers to make the python bindings work, even if the sources are removed after installation. Fixes #106

# v02-17

* 2021-11-05 Frank Gaede ([PR#143](https://github.com/iLCSoft/LCIO/pull/143))
  - add support for storing double values in LCParameters
      - used in run, event and collection parameters
      - example 
  
  ```cpp
  	DoubleVec dv ;
  	dv.push_back( 1.111111111111111111111111111111111111111111111111 ) ;
  	dv.push_back( 2.222222222222222222222222222222222222222222222222 ) ;
  	dv.push_back( 3.333333333333333333333333333333333333333333333333 ) ;
  	evt->parameters().setValues( "SomeDoubleNumbers" , dv ) ;
  
  ```
  - should resolve #138

* 2021-11-05 Bohdan Dudar ([PR#141](https://github.com/iLCSoft/LCIO/pull/141))
  - LCTrackerConf constructor now protected

* 2021-10-12 Thomas Madlener ([PR#137](https://github.com/iLCSoft/LCIO/pull/137))
  - Deprecate the C-API which is used by the fortran interface. However, since no one seems to be actively using that interface we introduce a deprecation warning for the C-API to see if that has any users outside of the internal fortran interface. **If you are seeing deprecation messages in your build outputs please let us know.**
  - Fix F77 tests and run them in the CI.
    - Degrade some compiler errors back to warnings for gcc10 as it has become more strict than previous versions.

* 2021-05-05 Thomas Madlener ([PR#136](https://github.com/iLCSoft/LCIO/pull/136))
  - Update the CI to use the cvmfs and lcg-view github actions for a more streamlined configuration. Fixes #135.
  - Make the python dictionary loading look on `LD_LIBRARY_PATH` first, before falling back to rely on the `LCIO` environment variable which has more assumptions built into it. Fixes #134

* 2021-05-05 Andrii Verbytskyi ([PR#132](https://github.com/iLCSoft/LCIO/pull/132))
  - Replace hardcoded lib with CMAKE_INSTALL_LIBDIR when appropriate

# v02-16-01

* 2021-01-06 Remi Ete ([PR#129](https://github.com/iLCSoft/LCIO/pull/129))
  - Fixed skip record condition in Random Access Manager

# v02-16

* 2020-12-03 Marko Petric ([PR#127](https://github.com/ilcsoft/lcio/pull/127))
  - Fix Python3 compatibility issue with next method defined

* 2020-12-03 Frank Gaede ([PR#126](https://github.com/ilcsoft/lcio/pull/126))
  - no longer build the out-of-date lsh example

* 2020-12-03 Marko Petric ([PR#125](https://github.com/ilcsoft/lcio/pull/125))
  - Migrate CI to GitHub Actions

* 2020-12-03 Remi Ete ([PR#124](https://github.com/ilcsoft/lcio/pull/124))
  - Updated SIO sources to v00-00-04

* 2020-12-03 Frank Gaede ([PR#123](https://github.com/ilcsoft/lcio/pull/123))
  - fix some warning:
         - deprecated dynamic exception specifications in Exceptions
  - fix the build from generated C++ files

* 2020-12-03 Frank Gaede ([PR#122](https://github.com/ilcsoft/lcio/pull/122))
  - fix in `UTIL::ProcessFlag`
        - fixed const correctness and make the operator<<(ostream) inline

* 2020-11-25 Marko Petric ([PR#121](https://github.com/ilcsoft/lcio/pull/121))
  - Update compiler flag handling
  - Resolve which type of linker is used and assign proper linking flags
  - Improve RPATH handling on macOS and add flag `LCIO_SET_RPATH` default `ON`

* 2020-11-05 Frank Gaede ([PR#119](https://github.com/ilcsoft/lcio/pull/119))
  - make compatible w/ clang 12 (on MacOs)
      - remove const from map key in LCRTRelations
      - map keys are immutable anyways

* 2020-10-23 Frank Gaede ([PR#117](https://github.com/ilcsoft/lcio/pull/117))
  - fix delphes2lcio
         - replace GetEntriesFast w/ GetEntries
         - fixes #113

# v02-15-04

* 2020-10-14 Frank Gaede ([PR#115](https://github.com/ilcsoft/lcio/pull/115))
  - fix in `delphes2lcio` example
       - avoid NANs in jet 4-momenta by using zero mass hyptheses
       - NB: the mass is still set independently to the specified value (following PandoraPFA)

* 2020-09-02 Frank Gaede ([PR#111](https://github.com/ilcsoft/lcio/pull/111))
  - delphes2lcio: fix PDG in PID objects of neutral hadrons

* 2020-09-01 Frank Gaede ([PR#110](https://github.com/ilcsoft/lcio/pull/110))
  - delphes2lcio: add optional extra PFO collections
          - add by default BCalPFOs created from branch BCalPhotons

# v02-15-03

* 2020-08-26 Frank Gaede ([PR#108](https://github.com/ilcsoft/lcio/pull/108))
  - fix in UTIL::ProcessFlag::decodeMCTruthProcess
        - protection for events where nMCParticles < 10
  - fix also, independently in delphes2lcio

# v02-15-02

* 2020-08-19 Frank Gaede ([PR#107](https://github.com/ilcsoft/lcio/pull/107))
  - add optional event meta data in delphes2lcio
        - meta data like cross sections can be added via the configuration file
        - see [../examples/cpp/delphes2lcio/README.md](../examples/cpp/delphes2lcio/README.md) for details

# v02-15-01

* 2020-08-14 Frank Gaede ([PR#104](https://github.com/ilcsoft/lcio/pull/104))
  - fix the compression level:  use zlib default compression again
          in `SIOWriter` and `MT::LCWriter`
  - update the build instructions with ROOT in README.md
         - and set LD_LIBRARY_PATH in setup.sh

# v02-15

* 2020-08-13 Frank Gaede ([PR#102](https://github.com/ilcsoft/lcio/pull/102))
  - fix `ProcessFlag::decodeMCTruthProcess()`
        - remove ISR photons in final state for DBD 250 GeV files

* 2020-08-12 Frank Gaede ([PR#101](https://github.com/ilcsoft/lcio/pull/101))
  - make delphes2lcio compatible w/ LCIO 2.14 again
           - by not calling `ProcessFlag::decodeMCTruthProcess()`
  - add `higgsss` and `unknown` to `ProcessFlag`
  - update logic in `ProcessFlag::decodeMCTruthProcess()` (from J.List, DESY)

* 2020-08-11 Frank Gaede ([PR#100](https://github.com/ilcsoft/lcio/pull/100))
  - improve the `delphes2lcio` tool
          - changes some default collection names
                  - Electrons/ Muons/ Photons -> IsolatedElectrons / IsolatedMuons / ...
                  - MCParticle -> MCParticles
          - update all examples and Readme.md accordingly
          - add event params: crossSection and ProcessID as available in Delphes
  - add method `ProcessFlag decodeMCTruthProcess(const EVENT::LCCollection*) `
          - with first example implementation -> needs iteration
          - call this in `delphes2lcio`
  - use `const EVENT::LCCollection*` in `UTIL::LCIterator`

* 2020-08-11 Frank Gaede ([PR#99](https://github.com/ilcsoft/lcio/pull/99))
  - fix memory leak in SIOReader
       - delete previous Event and RunHeader before reading a new one
       - this restores the old expected behavior of the `IO::LCReader`
       - for parallel event reading one needs to use the `MT::LCReader` directly and
          handle the memory accordingly (e.g. via use of std::unique_ptr)
               - see [$LCIO/src/cpp/src/EXAMPLE/lcio_parallel_processing.cc](../src/cpp/src/EXAMPLE/lcio_parallel_processing.cc)
  - fixes #97 and #98

* 2020-08-07 flagarde ([PR#96](https://github.com/ilcsoft/lcio/pull/96))
  - fix some warnings w/ exceptions

* 2020-08-07 flagarde ([PR#94](https://github.com/ilcsoft/lcio/pull/94))
  - Remove FindROOT.cmake
  - Use ROOTConfig.cmake provided by ROOT
  - Change ROOT_CINT_WRAPPER to ROOT_rootcint_CMD as it's provided by ROOT 6.04.18 ROOTConfig.cmake
       - fixes #68

* 2020-08-07 flagarde ([PR#94](https://github.com/ilcsoft/lcio/pull/94))


* 2020-07-22 JennyListDESY ([PR#92](https://github.com/ilcsoft/lcio/pull/92))
  - moved . ./setup.sh upwards as part of the installation instructions

* 2020-07-22 Marko Petric ([PR#91](https://github.com/ilcsoft/lcio/pull/91))
  - fix bug in build dependency, affecting only build on large number of cores

* 2020-07-22 JennyListDESY ([PR#90](https://github.com/ilcsoft/lcio/pull/90))
  - inserted a missing cd LCIO to the instructions

# v02-14-02

* 2020-07-09 Ete Remi ([PR#88](https://github.com/iLCSoft/LCIO/pull/88))
  - Fixed random access manager record reading
     - Crash observed for old files without random access record
  - Added unit test for random access record
     - Run `anajob` on a file without random access record

# v02-14-01

* 2020-07-02 Frank Gaede ([PR#87](https://github.com/ilcsoft/lcio/pull/87))
  - updated  documentation
      -  README.md with some basic introduction 
      - added slightly updated manual.pdf
  - minor fixes in delphes2lcio

# v02-14

* 2020-07-01 Frank Gaede ([PR#86](https://github.com/ilcsoft/lcio/pull/86))
  - new classes  `UTIL::EventSummary` and `UTIL::ProcessFlag`
          - used in delphes2lcio for writing event summary information and 
             encoding the generated Monte Carlo process 
  - improve delphes2lcio example
       - fill more quantities for EventSummary ( see  UTIL::EventSummary )
       - updated examples

* 2020-07-01 Remi Ete ([PR#85](https://github.com/ilcsoft/lcio/pull/85))
  - Fixed LCGenericObject collection flag IO writing
  - Fixed virtual override warning in SIOReader

* 2020-06-30 Ete Remi ([PR#79](https://github.com/ilcsoft/lcio/pull/79))
  - Externalized SIO package: https://github.com/iLCSoft/SIO
  - Use new SIO implementation:
    - Ensure thread-safety for multiple LCReader / LCWriter instance (removal of globals)
    - Possibility for adding new compression algorithm in future
    - Standard LCReader and LCWriter use the MT versions (see below) for implementation under the hood
  - Added new `MT::LCReader` and `MT::LCWriter`
     - Different interface for reading and writing events. Use smart pointers for event/run ownership
     - MT::LCWriter allows for writing concurrently event in a file from multiple threads. Event encoding and compression is standalone, so thread-safe, and I/O writing is done under a (fast) mutex.
  - Introduce new creation flag for standard LCReader: **LazyUnpacking**
     1. Reads an event from file 
     2. Move the compressed buffer inside an LCEvent object
     3. Perform un-compression and decoding only on demand when `LCEvent::getCollection()` is called
  - LCIO objects uids increment is now atomic (`AccessChecked`)
  - Introduced runtime extensions for LCEvent, accessible via `LCEvent::runtime()` method.
  - Introduced `LCIO_DEPRECATED` macro for deprecated methods in LCIODeprecated.h
  - LCIO extensions:
    - Removed global destructor mapping for each relation type, because not thread safe
    - Turned internal extension type to `std::shared_ptr<void>` to use type-safe extension destructor
  - Added example binaries:
     - `lcio_parallel_processing`: a small example to illustrates how to use the new MT classes. Implements a simple scheduling algorithm for parallel event processing
     - `lcio_parallel_read`: a small example of how to dump events and runs from multiple files at the same time in parallel
     - `lcio_performance`: printout file reading performance with and without lazy unpacking
  - Unit testing:
     - added unit test for lazy unpacking feature
     - switched back ON the unit test on RT extensions
  - Bug fixes
     - CXX standard was hardcoded to 14. Now set it to 14 if not specified via command line with i.e `-DCMAKE_CXX_STANDARD=17`  
  - Packaged builtin SIO in LCIO
     - Compile the SIO library (only) if not found on your system

* 2020-06-25 Frank Gaede ([PR#84](https://github.com/ilcsoft/lcio/pull/84))
  - improve the delphes2lcio tool
       - add configuration options for LCIO collection names and delphes branches to be used
       - the default configuration works for the ILD delphes card [https://github.com/ILDAnaSoft/ILDDelphes](https://github.com/ILDAnaSoft/ILDDelphes)
       - it can be overwritten with a simple configuration file see ./examples/delphes2lcio.cfg and *Advanced Topics* in the README.md
       - additional jet collections can be added to the event if available in the delphes output
               - current default are *Durham2Jets-Durham6Jets*

* 2020-06-17 Frank Gaede ([PR#82](https://github.com/ilcsoft/lcio/pull/82))
  - improve delphes2lcio example
        - add an `EventSummaries` collection to a last dummy event with evtNum=-99 runNum=-99
        - add example `higgs_recoil_plots_fast.C` demonstrating the usage

* 2020-06-15 Frank Gaede ([PR#81](https://github.com/ilcsoft/lcio/pull/81))
  - fixes for delphes2lcio example:
        - fix names of isolated particle collections
        - add example for drawing higgs recoil peak

* 2020-06-12 Frank Gaede ([PR#80](https://github.com/ilcsoft/lcio/pull/80))
  - add example delphes2lcio 
        - writes LCIO mini-DSTs from a Delphes simulation
        - see [../examples/cpp/delphes2lcio/README.md](../examples/cpp/delphes2lcio/README.md) for details

* 2020-04-17 Pere Mato ([PR#77](https://github.com/ilcsoft/lcio/pull/77))
  - Fine tune the latest fix to cope with new PyROOT to use old API for versions 6.20.X

# v02-13-03

* 2020-04-07 SFT Nightlies ([PR#74](https://github.com/ilcsoft/lcio/pull/74))
  - Small changes in HandleExceptions.py  to cope with the change of API foreseen for ROOT 6.22

# v02-13-02

* 2020-03-12 Marko Petric ([PR#72](https://github.com/iLCSoft/lcio/pull/72))
  -  Use "Always full RPATH" as as implemented in ROOT

# v02-13-01

* 2019-11-15 Frank Gaede ([PR#67](https://github.com/ilcsoft/LCIO/pull/67))
  - fix building for MacOS 10.14
      - rm superfluous `#include "/usr/include/sys/types.h"` 
         (thanks P.Mato)

* 2019-10-30 Remi Ete ([PR#66](https://github.com/ilcsoft/LCIO/pull/66))
  - Moved all stream operator overloads of EVENT class types into EVENT namespace (moved out from UTIL namespace)

# v02-13

* 2019-10-15 Marko Petric ([PR#61](https://github.com/iLCSoft/LCIO/pull/61))
  - Make pyLCIO compatible with Python3

* 2019-09-25 Frank Gaede ([PR#64](https://github.com/iLCSoft/LCIO/pull/64))
  - fix the writeEventTree test  (fixes #62)

* 2019-09-25 Marko Petric ([PR#63](https://github.com/iLCSoft/LCIO/pull/63))
  - Update CI to newer version of ROOT 6.18/04 (LCG_96b)
  - Fix issue that in the build folder the rootdicts were in the wrong location

* 2019-08-08 Frank Gaede ([PR#60](https://github.com/iLCSoft/LCIO/pull/60))
  - re-introduce `std::set_terminate` in `LCIOExceptionHandler`, which is not deprecated in c++17

* 2019-08-08 Ete Remi ([PR#54](https://github.com/iLCSoft/LCIO/pull/54))
  - Added executable to perform IO benchmarking on LCIO file reading

* 2019-07-19 Remi Ete ([PR#59](https://github.com/iLCSoft/LCIO/pull/59))
  - Added `lcio_perf` utility. Evaluate:
       - Total / mean writing time for 5000 events (sim job)
       - Total / mean reading time for 5000 events

* 2019-07-09 Marko Petric ([PR#58](https://github.com/iLCSoft/LCIO/pull/58))
  - Remove `std::set_unexpected` and `std::set_terminate` which were removed in C++17

# v02-12-01

* 2018-06-21 Frank Gaede ([PR#48](https://github.com/ilcsoft/lcio/pull/48))
  - protect against invalid ParticleID objects in `LCTOOLS::printReconstructedParticles()`
           - these where added as a nasty side effect in `PIDHandler::getParticleID()`
  - fix `PIDHandler::getParticleID()` accordingly
  - fixes #47

# v02-12

* 2018-01-10 Frank Gaede ([PR#43](https://github.com/ilcsoft/lcio/pull/43))
  -  Remove dynamic exception specification from C++ headers

* 2018-01-12 Frank Gaede ([PR#46](https://github.com/ilcsoft/lcio/pull/46))
  - fix unused warning in test_randomaccess.cc 
  - bug fix in cmake/LCIOConfig.cmake.in (fixes DD4hep part of #45)

* 2018-01-12 Frank Gaede ([PR#44](https://github.com/ilcsoft/lcio/pull/44))
  - fix last warnings (gcc5.4)
       - dont create LCIOLibDeps.cmake (fix Policy CMP0033) 
       - fix warnings in UTIL::LCFourVector

* 2018-03-23 Frank Gaede ([PR#35](https://github.com/ilcsoft/lcio/pull/35))
  - Remove using namespace from header files

# v02-11

* 2017-11-10 Frank Gaede ([PR#41](https://github.com/ilcsoft/lcio/pull/41))
  - add the step length to the MC-Contributions of the `SimCalorimeterHit`
    - add method `SimCalorimeterHit::getLengthCont(int) ` to access it 
    - only stored if `LCIO.CHBIT_STEP` is set (detailed shower mode)
    - implemented in C++ and Java (!?)
    - updated version number to v02-11
    - fixed some versions in ./cmake/lcio.xml.in

# v02-10-01

* 2017-10-12 Frank Gaede ([PR#40](https://github.com/ilcsoft/lcio/pull/40))
  - ensure correct version numbers in this and future releases
      - have all files that contain a version number configured by CMake
      - will allow to use tagging script for GitHub to make a new release
  - no code changes wrt. v02-10

# v02-10

* 2017-07-07 Andre Sailer ([PR#36](https://github.com/ilcsoft/lcio/pull/36))
  - CellIDDecoder: use static string instead of static pointer to string for defaultEncoding

* 2017-09-28 Marko Petric ([PR#38](https://github.com/ilcsoft/lcio/pull/38))
  - Correct LCIO_MINOR_VERSION

# v02-09

* 2017-05-08 Sebastien Binet ([PR#27](https://github.com/ilcsoft/lcio/pull/27))
  - add an example using `LCIO` from `Go`

* 2017-05-29 Marko Petric ([PR#30](https://github.com/ilcsoft/lcio/pull/30))
  - Added flag -Wheader-hygiene to warn about usage of namespace directive in global context in header

# v02-08

# v02-08

2017-04-11 Frank Gaede ([PR#26](https://github.com/iLCSoft/LCIO/pull/26))

- fix all remaining compiler warnings from -Weffc++
  - using -Wno-non-virtual-dtor ( requires gcc 6 ) on older compilers these warnings will appear but can be safely ignored

2017-03-03 Andre Sailer ([PR#24](https://github.com/iLCSoft/LCIO/pull/24))
- LCTrackerConf: bugfix for checks of encoding string field order

2017-03-03 Marko Petric ([PR#23](https://github.com/iLCSoft/LCIO/pull/23))
- Integate CI with Coverity scan via Travis cron jobs

2017-03-01 Marko Petric ([PR#22](https://github.com/iLCSoft/LCIO/pull/22))
- Update to the CI system:
  - Install directly cvmfs on base system, which removes the need for the parrot connector 
  - Replace CernVM docker with plain docker
  - This reduces the build run time from 15 min to 5 min

2017-03-06 Andre Sailer ([PR#21](https://github.com/iLCSoft/LCIO/pull/21))
- SimTrackerHit: Add Quality bitfield


2017-03-06 Andre Sailer ([PR#19](https://github.com/iLCSoft/LCIO/pull/19))
- LCTrackerConf: allow first field to be called "system"


# v02-07-05

Andre Sailer 2017-03-27 
  - ReleaseNotes: change formatting to markdown, tag headers only

Marko Petric 2017-03-23 
  - Update CONTRIBUTING
  - Add CONTRIBUTING.md and PULL_REQUEST_TEMPLATE

Oleksandr Viazlo 2017-03-21 
  - Replace ILDCellID0 with LCTrackerCellID

Frank Gaede 2017-03-17 
  - add LCTrackerConf.h with LCTrackerCellID

Frank Gaede 2017-03-02 
  - update ilcsoft_default_cxx_flags.cmake from ilcutil

Frank Gaede 2017-02-28 
  - fix all warnings in LCIO (llvm/clang 8.0)

Marko Petric 2016-12-14 
  - Add Coverity scan

Frank Gaede 2016-12-14 
  - fix lookup pathes of MacroCheckDep if relocated

Marko Petric 2016-12-08 
  - Fix diagnostic color handling
  - Add diagnostic color to external cmake flags
  - Update diagnostics-color to auto
  - Add build status to README
  - Include also building of ROOT dictionaries
  - Add diagnostics-color flag to compiler if supported
  - Clarify undefined symbol lookup on different linkers
  - Add CI

Frank Gaede 2016-12-08 
  - Update README.md
  -  replace README.html w/ README.md
  -  add LICENCE ( BSD3c ) to LCIO


# v02-07-04

F.Gaede
  - make LCIO relocatable ( as suggested by M.Petric/A.Sailer)
     - install cmake macros
     - use CMAKE_CURRENT_LIST_DIR in cmake config files

  - remove const from ILDCellID0::encoder_string
    - so it can be changed for CLIC/SiD as 
      as workaround  

# v02-07-03

F. Gaede:

    - fix issue w/ installing pcm files in cmake script 

 J.McCormick: 

    - apply Maurik Holtrop's patch for handling the case where 
      collection types change in C++ reader.
 
# v02-07-02

 F.Gaede:  - add EXAMPLE/readmcparticles.cc
           - fix some documentation 
                - Cluster: itheta/iPhi and deprecated methods in MCParticle
           - bug fix in  SIOReader::skipNEvents: 
                - allow to call with 0 as argument 
           - fix handling of default CXX_FLAGS 
             - can be overwritten on command line
           - install also the python source code and examples (M.Petric)

 J.McCormick: Implement handling of daughter to mother relationships in StdHep reader; 
              use separate classes for now:  LCStdHepRdrNew ( and EXAMPLE/stdhepjob_new.cc )
              - pending more testing.

# v02-07-01

 - made compatible with c++11
    - removed old flags: -Wall -ansi -pedantic -Wno-long-long -fno-strict-aliasing
    - fixed some "narrowing in initializer list" in examples
 - optional ROOT dictionary now requires ROOT6
    - adapted cmake code for generating the dictionary
    - adapted the example/test macros

  - no changes in lcio source code or EDM

# v02-07

 N.Graf
  - Added support for new momentumAtEndpoint feature
    in Java

 F.Gaede 
     C++ :
    - added MCParticle::getMomentumAtEndpoint()

    - removed long deprecated methods
       - MCParticle::getNumberOfParents()
       - MCParticle::getParent(int i)
       - MCParticle::getNumberOfDaughters()
       - MCParticle::getDaughter(int i)

    - fixed many compiler warnings for clang

    - added example for setting particle id to recjob

    - added IDs for coild and calo endcaps to UTIL/ILDConf.h

    - fixed setting of CMAKE_INSTALL_PREFIX to allow
      installation in 'usr/local'

    - fixed incorrect file names printed at the end of anajob

    - fixed compiler warnings for clang and c++11 in TESTs:
      non-constant-expression cannot be narrowed from type 'double' to 'float' in initializer list [-Wc++11-narrowing]

 T.Bisanz
    - made UTIL::LCTime and UTIL::LCStdHepRdr compatible with gcc 5.3.1
   
# v02-06
    - patched C++ documentation (F.Gaede):
      - create a search field on the doxygen page
      - allow to create doxygen only (no latex and/or html)

    - added method  ParticleIDVec UTIL::PIDHandler::getParticleIDs() (F.G.)

    - improved printing of ParticleIDs for ReconstructedParticles (F.G.)
      (e.g. in dumpevent)

    - use Jaxen runtime in siodump (J.McCormick)
 
    Note: in the previous release (v02-05) the versions for libraries and 
          documentation had not been updated and all showed v02-04-03,
          even though it had been tagged as v02-05. As this has been fixed
          now, a rebuid is necessary when installing this patch release.


# v02-05

	- Several changes by J. McCormick on the JAVA side:
		Work on LCIO-31 and LCIO-32.
		Fix for LCIO-30 by updating freehep-sio version.
		Updated the build system

	- Fixed setPositionVec (different types of hits in calo and tracker) - C. Grefe
	- Added proper handling of exceptions. Whenever an LCIO C++ exception is thrown, the corresponding python exception is raised instead - C. Grefe
	- Fixed a bug in getNumberOfEvents (spotted by Eduard) - C. Grefe	

	- write udrup parameter also when it is 0
      - turned LCIO_GENERATE_HEADERS off ( generation of headers with java is broken )


# v02-04-03

	- Added a statement about the minimum python version to use

	- Adapted the example to the changed interface in EventLoop.



# v02-04-02

     - add template std::string toString(const T* obj) for
       easy printing w/o operators (e.g. from Python)

     - added missing pragma link statements for Track classes
       (needed for ROOT dictionary/python binding)
     
     - cleanup of Cmake code for BUILD_ROOTDICT
     
     - fixed (again) target dylib2so for APPLE

     - Updated the StdHepReader to use the exact same interface as the LcioReader
       (Ch.Grefe)

# v02-04-01
    - LCReader/SIOReader sets the RunHeader parameter "LCIOFileName" to
      the current file name (usefull if a list of files is processed)

    
    - bug fix in SIOReader:  used to hold pointer to StringVec argument of
      SIOReader::open(StringVec& files) - now the filenames are copied to local
      class member variable 
   
    - added missing const keywords for getValue() and highestBit()
      in UTIL::BitField64  (Ch.Grefe)

# v02-04
  - Ch.Grefe, CERN
    added a new python binding: pyLCIO based on the optional ROOT dictionary
    see: $LCIO/examples/python/README for examples on how to use it

 
  - added new method to limit the collections that are going to be read 
    from a file:
      LCReader::setReadCollectionNames(const std::vector<std::string>& cn)
    - C++ only so far
    - anajob and dumpevent now read variable LCIO_READ_COL_NAMES with
      space separated list of colection names to read only these

  - pointers to LCObjects that are not in the event are now set to NULL
    
  - an Exception is thrown if a null pointer is found in a subset collection
    (e.g. if LCIO_READ_COL_NAMES is set inconsistently such that 
     the collection(s) holding the elements is not read )

    - this behavior can be deactivated with setting the environment variable
      LCIO_IGNORE_NULL_IN_SUBSET_COLLECTIONS=1
      if really needed, e.g. to read inconsistent or corrupt files

  - added build option for C++ only builds
    using pre-genetarated header files:
     cmake -DLCIO_GENERATE_HEADERS=OFF ..

    - note: header files will have to be updated manually by developers
      after changing the class layout

  - some minor fixes:
    - fix missing include issue in UTIL/LCIterator.h for latest ubuntu
    - update freehep-sio dep to 2.3
    - enforce cmake version >= 2.8



# v02-03-03

  - fixed bug in TrackerHitPlaneImpl and TrackerHitZCylinderImpl
    with size of covariance matrix being 0 (no set to 6)
    that caused corrupted LCIO files when any hits of these types
    where written as "TrackerHit" types, e.g. in a new subset collection
    that is made persistent ...
    this is no problem for the DBD standard reconstruction !


# v02-03-02

    - added lcio_merge_files tool
    - fixed cmake 2.6 incompatibility introduced in v02-00-02

# v02-03-01

  - changed printout of ID for MCParticles from hex to dec
    in LCTOOLS::printMCParticles() (used e.g. in dumpevent ) 

# v02-03
  
   - added new flag bit to simulator status word:
     MCParticle::BITOverlay=23 and methods
     MCParticle::isOverlay() 
     MCParticle::setOverlay(bool v)
     - print as 'o' in LCTOOLS/Operators
  
   -  added new methods (in C++)
       LCReader::getRuns(EVENT::IntVec & runs)  and
        LCReader::getEvents(EVENT::IntVec & events) 
      that return the run and event numbers in a given file
      - to be used in randomly reading events for bg overlay
      - implemented in SIOReader
      - minimal testing in test_randomaccess.cc
 
   - in Operators.cc:
     - print covariance matrix as lower triangle matrix for Track and TrackState
     - added GoodnessOfPID to dumpe of ReconstructedParticle

   - changed the gnu jel dependency in pom.xml to 2.0.1 from 0.9
     plus small change in SIODump.java

   - added example cpp/draw_event: displays one event in 
     ROOT-OGL and writes ascii file

   - made UTIL::LCIterator<T> compatibel with gcc 4.3.2
     (#include <typeinfo>, reported by M.Killenberg)
   

# v02-02
    - added new class LCIterator<T> for convenient loops over collections, e.g.:

        for(  LCIterator<Track> it( evt, "Tracks" ) ;  Track* trk = it.next()  ; ) {
   
           std::cout << trk->getTrackState( TrackState::AtIP ) << std::endl  ;
        }
       
        - added small test/example to ./src/cpp/src/TESTS/test_tracks.cc

    - Operators.cc: fixed scientific format for Track/TrackState 

    - changed TrackState (convenience) copy constructor argument type to reference instead of pointer

    - fixed potential memory leak by adding Track's assignment operator (c++ rule of three)
   
    - added LCIO_JAVA_USE_MAVEN option to build lcio.jar library using maven


# v02-01-02

    - fixed short print out of MCParticle
    - fixed warnings w/ unused variables (in function argument list)
    - fix in LCStdhepReader.cc (B.Vormwald) for dealing with multiple 94 objects
    - fixed typo in CPPFORT/lcmcp.h : lcmcpsetcolowflow -> lcmcpsetcolorflow

# v02-01-01

   - patch release with fixes for issues identified by coverity
     ( https://coverity.cern.ch/defects/index.htm?projectId=10017 )
      - uninitialized c'tors and memory leaks


# v02-01

    - minor release including following addons:
       - added helper structs to UTIL/ILDConf:
         ILDTrkHitTypeBit
         ILDTrkHitQualityBit
       - added helper class UTIL::BitSet32
         extension of std::bitset<32> for conveninent setting and 
         checking bits in flag words
        -> to replace LCFlagImpl (changed in examples and LCTOOLS)
       - added helper methods make_bitset(), set_bit() and unset_bit()

    - bug fixes:
       - in /trunk/src/java/hep/lcio/implementation/sio/SIOTrack.java:
            out.writeFloat(trackstate.getLocation()); -> writeInt
         
    
 
# v02-00-03

    - patch release including following patches/bug fixes:

          - bug fix for writing random access records with LCSplitWriter ( all but first file had corrupt TOC for random access)
            ( added clear() method to LCIORandomAccessMgr called when closing a file in LCWriter )
          - added 'undocumented' option -f to addRandomAccess.cc that will force the recreation of the random access records 
            -> can be used to fix files on writeable storage
          - added tool lcio_split_file.cc to split large files
          - bug fixed in lcio_check_col_elements where tests would still pass with a non-existing collection
          - updated member template specialization to conform to ISO C++03 14.2/4: see http://stackoverflow.com/questions/3786360/confusing-template-error

          - improved testing


# v02-00-02

    - patch release including following patches/bug fixes:

        - added printout of all TrackStates for Track
        - removed "using namespace lcio " from src/cpp/include/UTIL/CellIDEncoder.h
            -> avoid 'implicit' using declarations that might lead to ambiguous declarations of Exception
        - updated documentation od reference point in TrackState
        - allow runtime extensions also to be read on const pointers to LCObjects (made typemap mutable)
        - added lcio_check_col_elements tool for checking the number of elements from a given collection in a set of lcio files



# v02-00-01

    - patch release including following patches/bug fixes:

        - fixed constness of 'getter' methods' parameters
        - root dictionary library not beeing built correctly
        - src/cpp/include/IMPL/SimTrackerHitImpl.h: - workaround for ROOT bug ( don't store MCParcticle pointer )
        - src/cpp/include/rootDict/rootio_templates.h: - added missing template instantiations for vectors of LCIO classes
        - src/cpp/src/UTIL/LCStdHepRdr.cc: applied patch which tries to fix mc particles with gen status 2 and no daughters where a PDG of 94 is present (B. Vormwald)
        - SimCalorimeterHit::addMCParticleContribution did not add energy contribution to hit energy ( -> Mokka created sim hits with zero energy ...)
       - fixed printout of Vertex in LCTOOLS::printVertices() and thus in dumpevent
         


# v02-00
    
    - added multiple trackstates for Track
        
        - Original functions getX() of Track will return: trk.getTrackStates()[0].getX()
        
        - TrackState::getLocation() returns one of the predefined locations:
            AtOther = 0 ; // any location other than the ones defined below         
            AtIP = 1 ;
            AtFirstHit = 2 ;
            AtLastHit = 3 ;
            AtCalorimeter = 4 ;
            AtVertex = 5 ;
            LastLocation = AtVertex  ;

        - TrackState* getTrackState(int location)
            returns the trackstate at the given location, where location is given by one
            of the predefined locations specified above
        
        - TrackState* getClosestTrackState(float x, float y, float z)
            returns the trackstate closest to the given location
        
        - getTrackState( TrackState::AtFirstHit )->getReferencePoint()
        - getTrackState( TrackState::AtLastHit )->getReferencePoint()

        - deprecated isReferecePointPCA()


    - added TrackerHitPlane and TrackerHitZCylinder for extending the TrackerHit interface

    - added BitField64::reset()

    - added constructor in CellIDDecoder which takes a string argument for the encoding
    
    - added some useful definitions that are used in the ILD software framework
        - in particular for encoding and decoding cellIDs.

    - add spin and color flow information to MCParticle if available in stdhep file (LCStdHepRdr)
    
    - clarified use of addMCParticleContribution
        - now two method signatures for standard mode and 'detailed' mode
        - no excpetion is thrown if stepPos==Null in detailed mode - use (0.,0.,0.) instead



    - new tests added:

        - added some tests for the canonical ILD CellIDEncoding (under test_trackerhit)
        - added tests for TrackerHitPlane and TrackerHitZCylinder classes
        - added tests for the TrackState class
        - added a test for LCRTRelations (extensions)



    - bug fixes:

        - added printout of ndf for tracks
        - added printout of Cluster EnergyError
        - added printout of TrackerHit and SimTrackerHit cellid's in LCIO_LONG operators
        - fixed strict aliasing warning for RTRelations in gcc when compiled with -O3 (Release mode)
        - fixed constness of member functions (LCTime)
        - applied fortran patches for gcc4.4 on ubuntu 32bit (Phillip Klenze, MPI)
        - removed generation of cpp headers in build.xml (from ant aid.generate target)
        - removed duplicate FindJAVA.cmake module
        - fixed path to MacroCheckPackageLibs.cmake and MacroCheckPackageVersion.cmake in LCIOConfig.cmake


# v01-60

     - implemented new methods:

       		  LCReader::readRunHeader(int runNumber ) 

          -  C++ only:
                  LCReader::getNumberOfEvents() 
                  LCReader::getNumberOfRuns() 
             (anajob now prints number of runs and events in the files it is going to read)

     - added access mode to  
         LCReader::readRunHeader(int runNumber, int accessMode ) 
         LCReader::readEvent(int runNumber, int evtNumber, int accessMode ) 
     
     - added example for creating a simple NTuple from MCParticle collection
       (  ./examples/cpp/rootDict/writeNTuple.C )


    - added spin and color flow information to MCParticle:

        float[3] getSpin()
        int[2] getColorFlow()

        void setSpin( float[3] )
        void setColorFlow( int[2] )


    - added cellID0 and cellID1 to TrackerHit and SimTrackerHit:
        
        int getCellID0()
        int getCellID1()

        void setCellID0( int )
        void setCellID1( int )

        * in SimTrackerHit the methods get/setCellID() were deprecated
            these methods now call get/setCellID0()

    - added optional storing of the position where the energy deposition (step) occurred in SimCalorimeterHit
        only if bit flag LCIO.CHBIT_STEP is set

        float[3] getStepPosition( int i )

        * LCIO.CHBIT_PDG was deprecated
            should now use LCIO.CHBIT_STEP

    - added get/setEnergyError to Cluster (c++/java/fortran)

    - added ostream operators (H. Hoelbe)

    - added tool lcio_event_counter for counting events in slcio / stdhep files

    - anajob now prints number of runs and events in the files it is going to read

    - removed old makefiles used by ant.cpp

    - improved CMakeLists.txt (compatible with ilcsoft v01-11)
        - reduced build time of lcioDict library to 50%

    - bug fixes:
        fix printout of TrackerHit (last element of cov matrix was missing(wrong))
        fixed typo in CalorimeterHit documentation (CHBIT should be RCHBIT)
        some protections against NULL pointers (A.Sailer)
        added missing fortran wrappers from cfortran.h for CalorimeterHit ( CPPFORT/lccah.h )
        fixed several issues found by coverity (https://coverity.cern.ch)
        fixed bug with unsorted indices for direct access (java) - to be reviewed


	

# v01-51-02
     - patch release:
         fix for building the ROOT dictionary with ROOT 5.27.06
         fix for printing covariance matrix of TrackerHit in dumpevent
         missing documentation of TRAWBIT_CM in TrackerPulse (EVENT)

# v01-51-01
     - patch release:
         made compliant with gcc 4.4
         bug fix: readNextEvent/RunHeader did not work in direct access mode
         bug fixes in UTIL::Operators 
         bug fix in LCRTRelations for 64bit
         fix in sio/SIOEvent.java (missing LCRelations)
         made compliant with latest MacOSX (sio/include/SIO_functions.h)     

# v01-51
   
  changes to EDM:
  --------------
     
     - SimTrackerHit and TrackerHit:

       -  renamed get/setdEdx() in  to get/setEDep() 
          - energy deposited in the hit in GeV
          - getdEdx() is deprecated and will result in a printed WARNING

     - TrackerHit:

         - added get/setEdepError()


     - TrackerPulse:
  
         - added getCovarianceMatrix() for measurement errors of getCharge() and 
           getTime()   [LCIO-72]


  new features:
  --------------

    - improved direct access to events, by introducing 
      additional records LCIOindex, LCIORandomAccess written on close()
      ( no need for fast skip or creation of table of contents on open() )

       - implemented in C++ and Java

       - the binary 'addRandomAccess' can be used to add random access
         records to existing older files (if stored in writable file system) 


    - new ostream operator<<(T)  for LCIO classes in UTIL/Operators.h
      e.g.:
         cout << ((MCParticle*) c->getElementAt(i) )  << endl ; 
 
   

    - improved cmake support for LCIO developers  (J.Engels)

        - generation of *.h files w/o ant 
          -> proper dependiencies are observed for re-compiling

        - add target for building reference manual


    - LCWarning utility class for printing 'deprecated' warnings for   
      n times per job


    - made compatible w/ fcc 4.3.3


    - lsh.cc: quit if CTRL-D (EOF) is pressed
      (http://bugs.freehep.org/browse/LCIO-58)


    - added support for Java builds with Maven (J.McCormick)


  bug fixes:
  ----------
    - SIORelation.java: exchanged getFrom() and getTo()
      (SIODump had 'from' and 'to' exchanged)

    - LCTOOLS.cc: protected against missing tracks and clusters (null pointers)
      -> fixes seg. faults in dumpevent 

    - SIOLCCollection.java fixed bug with subset collections

//***********************************************************************


# v01-12-03
  - bug fix/patch release:
    read cluster hit weights as floats (was pointer) 
    (seg fault on 64 bit with C++ library)

# v01-12-02
  - bug fix/patch release:
    fix bug with backward compatibility for reading TrackerHits


# v01-12-01
   Experimental patch release:
   ---------------------------
   - optionally build a ROOT dictionary for LCIO
   - see: examples/cpp/rootDict/README for details
   - library and files fully compatible with v01-12


# v01-12
  - added quality bit field for TrackerHit (http://bugs.freehep.org/browse/LCIO-63)
  
  - new features in java/lcio:   
       - output directory in split command
       - Poisson distribution in merge command
  
  - bug fixes:
       - allow multiple open of file (http://bugs.freehep.org/browse/LCIO-60) in C++
       - fixed seg. fault with missing PID in LCTOOL::dumpEventDetailed
       - SIOReader: check if all files exist before start of reading (C++)
       - fixed support for optional linkage against libdcap (dCache I/O) - C++ only
       - minor fixes (includes etc.)

# v01-11
  - made direct (random) access mode in C++ optional:
    use  LCFactory::getInstance()->createLCReader(LCReader::directAccess) if
    direct access is need (default is off, i.e. 'no recreating of event map' 
    when opening a file )
  - bug fixes and improvements in C++
    (CMake build procedure, compatibility w/ gcc4.x, 32bit compatibility mode)

# v01-10-03
    - bug fix patch release:
        - fixed 'bug' in updateNextEvent: ignore daughters if the first 
             daughter index is illegal (0 in stdhep)
        -> needed in Mokka-06-07 for fixStdHepLeptonFSR


# v01-10-02
    - bug fix patch release:

    * fixed stdhepjob for running in 64 bit systems
    * made cmake 2.6 compliant
    * added 32 bit compatibility build option
    * added LCIO_LIBRARY_DIRS

# v01-10-01
   - bug fix patch release:


    * bug fix for printing DSTs with dumpevent 
      (Track collections with null pointers to tracks that are not stored)

    * improved format of dump of MCParticle collection


# v01-10
 
   - added optional attribute CalorimeterHit::getEnergyError() 
       -> set/check flag bit RCH_BIT_ENERGY_ERROR
       request from calice: [http://bugs.freehep.org/browse/LCIO-56]
  
    - optional linkage agains libdcap (dCache) for C++
       -> cmake ..... -DBUILD_WITH_DCAP=1 -D DCAP_HOME=dcap_home ....
   
    -  added PIDHandler class to UTIL (C++) for convenient access to ParticleID properties of 
      ReconstructedParticle (needed for LCFI flavour tag quantities)    
   
    - restructured test programs for C++ (make tests ; make test ; when buit with cmake)

     - made compatible w/ gcc4.3 C++ (A.Bulgheroni)


# v01-09
  no changes in file format

   - added event weight: LCEvent.getWeight()  (Java/C++)

   - added LCWriter::setCompressionLevel(int level)  (C++) 

   - lcio (java tool) and stdhepjob (C++) improved
       - made compatible with new stdhep files (Java/C++)
       - fill stdhep event weight ( and _idrup / user process id )
       - stdhepjob now build by default (can be used to convert stdhep to LCIO)
 
   - LCStdHepRdr now fill MCParticle::charge properly (C++)
 
   - Java uses new sio classes from hep.io.sio

   - LCIO (C++) no longer has optional direct dependency on CLHEP 
     ( only file is UTIL/LCFourVector.h that can be used in
       programs built with LCIO and CLHEP )   


# v01-08-05
  no changes in file format and API

  - experimental code for direct access (C++) 
    (LCReader builds event map at open())

  - readEvent( rNum , evtNum ) uses direct access

  - first version of cmake tests (for C++)  (JE)
    build with -DBUILD_LCIO_TESTJOBS=ON, afterwards call 'make test'

  - bug fixes:
    Change all gmake calls to make for Linux build  (JMC)
    predefined USE_CLHEP for doxygen (JE)
    dump format of TrackerPulses in LCTOOLS (AB)
    show proper include path in API doc

# v01-08-04
  bug fix release
  no changes in file format and API
  - minor fixes in Java code (JMC)
  - minor fixes for building with cmake 

  - changed order of modifyEvent and processEvent in SIOLCReader
    (needed for Marlin Overlay processor)
 
  - simple shell like tool for browsing lcio files in C++: .../EXAMPLES/lsh.cc
    ( use -DBUILD_LCIO_SHELL=true - requires ncurses and readline )
    [experimental code]
 
# v01-08-03
  bug fix release
  no changes in file format and API
  - minor fixes in Java code (TJ,JMC)
  - full support for building with cmake 

# v01-08-02
  bug fix release
  no changes in file format and API
    - fixed several problems with templates for newer gcc versions (>=3.4)
    - removed obsolte lcioframe example
    - support for building LCIO (C++) with cmake (experimental)

# v01-08-01
  bug fix release
  no changes in file format and API
    - fixed problems with storing large arrays in SIO (C++) 
    - fixed some issues with CellIDEncoder (C++) 
    - enforce valid collection names (like C/C++ variables)

# v01-08

   - introduced C++ runtime (user) extensions and relations
     see LCRTRelation and lcrtrelation.cc for documentation and examples

   - new Vertex class in LCIO - for a detailed description see the proposal at:
     http://forum.linearcollider.org/index.php?t=getfile&id=32

   - new Java lcio command line tool - for detailed documentation see:
     http://confluence.slac.stanford.edu/display/ilc/LCIO+Command+Line+Tool

   - modified function UTIL:LCTOOLS:dumpEvent() for a more readable format
     used e.g. in anajob.cc 

   - bug fixes:
	+ in SIOReader::skipNEvents() (skipped only n-1 with readStream())
        + collection parameters in LCRelationNavigator (From/ToType)
        + made id() const
        + bug with BitField64/CellIdDecoder
        + ...

# v01-07

   - optional python binding (Jeremy McCormick)

   - C++ utility code for encoding/decoding of cellids in hit classes:
     UTIL::CellIDEncoder and UTIL::CellIDDecoder 

   - introduced additional pathLength in SimTrackerHit for silicon tracker digitization 
     (stored optionally together with momentum)

   - introduced UTIL::LCSplitWriter to split larger files

   - fixed files size limit (2GB) in SIO C++ implementation

   - ant does not show "BUILD SUCCESSFUL" in case of error  anymore

# v01-06

   - added optional momentum[3] to SimTrackerHit (momentum of particle at position of hit)
 
   - added LCReader::skipNEvents() method to skip (the first) n events in a file

   - added option to dump the n-th event in a file to $LCIO/bin/dumpevent 

# v01-05
   - changed return values of energy, momentum and mass to double for MCParticle and 
     ReconstructedParticle - stored are still floats
     -> requires some trivial changes (float->double) in code where indicated by the compiler !

   - new template UTIL::LCTypedVector for creating std::vector<LCOBJECTTYPE>s  of LCCollections
 
   - added first implementations of generic tracker raw data classes (TPC, VTX, SiliconStrip,...)
     TrackerRawData, TrackerData, TrackerPulse

   - added macro to lcio.h for handling of uncaught exceptions if not Reader/Writer is used:
     #define HANDLE_LCIO_EXCEPTIONS 

   - added MACRO LCIO_PATCHVERSION_GE(  MAJV , MINV , PLEV) to check for patch level releases
 
   - changed printout of calorimeter hit cellids to decode M,S,I,J,K as defined in Mokka in LCTOOL::dumpEventDetailed

 - bug fixes and minor changes
   - C++ version :
    * fixed bug in LCTime ( first day of month ) and added LCTime::test(int nDates)
    * added -pedantic to GNUmakefile and removed additional ';'s at end of namespaces and macros
    * corrected documentation for SimCalorimeterHitImpl::addMCParticleContribution
    * corrected documentation for Track::getTrackerHits - need LCIO::TRBIT_HITS==1 
    * bug fix in UTIL::LCFourVector - c'tors take _const_ OBJECTTYPE* pointers now
    * removed deprecated function calls in LCStdHepRdr
    * fixed a memory leak in lStdHep.cc
    * MCParticle: proper treatment of endpoint bit (was always on)
    * Cluster: hit weights where always 0 
   - Java version
    * ReconstructedParticle.getParticleIDs() now sorted wrt. likelihood

# v01-04

 - added flag isSubset to LCCollection to allow to have collections in the event that 
   are subsets of other collections in the event. In case the collection is not transient
   only references/pointers are stored in the output file.

 - added implementation of LCGenericObjects in Java

 - API change:  Changed the return value of LCGenericObject::getTypeName() and 
   LCGenericObject::getDataDescription() from const std::string& to 
   const std::string to  make the implementation easier
   -> requires a trivial change in LCGenericObject subclasses

 - added UTIL::LCFixedObject template for user defined subclasses of LCGenericObjects
   with fixed size
   see: src/cpp/src/EXAMPLE/CalibrationConstant.h for example on how to use it

 - added UTIL::LCTime: helper class to convert between time stamps and calendar time

 - added LCIODEBUG environment variable to control compilation

 - added support for multiple read/and write streams in C++

 - added support for multiple events in C++

 - removed calls to deprecated functions in f77

 - bug fixes (C++):
     + dump only five elements of LCInt/Float/StringVecs in LCTOOLS
     + don't allow to add NULL collections to the event
     + changed LCEvent::getTimestamp() to long long (64bit) 
     + fixed bug when writing empty collection of LCGenericObjects
 
# v01-03-01
  bug fix release
  no changes in file format and API

# v01-03
  - added support for reading stdhep binary files  

  - added ant target 'doc.all' - requires javadoc, doxygen, pdflatex,latex2html
    builds all documentation on linux 
	
  - added isDefault flag to LCCollection   

  - LCGenericObject added

  - usage/logic of SimCalorimeterHit MCParticle contributions has changed
    now the detailed mode has one entry per simulator step that contributed to the hit 

  - > getNMCParticles renamed to getNMCContributions


  - added LCRelation

  - replaced LCIO::TPCBIT_PTR by LCIO::TPCBIT_NO_PTR and 
    LCIO::RCHBIT_PTR by LCIO::RCHBIT_NO_PTR and inverted logic  <<<<<< user action !
 
  - introduced RawCalorimeterHit with int amplitude and time

  - added time to CalorimterHit

  - added getParents()/getDaughters() to MCParticle

  - flag for transient collections -> not written to file

# v01-02 prebeta

   - new classes Track, Cluster, ReconstructedParticle 
  
   - ....

   - new package UTIL for C++
      -> LCTOOLS moved to UTIL

   - added LCFourVector (CLHEP::HepLorentzvector) as handle template
     for MCParticle

   - added 'chain reader' : LCReader::open(std::vector<std::string> fileNames  )   

# v01-00

   first public release

# v00-08  ( fg20030911 )


   - require version >= v00-08 in C++    

   - removed pointer tag from SimCalorimeterHit, CalorimeterHit and SimTrackerHit
     [Java and C++]

   - Access checked for read only mode in Java (Tony)

   - scripts for building under cygwin (Norman)

# v00-08beta ( fg20030911 )
  
   - added TPCHit class for real data TPC prototypes
     lcio.xml updated
     [C++ and Java]


   - removed DataNotAvailableExceptions from all get-methods that return object 
     pointers or referencs, e.g. MCParticle::getParent(), return null pointers instead
     [C++ and Java]
      updated manual.tex accordingly

   - manual.tex now able to create ps, pdf and html


# ( fg20030909 )

   - manual reviewed and updated, added section on Exceptions

   - all throw clauses in C++ include std::exception now
     -> similar behavior to Java Exceptions

   - implemented new 'access check' mechanism: 
     removed const methods from data entities using ReadOnlyExceptions instead
     introduced base class AccessChecked  [ C++ only - Java has ILCObject]

   - renamed analyze/update methods to processEvent/modifyEvent and 
     processRunHeader/modifyRunHeader in the LCRun(Event)Listener interfaces
     
   - added printout for unexpected or uncaught exceptions in C++

 

# v00-07beta ( fg 20030903 )

   - first version of user manual ('ant doc.manual' -> ./doc/manual.ps)

   - store Mass instead of Energy for MCParticle ( v > 0.7)
     [ C++ and Java ]

   - added scripts (bash) to call Java examples (bin/runSimJob.sh,...)

   - added integer user extensions (LCIntVec)
     [ C++  and Java ]

   - removed EndOfDataException from LCReader API
     EOF is signaled by null pointers again ( while ( evt=lcRdr->readNextEvent()) != 0)
     -> we only use exceptions to signal errors (and unexpected events)
     [ C++ only ]

   - new class 'CalorimeterHit' for real data 
     same as SimCalorimeterHit except for MCParticle link
     [ Java and C++ ]      

   - some code documentation for the C++ API from doxygen
     
   - modification of build.xml ( now 'ant   '  builds everything)


 

# v00-05beta ( fg 20030613 )

  - new package hep.lcio.data / namespace DATA
    defines pure data contents - this is now the interface that has to implemented
    for writing data with LCIO 
       -> LCWriter uses only this interface

  - added pointer tag to track and calorimeter hits for future reference to those

  - added enpoint to MCParticle

  - new methods in LCCollection addElement(), removeElement(int i)
 
  - new methods in MCParticle: getNumberOfDaughters(), getDaughter(int i)
     replacing getDaughters() and  getEndPoint()
 
  - sio-file extension '.slcio' not required by LCReader any more 
    [C++ only]
  
  - LCWriter::open() if no mode specified an exception is thrown
    if the file already exists
    [C++ only]

  - TrackerHit and CalorimeterHit renamed to SimTrackerHit and SimCalorimeterHit

  - New Exception hierarchy  for C++ according to exceptions used in Java 
 
  - Added some Exceptions to hep.lcio.exceptions (Java)

  - Exceptions in LCWriter interface

  - Exceptions in LCReader interface
    [C++ only] 
 
  - Added new default units GeV, mm, ns to comments/documentation

  
  - new method LCTOOLS::dumpEventDetailed() - prints everything in the event
    (bin/dumpevent simjob.slcio 2 2)
    C++ only (incomplete, TrackerHits and user extensions missing)


  - new method SIOWriter::readStream(int maxRecord)
    [C++ only]


# v00-04  (fg 20030514)

 - new sio-file extension '.slcio' (enforced by C++ version of LCReader/Writer)

 - introduced append mode for writing in LCWriter::open() 
   (constants LCIO::WRITE_NEW and LCIO::WRITE_APPEND )
   C++ only

 - SIO file buffers are flushed after each record
   
 - small program 'copyfix' added:
   Copies all complete events from a file (that has incomplete  
   records at the end due to a program crash) so the append mechanism can be used 
   to continue the program after the crash, e.g. a long running simulation job.
   
 - new method LCReader::readEvent( evtN, runN ) to access 
   given event ( demonstrating fast skip mechanism - no direct access)
   -> see example dumpevent.cc

 - duplicate event header information is not written any more (neither Java nor C++)
   -> event information always in two consecutive records of sio file:
      LCEventHeader: runNum, evtNum, collection names and types
      LCEvent: the data (collections)

 - compression of files now controled by constants 
   (SLCIO::compression, SIOFactory.compression) - set to true for C++ and Java
  
 - the ant target cpp needs aid.generate as a prerequisite (not automatically 
   included any more)

 - added this file: please document your (major) changes to LCIO here 




# v00-03  (fg 20030509)

 - no 0's at end of strings

 - flag bits for CalorimeterHits taken into account (also Java version)

 - Java version uses identical block names to C++ (as defined in lcio.xml)

 - small bug fix in Java version of SIO:
   src/java/hep/lcd/io/sio/SIOWriter.java writes record header length (as
   opposed to the record length)
   for this to take effect lcio.jar has to be before sio.jar in your
   classpath - otherwise C++ won't be able to read files generated from
   Java...
   needs to be checked for implications on hep.lcd and (if none) applied 
   to sio.jar

 - Java writes the (duplicate) header also for the event record

 - Old files cannot be read with this version !
