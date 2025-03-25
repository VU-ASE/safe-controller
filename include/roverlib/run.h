#ifndef RUN_H
#define RUN_H

#include "../rovercom/tuning/tuning.pb-c.h"
#include <stdio.h>
#include <stdio.h>
#include "bootinfo.h"
#include "callbacks.h"
#include <stdlib.h>
#include "configuration.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <zmq.h>
#include <stdbool.h>


#define MAX_MESSAGE_SIZE 4096

typedef struct {
    Service service;
    Service_configuration *config;
} Configuration_thread_args; 

// Thread that runs to update the service configuration that is shared with the user program
void *configuration_thread(void *arg);

// The run function takes in one argument:
// - The Main callback function, which is the user main function to run
int run(Main_callback main);

#endif // RUN_H