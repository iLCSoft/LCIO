package hep.lcio.implementation.event;

import hep.lcio.event.LCObject;
import hep.lcio.event.LCRelation;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Represents a set of relationships
 * @author Tony Johnson
 * @version $Id: ILCRelation.java,v 1.1 2004-05-24 03:33:02 tonyj Exp $
 */
public class ILCRelation implements LCRelation
{
   // FixMe: Is relationship reversible - I guess not, but should it be?
   // FixMe: Should this be class instead of String?
   
   private String from;
   private String to;
   // Entry maps from "from" to relationship. If there is only one relationship
   // for this "from" it will be the map entry, otherwise the map entry will be
   // a list of all the relationships involving this "from"
   private Map entryMap = new HashMap();
   private List entries = new ArrayList();
   private static class Relationship
   {
      Relationship(LCObject from, LCObject to, float weight)
      {
         this.from = from;
         this.to = to;
         this.weight = weight;
      }
      private LCObject from;
      private LCObject to;
      private float weight;
   }
   private Relationship findRelationship(Object from, int index)
   {
      Object result = entryMap.get(from);
      if (result == null) throw new ArrayIndexOutOfBoundsException();
      if (result instanceof List) return (Relationship) ((List)result).get(index);
      if (index != 0) throw new ArrayIndexOutOfBoundsException();
      return (Relationship) result;
   }
   
   protected ILCRelation(String from, String to)
   {
      this.from = from;
      this.to = to;
   }
   
   public void addRelation(LCObject from, LCObject to)
   {
      addRelation(from,to,1);
   }
   
   public void addRelation(LCObject from, LCObject to, float weight)
   {
      Relationship r = new Relationship(from,to,weight);
      entries.add(r);
      addMap(from,r);
   }
   private void addMap(LCObject object, Relationship r)
   {
      Object x = entryMap.get(object);
      if (x == null) entryMap.put(object,r);
      else if (x instanceof List) ((List)x).add(r);
      else
      {
         List l = new ArrayList();
         l.add(x);
         l.add(r);
         entryMap.put(object,l);
      }
   }
   public String getFromType()
   {
      return from;
   }
      
   public String getToType()
   {
      return to;
   }
   public LCObject getRelation(LCObject from)
   {
      return getRelation(from,0);
   }
   public float getWeight(LCObject from)
   {
      return getWeight(from,0);
   }  
   // FixMe: This should have a better name getFromObject(int index)?
   public LCObject getRelation(int index)
   {
      return ((Relationship) entries.get(index)).from;
   }
   
   public LCObject getRelation(LCObject from, int index)
   {
      Relationship r = findRelationship(from,index);
      return r.to;
   }
   
   public float getWeight(LCObject from, int index)
   {
      Relationship r = findRelationship(from,index);
      return r.weight;      
   }
   
   public int numberOfRelations()
   {
      //FixMe: Is this the number of froms? or the total number of relationships?
      return entries.size();
   }
   
   public int numberOfRelations(LCObject from)
   {
      if (from == null) return entries.size();
      else 
      {
         Object x = entryMap.get(from);
         if (x == null) return 0;
         if (x instanceof List) return ((List)x).size();
         return 1;
      }
   }
   
   public void useCaching(boolean val)
   {
      // FixMe: Not needed
   }
}
