package hep.lcio.util;

import hep.lcio.event.CalorimeterHit;
import hep.lcio.event.Cluster;
import hep.lcio.event.LCCollection;
import hep.lcio.event.LCEvent;
import hep.lcio.event.LCFloatVec;
import hep.lcio.event.LCGenericObject;
import hep.lcio.event.LCIO;
import hep.lcio.event.LCIntVec;
import hep.lcio.event.LCObject;
import hep.lcio.event.LCParameters;
import hep.lcio.event.LCRelation;
import hep.lcio.event.LCStrVec;
import hep.lcio.event.MCParticle;
import hep.lcio.event.ParticleID;
import hep.lcio.event.RawCalorimeterHit;
import hep.lcio.event.ReconstructedParticle;
import hep.lcio.event.Vertex;
import hep.lcio.event.SimCalorimeterHit;
import hep.lcio.event.SimTrackerHit;
import hep.lcio.event.TPCHit;
import hep.lcio.event.Track;
import hep.lcio.event.TrackerData;
import hep.lcio.event.TrackerHit;
import hep.lcio.event.TrackerPulse;
import hep.lcio.event.TrackerRawData;
import hep.lcio.implementation.io.LCFactory;
import hep.lcio.io.LCReader;

import java.io.File;
import java.io.PrintStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * A utility class for printing LCEvents and LCCollections of all the LCIO
 * types. It borrows heavily from Frank Gaede's C++ dumpevent utility found in
 * <code>src/cpp/UTIL/LCTOOLS.cc</code>.
 *
 * @author Jeremy McCormick <jeremym@slac.stanford.edu>
 */
public class Printer {
	PrintStream ps = System.out;
	int maxrec = 1000;
	Map printers = new HashMap();
	boolean printHeadersOnly = false;

	Printer() {
		List plist = new ArrayList();

		plist.add(new SimCalorimeterHitPrinter());
		plist.add(new SimTrackerHitPrinter());
		plist.add(new CalorimeterHitPrinter());
		plist.add(new TrackerHitPrinter());
		plist.add(new ClusterPrinter());
		plist.add(new LCGenericObjectPrinter());
		plist.add(new MCParticlePrinter());
		plist.add(new LCFloatVecPrinter());
		plist.add(new LCIntVecPrinter());
		plist.add(new LCStrVecPrinter());
		plist.add(new ReconstructedParticlePrinter());
		plist.add(new VertexPrinter());
		plist.add(new RawCalorimeterHitPrinter());
		plist.add(new TrackPrinter());
		plist.add(new TPCHitPrinter());
		plist.add(new TrackerDataPrinter());
		plist.add(new TrackerPulsePrinter());
		plist.add(new TrackerRawDataPrinter());
		plist.add(new LCRelationPrinter());

		for (int i = 0; i < plist.size(); i++) {
			LCTypePrinter printer = (LCTypePrinter) plist.get(i);
			printers.put(printer.type(), printer);
		}

		plist = null;
	}

	void setPrintStream(PrintStream ps) {
		this.ps = ps;
	}

	void print(File f, int nevents, int nskip, int maxrec,
			boolean printHeadersOnly) throws Exception {
		// Set maximum number of records per collection.
		this.maxrec = maxrec;

		this.printHeadersOnly = printHeadersOnly;

		// Create and open the reader.
		LCReader reader = LCFactory.getInstance().createLCReader();
		reader.open(f.getAbsolutePath());

		// Skip nskip events.
		for (int i = 0; i < nskip; i++) {
			reader.readNextEvent();
		}

		LCEvent event = reader.readNextEvent();
		int nread = 0;
		while (event != null) {
			printLCEvent(event);
			nread++;
			if (nevents != -1 && nread == nevents) {
				ps.println("Read maximum requested events.");
				break;
			}
			event = reader.readNextEvent();
		}
		ps.println(nread + " events read.");
	}

	void printLCEvent(LCEvent event) throws Exception {
		// Print the header information.
		ps.println(" ==============================================================");
		ps.println(" Run : " + event.getRunNumber());
		ps.println(" Event : " + event.getEventNumber());
		ps.println(" Detector : " + event.getDetectorName());
		ps.println(" Time Stamp : " + event.getTimeStamp());
		ps.println(" Weight : " + event.getWeight());
		ps.println(" ==============================================================");
		ps.println();

		// Print the event parameters.
		ps.println(" Event parameters : ");
		printParameters(event.getParameters());

		// Print summary of collections.
		printLCCollectionsSummary(event);

		// Print detailed dump of each collection by LCIO type.
		if (!printHeadersOnly)
			printLCCollectionsDetailed(event);

		ps.println();
	}

