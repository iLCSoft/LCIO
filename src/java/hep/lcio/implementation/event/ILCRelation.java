package hep.lcio.implementation.event;

import hep.lcio.event.LCObject;
import hep.lcio.event.LCRelation;

/**
 * Represents a single relationship
 * @author Tony Johnson
 * @version $Id: ILCRelation.java,v 1.4 2004-09-17 06:30:38 tonyj Exp $
 */
public class ILCRelation implements LCRelation
{
   protected LCObject from;
   protected LCObject to;
   protected float weight;
   
   protected ILCRelation()
   {
      
   }
   public ILCRelation(LCObject from, LCObject to)
   {
      this(from,to,1);
   }
   public ILCRelation(LCObject from, LCObject to, float weight)
   {
      this.from = from;
      this.to = to;
      this.weight = weight;
   }
   public LCObject getFrom()
   {
      return from;
   }
   public LCObject getTo()
   {
      return to;
   }
   public float getWeight()
   {
      return weight;
   }
   public void setWeight(float weight)
   {
      this.weight = weight;
   }
}
