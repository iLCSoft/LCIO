package hep.lcio.implementation.event;

import hep.lcio.event.ParticleID;


/**
 * @author Tony Johnson
 * @version $Id: IParticleID.java,v 1.2 2004-09-16 07:15:33 gaede Exp $
 */
public class IParticleID extends ILCObject implements ParticleID
{
   protected String identifier;
   protected float logLikelihood;
   protected int pdg;
   protected float[] parameters;
   protected int type;
   
   
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