	void printLCCollectionsSummary(LCEvent event) {
		ps.println(" COLLECTIONS SUMMARY");
		ps.println();
		ps.format(" %-30s %-20s %s\n", "Collection Name", "Collection Type",
				"Size");
		ps.println(" -----------------------------------------------------------------");

		String collnames[] = event.getCollectionNames();
		for (int i = 0; i < collnames.length; i++) {
			LCCollection coll = event.getCollection(collnames[i]);
			ps.format(" %-30s %-20s %d\n", collnames[i], coll.getTypeName(),
					Integer.valueOf(coll.getNumberOfElements()));
		}
	}

	void printLCCollectionsDetailed(LCEvent event) {
		String collnames[] = event.getCollectionNames();
		for (int i = 0; i < collnames.length; i++) {
			LCCollection coll = event.getCollection(collnames[i]);

			ps.println(" -----------------------------------------------------------------");
			ps.println();
			ps.println(" Collection Name : " + collnames[i]);
			ps.println(" Collection Type : " + coll.getTypeName());
			ps.println(" Number of Elements : " + coll.size());
			ps.format(" Flag Word: 0x%x\n", Integer.valueOf(coll.getFlag()));

			System.out.println(" Collection Parameters : ");
			printParameters(coll.getParameters());

			printLCCollection(coll);
		}
	}

	void printLCCollection(LCCollection coll) {
		ps.println();

		String typename = coll.getTypeName();

		int nhits = coll.getNumberOfElements();
		int nprint = nhits > maxrec ? maxrec : nhits;

		LCTypePrinter printer = (LCTypePrinter) printers.get(typename);

		if (printer != null) {
			printer.print(coll, nprint);
		} else {
			ps.println("WARNING: Skipping unknown collection type <" + typename + ">");
		}

		ps.println();
	}

	void printParameters(LCParameters param) {
		String floatkeys[] = param.getFloatKeys();
		for (int i = 0; i < floatkeys.length; i++) {
			String floatkey = floatkeys[i];
			float val = param.getFloatVal(floatkey);
			ps.format(" %s = %5.3e\n", floatkey, Float.valueOf(val));
		}

		String intkeys[] = param.getIntKeys();
		for (int i = 0; i < intkeys.length; i++) {
			String intkey = intkeys[i];
			int val = param.getIntVal(intkey);
			ps.format(" %s %d\n", intkey, Integer.valueOf(val));
		}

		String strkeys[] = param.getStringKeys();
		for (int i = 0; i < strkeys.length; i++) {
			String strkey = strkeys[i];
			String val = param.getStringVal(strkey);
			ps.print(" " + strkey + " = " + val + "\n");
		}

		ps.println();
	}

	abstract class LCTypePrinter {
		abstract void print(LCCollection coll, int nprint);

		abstract String type();
	}

	class LCRelationPrinter extends LCTypePrinter {
		void print(LCCollection coll, int nprint) {
			ps.println(" fromType : "
					+ coll.getParameters().getStringVal("FromType"));
			ps.println(" toType : "
					+ coll.getParameters().getStringVal("ToType"));

			ps.println(" [from_id]  |  [to_id]   | weight ");

			for (int i = 0; i < nprint; i++) {
				LCRelation rel = (LCRelation) coll.getElementAt(i);
				LCObject from = rel.getFrom();
				LCObject to = rel.getTo();
				int fromVal = 0;
				int toVal = 0;
				if (from != null) {
					fromVal = from.hashCode();
				}
				if (to != null) {
					toVal = to.hashCode();
				}
				ps.format(" [%08x] |  [[%08x]   | %5.3e \n",
						Integer.valueOf(fromVal),
						Integer.valueOf(toVal),
						Double.valueOf(rel.getWeight()));
			}
		}

		String type() {
			return LCIO.LCRELATION;
		}
	}

