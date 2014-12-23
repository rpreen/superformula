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
 * The supershape genetic algorithm module.
 *
 * Provides functions to create a population of random real-valued superformula
 * parameters; the creation of offspring through selection and mutation; and the
 * target-based evaluation of an individual.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include "constants.h"
#include "random.h"
#include "write_stl.h"
#include "superformula.h"
#include "ga.h"

void mutate(SF *s);
int negative_tournament();
int tournament();
int compare_fitness(const void *a, const void *b);

_Bool target_grid[GRID_SIZE][GRID_SIZE][GRID_SIZE];
double target_genome[] = { 6.0, 5.0, 10.0, 10.0, 4.0, 10.0, 10.0, 10.0 }; // star
//double target_genome[] = { 3.0, 1.5, 12.0, 3.0, 0.0, 3.0, 0.0, 0.0 }; // heart
//double target_genome[] = { 4.0, 10.0, 10.0, 10.0, 4.0, 10.0, 10.0, 10.0 }; // cube

void init_ga()
{
	// create target
	draw_super_formula(target_genome, target_grid);
	//write_stl(target_grid, "target.stl");
	// create the initial population
	for(int i = 0; i < POPSIZE; i++) {
		pop[i].fitness = -1.0;
		for(int j = 0; j < NUM_PARAMS; j++)
			pop[i].genome[j] = drand() * MAX_COND;
	}
	// evaluate initial population
	evals = 0;
	eval_all();
}

void eval_all()
{
	for(int i = 0; i < POPSIZE; i++)
		eval(&pop[i]);
}

_Bool eval(SF *s)
{
	// don't proceed if already evaluated
	if(s->fitness >= 0.0)
		return false;
	// draw individual
	_Bool grid[GRID_SIZE][GRID_SIZE][GRID_SIZE];
	draw_super_formula(s->genome, grid);
	// filter completely empty or full grids
	int voxels = 0;
	for(int z = 0; z < GRID_SIZE; z++)
		for(int y = 0; y < GRID_SIZE; y++)
			for(int x = 0; x < GRID_SIZE; x++)
				if(grid[z][y][x])
					voxels++;
	if(voxels < TOTAL_VOXELS * 0.005 || voxels > TOTAL_VOXELS * 0.995) {
		s->fitness = 0.0;
		return false;
	}
	// compare with target
	double error = 0.0;
	for(int z = 0; z < GRID_SIZE; z++)
		for(int y = 0; y < GRID_SIZE; y++)
			for(int x = 0; x < GRID_SIZE; x++)
				if(grid[z][y][x] != target_grid[z][y][x])
					error += 1.0;
	// assign fitness in range [0,1]
	s->fitness = (-(error / TOTAL_VOXELS)) + 1.0;
	evals++;
	return true;
}

void add_offspring(SF *child)
{
	int replace = negative_tournament();
	pop[replace] = *child;
}

SF create_child()
{
	SF child;
	int parent = tournament();
	memcpy(&child, &pop[parent], sizeof(SF));
	child.fitness = -1.0;
	mutate(&child);
	return child;
}

int tournament()
{
	int best = irand(0, POPSIZE);
	double fbest = pop[best].fitness;
	for(int i = 0; i < TSIZE; i++) {
		int competitor = irand(0, POPSIZE);
		double fcompetitor = pop[competitor].fitness;
		if(fcompetitor > fbest) {
			best = competitor;
			fbest = fcompetitor;
		}
	}
	return best;
}

int negative_tournament()
{
	// sort the population so we can protect the top individuals
	qsort(&pop, POPSIZE, sizeof(SF), compare_fitness);
	int worst = irand(TOP_PROTECT, POPSIZE);
	double fworst = pop[worst].fitness;
	for(int i = 0; i < TSIZE; i++) {
		int competitor = irand(TOP_PROTECT, POPSIZE);
		double fcompetitor = pop[competitor].fitness;
		if(fcompetitor < fworst) {
			worst = competitor;
			fworst = fcompetitor;
		}
	}
	return worst;
}

int fittest_individual()
{
	int best = 0;
	double fbest = pop[0].fitness;
	for(int i = 1; i < POPSIZE; i++) {
		if(pop[i].fitness > fbest) {
			best = i;
			fbest = pop[i].fitness;
		}
	}
	return best;
}

double avg_fit()
{
	double avg = 0.0;
	for(int i = 0; i < POPSIZE; i++)
		avg += pop[i].fitness;
	avg /= POPSIZE;
	return avg;
}

void print_sf(SF *s)
{
	printf("fitness = %.5f\n", s->fitness);
	printf("genome = [ %.5f", s->genome[0]);
	for(int i = 1; i < NUM_PARAMS; i++)
		printf(", %.5f", s->genome[i]);
	printf(" ]\n");
}

void mutate(SF *s)
{
	for(int i = 0; i < NUM_PARAMS; i++) {
		if(drand() <= MUT_RATE) {
			s->genome[i] += (drand()*2.0*MUT_STEP)-MUT_STEP;
			// bounds enforcement
			if(s->genome[i] > MAX_COND)
				s->genome[i] = MAX_COND;
			else if(s->genome[i] < 0.0)
				s->genome[i] = 0.0;
		}
	}
}

int compare_fitness(const void *a, const void *b)
{
	SF *ia = (SF *)a;
	SF *ib = (SF *)b;
	if(ia->fitness > ib->fitness)
		return -1;
	else
		return 1;
}
