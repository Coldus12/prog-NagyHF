//
// Created by coldus on 12/1/19.
//

#ifndef NAGYHF_RECORDS_H
#define NAGYHF_RECORDS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct double_array{double* data; int size} DoubleArray;

void init_double_array(DoubleArray *array, int size);
void resize_double_array(DoubleArray *array, int new_size);
void add_double_to_array(DoubleArray *da, double number);
void free_double_array(DoubleArray *array);
void sort_double_array(double *array, int min, int max);
DoubleArray load_time(char* MapName);
void save_time(double time, char* path_to_map);

#endif //NAGYHF_RECORDS_H