	class SimCalorimeterHitPrinter extends LCTypePrinter {
		void print(LCCollection coll, int nprint) {
			int flag = coll.getFlag();

			ps.println(" Flags : ");
			ps.println("   -> LCIO::CHBIT_LONG   : "
					+ ((flag & (1 << LCIO.CHBIT_LONG)) != 0));
			ps.println("      LCIO::CHBIT_BARREL : "
					+ ((flag & (1 << LCIO.CHBIT_BARREL)) != 0));
			ps.println("      LCIO::CHBIT_ID1   :  "
					+ ((flag & (1 << LCIO.CHBIT_ID1)) != 0));
			ps.println("      LCIO::CHBIT_PDG    : "
					+ ((flag & (1 << LCIO.CHBIT_PDG)) != 0));

			boolean havepos = ((flag & (1 << LCIO.CHBIT_LONG)) != 0);

			ps.println();
			ps.println(" [   id   ] | [cellID0] | [cellID1]  |   energy  |        position (x,y,z)          | nMCParticles ");
			ps.println("           -> MC contribution: prim. PDG |  energy | time | sec. PDG  ");
			ps.println();

			for (int i = 0; i < nprint; i++) {
				SimCalorimeterHit hit = (SimCalorimeterHit) coll
						.getElementAt(i);

				int id0 = hit.getCellID0();
				int id1 = hit.getCellID1();

				if (havepos) {
					ps.format(" [%08x] | %08x | %08x |"
							+ " %5.3e | (%5.3e,%5.3e,%5.3e) | %d\n",
							Integer.valueOf(hit.hashCode()),
							Integer.valueOf(id0), Integer.valueOf(id1),
							Float.valueOf(hit.getEnergy()),
							Float.valueOf(hit.getPosition()[0]),
							Float.valueOf(hit.getPosition()[1]),
							Float.valueOf(hit.getPosition()[2]),
							Integer.valueOf(hit.getNMCContributions()));
				} else {
					ps.format(
							" [%08x] |%08x | %08x |"
									+ " %5.3e |    no position available         | %d\n",
							Integer.valueOf(hit.hashCode()),
							Integer.valueOf(id0), Integer.valueOf(id1),
							Float.valueOf(hit.getEnergy()),
							Integer.valueOf(hit.getNMCContributions()));
				}

				for (int j = 0; j < hit.getNMCContributions(); j++) {
					try {
						ps.print("           -> "
								+ hit.getParticleCont(j).getPDG() + ": "
								+ hit.getEnergyCont(j) + " | "
								+ hit.getTimeCont(j) + " | ");
					} catch (Exception e) {
					}

					if ((flag & (1 << LCIO.CHBIT_PDG)) != 0) {
						ps.print(hit.getPDGCont(j));
					} else {
						ps.print(" no PDG");
					}

					ps.print("\n");
				}
			}
		}

		String type() {
			return LCIO.SIMCALORIMETERHIT;
		}
	}

	class SimTrackerHitPrinter extends LCTypePrinter {
		void print(LCCollection coll, int nprint) {
			int flag = coll.getFlag();

			ps.println(" Flags : ");
			ps.println("   -> LCIO::THBIT_BARREL   : "
					+ ((flag & (1 << LCIO.THBIT_BARREL)) != 0));
			ps.println("      LCIO::THBIT_MOMENTUM : "
					+ ((flag & (1 << LCIO.THBIT_MOMENTUM)) != 0));

			boolean pstored = ((flag & (1 << LCIO.THBIT_MOMENTUM)) != 0);

			// ps.println(" [   id   ] | cellID[indices] | position (x,y,z) | dEdx | time  | PDG of MCParticle");
			ps.println(" [   id   ] | cellID[indices] | position (x,y,z) | EDep | time  | PDG of MCParticle");

			if (pstored)
				ps.println(" | (px, py, pz) | pathLength ");

			ps.println();

			for (int i = 0; i < nprint; i++) {
				SimTrackerHit hit = (SimTrackerHit) coll.getElementAt(i);

				int pdgid = 0;

				if (hit.getMCParticle() != null) {
					pdgid = hit.getMCParticle().getPDG();
				}

				ps.format(
						" [%08x] | [%08x] | (%5.3e, %5.3e, %5.3e) | %e | %e | %d",
						Integer.valueOf(hit.hashCode()),
						Integer.valueOf(hit.getCellID()),
						Double.valueOf(hit.getPosition()[0]),
						Double.valueOf(hit.getPosition()[1]),
						Double.valueOf(hit.getPosition()[2]),
						// Float.valueOf(hit.getdEdx()),
						Float.valueOf(hit.getEDep()),
						Float.valueOf(hit.getTime()), Integer.valueOf(pdgid));

				if (pstored) {
					ps.format(" | (%5.3e %5.3e %5.3e) | %e",
							Float.valueOf(hit.getMomentum()[0]),
							Float.valueOf(hit.getMomentum()[1]),
							Float.valueOf(hit.getMomentum()[2]),
							Float.valueOf(hit.getPathLength()));
				}

				ps.print("\n");
			}
		}

		String type() {
			return LCIO.SIMTRACKERHIT;
		}
	}

