#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <stdlib.h>
#include <stdio.h>
#include <hashtable.h>
#include <pthread.h> 
#include "bootinfo.h"

/**
 * A "configuration" object holds the runtime options for a user program during the entire run.
 * its values and options are maintained by roverlib - so that if tunings arrive over the air (ota), 
 * they will be applied to the user program which can then access them. Because the configuration object is 
 * updated in a separate thread, the user program can access the keys and values of the configuration object while an update is being applied.
 * hence "safe" methods are defined, which use a mutex to ensure that the user program and roverlib can safely concurrently read/write the configuration object.
 *
 * @note: this implementation depends on the https://github.com/joelguittet/c-hashtable library, which was used by quicktype already, so easy to reuse
 */

typedef struct {
    hashtable_t* float_values;
    hashtable_t* string_values;
    hashtable_t* tunable;
    pthread_rwlock_t *rwlock;
    int64_t last_update;
} Service_configuration;

Service_configuration new_service_configuration(Service service);

void set_float_value(Service_configuration *config, char *key, double value);

void set_string_value(Service_configuration *config, char *key, char *value);

//
// Methods to be used by the user program
// nb: we force the user to be very explicit about the type of the configuration value they want to fetch, to avoid runtime errors
//

// Gets the float value of the configuration option with the given name, returns NULL if the option does not exist or does not exist for this type
// Reading is NOT thread-safe, but we accept the risks because we assume that the user program will read the configuration values repeatedly
// If you want to read the configuration values with concurrency-safety, use the _safe methods
double* get_float_value(Service_configuration *config, char *key);
double* get_float_value_safe(Service_configuration *config, char *key);

// Gets the string value of the configuration option with the given name, returns NULL if the option does not exist or does not exist for this type
// Reading is NOT thread-safe, but we accept the risks because we assume that the user program will read the configuration values repeatedly
// If you want to read the configuration values with concurrency-safety, use the _safe methods
char* get_string_value(Service_configuration *config, char *key);
char* get_string_value_safe(Service_configuration *config, char *key);

#endif // CONFIGURATION_H