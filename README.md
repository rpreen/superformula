A genetic algorithm to perform target-based evolution of 3D supershapes
produced by Gielis superformula encoded in a voxelised format and exported to
STL for 3D printing if desired. 

See <http://arxiv.org/abs/1204.4107> for details.

============ 
Compilation: 
============

To build the executable named "sfnn" simply run "make".

====== 
Usage: 
======

To initiate the target-based evolution of supershapes simply run the executable
and specify a file to write output (e.g., "./sfnn output.dat"). Most parameters
are in constants.h. Default set to perform 10000 evaluations using 8 supershape
parameters targetting a star shape.

-----------
Additional:
-----------

A supershape visualisation tool and its source code produced by Martin
Schneider, licensed under Creative Commons Attribution Share Alike 3.0 and GNU
GPL license, can be found at <http://openprocessing.org/visuals/?visualID=2638>