	class CalorimeterHitPrinter extends LCTypePrinter {
		void print(LCCollection coll, int nprint) {
			ps.println(" [   id   ] | [cellID0] | [cellID1]  |   energy  | energyerr |        position (x,y,z)          ");

			for (int i = 0; i < nprint; i++) {
				CalorimeterHit hit = (CalorimeterHit) coll.getElementAt(i);

				int id0 = hit.getCellID0();
				int id1 = hit.getCellID1();

				int flag = coll.getFlag();

				ps.format(" [%08x] | [%08x] | [%08x] | %5.3e | ",
						Integer.valueOf(hit.hashCode()), Integer.valueOf(id0),
						Integer.valueOf(id1), Double.valueOf(hit.getEnergy()));

				if ((flag & (1 << LCIO.RCHBIT_ENERGY_ERROR)) != 0) {
					ps.format("%5.3e | ", Double.valueOf(hit.getEnergyError()));
				} else {
					ps.format(" - NA - | ");
				}

				if ((flag & (1 << LCIO.CHBIT_LONG)) != 0) {
					ps.format("(%5.3e,%5.3e,%5.3e)\n",
							Double.valueOf(hit.getPosition()[0]),
							Double.valueOf(hit.getPosition()[1]),
							Double.valueOf(hit.getPosition()[2]));
				} else {
					ps.format("    no position available         \n");
				}

				// ps.print("        id-fields: (" + idDecoder( hit
				// ).valueString() << ")" << std::endl ;
			}
		}

		String type() {
			return LCIO.CALORIMETERHIT;
		}
	}

	class TrackerHitPrinter extends LCTypePrinter {
		void print(LCCollection coll, int nprint) {
			ps.println(" [id] | position (x,y,z)                | time     |  type |   EDep    | EDepError");
			for (int i = 0; i < nprint; i++) {
				TrackerHit hit = (TrackerHit) coll.getElementAt(i);

				// ps.format(" [%08x] | (%5.3e,%5.3e,%5.3e) | %5.3e | %5.3e | [%d] \n",
				ps.format(
						" [%08x] | (%5.3e,%5.3e,%5.3e) | %5.3e | [%d] | %4.3e | %4.3e \n",
						Integer.valueOf(hit.hashCode()),
						Double.valueOf(hit.getPosition()[0]),
						Double.valueOf(hit.getPosition()[1]),
						Double.valueOf(hit.getPosition()[2]),
						// Double.valueOf(hit.getdEdx()),
						Double.valueOf(hit.getTime()),
						Integer.valueOf(hit.getType()),
						Double.valueOf(hit.getEDep()),
						Double.valueOf(hit.getEDepError()));
			}
		}

		String type() {
			return LCIO.TRACKERHIT;
		}
	}

	class ClusterPrinter extends LCTypePrinter {
		void print(LCCollection coll, int nprint) {
			ps.println(" [   id   ] | type | energy    |      position ( x,y,z)          |  itheta   |  iphi ");

			for (int i = 0; i < nprint; i++) {
				Cluster clu = (Cluster) coll.getElementAt(i);

				ps.format(
						" [%08x] | %d | %5.3e | (%5.3e,%5.3e,%5.3e) | %4.2e | %4.2e \n",
						Integer.valueOf(clu.hashCode()),
						Integer.valueOf(clu.getType()),
						Double.valueOf(clu.getEnergy()),
						Double.valueOf(clu.getPosition()[0]),
						Double.valueOf(clu.getPosition()[1]),
						Double.valueOf(clu.getPosition()[2]),
						Float.valueOf(clu.getITheta()),
						Float.valueOf(clu.getIPhi()));

				ps.print(" errors (6 pos)/( 3 dir): (");

				for (int j = 0; j < 6; j++) {
					ps.format("%4.2e, ",
							Double.valueOf(clu.getPositionError()[j]));
				}

				ps.print(")/(");

				for (int j = 0; j < 3; j++) {
					ps.format("%4.2e, ",
							Double.valueOf(clu.getDirectionError()[j]));
				}

				ps.println(")");

				ps.print(" clusters(e): ");

				List clusters = clu.getClusters();

				for (int j = 0; j < clusters.size(); j++) {
					if (clusters.get(j) != null) {
						ps.format("%4.2e, ", Double.valueOf(((Cluster) clusters
								.get(j)).getEnergy()));
					} else {
						ps.format("WARNING: Sub-cluster " + j + " is null!\n");
					}
				}

				ps.println();

				int flag = coll.getFlag();

				if ((flag & (1 << LCIO.CLBIT_HITS)) != 0) {
					ps.print(" hits ->");
					List hits = clu.getCalorimeterHits();
					float contr[] = clu.getHitContributions();

					for (int j = 0; j < hits.size(); j++) {
						ps.format(" (%4.3e), ", Double.valueOf(contr[j]));
					}
				}

				ps.println();
			}
		}

		String type() {
			return LCIO.CLUSTER;
		}
	}

