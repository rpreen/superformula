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
#define M_PI 3.14159265358979323846264338327

// Output parameters
#define DISP_EVALS 100
// Shape parameters
#define NUM_PARAMS 8
#define GRID_SIZE 50
#define TOTAL_VOXELS 125000 // pow(GRID_SIZE, 3)
#define MAX_COND 20
// GA parameters
#define MAX_EVALS 10000
#define POPSIZE 200
#define MUT_STEP 5.0
#define MUT_RATE 0.25
#define TSIZE 2
#define TOP_PROTECT 1
