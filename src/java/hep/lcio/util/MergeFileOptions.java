package hep.lcio.util;

import hep.lcio.event.LCEvent;
import hep.lcio.implementation.io.LCFactory;
import hep.lcio.io.LCReader;

import java.io.File;
import java.io.IOException;

public final class MergeFileOptions
{		
	int nreads = 1;
	float startt = 0;
	float dt = 0;
	File f;		
	LCReader reader = LCFactory.getInstance().createLCReader();
	
	MergeFileOptions(File f, int nreads, float startt, float dt) throws IOException
	{
		this.f = f;
		this.nreads = nreads;
		this.startt = startt;
		this.dt = dt;
		
		open();
	}
	
	MergeFileOptions(File f) throws IOException
	{
		this.f = f;
		
		open();
	}
	
	public int nreads()
	{
		return nreads;	
	}
	
	public float startt()
	{
		return startt;
	}
	
	public float dt()
	{
		return dt;
	}
	
	public File file()
	{
		return f;
	}
	
	public LCReader reader()
	{
		return reader;
	}
	
	public void open() throws IOException
	{
		reader.open(file().getAbsolutePath());
	}
	
	public void close() throws IOException
	{
		reader.close();
	}
	
	public LCEvent nextEvent() throws IOException
	{
		return reader.readNextEvent();
	}
}