	class LCGenericObjectPrinter extends LCTypePrinter {
		void print(LCCollection coll, int nprint) {
			int flag = coll.getFlag();

			boolean isFixedSize = ((flag & (1 << LCIO.GOBIT_FIXED)) != 0);

			ps.println(" "
					+ coll.getParameters().getStringVal("DataDescription")
					+ " - isFixedSize: " + (isFixedSize ? "true" : "false"));

			for (int i = 0; i < nprint; i++) {
				LCGenericObject obj = (LCGenericObject) coll.getElementAt(i);

				for (int j = 0; j < obj.getNInt(); j++) {
					ps.print(" i:");
					ps.print(obj.getIntVal(j));
				}

				for (int j = 0; j < obj.getNDouble(); j++) {
					ps.print(" d:");
					ps.print(obj.getDoubleVal(j));
				}

				for (int j = 0; j < obj.getNFloat(); j++) {
					ps.print(" f:");
					ps.print(obj.getFloatVal(j));
				}
			}
		}

		String type() {
			return LCIO.LCGENERICOBJECT;
		}
	}

	class MCParticlePrinter extends LCTypePrinter {
		void print(LCCollection coll, int nprint) {
			Map mcpmap = new HashMap();

			for (int i = 0; i < coll.size(); i++) {
				mcpmap.put(coll.getElementAt(i), Integer.valueOf(i));
			}

			ps.println(" index | [   id   ] | index [parents] | [daughters] | PDG | (px, py, pz) | GenStatus | SimStatus | vertex (x,y,z) | endpoint(x,y,z)"
					+ " | mass | charge | energy");

			for (int i = 0; i < coll.size(); i++) {
				MCParticle part = (MCParticle) coll.getElementAt(i);

				ps.print(" " + i + " | ");

				ps.format("[%08x] | [", Integer.valueOf(part.hashCode()));

				for (int k = 0; k < part.getParents().size(); k++) {
					if (k > 0)
						ps.print(",");
					ps.print(mcpmap.get(part.getParents().get(k)));
				}

				ps.print("] | [");

				for (int k = 0; k < part.getDaughters().size(); k++) {
					if (k > 0)
						ps.print(",");
					ps.print(mcpmap.get(part.getDaughters().get(k)));
				}

				ps.format(
						"] | %d | (%5.3e, %5.3e, %5.3e) | %d | %08x | (%5.3e, %5.3e, %5.3e) | (",
						Integer.valueOf(part.getPDG()),
						Double.valueOf(part.getMomentum()[0]),
						Double.valueOf(part.getMomentum()[1]),
						Double.valueOf(part.getMomentum()[2]),
						Integer.valueOf(part.getGeneratorStatus()),
						Integer.valueOf(part.getSimulatorStatus()),
						Double.valueOf(part.getVertex()[0]),
						Double.valueOf(part.getVertex()[1]),
						Double.valueOf(part.getVertex()[2]));

				try {
					part.getEndpoint();
					ps.format("%e %e %e",
							Double.valueOf(part.getEndpoint()[0]),
							Double.valueOf(part.getEndpoint()[1]),
							Double.valueOf(part.getEndpoint()[2]));
				} catch (Exception e) {
					ps.print(" not set");
				}

				ps.format(") | %e | %e | %e", Double.valueOf(part.getMass()),
						Double.valueOf(part.getCharge()),
						Double.valueOf(part.getEnergy()));

				ps.println();
			}
		}

		String type() {
			return LCIO.MCPARTICLE;
		}
	}

	class LCFloatVecPrinter extends LCTypePrinter {
		void print(LCCollection coll, int nprint) {
			ps.println(" element index: val0, val1, ...");
			for (int i = 0; i < nprint; i++) {
				LCFloatVec vec = (LCFloatVec) coll.getElementAt(i);
				ps.print(i + ": ");
				float arr[] = vec.toArray();
				for (int j = 0; j < arr.length; j++) {
					ps.print(arr[j]);
					if (j < arr.length - 1)
						ps.print(", ");
				}
			}
		}

		String type() {
			return LCIO.LCFLOATVEC;
		}
	}

	class LCIntVecPrinter extends LCTypePrinter {
		void print(LCCollection coll, int nprint) {
			ps.println(" element index: val0, val1, ...");
			for (int i = 0; i < nprint; i++) {
				LCIntVec vec = (LCIntVec) coll.getElementAt(i);
				ps.print(i + ": ");
				int arr[] = vec.toArray();
				for (int j = 0; j < arr.length; j++) {
					ps.print(arr[j]);
					if (j < arr.length - 1)
						ps.print(", ");
				}
			}
		}

		String type() {
			return LCIO.LCINTVEC;
		}
	}

	class LCStrVecPrinter extends LCTypePrinter {
		void print(LCCollection coll, int nprint) {
			ps.println(" element index: val0, val1, ...");
			for (int i = 0; i < nprint; i++) {
				LCStrVec vec = (LCStrVec) coll.getElementAt(i);
				ps.print(i + ": ");
				String arr[] = vec.toArray();
				for (int j = 0; j < arr.length; j++) {
					ps.print(arr[j]);
					if (j < arr.length - 1)
						ps.print(", ");
				}
			}
		}

		String type() {
			return LCIO.LCSTRVEC;
		}
	}

