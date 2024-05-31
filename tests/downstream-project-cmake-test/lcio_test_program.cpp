#include "EVENT/LCEvent.h"
#include "IMPL/LCCollectionVec.h"
#include "IMPL/LCEventImpl.h"
#include "IMPL/MCParticleImpl.h"
#include "MT/LCWriter.h"

int main() {
  auto mcp = new IMPL::MCParticleImpl();
  mcp->setPDG(11);

  auto coll = new IMPL::LCCollectionVec(EVENT::LCIO::MCPARTICLE);
  coll->addElement(mcp);

  auto event = new IMPL::LCEventImpl();
  event->addCollection(coll, "mcps");

  auto writer = MT::LCWriter();
  writer.open("test.slcio");
  writer.writeEvent(event);

  return 0;
}
