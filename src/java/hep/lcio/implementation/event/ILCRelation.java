package hep.lcio.implementation.event;

import hep.lcio.event.LCObject;
import hep.lcio.event.LCRelation;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

/**
 * Represents a set of relationships
 * @author Tony Johnson
 * @version $Id: ILCRelation.java,v 1.3 2004-09-06 14:46:14 gaede Exp $
 */
public class ILCRelation implements LCRelation
{



//    private String to;
//    // Entry maps from "from" to relationship. If there is only one relationship
//    // for this "from" it will be the map entry, otherwise the map entry will be
//    // a list of all the relationships involving this "from"
//    private Map fromMap = null;
//    private Map toMap = null;
//    private List entries = new ArrayList();
//    private static class Relationship
//    {
//       Relationship(LCObject from, LCObject to, float weight)
//       {
//          this.from = from;
//          this.to = to;
//          this.weight = weight;
//       }
//       private LCObject from;
//       private LCObject to;
//       private float weight;
//    }
//    private static class Value
//    {
//       private int val;
//       private int[] array;
//       Value(int val)
//       {
//          this.val = val;
//       }
//       void createArray()
//       {
//          array = new int[val];
//       }
//       void push(int n)
//       {
//          array[array.length-val] = n;
//          val--;
//       }
//    }
   
//    protected ILCRelation(String from, String to)
//    {
//       this.from = from;
//       this.to = to;
//    }
   
//    public String getFromType()
//    {
//       return from;
//    }  
//    public String getToType()
//    {
//       return to;
//    }
//    public LCObject getFrom(int index)
//    {
//       return ((Relationship) entries.get(index)).from;
//    }
//    public LCObject getTo(int index)
//    {
//       return ((Relationship) entries.get(index)).to;
//    }
//    public float getWeight(int index)
//    {
//       return ((Relationship) entries.get(index)).weight;
//    }
//    public int numberOfRelations()
//    {
//       return entries.size();
//    }
//    public void removeRelation(int index)
//    {
//       entries.remove(index);
//       fromMap = toMap = null;
//    }  
//    public void addRelation(LCObject from, LCObject to)
//    {
//       addRelation(from,to,1.0f);
//    }
//    public void addRelation(LCObject from, LCObject to, float weight)
//    {
//       Relationship r = new Relationship(from,to,weight);
//       entries.add(r);
//       fromMap = toMap = null;
//    }
//    public int[] getRelationsFrom(LCObject from)
//    {
//       if (fromMap == null) buildFromMap();
//       return (int[]) fromMap.get(from);
//    }
//    public int[] getRelationsTo(LCObject to)
//    {
//       if (toMap == null) buildToMap();
//       return (int[]) toMap.get(to);
//    }
//    private void buildFromMap()
//    {
//       fromMap = new HashMap();
//       for (Iterator i = entries.iterator(); i.hasNext(); )
//       {
//          Relationship rel = (Relationship) i.next();
//          Value v = (Value) fromMap.get(rel.from);
//          if (v == null)
//          {
//             v = new Value(1);
//             fromMap.put(rel.from,v);
//          }
//          else v.val++;
//       }
//       for (Iterator i = fromMap.values().iterator(); i.hasNext(); )
//       {
//          Value v = (Value) i.next();
//          v.createArray();
//       }
//       int n = 0;
//       for (Iterator i = entries.iterator(); i.hasNext(); n++)
//       {
//          Relationship rel = (Relationship) i.next();
//          Value v = (Value) fromMap.get(rel.from);
//          v.push(n);
//       }
//       for (Iterator i = fromMap.entrySet().iterator(); i.hasNext();)
//       {
//          Map.Entry entry = (Map.Entry) i.next();
//          Value v = (Value) entry.getValue();
//          entry.setValue(v.array);
//       }
//    }
//    private void buildToMap()
//    {
//       toMap = new HashMap();
//       for (Iterator i = entries.iterator(); i.hasNext(); )
//       {
//          Relationship rel = (Relationship) i.next();
//          Value v = (Value) toMap.get(rel.to);
//          if (v == null)
//          {
//             v = new Value(1);
//             toMap.put(rel.from,v);
//          }
//          else v.val++;
//       }
//       for (Iterator i = toMap.values().iterator(); i.hasNext(); )
//       {
//          Value v = (Value) i.next();
//          v.createArray();
//       }
//       int n = 0;
//       for (Iterator i = entries.iterator(); i.hasNext(); n++)
//       {
//          Relationship rel = (Relationship) i.next();
//          Value v = (Value) toMap.get(rel.to);
//          v.push(n);
//       }
//       for (Iterator i = toMap.entrySet().iterator(); i.hasNext();)
//       {
//          Map.Entry entry = (Map.Entry) i.next();
//          Value v = (Value) entry.getValue();
//          entry.setValue(v.array);
//       }
//    }
	/* (non-Javadoc)
	 * @see hep.lcio.event.LCRelation#getFrom()
	 */
	public LCObject getFrom() {
		// TODO Auto-generated method stub
		return null;
	}

	/* (non-Javadoc)
	 * @see hep.lcio.event.LCRelation#getTo()
	 */
	public LCObject getTo() {
		// TODO Auto-generated method stub
		return null;
	}

	/* (non-Javadoc)
	 * @see hep.lcio.event.LCRelation#getWeight()
	 */
	public float getWeight() {
		// TODO Auto-generated method stub
		return 0;
	}

}