	class VertexPrinter extends LCTypePrinter {
		void print(LCCollection coll, int nprint) {
			ps.println(" [   id   ] | pri |    alg. type    | chi2 | prob. |          position ( x,y,z)       | [assRecP]");

			for (int i = 0; i < nprint; i++) {
				Vertex v = (Vertex) coll.getElementAt(i);

				ps.format(
						" [%08x] | %3s | %15s | %4.2e | %4.2e | (%5.3e,%5.3e,%5.3e) | [%08x] \n",
						Integer.valueOf(v.hashCode()),
						v.isPrimary() ? "yes" : "no",
						v.getAlgorithmType(),
						Double.valueOf(v.getChi2()),
						Double.valueOf(v.getProbability()),
						Double.valueOf(v.getPosition()[0]),
						Double.valueOf(v.getPosition()[1]),
						Double.valueOf(v.getPosition()[2]),
						(v.getAssociatedParticle() != null ? Integer.valueOf(v
								.getAssociatedParticle().hashCode())
								: new Integer(0)));

				// Print covariance matrix.
				ps.print("    covariance( px,py,pz) : (");
				for (int j = 0; j < 6; j++) {
					ps.format("%4.2e, ", Double.valueOf(v.getCovMatrix()[j]));
				}
				ps.println(")");
			}
		}

		String type() {
			return LCIO.VERTEX;
		}
	}

	class ReconstructedParticlePrinter extends LCTypePrinter {
		void print(LCCollection coll, int nprint) {
			ps.println(" [   id   ] | com | type |     momentum( px,py,pz)         | energy   | mass     | charge    |          position ( x,y,z)       | [pidUsed] | [sVtx] | [eVtx]");

			for (int i = 0; i < nprint; i++) {
				ReconstructedParticle recp = (ReconstructedParticle) coll
						.getElementAt(i);

				boolean compound = recp.isCompound();
				int type = recp.getType();

				int pidused = 0;
				if (recp.getParticleIDUsed() != null) {
					pidused = recp.getParticleIDUsed().hashCode();
				}

				ps.format(
						" [%08x] | %3s | %2d | (%5.3e,%5.3e,%5.3e) | %4.2e | %4.2e | %4.2e | (%5.3e,%5.3e,%5.3e) | [%08x] | [%08x] | [%08x]\n",
						Integer.valueOf(recp.hashCode()),
						compound ? "yes" : "no",
						Integer.valueOf(type),
						Double.valueOf(recp.getMomentum()[0]),
						Double.valueOf(recp.getMomentum()[1]),
						Double.valueOf(recp.getMomentum()[2]),
						Double.valueOf(recp.getEnergy()),
						Double.valueOf(recp.getMass()),
						Double.valueOf(recp.getCharge()),
						Double.valueOf(recp.getReferencePoint()[0]),
						Double.valueOf(recp.getReferencePoint()[1]),
						Double.valueOf(recp.getReferencePoint()[2]),
						Integer.valueOf(pidused),
						(recp.getStartVertex() != null ? Integer.valueOf(recp
								.getStartVertex().hashCode()) : 0),
						(recp.getEndVertex() != null ? Integer.valueOf(recp
								.getEndVertex().hashCode()) : 0));

				// Print covariance matrix.
				ps.print("    covariance( px,py,pz,E) : (");
				for (int j = 0; j < 10; j++) {
					ps.format("%4.2e, ", Double.valueOf(recp.getCovMatrix()[j]));
				}
				ps.println(")");

				// Print particles.
				ps.print("    particles ( [   id   ] ):");
				for (int j = 0; j < recp.getParticles().size(); j++) {
					ps.format(
							"[%08x], ",
							Integer.valueOf(recp.getParticles().get(j)
									.hashCode()));
				}
				ps.println();

				// Print tracks.
				ps.print("    tracks ( [   id   ] ): ");
				ps.println("number of tracks: " + recp.getTracks().size());
				for (int j = 0; j < recp.getTracks().size(); j++) {
					if (recp.getTracks().get(j) != null) {
						ps.format(
								"[%08x], ",
								Integer.valueOf(recp.getTracks().get(j)
										.hashCode()));
					} else {
						ps.format("WARNING: Track at index " + j
								+ " is null!\n");
					}
				}
				ps.println();

				// Print clusters.
				ps.print("    clusters ( [   id   ] ): ");
				for (int j = 0; j < recp.getClusters().size(); j++) {
					ps.format(
							"[%08x], ",
							Integer.valueOf(recp.getClusters().get(j)
									.hashCode()));
				}
				ps.println();

				// Print the Particle IDs.
				ps.print("    particle ids ( [id], PDG, (type)): ");
				for (int j = 0; j < recp.getParticleIDs().size(); j++) {
					ParticleID pid = (ParticleID) recp.getParticleIDs().get(j);
					ps.format("[%08x], %6d, (%6d)  ",
							Integer.valueOf(pid.hashCode()),
							Integer.valueOf(pid.getPDG()),
							Integer.valueOf(pid.getType()));
				}
				ps.println();
			}
		}

