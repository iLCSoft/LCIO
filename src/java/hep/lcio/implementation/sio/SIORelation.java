package hep.lcio.implementation.sio;

import hep.lcd.io.sio.SIOInputStream;
import hep.lcd.io.sio.SIOOutputStream;
import hep.lcd.io.sio.SIORef;
import hep.lcio.event.LCRelation;
import hep.lcio.implementation.event.ILCRelation;
import java.io.IOException;

/**
 *
 * @author Tony Johnson
 * @version $Id: SIORelation.java,v 1.1 2004-07-07 05:32:09 tonyj Exp $
 */
class SIORelation extends ILCRelation
{
   SIORelation(SIOInputStream in, SIOEvent owner, String from, String to, int flag, int major, int minor) throws IOException
   {
      super(from,to);
      int n = in.readInt();
      for (int i=0; i<n; i++)
      {
         SIORef f = in.readPntr();
         SIORef t = in .readPntr();
         float weight = 1;
         if ((flag & 1<<31) != 0) weight = in.readFloat();
      }
   }
   static void write(LCRelation relation, SIOOutputStream out, int flag) throws IOException
   { 
      int n = relation.numberOfRelations();
      out.writeInt(n);
      for (int i=0; i<n; i++)
      {
         out.writePntr(relation.getFrom(i));
         out.writePntr(relation.getTo(i));
         if ((flag & 1<<31) != 0) out.writeFloat(relation.getWeight(i));
      }
   }
}
