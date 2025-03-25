#include "../include/roverlib/configuration.h"

Service_configuration new_service_configuration(Service service) {
    Service_configuration config;
    config.float_values = hashtable_create(16, false);
    config.string_values = hashtable_create(16, false);
    config.tunable = hashtable_create(16, false);
    config.rwlock = (pthread_rwlock_t *)malloc(sizeof(pthread_rwlock_t));
    pthread_rwlock_init(config.rwlock, NULL);
    config.last_update = 0;

    Configuration *option = list_get_head(service.configuration);
    while (option != NULL) {
        switch (*option->type) {
            case TYPE_NUMBER:
                hashtable_add(config.float_values, option->name, &option->value->value.number, sizeof(double));
                break;
            case TYPE_STRING:
                hashtable_add(config.string_values, option->name, option->value->value.string, strlen(option->value->value.string));
                break;
        }
        if (option->tunable) {
            hashtable_add(config.tunable, option->name, NULL, 0);
        }

        option = list_get_next(service.configuration);        
    }

    return config;
}

void set_float_value(Service_configuration *config, char *key, double value) {
    pthread_rwlock_wrlock(config->rwlock);

    // Check if this key is tunable
    if (hashtable_has_key(config->tunable, key)) {
        // If it is tunable, update the value
        hashtable_remove(config->float_values, key);
        hashtable_add(config->float_values, key, &value, sizeof(double));
    }

    pthread_rwlock_unlock(config->rwlock);
}

void set_string_value(Service_configuration *config, char *key, char *value) {
    pthread_rwlock_wrlock(config->rwlock);

    // Check if this key is tunable
    if (hashtable_has_key(config->tunable, key)) {
        // If it is tunable, update the value
        hashtable_remove(config->string_values, key);
        hashtable_add(config->string_values, key, value, strlen(value));
    }

    pthread_rwlock_unlock(config->rwlock);
}

//
// Methods to be used by the user program
// nb: we force the user to be very explicit about the type of the configuration value they want to fetch, to avoid runtime errors
//

// Gets the float value of the configuration option with the given name, returns NULL if the option does not exist or does not exist for this type
// Reading is NOT thread-safe, but we accept the risks because we assume that the user program will read the configuration values repeatedly
// If you want to read the configuration values with concurrency-safety, use the _safe methods
double* get_float_value(Service_configuration *config, char *key) {
    return hashtable_lookup(config->float_values, key);
}
double* get_float_value_safe(Service_configuration *config, char *key) {
    pthread_rwlock_rdlock(config->rwlock);
    double *value = hashtable_lookup(config->float_values, key);
    pthread_rwlock_unlock(config->rwlock);
    return value;
}

// Gets the string value of the configuration option with the given name, returns NULL if the option does not exist or does not exist for this type
// Reading is NOT thread-safe, but we accept the risks because we assume that the user program will read the configuration values repeatedly
// If you want to read the configuration values with concurrency-safety, use the _safe methods
char* get_string_value(Service_configuration *config, char *key) {
    return hashtable_lookup(config->string_values, key);
}
char* get_string_value_safe(Service_configuration *config, char *key) {
    pthread_rwlock_rdlock(config->rwlock);
    char *value = hashtable_lookup(config->string_values, key);
    pthread_rwlock_unlock(config->rwlock);
    return value;
}
