package hep.lcio.implementation.event;

import java.util.HashMap;
import java.util.Map;

import hep.lcio.event.LCParameters;

/** Implementation of generic named parameters.
 * @author gaede
 * @version $Id: ILCParameters.java,v 1.2 2004-09-24 10:39:29 tonyj Exp $
 */
public class ILCParameters extends ILCObject implements LCParameters
{
   protected Map _intMap = new HashMap() ;
   protected Map _floatMap = new HashMap() ;
   protected Map _stringMap = new HashMap() ;
   
   public int getIntVal(String key)
   { 
      int[] vals = (int[]) _intMap.get( key ) ;
      if(vals==null||vals.length==0)
         return 0 ;
      
      return vals[0] ;
   }
   
   public float getFloatVal(String key)
   {
      
      float[] vals = (float[]) _floatMap.get( key ) ;
      if(vals==null||vals.length==0)
         return 0 ;
      
      return vals[0] ;
   }
   
   public String getStringVal(String key)
   {
      
      String[] vals = (String[]) _stringMap.get( key ) ;
      if(vals==null||vals.length==0)
         return "" ;
      
      return vals[0] ;
   }
   
   public int[] getIntVals(String key)
   {
      return (int[]) _intMap.get(key) ;
   }
   
   public float[] getFloatVals(String key)
   {
      return (float[]) _floatMap.get(key) ;
   }
   
   public String[] getStringVals(String key)
   {
      return (String[]) _stringMap.get(key) ;
   }
   
   public String[] getIntKeys()
   {
      Object[] ob = _intMap.keySet().toArray() ;
      String[] keys = new String[ ob.length ] ;
      for (int i = 0; i < keys.length; i++)
      {
         keys[i] = (String) ob[i] ;
      }
      return keys  ;
   }
   
   public String[] getFloatKeys()
   {
      Object[] ob = _floatMap.keySet().toArray() ;
      String[] keys = new String[ ob.length ] ;
      for (int i = 0; i < keys.length; i++)
      {
         keys[i] = (String) ob[i] ;
      }
      return keys  ;
   }
   
   public String[] getStringKeys()
   {
      Object[] ob = _stringMap.keySet().toArray() ;
      String[] keys = new String[ ob.length ] ;
      for (int i = 0; i < keys.length; i++)
      {
         keys[i] = (String) ob[i] ;
      }
      return keys  ;
   }
   
   public int getNInt(String key)
   {
      int[] a = (int[]) _intMap.get(key) ;
      if( a ==null ) return 0 ;
      return a.length ;
   }
   
   public int getNFloat(String key)
   {
      float[] a = (float[]) _floatMap.get(key) ;
      if( a ==null ) return 0 ;
      return a.length ;
   }
   
   public int getNString(String key)
   {
      String[] a = (String[]) _stringMap.get(key) ;
      if( a ==null ) return 0 ;
      return a.length ;
   }
   
   public void setValue(String key, int value)
   {
      checkAccess() ;
      int[] values =
      { value } ;
      _intMap.put(key,values) ;
   }
   
   public void setValue(String key, float value)
   {
      checkAccess() ;
      float[] values =
      { value } ;
      _floatMap.put(key,values) ;
   }
   
   public void setValue(String key, String value)
   {
      checkAccess() ;
      String[] values =
      { value } ;
      _stringMap.put(key,values) ;
   }
   
   public void setValues(String key, int[] values)
   {
      checkAccess() ;
      _intMap.put(key,values) ;
   }
   
   public void setValues(String key, float[] values)
   {
      checkAccess() ;
      _floatMap.put(key,values) ;
   }
   
   public void setValues(String key, String[] values)
   {
      checkAccess() ;
      _stringMap.put(key,values) ;
   }
   
}
