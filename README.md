A genetic algorithm to perform target-based evolution of 3D supershapes
produced by Gielis superformula encoded in a voxelised format and exported to
STL for 3D printing if desired. 

See <http://arxiv.org/abs/1204.4107> for details.

============ 
Compilation: 
============

"make"			Builds all source files into an executable named "sfnn".  
"make OPT=0"	Builds without compiler optimisations. 
"make clean"	Removes all compiled object files.

====== 
Usage: 
======

The source file with the main function is sfnn.c.

The following command will initiate the target-based evolution of supershapes.
Most parameters are in constants.h.  Currently set to run for 10,000
evaluations using 8 supershape parameters against a star shape target. The only
required argument is the file name for the system performance output. E.g.:

./sfnn output.dat	  

===========
Additional:
===========

A supershape visualisation tool and its source code produced by Martin
Schneider, licensed under Creative Commons Attribution Share Alike 3.0 and GNU
GPL license, can be found at <http://openprocessing.org/visuals/?visualID=2638>
