#ifndef HEADERS_H_
#define HEADERS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>

#include <arpa/inet.h>
#include <netdb.h>

#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <signal.h>
#include <errno.h>
#include <termios.h>

void ctrl_z_handler(int signum);

#include "prompt.h"
#include "warp.h"
#include "peek.h"
#include "proclore.h"
#include "pastevents.h"
#include "seek.h"
#include "handle_input.h"
#include "execute_command.h"
#include "processes.h"
#include "newbg.h"
#include "piping.h"
#include "iMan.h"
#include "neonate.h"

#endif