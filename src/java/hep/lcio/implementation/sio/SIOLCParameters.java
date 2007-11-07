package hep.lcio.implementation.sio;

import hep.io.sio.SIOInputStream;
import hep.io.sio.SIOOutputStream;

import hep.lcio.event.LCParameters;

import hep.lcio.implementation.event.ILCParameters;

import java.io.IOException;


/**
 *
 * @author Frank Gaede
 * @version $Id: SIOLCParameters.java,v 1.3 2007-11-07 20:46:23 jeremy Exp $
 */
class SIOLCParameters extends ILCParameters
{
   SIOLCParameters(SIOInputStream in) throws IOException
   {
	int nIntParameters =  in.readInt(); 
	for (int i = 0; i < nIntParameters; i++) {
	  String key = in.readString();
	  int nInt = in.readInt() ;
	  int[] iv = new int[nInt] ;
	  for (int j = 0; j < nInt; j++) {
		  iv[j] = in.readInt() ;
	  }
	  _intMap.put( key , iv ) ; 
	}	
	int nFloatParameters =  in.readInt(); 
	for (int i = 0; i < nFloatParameters; i++) {
	  String key = in.readString();
	  int nFloat = in.readInt() ;
	  float[] fv = new float[nFloat] ;
	  for (int j = 0; j < nFloat; j++) {
		  fv[j] = in.readFloat() ;
	  }
	  _floatMap.put( key , fv ) ; 
	}	

	int nStringParameters =  in.readInt(); 
	for (int i = 0; i < nStringParameters; i++) {
	  String key = in.readString();
	  int nString = in.readInt() ;
	  String[] sv = new String[nString] ;
	  for (int j = 0; j < nString; j++) {
		  sv[j] = in.readString() ;
	  }
	  _stringMap.put( key , sv ) ; 
	}	


   }

   static void write(LCParameters params, SIOOutputStream out) throws IOException
   {

//	if (params instanceof SIOLCParameters)
//	   ((SIOLCParameters) params).write(out);
//	else

	String[] keys = params.getIntKeys() ;
	int nKeys = keys.length;  
	out.writeInt( nKeys ) ;
	for (int i = 0; i < nKeys; i++) {
	  out.writeString( keys[i] ) ;
	  int[] v = params.getIntVals( keys[i] ) ;
	  int nVal = v.length ;
	  out.writeInt(nVal) ;
	  for (int j = 0; j < nVal; j++) {
		 out.writeInt( v[j] ) ; 
	  }
	}	
	keys = params.getFloatKeys() ;
	nKeys = keys.length;  
	out.writeInt( nKeys ) ;
	for (int i = 0; i < nKeys; i++) {
	  out.writeString( keys[i] ) ;
	  float[] v = params.getFloatVals( keys[i] ) ;
	  int nVal = v.length ;
	  out.writeInt(nVal) ;
	  for (int j = 0; j < nVal; j++) {
		 out.writeFloat( v[j] ) ; 
	  }
	}	
	keys = params.getStringKeys() ;
	nKeys = keys.length;  
	out.writeInt( nKeys ) ;
	for (int i = 0; i < nKeys; i++) {
	  out.writeString( keys[i] ) ;
	  String[] v = params.getStringVals( keys[i] ) ;
	  int nVal = v.length ;
	  out.writeInt(nVal) ;
	  for (int j = 0; j < nVal; j++) {
		 out.writeString( v[j] ) ; 
	  }
	}	
   }

}
