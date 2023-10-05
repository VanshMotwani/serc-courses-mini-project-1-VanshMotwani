#ifndef __PASTEVENTS_H
#define __PASTEVENTS_H

void add_in_array(char** pasteventsarr, char* newelem, int* arrlen, int* start, int* cur);

void populate_array_from_file(char** pasteventsarr, int* arrlen, int* start, int* cur);

void populate_pastevents_arr(char* input, char** pasteventsarr, char* prevInput, int* arrlen, int* start, int* cur, char* newinput, int* isPastExecPresent);

void add_to_pastevents_arr(char* input, char** pasteventsarr, char* prevInput, int* arrlen, int* start, int* cur, char* newinput, int* isPastExecPresent);

#endif