		String type() {
			return LCIO.RECONSTRUCTEDPARTICLE;
		}
	}

	class ParticleIDPrinter extends LCTypePrinter {
		void print(LCCollection coll, int nprint) {
			ps.println(" [   id   ] | algo.type | like | pdg | type");

			for (int i = 0; i < nprint; i++) {
				ParticleID id = (ParticleID) coll.getElementAt(i);

				ps.format(" %08 | %d | %e | %d | %d ",
						Integer.valueOf(id.hashCode()),
						Integer.valueOf(id.getAlgorithmType()),
						Double.valueOf(id.getLikelihood()),
						Integer.valueOf(id.getPDG()),
						Integer.valueOf(id.getType()));

				ps.print("params : (");
				for (int j = 0; j < id.getParameters().length; j++) {
					ps.print(id.getParameters()[j] + ", ");
				}
				ps.print(")");
			}
		}

		String type() {
			return LCIO.PARTICLEID;
		}
	}

	class RawCalorimeterHitPrinter extends LCTypePrinter {
		void print(LCCollection coll, int nprint) {
			ps.println(" [   id   ] |  [cellID0] | [cellID1]  | amplitude |  time  ");

			for (int i = 0; i < nprint; i++) {
				RawCalorimeterHit hit = (RawCalorimeterHit) coll
						.getElementAt(i);
				ps.format(" [0x%08x] | [0x%08x] | [0x%08x] | %10d | %10d \n",
						Integer.valueOf(hit.hashCode()),
						Integer.valueOf(hit.getCellID0()),
						Integer.valueOf(hit.getCellID1()),
						Integer.valueOf(hit.getAmplitude()),
						Integer.valueOf(hit.getTimeStamp()));
			}
		}

		String type() {
			return LCIO.RAWCALORIMETERHIT;
		}
	}

	class TrackPrinter extends LCTypePrinter {
		void print(LCCollection coll, int nprint) {
			int flag = coll.getFlag();

			boolean havehits = ((flag & (1 << LCIO.TRBIT_HITS)) != 0);

			ps.println(" [   id   ] |   type   |    d0    |  phi     | omega    |    z0     | tan lambda|   reference point(x,y,z)        |    dEdx  |  dEdxErr |   chi2   ");

			for (int i = 0; i < nprint; i++) {
				Track trk = (Track) coll.getElementAt(i);

				ps.format(
						" [%08x] | %08x | %4.2e | %4.2e | %4.2e | %5.3e | %5.3e | (%5.3e,%5.3e,%5.3e) | %4.2e | %4.2e | %4.2e \n",
						Integer.valueOf(trk.hashCode()),
						Integer.valueOf(trk.getType()),
						Double.valueOf(trk.getD0()),
						Double.valueOf(trk.getPhi()),
						Double.valueOf(trk.getOmega()),
						Double.valueOf(trk.getZ0()),
						Double.valueOf(trk.getTanLambda()),
						Double.valueOf(trk.getReferencePoint()[0]),
						Double.valueOf(trk.getReferencePoint()[1]),
						Double.valueOf(trk.getReferencePoint()[2]),
						Double.valueOf(trk.getdEdx()),
						Double.valueOf(trk.getdEdxError()),
						Double.valueOf(trk.getChi2()));

				// Print errors.
				ps.print(" errors: ");
				for (int j = 0; j < 15; j++) {
					ps.format("%4.2e, ", Double.valueOf(trk.getCovMatrix()[j]));
				}

				// Print associated tracks.
				ps.print(" tracks(id): ");
				List tracks = trk.getTracks();

				for (int j = 0; j < tracks.size(); j++) {
					ps.format("[%08x], ",
							Integer.valueOf(tracks.get(j).hashCode()));
				}
				ps.println();

				// Print hits if stored.
				if (havehits) {
					ps.print(" hits ->");
					List hits = trk.getTrackerHits();
					if (hits == null || hits.size() == 0)
						continue;
					for (int j = 0; j < hits.size(); j++) {
						// Guard against null hit. For some reason, above checks
						// for null
						// list and size of zero don't cover all cases where
						// this may fail.
						try {
							ps.format("[%08x] ",
									Integer.valueOf(hits.get(j).hashCode()));
						} catch (NullPointerException x) {
						}
					}
					ps.println();
				}

				// Print radius of innermost hit.
				ps.print(" radius of innermost hit "
						+ trk.getRadiusOfInnermostHit() + " / mm , ");

				// Print subdetector hit numbers.
				ps.print(" subdetector Hit numbers : ");
				for (int j = 0; j < trk.getSubdetectorHitNumbers().length; j++) {
					ps.print(trk.getSubdetectorHitNumbers()[j] + ", ");
				}
				ps.println();
			}
		}

