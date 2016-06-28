/*
 * Copyright (C) 2016 Richard Preen <rpreen@gmail.com>
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
 * The main supershape evolution module.
 *
 * Initialises the random number generator, creates and evaluates an initial
 * population of random parameters representing supershapes, then uses a
 * steady-state genetic algorithm to evolve the population whereby individuals
 * are scored based on how closely they match a given target shape.
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

int main()
{
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

	return EXIT_SUCCESS;
}

void display()
{
	SF best = pop[fittest_individual()];
	printf("%d %.8f %.8f\n", evals, best.fitness, avg_fit());
	fflush(stdout);
}
