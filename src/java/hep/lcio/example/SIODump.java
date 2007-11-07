package hep.lcio.example;

import hep.io.sio.*;

import java.io.EOFException;
import java.io.FileInputStream;
import java.io.IOException;
import java.text.DecimalFormat;


/** Small programm that dumps the structure and contents of an (LCIO) SIO file to the screen.
 *
 * @author Frank Gaede
 * @version $Id: SIODump.java,v 1.5 2007-11-07 20:46:22 jeremy Exp $
 */
public class SIODump
{
   /**
    * @param args the command line arguments
    */
   public static void main(String[] args)
   {
      SIODump dmp = new SIODump();

      if (args.length == 0)
      {
         System.out.println(" usage: java -cp tools/sio.jar:lib/lcio.jar " + "hep.lcio.example.SIODump filename [option]!");
         System.out.println("\t options:  h - hex [default] ");
         System.out.println("\t           d - integer ");
         System.out.println("\t           f - float ");
         System.out.println("\t           n - no contents ");
         System.exit(1);
      }

      // create sio reader
      SIOReader reader = null;
      try
      {
         reader = new SIOReader(new FileInputStream(args[0]));
      }
      catch (IOException x)
      {
         System.out.println(" couldn't open file : " + args[0]);
         System.exit(1);
      }

      System.out.println(" opened file : " + args[0]);

      // loop over records
      SIORecord record;
      try
      {
         for (;;)
         { // ever ...
            record = reader.readRecord();

            System.out.println(" record : " + record.getRecordName() + "length: " + record.getRecordLength());

            //loop over blocks
            SIOBlock block;
            DecimalFormat df = new DecimalFormat("00");
            while ((block = record.getBlock()) != null)
            {
               System.out.println("\t block: " + block.getBlockName() + " \t[" + block.getBlockLength() + "]" + " v" + df.format(block.getMajorVersion()) + "-" + df.format(block.getMinorVersion()));

               // dump data
               SIOInputStream in = block.getData();

               if (args.length < 2)
                  dmp.dumpHexData(in);
               else if (args[1].equals("c"))
                  dmp.dumpCharData(in);
               else if (args[1].equals("d"))
                  dmp.dumpIntData(in);
               else if (args[1].equals("n")) /* no contents */
                  ;
               else
                  dmp.dumpHexData(in);
            }
         }
      }
      catch (EOFException e) {}
      catch (IOException e)
      {
         e.printStackTrace();
      }
   }

   void dumpCharData(SIOInputStream in)
   {
      int wordsPerLine = 4;

      try
      {
         for (int count = 0;; count++)
         {
            byte[] ascii = new byte[4];
            in.readFully(ascii);

            if ((count % wordsPerLine) == 0)
               System.out.print("\t ");

            DecimalFormat fm = new DecimalFormat("000");
            for (int i = 0; i < 4; i++)
            {
               // '0' = 48 ; 'z' = 122
               if ((47 < ascii[i]) && (ascii[i] < 123))
               {
                  byte[] sByte = { ascii[i] };
                  String s = new String(sByte);
                  System.out.print("\\ " + s + " ");
               }
               else
               {
                  int unsigned = (0xff & ascii[i]);
                  System.out.print("\\" + fm.format(unsigned));
               }
            }
            if ((count % wordsPerLine) == (wordsPerLine - 1))
               System.out.println();
         }
      }
      catch (IOException e) {}
      System.out.println();
   }

   void dumpHexData(SIOInputStream in)
   {
      int digitsPerLine = 8;

      try
      {
         for (int count = 0;; count++)
         {
            int i = in.readInt();

            if ((count % digitsPerLine) == 0)
               System.out.print("\t ");

            StringBuffer sb = new StringBuffer();
            sb.append(Integer.toHexString(i));
            while (sb.length() < 8)
               sb.insert(0, '0');

            System.out.print(sb + " ");

            if ((count % digitsPerLine) == (digitsPerLine - 1))
               System.out.println();
         }
      }
      catch (IOException e) {}
      System.out.println();
   }

   void dumpIntData(SIOInputStream in)
   {
      int wordsPerLine = 8;

      try
      {
         for (int count = 0;; count++)
         {
            int i = in.readInt();

            if ((count % wordsPerLine) == 0)
               System.out.print("\t ");

            if (i > -1)
            {
               DecimalFormat fm = new DecimalFormat("0000000000000");
               System.out.print(fm.format(i) + " ");
            }
            else
            {
               DecimalFormat fm = new DecimalFormat("-000000000000");
               System.out.print(fm.format(-i) + " ");
            }
            if ((count % wordsPerLine) == (wordsPerLine - 1))
               System.out.println();
         }
      }
      catch (IOException e) {}
      System.out.println();
   }
}
