#ifndef STREAMS_H
#define STREAMS_H

#include <stdlib.h>
#include <zmq.h>
#include <list.h>
#include "../rovercom/outputs/wrapper.pb-c.h"
#include "bootinfo.h"

//
// It might seem cumbersome to define a new struct with the same contents twice, but this allows for more powerful compiler warnings and type checking 
// (i.e. avoiding passing a read stream to a write function)
//

typedef struct {
    char *address;          // Pointer to a dynamically allocated string for the address
    void *socket;           // Can be null if not initialized
    int64_t bytes;          // Amount of bytes read/written so far
} write_stream;

typedef struct {
    char *address;          // Pointer to a dynamically allocated string for the address
    void *socket;           // Can be null if not initialized
    int64_t bytes;          // Amount of bytes read/written so far
} read_stream;

// Create a stream based on output name (as is defined in the service.yaml)
write_stream* get_write_stream(Service* service, char* name);

// Create a stream based on input of another service and its stream (as is defined in the service.yaml)
read_stream* get_read_stream(Service* service, char* dependency_name, char* stream_name);

// Initial setup of a stream (done lazily, on first read)
// returns nonzero on error
int init_read_stream(read_stream *stream);

// Initial setup of a stream (done lazily, on first write)
// returns nonzero on error
int init_write_stream(write_stream *stream);

// Read bytes from a stream
// returns the number of bytes read, or -1 on error
int read_bytes(read_stream *stream, void *buffer, int size);

// Read a rovercom sensor output message from the stream
// (you will need to switch on the returned message type to cast it to the correct type)
// returns the message, or NULL on error
// nb: it is the caller's responsibility to free the message
ProtobufMsgs__SensorOutput* read_pb(read_stream* stream);

// Write bytes to a stream
// returns the number of bytes written, or -1 on error
int write_bytes(write_stream *stream, void *buffer, int size);

// Write a rovercom sensor output message to the stream
// returns the number of bytes written, or -1 on error
int write_pb(write_stream* stream, ProtobufMsgs__SensorOutput* message);

#endif // STREAMS_H