		String type() {
			return LCIO.TRACK;
		}
	}

	class TPCHitPrinter extends LCTypePrinter {
		void print(LCCollection coll, int nprint) {
			int flag = coll.getFlag();

			ps.println(" [   id   ]  | [cellID0] | time | charge | quality ");
			ps.println("  -> raw data (bytes) : ");

			for (int i = 0; i < nprint; i++) {
				TPCHit hit = (TPCHit) coll.getElementAt(i);

				ps.format(" [%08x] | %e | %e | %e",
						Integer.valueOf(hit.hashCode()),
						Double.valueOf(hit.getTime()),
						Double.valueOf(hit.getCharge()),
						Double.valueOf(hit.getQuality())

				);

				boolean raw = ((flag & (1 << LCIO.TPCBIT_RAW)) != 0);
				int nwords = hit.getNRawDataWords();
				ps.print("  ->  ");

				for (int j = 0; j < nwords; j++) {
					ps.format(" [%08]", Integer.valueOf(hit.getRawDataWord(j)));
				}

				ps.println();
			}
		}

		String type() {
			return LCIO.TPCHIT;
		}
	}

	class TrackerDataPrinter extends LCTypePrinter {
		void print(LCCollection coll, int nprint) {
			ps.println(" [   id   ] |  [cellID0] |  [cellID1] |   time    | chargeADC ");

			for (int i = 0; i < nprint; i++) {
				TrackerData hit = (TrackerData) coll.getElementAt(i);

				ps.format(" [%08x] | %08x | %08x | %5.3f  | ",
						Integer.valueOf(hit.hashCode()),
						Integer.valueOf(hit.getCellID0()),
						Integer.valueOf(hit.getCellID1()),
						Float.valueOf(hit.getTime()));

				float[] charge = hit.getChargeValues();
				if (charge != null) {
					for (int j = 0; j < charge.length; j++) {
						ps.print(charge[j] + ",");
					}
				}
				ps.println();
			}
		}

		String type() {
			return LCIO.TRACKERDATA;
		}
	}

	class TrackerPulsePrinter extends LCTypePrinter {
		void print(LCCollection coll, int nprint) {
			// ps.println(" [   id   ] |  [cellID0]  |  [cellID1]  |  time | charge | quality  | corr.Data  ");
			ps.println(" [   id   ] |  [cellID0]  |  [cellID1]  |  time | charge | quality  | corr.Data  | cov(c,c), cov(t,c), cov(t,t)");

			for (int i = 0; i < nprint; i++) {
				TrackerPulse hit = (TrackerPulse) coll.getElementAt(i);

				ps.format(" [%08x] | %08x | %08x | %5.3f | %5.3f | %08x | ",
						Integer.valueOf(hit.hashCode()),
						Integer.valueOf(hit.getCellID0()),
						Integer.valueOf(hit.getCellID1()),
						Float.valueOf(hit.getTime()),
						Float.valueOf(hit.getCharge()),
						Integer.valueOf(hit.getQuality()));

				TrackerData corr = hit.getTrackerData();
				ps.format("[%08x] ",
						Integer.valueOf(corr != null ? corr.hashCode() : 0));

				// Print covariance matrix.
				ps.print("  cov( cc,tc,tt) : (");
				for (int j = 0; j < hit.getCovMatrix().length; j++) {
					ps.format("%4.2e, ", Double.valueOf(hit.getCovMatrix()[j]));
				}
				ps.println(")");
			}
		}

		String type() {
			return LCIO.TRACKERPULSE;
		}
	}

	class TrackerRawDataPrinter extends LCTypePrinter {
		void print(LCCollection coll, int nprint) {
			ps.println(" [   id   ] |  [cellID0]   |  [cellID1] |   time    | chargeADC ");
			for (int i = 0; i < nprint; i++) {
				TrackerRawData hit = (TrackerRawData) coll.getElementAt(i);

				ps.format(" [%08x] | [%08x] | [%08x] | %10d | ",
						Integer.valueOf(hit.hashCode()),
						Integer.valueOf(hit.getCellID0()),
						Integer.valueOf(hit.getCellID1()),
						Integer.valueOf(hit.getTime()));

				short charge[] = hit.getADCValues();

				if (charge != null) {
					for (int j = 0; j < charge.length; j++) {
						ps.print(charge[j] + ",");
					}
				} else {
					ps.print("none");
				}

				ps.println();
			}
		}

		String type() {
			return LCIO.TRACKERRAWDATA;
		}
	}
}
