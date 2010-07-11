#!/usr/bin/env groovy

import gnu.io.*;
import gnu.io.CommPort;
import gnu.io.CommPortIdentifier;
import gnu.io.SerialPort;
import gnu.io.SerialPortEvent;
import gnu.io.SerialPortEventListener;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;




static InputStream connect ( String portName ) throws Exception
{
	CommPortIdentifier portIdentifier = CommPortIdentifier.getPortIdentifier(portName);
	if ( portIdentifier.isCurrentlyOwned() )
	{
			System.out.println("Error: Port is currently in use");
	}
	else
	{
		CommPort commPort = portIdentifier.open(this.getClass().getName(),2000);
		
		if ( commPort instanceof SerialPort )
		{
				SerialPort serialPort = (SerialPort) commPort;
				serialPort.setSerialPortParams(115200,SerialPort.DATABITS_8,SerialPort.STOPBITS_1,SerialPort.PARITY_NONE);
				
InputStream instream = serialPort.getInputStream();
				OutputStream out = serialPort.getOutputStream();
											 
				//serialPort.addEventListener(new SerialReader(in));
				//serialPort.notifyOnDataAvailable(true);
				return instream;
		}
		else
		{
				System.out.println("Error: Only serial ports are handled by this example."); 
				return null;
		}
	}     
}


static void listPorts()
{
		java.util.Enumeration<CommPortIdentifier> portEnum = CommPortIdentifier.getPortIdentifiers();
		while ( portEnum.hasMoreElements() ) 
		{
				CommPortIdentifier portIdentifier = portEnum.nextElement();
				System.out.println(portIdentifier.getName()  +  " - " +  getPortTypeName(portIdentifier.getPortType()) );
		}        
}

static String getPortTypeName ( int portType )
{
		switch ( portType )
		{
				case CommPortIdentifier.PORT_I2C:
						return "I2C";
				case CommPortIdentifier.PORT_PARALLEL:
						return "Parallel";
				case CommPortIdentifier.PORT_RAW:
						return "Raw";
				case CommPortIdentifier.PORT_RS485:
						return "RS485";
				case CommPortIdentifier.PORT_SERIAL:
						return "Serial";
				default:
						return "unknown type";
		}
}

/**
 * @return    A HashSet containing the CommPortIdentifier for all serial ports that are not currently being used.
 */
public static HashSet<CommPortIdentifier> getAvailableSerialPorts() {
		HashSet<CommPortIdentifier> h = new HashSet<CommPortIdentifier>();
		Enumeration thePorts = CommPortIdentifier.getPortIdentifiers();
		while (thePorts.hasMoreElements()) {
				CommPortIdentifier com = (CommPortIdentifier) thePorts.nextElement();
				switch (com.getPortType()) {
				case CommPortIdentifier.PORT_SERIAL:
						try {
								CommPort thePort = com.open("CommUtil", 50);
								thePort.close();
								h.add(com);
						} catch (PortInUseException e) {
								System.out.println("Port, "  + com.getName() + ", is in use.");
						} catch (Exception e) {
								System.err.println("Failed to open port " +  com.getName());
								e.printStackTrace();
						}
				}
		}
		return h;
}


def File damagedpackets = new File("./data/damaged.txt")
def File overflowedpackets = new File("./data/overflowed.txt")

//listPorts();
def String portname;
getAvailableSerialPorts().each{port -> println port.getName(); portname =port.getName()}

InputStream stream = connect(portname);
byte[] buffer = new byte[1024];
int data;

while(true)
{
	try
	{
		int len = 0;
		while ( ( data = stream.read()) > -1 )
		{
				if ( data == '\n' ) {
						break;
				}
				buffer[len++] = (byte) data;
		}
		def line = new String(buffer,0,len);
		
		if(line.startsWith("BaseStation"))
		{
			
		}
		else if(line.startsWith("D!"))
		{
			damagedpackets << line+"\n"
		}
		else if(line.startsWith("Of"))
		{
			overflowedpackets << line+"\n"
		}
		else if(line.startsWith("Ok"))
		{
			array = line.split("\t",0)
			new File("./data/day${array[3]}.txt") << line+"\n"
		}
		
		if(line.size()!=0) System.out.println(line);
                Thread.sleep(200) 
	}
	catch ( IOException e )
	{
		e.printStackTrace();
		System.exit(-1);
	}   
}          
