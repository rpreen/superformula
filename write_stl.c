/*
 * Copyright (C) 2012--2015 Richard Preen <rpreen@gmail.com>
 * See <http://arxiv.org/abs/1204.4107> for details.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 **************
 * Description:
 **************
 * The STL writing module.
 *
 * Provides functionality to encode a 3D binary voxel array in ASCII STL
 * <https://en.wikipedia.org/wiki/STL_%28file_format%29> for use in CAD
 * software such as MeshLab <http://meshlab.sourceforge.net>.
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include "constants.h"
#include "write_stl.h"

// Makerbot 2 Replicator has a default resolution of 0.2mm
// SCALE is the size in mm of a voxel
#define SCALE 0.2 // 0.2 mm * 250 voxels = 50 mm cubed workspace

void dovoxel(int x, int y, int z, _Bool voxels[GRID_SIZE][GRID_SIZE][GRID_SIZE]);
int getvoxel(int x, int y, int z, _Bool voxels[GRID_SIZE][GRID_SIZE][GRID_SIZE]);
void doface(int x, int y, int z, int x0, int y0, int z0, int x1, int y1, int z1,
		int x2, int y2, int z2, int x3, int y3, int z3, int nx, int ny, int nz);
void dotri(int x0, int y0, int z0, int x1, int y1, int z1, int x2, int y2, 
		int z2, int nx, int ny, int nz);
double topos(int i);

FILE *stl;

void write_stl(_Bool g[GRID_SIZE][GRID_SIZE][GRID_SIZE], const char *filename)
{
	stl = fopen(filename, "wt");
	if(stl == 0) {
		printf("Error: could not open stl file for writing: %s. %s.\n", 
				filename, strerror(errno));
		return;
	}
	fprintf(stl, "solid shape\n");
	for(int z = 0; z < GRID_SIZE; z++) {
		for(int x = 0; x < GRID_SIZE; x++) {
			for(int y = 0; y < GRID_SIZE; y++)
				dovoxel(x,y,z, g);
		}
	}
	fprintf(stl, "endsolid shape\n");
	fclose(stl);
}

void dovoxel(int x, int y, int z, _Bool voxels[GRID_SIZE][GRID_SIZE][GRID_SIZE])
{
	if(getvoxel(x,y,z, voxels) != 0) {
		if(getvoxel(x,y,z-1, voxels) == 0)
			doface(x,y,z, 0,1,0, 1,1,0, 1,0,0, 0,0,0, 0,0,-1);
		if(getvoxel(x,y,z+1, voxels) == 0)
			doface(x,y,z, 0,0,1, 1,0,1, 1,1,1, 0,1,1, 0,0,1);
		if(getvoxel(x-1,y,z, voxels) == 0)
			doface(x,y,z, 0,0,1, 0,1,1, 0,1,0, 0,0,0, -1,0,0);
		if(getvoxel(x+1,y,z, voxels) == 0)
			doface(x,y,z, 1,0,0, 1,1,0, 1,1,1, 1,0,1, 1,0,0);
		if(getvoxel(x,y-1,z, voxels) == 0)
			doface(x,y,z, 0,0,0, 1,0,0, 1,0,1, 0,0,1, 0,-1,0);
		if(getvoxel(x,y+1,z, voxels) == 0)
			doface(x,y,z, 0,1,1, 1,1,1, 1,1,0, 0,1,0, 0,1,0);
	}
}

int getvoxel(int x, int y, int z, _Bool voxels[GRID_SIZE][GRID_SIZE][GRID_SIZE])
{
	if((x>=GRID_SIZE) || (x<0)) return 0;
	if((y>=GRID_SIZE) || (y<0)) return 0;
	if((z>=GRID_SIZE) || (z<0)) return 0;
	if(voxels[z][x][y]) return 1;
	else return 0;
}

void doface(int x, int y, int z, int x0, int y0, int z0, int x1, int y1, int z1,
		int x2, int y2, int z2, int x3, int y3, int z3, int nx, int ny, int nz)
{
	dotri(x+x0, y+y0, z+z0, x+x1, y+y1, z+z1, x+x2, y+y2, z+z2, nx,ny,nz);
	dotri(x+x2, y+y2, z+z2, x+x3, y+y3, z+z3, x+x0, y+y0, z+z0, nx,ny,nz);
}

void dotri(int x0, int y0, int z0, int x1, int y1, int z1, int x2, int y2,
		int z2, int nx, int ny, int nz)
{
	fprintf(stl, "facet normal %d %d %d\n  outer loop\n", nx, ny, nz);
	fprintf(stl, "    vertex %.5f %.5f %.5f\n", topos(x0), topos(y0), topos(z0));
	fprintf(stl, "    vertex %.5f %.5f %.5f\n", topos(x1), topos(y1), topos(z1));
	fprintf(stl, "    vertex %.5f %.5f %.5f\n", topos(x2), topos(y2), topos(z2));
	fprintf(stl, "  end loop\n  end facet\n");
}

double topos(int i)
{
	return (double) i * SCALE;
}
