package hep.lcio.implementation.event;

import hep.lcio.event.ParticleID;


/**
 * @author Tony Johnson
 * @version $Id: IParticleID.java,v 1.3 2004-09-16 10:46:36 gaede Exp $
 */
public class IParticleID extends ILCObject implements ParticleID
{
   protected int algorithmType ;
   protected float logLikelihood;
   protected int pdg;
   protected float[] parameters;
   protected int type;
   
   
   
   public float getLoglikelihood()
   {
      return logLikelihood;
   }
   
   public int getPDG()
   {
      return pdg;
   }
   
   public float[] getParameters()
   {
      return parameters;
   }
   
   public int getType()
   {
      return type;
   }
   
   public void setPDG(int PDG)
   {
      checkAccess();
      this.pdg = PDG;
   }
   
   public void setLoglikelihood(float loglikelihood)
   {
      checkAccess();
      this.logLikelihood = loglikelihood;
   }
   
   public void setParameters(float[] parameters)
   {
     checkAccess();
     this.parameters = parameters;
   }

   public void setType(int type)
   {
      checkAccess();
      this.type = type;
   }
   
   public int getAlgorithmType() {
     return algorithmType;
   }

   public void setAlgorithmType(int i) {
     algorithmType = i;
   }

}
