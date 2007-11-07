package hep.lcio.implementation.sio;

import hep.io.sio.SIOInputStream;
import hep.io.sio.SIOOutputStream;

import hep.lcio.event.LCIO;

import hep.lcio.event.TPCHit;
import hep.lcio.implementation.event.ITPCHit;

import java.io.IOException;


/**
 *
 * @author Tony Johnson
 * @version $Id: SIOTPCHit.java,v 1.8 2007-11-07 20:46:23 jeremy Exp $
 */
class SIOTPCHit extends ITPCHit
{
   SIOTPCHit(SIOInputStream in, int flags, SIOEvent owner, int major, int minor) throws IOException
   {
      setParent(owner);
      cellID = in.readInt();
      time = in.readFloat();
      charge = in.readFloat();
      quality = in.readInt();
      
	if ((flags & (1 << LCIO.TPCBIT_RAW)) != 0)
      {
         int size = in.readInt() ;
         rawDataArray = new int[ size ] ;
         for (int i = 0; i < size; i++)
         {
            rawDataArray[i] = in.readInt() ;
         }
      }
	double version  = (double) major + ( (double) minor ) /  10. ;

// 	if ((flags & (1 << LCIO.TPCBIT_PTR)) != 0  && version > 1.0 )
//       in.readPTag(this) ;

	if( version > 1.2 ) { // the logic has been reverted in v1.3 !
	    if ( (flags & (1 << LCIO.TPCBIT_NO_PTR)) == 0 )  in.readPTag(this) ;
	} else {
	    if ( (flags & (1 << LCIO.TPCBIT_NO_PTR)) != 0 )  in.readPTag(this) ;
	}
	
   }
   
   static void write(TPCHit hit, SIOOutputStream out, int flags) throws IOException
   {
      if (hit instanceof SIOTPCHit)
         ((SIOTPCHit) hit).write(out, flags);
      else
      {
         out.writeInt(hit.getCellID());
         out.writeFloat(hit.getTime());
         out.writeFloat(hit.getCharge());
         out.writeInt(hit.getQuality());
         
         if ((flags & (1 << LCIO.TPCBIT_RAW)) != 0)
         {
            out.writeInt( hit.getNRawDataWords() );
            for (int i = 0; i < hit.getNRawDataWords() ; i++)
            {
               out.writeInt( hit.getRawDataWord(i) ) ;
            }
         }
		if ((flags & (1 << LCIO.TPCBIT_NO_PTR)) == 0)
          out.writePTag(hit) ;
      }
   }
   
   private void write(SIOOutputStream out, int flags) throws IOException
   {
      out.writeInt(cellID);
      out.writeFloat(time);
      out.writeFloat(charge);
      out.writeInt(quality);
      
      if ((flags & (1 << LCIO.TPCBIT_RAW)) != 0)
      {
         out.writeInt(rawDataArray.length);
         for (int i = 0; i < rawDataArray.length; i++)
         {
            out.writeInt( rawDataArray[i]);
         }
      }
	 if ((flags & (1 << LCIO.TPCBIT_NO_PTR)) == 0)
	   out.writePTag(this) ;
   }
}
