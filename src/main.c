#include "../include/roverlib.h"
#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

bool stop = false;
pthread_mutex_t stopLock;

// Small struct to wrap data used in check_distance thread
typedef struct{
    read_stream* distanceInput;
    double brakeDistance;
    Service_configuration* config;
} CheckDistanceData;

typedef struct{
    double proportionalGain;
    double integralGain;
    double derivativeGain;
} PID;

double get_steer_value(PID *pid, double referenceSignal, double actualSignal, double samplingInterval) {
    double error = referenceSignal - actualSignal;
    
    double P = pid->proportionalGain * error;
    double I = pid->integralGain * (error * samplingInterval); 
    double D = pid->derivativeGain * (error) / samplingInterval;
    
    double steerValue = P + I + D;

    if (steerValue > 1) {
        steerValue = 1;
    } else if (steerValue < -1) {
        steerValue = -1;
    }

    steerValue = -steerValue;

    return steerValue;
}



long long current_time_millis() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long long)(tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
}

void* check_distance(void* arg){
    CheckDistanceData d = *(CheckDistanceData*)arg;

    while(1){
        ProtobufMsgs__SensorOutput* data = read_pb(d.distanceInput);
        if(data == NULL){
            printf("Error reading stream\n");
            return NULL;
        }

        // check for new tuning values
        d.brakeDistance = *(get_float_value(d.config, "brake-distance"));

        // Get distance data
        ProtobufMsgs__DistanceSensorOutput* distanceData = (data->distanceoutput);
        if(distanceData == NULL){
            printf("Received sensor data that was not distance data\n");
            continue;
        }
        
        // Don't lock if the variable doesn't change, this is the only place where this variable is edited.
        if((distanceData->distance < d.brakeDistance) != stop){
            pthread_mutex_lock(&stopLock);
            stop = distanceData->distance < d.brakeDistance;
            printf("Changed stop: %d\n", stop);
            pthread_mutex_unlock(&stopLock);
        }

    }
}

