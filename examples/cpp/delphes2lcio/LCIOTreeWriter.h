#ifndef LCIOTreeWriter_h
#define LCIOTreeWriter_h

/** \class LCIOTreeWriter
 *
 *  Class handling output of ROOT tree in LCIO
 *
 *  \author F.Gaede, DESY
 *  \date June 2020
 *
 */

#include "TNamed.h"
#include "ExRootAnalysis/ITreeWriter.h"
#include <set>

class TFile;
class TTree;
class TClass;
class ExRootTreeBranch;
#include "IO/LCWriter.h"

class LCIOTreeWriter: public ITreeWriter
{
public:
  LCIOTreeWriter(IO::LCWriter *writer, const char *treeName = "Analysis");
  ~LCIOTreeWriter();

  virtual  void SetTreeFile(TFile *file) override  { /* no_op*/ }
  virtual void SetTreeName(const char *name) override  { fTreeName = name; }
  virtual void SetName(const char *name) override { TNamed::SetName(name) ;}

  ExRootTreeBranch *NewBranch(const char *name, TClass *cl) override ;

  virtual void Clear() override ;
  virtual void Fill() override ;
  virtual void Write() override ;

private:
  TTree *NewTree();
  TTree *fTree; //!
  TString fTreeName; //!
  IO::LCWriter *fWriter;
  std::set<ExRootTreeBranch *> fBranches; //!

//  ClassDef(LCIOTreeWriter, 1)
};

#endif /* LCIOTreeWriter */
