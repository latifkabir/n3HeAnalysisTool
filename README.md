n3He Analysis Tools
=====================
n3He Analysis tool integrated in ROOT. Its an extension to ROOT with additional analysis functionality added required for the n3He experiment at Oak Ridge National Lab.

What's inside each directory:
-----------------------------
* libn3He : The main n3He Analysis Tool integrated in ROOT. It has everything for the analysis.
* Tutorials : This is the tutorial section & contains example analysis scripts,its the building blocks using which "libn3He" is made of. These individual script is to facilitate understand different functionality & technique used in the analysis tool. 
* n3HeData : The n3He data browser.
* Results : Analysis results for n3He data obtained using the "n3He Analysis Tool".

Instruction:
--------------
* Compile the make file inside libn3He directory typing "make" from that directory.
* Before you try it you MUST do the following:
      1. Put the following command into your ~/.bashrc file:

           ```
	      if [ -f /path/to/libn3He/bin/thisn3He.sh ]; then

	      . /path/to/libn3He/bin/thisn3He.sh

	      fi
           ```

	  2. Now copy the rootlogon.C file in the directory 'macros' under ROOT installation directory.  
	  
* Now from a new terminal start root doing "root -l".
* For a list of available options Type "Help()"

 
![](./demo.jpg "n3He Analysis Tool in Action")


