#ifndef __SEEK_H
#define __SEEK_H

void seek(char** correct_Paths, int* arrlen, char* target_path, char* to_find, int onlyFile, int onlyDir, int eflag, int* presenceOfFile, int* presenceOfDir, char** color_arr);

void seek_file(char** command_line_args, int cnt, int* presenceOfFile, int* presenceOfDir);

#endif