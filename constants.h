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

#define max( a, b ) ( ((a) > (b)) ? (a) : (b) )
#define min( a, b ) ( ((a) < (b)) ? (a) : (b) )

// Output parameters
#define DISP_EVALS 100 // number of evaluations to average performance
// Shape parameters
#define NUM_PARAMS 8 // number of superformula parameters to evolve
#define GRID_SIZE 50 // x,y,z number of voxels in which to draw supershapes
#define TOTAL_VOXELS 125000 // pow(GRID_SIZE, 3)
#define MAX_COND 20 // maximum value of a superformula parameter
// GA parameters
#define MAX_EVALS 10000 // number of evaluations to perform
#define POPSIZE 200  // maximum population size
#define MUT_STEP 5.0  // maximum mutation amount per allele
#define MUT_RATE 0.25 // per allele probability of mutation
#define TSIZE 2 // tournament size for selection and deletion
#define TOP_PROTECT 1 // fittest n individuals to make immune from deletion
