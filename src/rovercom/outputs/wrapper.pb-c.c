/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: outputs/wrapper.proto */

/* Do not generate deprecated warnings for self */
#ifndef PROTOBUF_C__NO_DEPRECATED
#define PROTOBUF_C__NO_DEPRECATED
#endif

#include "rovercom/outputs/wrapper.pb-c.h"
void   protobuf_msgs__sensor_output__init
                     (ProtobufMsgs__SensorOutput         *message)
{
  static const ProtobufMsgs__SensorOutput init_value = PROTOBUF_MSGS__SENSOR_OUTPUT__INIT;
  *message = init_value;
}
size_t protobuf_msgs__sensor_output__get_packed_size
                     (const ProtobufMsgs__SensorOutput *message)
{
  assert(message->base.descriptor == &protobuf_msgs__sensor_output__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t protobuf_msgs__sensor_output__pack
                     (const ProtobufMsgs__SensorOutput *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &protobuf_msgs__sensor_output__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t protobuf_msgs__sensor_output__pack_to_buffer
                     (const ProtobufMsgs__SensorOutput *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &protobuf_msgs__sensor_output__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
ProtobufMsgs__SensorOutput *
       protobuf_msgs__sensor_output__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (ProtobufMsgs__SensorOutput *)
     protobuf_c_message_unpack (&protobuf_msgs__sensor_output__descriptor,
                                allocator, len, data);
}
void   protobuf_msgs__sensor_output__free_unpacked
                     (ProtobufMsgs__SensorOutput *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &protobuf_msgs__sensor_output__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
static const ProtobufCFieldDescriptor protobuf_msgs__sensor_output__field_descriptors[12] =
{
  {
    "sensorId",
    1,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_UINT32,
    0,   /* quantifier_offset */
    offsetof(ProtobufMsgs__SensorOutput, sensorid),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "timestamp",
    2,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_UINT64,
    0,   /* quantifier_offset */
    offsetof(ProtobufMsgs__SensorOutput, timestamp),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "status",
    3,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_UINT32,
    0,   /* quantifier_offset */
    offsetof(ProtobufMsgs__SensorOutput, status),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "cameraOutput",
    4,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(ProtobufMsgs__SensorOutput, sensor_output_case),
    offsetof(ProtobufMsgs__SensorOutput, cameraoutput),
    &protobuf_msgs__camera_sensor_output__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "distanceOutput",
    5,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(ProtobufMsgs__SensorOutput, sensor_output_case),
    offsetof(ProtobufMsgs__SensorOutput, distanceoutput),
    &protobuf_msgs__distance_sensor_output__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "speedOutput",
    6,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(ProtobufMsgs__SensorOutput, sensor_output_case),
    offsetof(ProtobufMsgs__SensorOutput, speedoutput),
    &protobuf_msgs__speed_sensor_output__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "controllerOutput",
    7,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(ProtobufMsgs__SensorOutput, sensor_output_case),
    offsetof(ProtobufMsgs__SensorOutput, controlleroutput),
    &protobuf_msgs__controller_output__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "imuOutput",
    8,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(ProtobufMsgs__SensorOutput, sensor_output_case),
    offsetof(ProtobufMsgs__SensorOutput, imuoutput),
    &protobuf_msgs__imu_sensor_output__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "batteryOutput",
    9,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(ProtobufMsgs__SensorOutput, sensor_output_case),
    offsetof(ProtobufMsgs__SensorOutput, batteryoutput),
    &protobuf_msgs__battery_sensor_output__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "rpmOuput",
    10,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(ProtobufMsgs__SensorOutput, sensor_output_case),
    offsetof(ProtobufMsgs__SensorOutput, rpmouput),
    &protobuf_msgs__rpm_sensor_output__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "luxOutput",
    11,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(ProtobufMsgs__SensorOutput, sensor_output_case),
    offsetof(ProtobufMsgs__SensorOutput, luxoutput),
    &protobuf_msgs__lux_sensor_output__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "laptimeOutput",
    12,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(ProtobufMsgs__SensorOutput, sensor_output_case),
    offsetof(ProtobufMsgs__SensorOutput, laptimeoutput),
    &protobuf_msgs__lap_time_output__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned protobuf_msgs__sensor_output__field_indices_by_name[] = {
  8,   /* field[8] = batteryOutput */
  3,   /* field[3] = cameraOutput */
  6,   /* field[6] = controllerOutput */
  4,   /* field[4] = distanceOutput */
  7,   /* field[7] = imuOutput */
  11,   /* field[11] = laptimeOutput */
  10,   /* field[10] = luxOutput */
  9,   /* field[9] = rpmOuput */
  0,   /* field[0] = sensorId */
  5,   /* field[5] = speedOutput */
  2,   /* field[2] = status */
  1,   /* field[1] = timestamp */
};
static const ProtobufCIntRange protobuf_msgs__sensor_output__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 12 }
};
const ProtobufCMessageDescriptor protobuf_msgs__sensor_output__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "protobuf_msgs.SensorOutput",
  "SensorOutput",
  "ProtobufMsgs__SensorOutput",
  "protobuf_msgs",
  sizeof(ProtobufMsgs__SensorOutput),
  12,
  protobuf_msgs__sensor_output__field_descriptors,
  protobuf_msgs__sensor_output__field_indices_by_name,
  1,  protobuf_msgs__sensor_output__number_ranges,
  (ProtobufCMessageInit) protobuf_msgs__sensor_output__init,
  NULL,NULL,NULL    /* reserved[123] */
};
