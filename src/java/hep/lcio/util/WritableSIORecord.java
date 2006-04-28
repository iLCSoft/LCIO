package hep.lcio.util;

import hep.io.xdr.*;
import java.io.*;

class WritableSIORecord
{
	private int headerLength;
	private int frame;
	private int control;
	private int compressedLength;
	private int uncompressedLength;
	private int nameLength;
	private String name;
	private byte[] data;

	String getName()
	{
		return name;
	}

	void read(XDRInputStream xdr) throws IOException
	{
		// Now read the record header for the next record
		long startPos = xdr.getBytesRead();
		headerLength = xdr.readInt();
		frame = xdr.readInt();
		if (frame != 0xabadcafe)
			throw new IOException("Framing error");
		control = xdr.readInt();
		if ((control & 0xfffe) != 0)
			throw new IOException("Bad control word");

		boolean compressed = (control & 1) != 0;
		compressedLength = xdr.readInt();
		uncompressedLength = xdr.readInt();

		// Sanity check
		if (!compressed && compressedLength != uncompressedLength)
			throw new IOException("Data is insane");

		nameLength = xdr.readInt();
		if (nameLength > headerLength - xdr.getBytesRead() + startPos)
			throw new IOException("Record name is insane");
		name = xdr.readString(nameLength);

		// read the (compressed) record

		if (data == null || compressedLength > data.length)
			data = new byte[compressedLength];
		int rc = xdr.read(data, 0, compressedLength);
		
		xdr.pad();
		if (rc != compressedLength)
			throw new IOException("Read error");
	}

	void write(XDROutputStream out) throws IOException
	{
		out.writeInt(headerLength);
		out.writeInt(frame);
		out.writeInt(control);
		out.writeInt(compressedLength);
		out.writeInt(uncompressedLength);
		out.writeInt(nameLength);
		out.writeStringChars(name);
		out.write(data, 0, compressedLength);
		out.pad();
	}
}