int user_program(Service service, Service_configuration *configuration){

    //
	// Set up stream to read track from
	//
    read_stream* imagingInput = get_read_stream(&service, "imaging", "path");
    if(imagingInput == NULL){
        printf("No Read Stream for imaging provided\n");
        return 1;
    }

    //
	// Set up stream to read distance from
	//
    read_stream* distanceInput = get_read_stream(&service, "distance", "distance");
    if(imagingInput == NULL){
        printf("No Read Stream for distance provided\n");
        return 1;
    }

    //
	// Set up stream to write actuator data to
	//
    write_stream* actuatorOutput = get_write_stream(&service, "decision");

    if(configuration == NULL){
        printf("No Configuration provided\n");
        return 1;
    }

    // Get PID tuning values
    double* kp = get_float_value_safe(configuration, "kp");
    if(kp == NULL){
        printf("Could not get kp from configuration\n");
        return 1;
    }

    double* ki = get_float_value_safe(configuration, "ki");
    if(ki == NULL){
        printf("Could not get ki from configuration\n");
        return 1;
    }

    double* kd = get_float_value_safe(configuration, "kd");
    if(kd == NULL){
        printf("Could not get kd from configuration\n");
        return 1;
    }

    double* speed = get_float_value_safe(configuration, "speed");
    if(speed == NULL){
        printf("Could not get speed from configuration\n");
        return 1;
    }

    double* desiredTrajectoryPointFloat = get_float_value_safe(configuration, "desired-trajectory-point");
    if(desiredTrajectoryPointFloat == NULL){
        printf("Could not get desired-trajectory-point from configuration\n");
        return 1;
    }

    int desiredTrajectoryPoint = (int)(*desiredTrajectoryPointFloat);

    PID pid;
    pid.proportionalGain = *kp;
    pid.integralGain = *ki;
    pid.derivativeGain = *kd;

    CheckDistanceData d;
    d.brakeDistance = *get_float_value_safe(configuration, "brake-distance");
    if(desiredTrajectoryPointFloat == NULL){
        printf("Could not get brake-distance from configuration\n");
        return 1;
    }
    d.distanceInput = distanceInput;
    d.config = configuration;

    pthread_mutex_init(&stopLock, NULL);

    pthread_t threadLock;

    pthread_create(&threadLock, NULL, check_distance, &d);
    
    // Main loop, subscribe to trajectory data and send decision data
    while(1){
        ProtobufMsgs__SensorOutput* data = read_pb(imagingInput);
        if(data == NULL){
            printf("Error reading stream\n");
            return 1;
        }

        // Get imaging data
        ProtobufMsgs__CameraSensorOutput* imagingData = (data->cameraoutput);
        if(imagingData == NULL){
            printf("Received sensor data that was not camera data\n");
            continue;
        }

        // Get trajectory
        ProtobufMsgs__CameraSensorOutput__Trajectory* trajectory = (imagingData->trajectory);
        if(trajectory == NULL){
            printf("Received sensor data that was not trajectory data\n");
            continue;
        }

        speed = get_float_value(configuration, "speed");
        if(speed == NULL){
            printf("Could not get speed from configuration\n");
            return 1;
        }

        double* newKp = get_float_value(configuration, "kp");
        double* newKd = get_float_value(configuration, "kd");
        double* newKi = get_float_value(configuration, "ki");

        if(newKp != NULL && newKd != NULL && newKi != NULL && (*newKp != *kp || *newKd != *kd || *newKi != *ki)){
            *kp = *newKp;
            *kd = *newKd;
            *ki = *newKi;

            PID pid;
            pid.proportionalGain = *kp;
            pid.integralGain = *ki;
            pid.derivativeGain = *kd;

            printf("kp: %f, kd: %f, ki: %f. Updated PID controller", *kp, *kd, *ki);
        }

        // Get the first trajectory point
        ProtobufMsgs__CameraSensorOutput__Trajectory__Point** trajectoryPoints = trajectory->points;

        if(trajectoryPoints == NULL){
            printf("no trajectory points\n");
            continue;
        }

        if(trajectoryPoints[0] == NULL){
            printf("Received sensor data that had no trajectory points\n");
            continue;
        }
		
        // This is the middle of the longest consecutive slice, it should be in the middle of the image (horizontally)
        ProtobufMsgs__CameraSensorOutput__Trajectory__Point* firstPoint = trajectoryPoints[0];

        double steerValue = get_steer_value(&pid, (double)desiredTrajectoryPoint, firstPoint->x, 0.1);

        // Write message to actuator
        ProtobufMsgs__SensorOutput msg = PROTOBUF_MSGS__SENSOR_OUTPUT__INIT;

        msg.sensorid = 2;
        msg.timestamp = current_time_millis();
        msg.status = 0;

        ProtobufMsgs__ControllerOutput controllerOutput = PROTOBUF_MSGS__CONTROLLER_OUTPUT__INIT;

        // Acquire lock to read stop value
        pthread_mutex_lock(&stopLock);
        if(!stop){
            controllerOutput.leftthrottle = (float)(*speed);
            controllerOutput.rightthrottle = (float)(*speed);
        }
        else{
            controllerOutput.leftthrottle = 0;
            controllerOutput.rightthrottle = 0;
        }
        pthread_mutex_unlock(&stopLock);

        controllerOutput.steeringangle = (float)(steerValue);
        
        controllerOutput.frontlights = 0;
        controllerOutput.fanspeed = 0;

        msg.controlleroutput = &controllerOutput;
        msg.sensor_output_case = PROTOBUF_MSGS__SENSOR_OUTPUT__SENSOR_OUTPUT_CONTROLLER_OUTPUT;

        int res = write_pb(actuatorOutput, &msg);
        if(res < 0){
            printf("Failed to send controller output\n");
            continue;
        }             

        printf("Sent controller output\n");
    }
}

int main(){
    return run(user_program);
}

