
==========================================n3He User End Analysis Scripts========================================
1. This is the analysis directory built on top of core n3He library (i.e. libn3He.so).
2. The analysis scripts are compiled for better performance and control to be used as shared library for ROOT.
3. So the makefile links the analysis scripts to libn3He.so and during copilation utilizes the headers fron libn3He/src directory.
4. Each time you add a new script (function) just add that function to LinkDef.h
5. All analysis must have same .cc extention for the make file to get those names automatically. Other scripts that you do not want
to compile , give those extension .cpp.