package hep.lcio.implementation.event;

import hep.lcio.event.ParticleID;


/**
 * @author Tony Johnson
 * @version $Id: IParticleID.java,v 1.1 2004-09-13 22:43:08 tonyj Exp $
 */
public class IParticleID extends ILCObject implements ParticleID
{
   protected float goodnessOfPID;
   protected String identifier;
   protected float logLikelihood;
   protected int pdg;
   protected float[] parameters;
   protected int type;
   
   public float getGoodnessOfPID()
   {
      return goodnessOfPID;
   }   
   
   public String getIdentifier()
   {
      return identifier;
   }
   
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
   
   public void setGoodnessOfPID(float goodnessOfPID)
   {
      checkAccess();
      this.goodnessOfPID = goodnessOfPID;
   }
   
   public void setIdentifier(String identifier)
   {
      checkAccess();
      this.identifier = identifier;
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
   
}
