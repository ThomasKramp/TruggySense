SPICE-Library for LTspice (Linear Technologies)
for TDK Electronics: TVS.lib
Version 1.1.1
assembled on Feb 13 2023

******************* preliminary **********************

This SPICE library contains a model for the TVS
diode types SD0201SL_ULC101, SD01005SL_ULC101, SD0201SL_ULC102,
SD01005SL_ULC102, SD0201SL_ULC103, SD0201SL_GP101, 
SD0201SL_GP103, SD0201SL_GP104 and SD01005SL_GP101.
The type can be changed with the attribute "SpiceModel" by a
drop-down menu, showing all availabe ones. Beside that
no additional parameters are needed to run the model.

The models describe the behaviour of typical 
elements. Please keep in mind, that real components
can vary in the range given in the datasheets.

Library listing of all parts:
 - File TVS.sub
	SD0201SL_ULC101 
	SD01005SL_ULC101
	SD0201SL_ULC102
	SD01005SL_ULC102
	SD0201SL_ULC103
	SD0201SL_GP101
	SD01005SL_GP101
	SD0201SL_GP103
	SD0201SL_GP104
	

Installation of TVS Model Library
===============================================================

The TVS diode model library for the circuit design program 
LTspice comprises the following 3 files:

TVS.sub	ASCII data file:
	        	This files respresents the actual library 
				for the TVS diode model and contains the data 
				required for simulation.
		
TVS.asy	Symbol data file:
	        	This file contains the graphics information
		        required by the graphic user interface for 
				the TVS diode model.
		
./examples/example1.asc
                LTspice schematic file:
				This file contains a testbench to experience
				the setup and behaviour of the TVS simulation models
				with a 8/20 pulse. 
				Attention: Set Trtol to 5, to avoid artifacts.
				
./examples/example2.asc
                LTspice schematic file:
				This file contains a testbench to experience
				the setup and behaviour of the TVS simulation models
				with a 1ns rise time TLP pulse. 
				Attention: If the TLP file is not found automatically, 
					please select manually by right click on 
					voltage source V2 and select one of the pulse 
					files out of folder "./pulse/". 
				
./examples/pulse/xV_1ns.txt
				Text document defining the TLP pulses for x Volts, availabe
				for voltages between 10 and 1000V.


LTspice IV:

1)  Change to the library containing folder of your LTspice
    installation, typically located at (can deviate depending 
	on installation):
    C:/Program Files/LTC/LTspiceIV/lib
    
2)  Place the library file (TVS.sub) in the folder sub.

3)  Place the symbol file (TVS.asy) in the folder sym/TDK 
	(create if missing)
 
4)  On the next start of LTspice you will find the elements
    using:
    Edit  -> Component (F2) -> TDK -> 	TVS
  
5)  To select one element out of this library make a right click
    on the Symbol. Doubleclick on the "Value" field on the right
    side of the "Spice Model" line to open the drop down menu.


