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
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include "constants.h"
#include "ga.h"
#include "random.h"
#include "superformula.h"
#include "write_stl.h"

void display();
FILE *out;

int main(int argc, char *argv[])
{
	// must specify a file to write output
	if(argc != 2) {
		printf("usage: %s output_file_name\n", argv[0]);
		return EXIT_FAILURE;
	}
	else
	{
		// open file for writing output
		out = fopen(argv[1], "wt");
		if(out == 0) {
			printf("Error: could not open output file: %s. %s.\n", 
					argv[1], strerror(errno));
			return EXIT_FAILURE;
		}
	}
	init_random(); // set the random generator seed
	init_ga(); // initialise and evaluate population
	display(); // display initial state
	// evolve population
	while(evals < MAX_EVALS) {
		SF child = create_child();
		eval(&child);
		add_offspring(&child);
		// display output when necessary
		if(evals % DISP_EVALS == 0)
			display();
	}
	// print best shape found
	SF best = pop[fittest_individual()];
	print_sf(&best);
	// write best to stl
//	_Bool grid[GRID_SIZE][GRID_SIZE][GRID_SIZE];
//	draw_super_formula( best.genome, grid );
//	write_stl( grid, "best.stl" );
	fclose(out);
	return EXIT_SUCCESS;
}

void display()
{
	SF best = pop[fittest_individual()];
	printf("(Evals) %d (Best) %.8f (Avg) %.8f\n", 
			evals, best.fitness, avg_fit());
	fprintf(out, "%d,%.8f,%.8f\n", 
			evals, best.fitness, avg_fit());
	fflush(stdout);
	fflush( out );
}
