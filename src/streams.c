#include "../include/roverlib/streams.h"

// Create a stream based on output name (as is defined in the service.yaml)
write_stream* get_write_stream(Service* service, char* name) {
    // Check if this service has an output with the given name
    struct Output *output = list_get_head(service->outputs);
    while (output != NULL) {
        if (strcmp(output->name, name) == 0) {
            // Malloc a new stream
            write_stream *stream = (write_stream *)malloc(sizeof(write_stream));
            if (stream == NULL) {
                return NULL;
            }
            stream->address = output->address;
            stream->socket = NULL;
            stream->bytes = 0;
            return stream;
        }
        output = list_get_next(service->outputs);
    }
    return NULL;
}

// Create a stream based on input of another service and its stream (as is defined in the service.yaml)
read_stream* get_read_stream(Service* service, char* dependency_name, char* stream_name) {
    // Check if this service has an input with the given name
    struct Input *input = list_get_head(service->inputs);
    while (input != NULL) {
        if (strcmp(input->service, dependency_name) == 0) {
            // Check if this input has a stream with the given name
            struct Stream *orig_stream = list_get_head(input->streams);
            while (orig_stream != NULL) {
                if (strcmp(orig_stream->name, stream_name) == 0) {
                    // Malloc a new stream
                    read_stream *stream = (read_stream *)malloc(sizeof(read_stream));
                    if (stream == NULL) {
                        return NULL;
                    }
                    stream->address = orig_stream->address;
                    stream->socket = NULL;
                    stream->bytes = 0;
                    return stream;
                }
                orig_stream = list_get_next(input->streams);
            }
        }
        input = list_get_next(service->inputs);
    }
    return NULL;
}

// Initial setup of a stream (done lazily, on first read)
// returns nonzero on error
int init_read_stream(read_stream *stream) {
    if (stream->socket != NULL) {
        return 0;
    }

    void *context = zmq_ctx_new();
    if (context == NULL) {
        return 1;
    }
    void *socket = zmq_socket(context, ZMQ_SUB);
    if (socket == NULL) {
        zmq_ctx_destroy(context);
        return 1;
    }

    int res = zmq_connect(socket, stream->address);
    if (res != 0) {
        zmq_close(socket);
        zmq_ctx_destroy(context);
        return 1;
    }

    res = zmq_setsockopt(socket, ZMQ_SUBSCRIBE, "", 0);
    if (res != 0) {
        zmq_close(socket);
        zmq_ctx_destroy(context);
        return 1;
    }

    stream->socket = socket;
    stream->bytes = 0;
    return 0;
}

// Initial setup of a stream (done lazily, on first write)
// returns nonzero on error
int init_write_stream(write_stream *stream) {
    if (stream->socket != NULL) {
        return 0;
    }

    void *context = zmq_ctx_new();
    if (context == NULL) {
        printf("Failed to create ZMQ context\n");
        return 1;
    }
    void *socket = zmq_socket(context, ZMQ_PUB);
    if (socket == NULL) {
        printf("Failed to create ZMQ socket\n");
        zmq_ctx_destroy(context);
        return 1;
    }
    int res = zmq_bind(socket, stream->address);
    if (res != 0) {
        printf("Failed to bind ZMQ socket\n");
        zmq_close(socket);
        zmq_ctx_destroy(context);
        return 1;
    }

    stream->socket = socket;
    stream->bytes = 0;
    return 0;
}

// Read bytes from a stream
// returns the number of bytes read, or -1 on error
int read_bytes(read_stream *stream, void *buffer, int size) {
    if (stream->socket == NULL) {
        int res = init_read_stream(stream);
        if (res != 0) {
            return -1;
        }
    }

    int len = zmq_recv(stream->socket, buffer, size, 0);
    if (len < 0) {
        return -1;
    }

    stream->bytes += len;
    return len;
}

// Read a rovercom sensor output message from the stream
// (you will need to switch on the returned message type to cast it to the correct type)
// returns the message, or NULL on error
// nb: it is the caller's responsibility to free the message
ProtobufMsgs__SensorOutput* read_pb(read_stream* stream) {
    zmq_msg_t msg;
    // Initialize a message container
    if (zmq_msg_init(&msg) != 0) {
        return NULL;
    }

    // Receive the message into the zmq_msg_t container
    if (stream->socket == NULL) {
        int res = init_read_stream(stream);
        if (res != 0) {
            return NULL;
        }
    }
    
    int len = zmq_msg_recv(&msg, stream->socket, 0);
    if (len < 0) {
        zmq_msg_close(&msg);
        return NULL;
    }
    stream->bytes += len;
    // Get the size of the received message
    size_t msg_size = zmq_msg_size(&msg);
    // Dynamically allocate a buffer large enough to hold the message
    uint8_t *buf = malloc(msg_size);
    if (!buf) {
        zmq_msg_close(&msg);
        return NULL;
    }
    // Copy the message data into our buffer
    memcpy(buf, zmq_msg_data(&msg), msg_size);
    zmq_msg_close(&msg);

    // Unpack the Protobuf message from the buffer
    ProtobufMsgs__SensorOutput *message = protobuf_msgs__sensor_output__unpack(NULL, msg_size, buf);
    free(buf);

    return message;
}

// Write bytes to a stream
// returns the number of bytes written, or -1 on error
int write_bytes(write_stream *stream, void *buffer, int size) {
    if (stream->socket == NULL) {
        int res = init_write_stream(stream);
        if (res != 0) {
            printf("Failed to initialize write stream\n");
            return -1;
        }
    }

    int len = zmq_send(stream->socket, buffer, size, 0);
    if (len < 0) {
        printf("Failed w sending\n");
        return -1;
    }

    stream->bytes += len;
    return len;
}

// Write a rovercom sensor output message to the stream
// returns the number of bytes written, or -1 on error
int write_pb(write_stream* stream, ProtobufMsgs__SensorOutput* message) {
    // Pack the message
    size_t len = protobuf_msgs__sensor_output__get_packed_size(message);
    uint8_t buf[len];
    size_t res = protobuf_msgs__sensor_output__pack(message, buf);
    if (res != len) {
        printf("Res not len\n");
        return -1;
    }

    return write_bytes(stream, buf, len);
}

