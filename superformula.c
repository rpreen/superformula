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
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "constants.h"
#include "superformula.h"
#include "write_stl.h"

#define RES 0.002 // resolution to draw the supershape

void superduperformula(double u, double v, double superf[NUM_PARAMS], double cords[3]);
double superformula(double phi, double a, double b, double m, double n1, double n2, double n3);
void fillgrid(_Bool g[GRID_SIZE][GRID_SIZE][GRID_SIZE]);
void filly(int center, int z, _Bool g[GRID_SIZE][GRID_SIZE][GRID_SIZE]);
void fillx(int center, int z, int y, _Bool g[GRID_SIZE][GRID_SIZE][GRID_SIZE]);

//int main()
//{
//	//double supertest[NUM_PARAMS] = { 3.0, 1.5, 12.0, 3.0, 0.0, 3.0, 0.0, 0.0 }; // heart
//	double supertest[NUM_PARAMS] = { 6.0, 5.0, 10.0, 10.0, 4.0, 10.0, 10.0, 10.0 }; // star
//	//double supertest[NUM_PARAMS] = { 4.0, 10.0, 10.0, 10.0, 4.0, 10.0, 10.0, 10.0 }; // cube
//
//	_Bool test_grid[GRID_SIZE][GRID_SIZE][GRID_SIZE];
//	draw_super_formula(supertest, test_grid);
//	write_stl(test_grid, "test.stl");
//}

void draw_super_formula(double genome[NUM_PARAMS], _Bool grid[GRID_SIZE][GRID_SIZE][GRID_SIZE])
{
	// initialise empty grid
	for(int z = 0; z < GRID_SIZE; z++)
		for(int y = 0; y < GRID_SIZE; y++)
			for(int x = 0; x < GRID_SIZE; x++)
				grid[z][y][x] = false;
	// draw superformula
	double cords[3];
	for(double u=0.0; u<1.0; u+=RES) {
		for(double v=0.0; v<1.0; v+=RES) {
			superduperformula(u,v,genome,cords);
			int z = (int)(cords[2]+GRID_SIZE)/2;
			int y = (int)(cords[1]+GRID_SIZE)/2;
			int x = (int)(cords[0]+GRID_SIZE)/2;
			if(x<GRID_SIZE && x>=0 && y<GRID_SIZE && y>=0 && z<GRID_SIZE && z>=0)
				grid[z][y][x] = true;
		}
	}
	// attempt to make the shape solid
	fillgrid(grid);
}

void superduperformula(double u, double v, double genome[NUM_PARAMS], double cords[3])
{
	//--- general rules:
	// super shape: c1=1.0, c2=1.0, c3=0.0, t1=0.0, t2=0.0, d1=0.0, d2=0.0;
	// super shell: c2=1.0, c3=0.0, t1=0.0;
	// super donut: c1=1.0, c2=2.0, t2=0.0, d1=0.0, d2=0.0;
	//--- suggested maximum ranges:
	// [0,100] = m1, n11, n12, n13, m2, n21, n22, n23, t1, t2, d1, d2;
	// [0,2] = c2;
	// [0,10] = c1, c3;
	// super shape fixed params
	double c1=1.0, c2=1.0, c3=0.0, t1=0.0, t2=0.0, d1=0.0, d2=0.0;
	double r0=25.0;
	// unique to this individual
	double m1=genome[0], n11=genome[1], n12=genome[2], n13=genome[3],
		   m2=genome[4], n21=genome[5], n22=genome[6], n23=genome[7];
	double t2c = r0 * pow(c2, d2) * t2 * c1 / 2.0;
	t2 =  t2 * c1 * u;
	d1 = pow(u * c1, d1);
	d2 = pow(u * c2, d2);
	u = (((M_PI*2.0)*u)-M_PI)*c1;
	v = ((M_PI*v)-(M_PI/2.0))*c2;
	double v2 = v + c3 * u;
	double r1 = superformula(u, 1.0, 1.0, m1, n11, n12, n13);
	double r2 = superformula(v, 1.0, 1.0, m2, n21, n22, n23);
	cords[0] = r0 * r1 * (t1 + d1 * r2 * cos(v2)) * sin(u); // x
	cords[1] = r0 * r1 * (t1 + d1 * r2 * cos(v2)) * cos(u); // y
	cords[2] = r0 * d2 * (r2 * sin(v2) - t2) + t2c; // z
}

double superformula(double phi, double a, double b, double m, double n1, double n2, double n3)
{
	return pow(pow(fabs(cos(m*phi/4.0)/a),n2)+pow(fabs(sin(m*phi/4.0)/b),n3),-1.0/n1);
}

void fillgrid(_Bool g[GRID_SIZE][GRID_SIZE][GRID_SIZE])
{
	int center = GRID_SIZE / 2;
	for(int z = center; z > 0; z--) {
		if(z == 0 || g[z][center][center] == false)
			filly(center, z, g);
		else
			break;
	}
	for(int z = center+1; z < GRID_SIZE; z++) {
		if(z == GRID_SIZE-1 || g[z][center][center] == false)
			filly(center, z, g);
		else
			break;
	}
}

void filly(int center, int z, _Bool g[GRID_SIZE][GRID_SIZE][GRID_SIZE])
{
	for(int y = center; y > 0; y--) {
		if(y == 0 || g[z][y][center] == false)
			fillx(center, z, y, g);
		else
			break;
	}
	for(int y = center+1; y < GRID_SIZE; y++) {
		if(y == GRID_SIZE-1 || g[z][y][center] == false)
			fillx(center, z, y, g);
		else
			break;
	}
}

void fillx(int center, int z, int y, _Bool g[GRID_SIZE][GRID_SIZE][GRID_SIZE])
{
	for(int x = center; x > 0; x--) {
		if(x == 0 || g[z][y][x] == false)
			g[z][y][x] = true;
		else
			break;
	}
	for(int x = center+1; x < GRID_SIZE; x++) {
		if(x == GRID_SIZE-1 || g[z][y][x] == false)
			g[z][y][x] = true;
		else
			break;
	}
}
