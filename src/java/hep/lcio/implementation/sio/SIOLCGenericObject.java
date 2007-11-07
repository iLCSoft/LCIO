package hep.lcio.implementation.sio;
import hep.io.sio.SIOInputStream;
import hep.io.sio.SIOOutputStream;
import hep.lcio.event.LCGenericObject;
import hep.lcio.event.LCIO;
import hep.lcio.implementation.event.ILCGenericObject;
import java.io.IOException;

/**
 *
 * @author gaede
 * @version $Id: SIOLCGenericObject.java,v 1.2 2007-11-07 20:46:22 jeremy Exp $
 */
class SIOLCGenericObject extends ILCGenericObject
{

	/** Creates a new instance of SIOLCGenericObject with variable size*/
   public SIOLCGenericObject(SIOInputStream in, SIOEvent owner, 
   							int major, int minor) 
   							throws IOException {
   	super() ;
   	int nInt = nInt = in.readInt();
   	int nFloat = in.readInt();
   	int nDouble = in.readInt();
    read(in,owner,major,minor,nInt,nFloat,nDouble ) ; 
   }

	/** Creates a new instance of SIOLCGenericObject with fixed size*/
   public SIOLCGenericObject(SIOInputStream in, SIOEvent owner, 
			int major, int minor, int nInt, int nFloat, int nDouble) 
			throws IOException {
   	super(nInt,nFloat,nDouble) ;	
    read(in,owner,major,minor,nInt,nFloat,nDouble ) ; 
   }
   
   protected void read(SIOInputStream in, SIOEvent owner, int major, int minor,
   						int nInt, int nFloat, int nDouble )
   						throws IOException{
   		
   	int[] intVals = new int[nInt] ;
   	for (int i=0; i<nInt; i++){
      intVals[i] = in.readInt();
    }
    setIntVals(intVals) ;
   	float[] floatVals = new float[nFloat] ;
   	for (int i=0; i<nFloat; i++){
      floatVals[i] = in.readFloat();
    }
   	setFloatVals(floatVals) ;
   	double[] doubleVals = new double[nDouble] ;
   	for (int i=0; i<nDouble; i++){
     doubleVals[i] = in.readDouble();
    }
   	setDoubleVals(doubleVals) ;
   	
    in.readPTag(this);
   }

   static void write(LCGenericObject object, SIOOutputStream out, int flag) throws IOException
   {
      if (object instanceof SIOLCGenericObject)
         ((SIOLCGenericObject) object).write(out,flag);
      else
      {
        if( (flag & (1 << LCIO.GOBIT_FIXED)) == 0 ){
           out.writeInt(object.getNInt());
           out.writeInt(object.getNFloat());
           out.writeInt(object.getNDouble());
        }
        for(int i=0;i<object.getNInt();i++)
            out.writeInt( object.getIntVal(i)) ;
        for(int i=0;i<object.getNFloat();i++)
            out.writeFloat( object.getFloatVal(i)) ;
        for(int i=0;i<object.getNDouble();i++)
            out.writeDouble( object.getDoubleVal(i)) ;
        
        out.writePTag(object);
      }
   }
   private void write(SIOOutputStream out, int flag) throws IOException
   {
    if( (flag & (1 << LCIO.GOBIT_FIXED)) == 0 ){
        out.writeInt(_intVec.length);
        out.writeInt(_floatVec.length);
        out.writeInt(_doubleVec.length);
     }
     for(int i=0;i<_intVec.length;i++)
         out.writeInt( _intVec[i]) ;
     for(int i=0;i<_floatVec.length ;i++)
         out.writeFloat( _floatVec[i] ) ;
     for(int i=0;i<_doubleVec.length;i++)
         out.writeDouble( _doubleVec[i] ) ;
      out.writePTag(this);
   }
   
   
}
