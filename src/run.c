#include "../include/roverlib/run.h"

// Thread that runs to update the service configuration that is shared with the user program
void *configuration_thread(void *arg) {
    if (arg == NULL) {
        printf("No arguments passed to configuration thread\n");
        return NULL;
    }
    Configuration_thread_args *args = (Configuration_thread_args *)arg; 
    printf("Configuration thread started from me\n");

    // Initialize ZeroMQ context and REQ socket to fetch tuning updates on
    void *context = zmq_ctx_new();
    if (context == NULL) {
        printf("Failed to create ZMQ context: %d\n", zmq_errno());
        return NULL;
    }
    void *socket = zmq_socket(context, ZMQ_REQ);
    if (socket == NULL) {
        printf("Failed to create ZMQ socket %d\n", zmq_errno());
        zmq_ctx_destroy(context);
        return NULL;
    }

    const char *address = args->service.tuning->address;
    int res = zmq_connect(socket, address);
    if (res != 0) {
        printf("Failed to connect to ZMQ socket at %s: %d/%d\n", address, res, zmq_errno());
        zmq_close(socket); 
        zmq_ctx_destroy(context);
        return NULL;
    }

    // Subscribe to all messages
    res = zmq_setsockopt(socket, ZMQ_SUBSCRIBE, "", 0);
    if (res != 0) {
        printf("Failed to subscribe to all messages on ZMQ socket: %d/%d\n", res, zmq_errno());
        zmq_close(socket); 
        zmq_ctx_destroy(context);
        return NULL;
    }

    printf("Connected to ZMQ socket at %s\n", address);

    //
    // Main loop, watching for updates to the configuration and applying them
    //

    // Buffer to receive messages
    char buffer[MAX_MESSAGE_SIZE];
    int len = 0;
    while (true) {
        int len = zmq_recv(socket, buffer, sizeof(buffer) - 1, 0);
        if (len < 0) {
            perror("Failed to receive message");
            continue;
        }
        buffer[len] = '\0';

        // Convert from over-the-wire format to C struct using protobuf
        ProtobufMsgs__TuningState *tuning_state = protobuf_msgs__tuning_state__unpack(NULL, len, buffer);
        if (tuning_state == NULL) {
            printf("Failed to unpack tuning state message\n");
            continue;
        }

        // Ignore if has an older timestamp
        if (tuning_state->timestamp <= args->config->last_update) {
            printf("Ignoring tuning state message with timestamp %lu\n", tuning_state->timestamp);
            protobuf_msgs__tuning_state__free_unpacked(tuning_state, NULL);
            continue;
        }

        // Update the configuration (setX will ignore values that are not tunable)
        for (int i = 0; i < tuning_state->n_dynamicparameters; i++) {
            ProtobufMsgs__TuningState__Parameter *param = tuning_state->dynamicparameters[i];

            if (param->parameter_case == PROTOBUF_MSGS__TUNING_STATE__PARAMETER__PARAMETER_NUMBER) {
                set_float_value(args->config, param->number->key, param->number->value);
            } else if (param->parameter_case == PROTOBUF_MSGS__TUNING_STATE__PARAMETER__PARAMETER_STRING) {
                set_string_value(args->config, param->string->key, param->string->value);
            }
        }
        protobuf_msgs__tuning_state__free_unpacked(tuning_state, NULL);
    }

    // Cleanup
    zmq_close(socket);
    zmq_ctx_destroy(context);

    return NULL;
}

// The run function takes in one argument:
// - The Main callback function, which is the user main function to run
int run(Main_callback main) {
  if (main == NULL) {
    printf("No main function to call was specified\n");
    return 1;
  }

  // 
  // Parse service configuration from ASE_SERVICE variable as injected by roverd
  // 

  char *service_env = getenv("ASE_SERVICE");
  if (service_env == NULL) {
    printf("No service definition found in environment variable ASE_SERVICE. Are you sure that this service is started by roverd?\n");
    return 1;
  }

  struct Service *service = cJSON_ParseService(service_env);
  if (service == NULL) {
    printf("Failed to parse service definition from ASE_SERVICE environment variable\n");
    return 1;
  }

  Service_configuration config = new_service_configuration(*service);

  //
  // Set up a thread to listen for configuration updates
  // (the user program can fetch the latest value from the configuration object)
  //
  if (service->tuning != NULL && *service->tuning->enabled == true) {
    printf("Tuning enabled for this service, starting configuration thread\n");
    pthread_t thread;
    Configuration_thread_args args;
    args.service = *service;
    args.config = &config;

    int result = pthread_create(&thread, NULL, configuration_thread, &args);
    if (result != 0) {
        fprintf(stderr, "Error: pthread_create failed with code %d\n", result);
        return 1;
    }
    printf("Configuration thread started\n");

  // pthread_join(thread, NULL);
  } else {
    printf("Tuning was not enabled for this service, so no configuration thread was started\n");
  }

  // Run the user program
  return main(*service, &config);
}
