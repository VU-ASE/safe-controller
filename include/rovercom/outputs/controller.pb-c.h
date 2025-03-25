/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: outputs/controller.proto */

#ifndef PROTOBUF_C_outputs_2fcontroller_2eproto__INCLUDED
#define PROTOBUF_C_outputs_2fcontroller_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1003000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1003003 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif


typedef struct _ProtobufMsgs__ControllerOutput ProtobufMsgs__ControllerOutput;


/* --- enums --- */


/* --- messages --- */

struct  _ProtobufMsgs__ControllerOutput
{
  ProtobufCMessage base;
  /*
   * Steering angle (-1.0 to 1.0 <-> left - right)
   */
  float steeringangle;
  /*
   * Throttle (-1.0 to 1.0 <-> full reverse - full forward)
   */
  float leftthrottle;
  float rightthrottle;
  /*
   * Onboard lights (0.0 to 1.0 <-> off - on)
   */
  protobuf_c_boolean frontlights;
  /*
   * Fan speed (0.0 to 1.0 <-> off - full speed)
   */
  float fanspeed;
  /*
   * Useful for debugging
   */
  /*
   * the error value before quadratic scaling
   */
  float rawerror;
  /*
   * the error value after quadratic scaling, but before PID
   */
  float scalederror;
};
#define PROTOBUF_MSGS__CONTROLLER_OUTPUT__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&protobuf_msgs__controller_output__descriptor) \
    , 0, 0, 0, 0, 0, 0, 0 }


/* ProtobufMsgs__ControllerOutput methods */
void   protobuf_msgs__controller_output__init
                     (ProtobufMsgs__ControllerOutput         *message);
size_t protobuf_msgs__controller_output__get_packed_size
                     (const ProtobufMsgs__ControllerOutput   *message);
size_t protobuf_msgs__controller_output__pack
                     (const ProtobufMsgs__ControllerOutput   *message,
                      uint8_t             *out);
size_t protobuf_msgs__controller_output__pack_to_buffer
                     (const ProtobufMsgs__ControllerOutput   *message,
                      ProtobufCBuffer     *buffer);
ProtobufMsgs__ControllerOutput *
       protobuf_msgs__controller_output__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   protobuf_msgs__controller_output__free_unpacked
                     (ProtobufMsgs__ControllerOutput *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*ProtobufMsgs__ControllerOutput_Closure)
                 (const ProtobufMsgs__ControllerOutput *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCMessageDescriptor protobuf_msgs__controller_output__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_outputs_2fcontroller_2eproto__INCLUDED */
