#ifndef __NEONATE_H
#define __NEONATE_H

void neonate(char* time_arg);
int readMostRecentPID();
void enableRawMode();
void disableRawMode();
void sigint_handler(int signum);

#endif