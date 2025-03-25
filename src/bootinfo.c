#include "../include/roverlib/bootinfo.h"

enum Type cJSON_GetTypeValue(const cJSON * j) {
    enum Type x = 0;
    if (NULL != j) {
        if (!strcmp(cJSON_GetStringValue(j), "number")) x = TYPE_NUMBER;
        else if (!strcmp(cJSON_GetStringValue(j), "string")) x = TYPE_STRING;
    }
    return x;
}

cJSON * cJSON_CreateType(const enum Type x) {
    cJSON * j = NULL;
    switch (x) {
        case TYPE_NUMBER: j = cJSON_CreateString("number"); break;
        case TYPE_STRING: j = cJSON_CreateString("string"); break;
    }
    return j;
}

struct Value * cJSON_GetValueValue(const cJSON * j) {
    struct Value * x = cJSON_malloc(sizeof(struct Value));
    if (NULL != x) {
        memset(x, 0, sizeof(struct Value));
        if (cJSON_IsNumber(j)) {
            x->type = cJSON_Number;
            x->value.number = cJSON_GetNumberValue(j);
        }
        else if (cJSON_IsString(j)) {
            x->type = cJSON_String;
            x->value.string = strdup(cJSON_GetStringValue(j));
        }
    }
    return x;
}

cJSON * cJSON_CreateValue(const struct Value * x) {
    cJSON * j = NULL;
    if (NULL != x) {
        if (cJSON_Number == x->type) {
            j = cJSON_CreateNumber(x->value.number);
        }
        else if (cJSON_String == x->type) {
            j = cJSON_CreateString(x->value.string);
        }
    }
    return j;
}

void cJSON_DeleteValue(struct Value * x) {
    if (NULL != x) {
        if (cJSON_Number == x->type) {
        }
        else if (cJSON_String == x->type) {
            cJSON_free(x->value.string);
        }
        cJSON_free(x);
    }
}

struct Configuration * cJSON_ParseConfiguration(const char * s) {
    struct Configuration * x = NULL;
    if (NULL != s) {
        cJSON * j = cJSON_Parse(s);
        if (NULL != j) {
            x = cJSON_GetConfigurationValue(j);
            cJSON_Delete(j);
        }
    }
    return x;
}

struct Configuration * cJSON_GetConfigurationValue(const cJSON * j) {
    struct Configuration * x = NULL;
    if (NULL != j) {
        if (NULL != (x = cJSON_malloc(sizeof(struct Configuration)))) {
            memset(x, 0, sizeof(struct Configuration));
            if (cJSON_HasObjectItem(j, "name")) {
                x->name = strdup(cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(j, "name")));
            }
            if (cJSON_HasObjectItem(j, "tunable")) {
                if (NULL != (x->tunable = cJSON_malloc(sizeof(bool)))) {
                    *x->tunable = cJSON_IsTrue(cJSON_GetObjectItemCaseSensitive(j, "tunable"));
                }
            }
            if (cJSON_HasObjectItem(j, "type")) {
                if (NULL != (x->type = cJSON_malloc(sizeof(enum Type)))) {
                    *x->type = cJSON_GetTypeValue(cJSON_GetObjectItemCaseSensitive(j, "type"));
                }
            }
            if (cJSON_HasObjectItem(j, "value")) {
                x->value = cJSON_GetValueValue(cJSON_GetObjectItemCaseSensitive(j, "value"));
            }
        }
    }
    return x;
}

cJSON * cJSON_CreateConfiguration(const struct Configuration * x) {
    cJSON * j = NULL;
    if (NULL != x) {
        if (NULL != (j = cJSON_CreateObject())) {
            if (NULL != x->name) {
                cJSON_AddStringToObject(j, "name", x->name);
            }
            if (NULL != x->tunable) {
                cJSON_AddBoolToObject(j, "tunable", *x->tunable);
            }
            if (NULL != x->type) {
                cJSON_AddItemToObject(j, "type", cJSON_CreateType(*x->type));
            }
            if (NULL != x->value) {
                cJSON_AddItemToObject(j, "value", cJSON_CreateValue(x->value));
            }
        }
    }
    return j;
}

char * cJSON_PrintConfiguration(const struct Configuration * x) {
    char * s = NULL;
    if (NULL != x) {
        cJSON * j = cJSON_CreateConfiguration(x);
        if (NULL != j) {
            s = cJSON_Print(j);
            cJSON_Delete(j);
        }
    }
    return s;
}

void cJSON_DeleteConfiguration(struct Configuration * x) {
    if (NULL != x) {
        if (NULL != x->name) {
            cJSON_free(x->name);
        }
        if (NULL != x->tunable) {
            cJSON_free(x->tunable);
        }
        if (NULL != x->type) {
            cJSON_free(x->type);
        }
        if (NULL != x->value) {
            cJSON_DeleteValue(x->value);
        }
        cJSON_free(x);
    }
}

struct Stream * cJSON_ParseStream(const char * s) {
    struct Stream * x = NULL;
    if (NULL != s) {
        cJSON * j = cJSON_Parse(s);
        if (NULL != j) {
            x = cJSON_GetStreamValue(j);
            cJSON_Delete(j);
        }
    }
    return x;
}

struct Stream * cJSON_GetStreamValue(const cJSON * j) {
    struct Stream * x = NULL;
    if (NULL != j) {
        if (NULL != (x = cJSON_malloc(sizeof(struct Stream)))) {
            memset(x, 0, sizeof(struct Stream));
            if (cJSON_HasObjectItem(j, "address")) {
                x->address = strdup(cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(j, "address")));
            }
            if (cJSON_HasObjectItem(j, "name")) {
                x->name = strdup(cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(j, "name")));
            }
        }
    }
    return x;
}

cJSON * cJSON_CreateStream(const struct Stream * x) {
    cJSON * j = NULL;
    if (NULL != x) {
        if (NULL != (j = cJSON_CreateObject())) {
            if (NULL != x->address) {
                cJSON_AddStringToObject(j, "address", x->address);
            }
            if (NULL != x->name) {
                cJSON_AddStringToObject(j, "name", x->name);
            }
        }
    }
    return j;
}

char * cJSON_PrintStream(const struct Stream * x) {
    char * s = NULL;
    if (NULL != x) {
        cJSON * j = cJSON_CreateStream(x);
        if (NULL != j) {
            s = cJSON_Print(j);
            cJSON_Delete(j);
        }
    }
    return s;
}

void cJSON_DeleteStream(struct Stream * x) {
    if (NULL != x) {
        if (NULL != x->address) {
            cJSON_free(x->address);
        }
        if (NULL != x->name) {
            cJSON_free(x->name);
        }
        cJSON_free(x);
    }
}

struct Input * cJSON_ParseInput(const char * s) {
    struct Input * x = NULL;
    if (NULL != s) {
        cJSON * j = cJSON_Parse(s);
        if (NULL != j) {
            x = cJSON_GetInputValue(j);
            cJSON_Delete(j);
        }
    }
    return x;
}

struct Input * cJSON_GetInputValue(const cJSON * j) {
    struct Input * x = NULL;
    if (NULL != j) {
        if (NULL != (x = cJSON_malloc(sizeof(struct Input)))) {
            memset(x, 0, sizeof(struct Input));
            if (cJSON_HasObjectItem(j, "service")) {
                x->service = strdup(cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(j, "service")));
            }
            if (cJSON_HasObjectItem(j, "streams")) {
                list_t * x1 = list_create(false, NULL);
                if (NULL != x1) {
                    cJSON * e1 = NULL;
                    cJSON * j1 = cJSON_GetObjectItemCaseSensitive(j, "streams");
                    cJSON_ArrayForEach(e1, j1) {
                        list_add_tail(x1, cJSON_GetStreamValue(e1), sizeof(struct Stream *));
                    }
                    x->streams = x1;
                }
            }
        }
    }
    return x;
}

cJSON * cJSON_CreateInput(const struct Input * x) {
    cJSON * j = NULL;
    if (NULL != x) {
        if (NULL != (j = cJSON_CreateObject())) {
            if (NULL != x->service) {
                cJSON_AddStringToObject(j, "service", x->service);
            }
            if (NULL != x->streams) {
                cJSON * j1 = cJSON_AddArrayToObject(j, "streams");
                if (NULL != j1) {
                    struct Stream * x1 = list_get_head(x->streams);
                    while (NULL != x1) {
                        cJSON_AddItemToArray(j1, cJSON_CreateStream(x1));
                        x1 = list_get_next(x->streams);
                    }
                }
            }
        }
    }
    return j;
}

char * cJSON_PrintInput(const struct Input * x) {
    char * s = NULL;
    if (NULL != x) {
        cJSON * j = cJSON_CreateInput(x);
        if (NULL != j) {
            s = cJSON_Print(j);
            cJSON_Delete(j);
        }
    }
    return s;
}

void cJSON_DeleteInput(struct Input * x) {
    if (NULL != x) {
        if (NULL != x->service) {
            cJSON_free(x->service);
        }
        if (NULL != x->streams) {
            struct Stream * x1 = list_get_head(x->streams);
            while (NULL != x1) {
                cJSON_DeleteStream(x1);
                x1 = list_get_next(x->streams);
            }
            list_release(x->streams);
        }
        cJSON_free(x);
    }
}

struct Output * cJSON_ParseOutput(const char * s) {
    struct Output * x = NULL;
    if (NULL != s) {
        cJSON * j = cJSON_Parse(s);
        if (NULL != j) {
            x = cJSON_GetOutputValue(j);
            cJSON_Delete(j);
        }
    }
    return x;
}

struct Output * cJSON_GetOutputValue(const cJSON * j) {
    struct Output * x = NULL;
    if (NULL != j) {
        if (NULL != (x = cJSON_malloc(sizeof(struct Output)))) {
            memset(x, 0, sizeof(struct Output));
            if (cJSON_HasObjectItem(j, "address")) {
                x->address = strdup(cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(j, "address")));
            }
            if (cJSON_HasObjectItem(j, "name")) {
                x->name = strdup(cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(j, "name")));
            }
        }
    }
    return x;
}

cJSON * cJSON_CreateOutput(const struct Output * x) {
    cJSON * j = NULL;
    if (NULL != x) {
        if (NULL != (j = cJSON_CreateObject())) {
            if (NULL != x->address) {
                cJSON_AddStringToObject(j, "address", x->address);
            }
            if (NULL != x->name) {
                cJSON_AddStringToObject(j, "name", x->name);
            }
        }
    }
    return j;
}

char * cJSON_PrintOutput(const struct Output * x) {
    char * s = NULL;
    if (NULL != x) {
        cJSON * j = cJSON_CreateOutput(x);
        if (NULL != j) {
            s = cJSON_Print(j);
            cJSON_Delete(j);
        }
    }
    return s;
}

void cJSON_DeleteOutput(struct Output * x) {
    if (NULL != x) {
        if (NULL != x->address) {
            cJSON_free(x->address);
        }
        if (NULL != x->name) {
            cJSON_free(x->name);
        }
        cJSON_free(x);
    }
}

struct Tuning * cJSON_ParseTuning(const char * s) {
    struct Tuning * x = NULL;
    if (NULL != s) {
        cJSON * j = cJSON_Parse(s);
        if (NULL != j) {
            x = cJSON_GetTuningValue(j);
            cJSON_Delete(j);
        }
    }
    return x;
}

struct Tuning * cJSON_GetTuningValue(const cJSON * j) {
    struct Tuning * x = NULL;
    if (NULL != j) {
        if (NULL != (x = cJSON_malloc(sizeof(struct Tuning)))) {
            memset(x, 0, sizeof(struct Tuning));
            if (cJSON_HasObjectItem(j, "address")) {
                x->address = strdup(cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(j, "address")));
            }
            if (cJSON_HasObjectItem(j, "enabled")) {
                if (NULL != (x->enabled = cJSON_malloc(sizeof(bool)))) {
                    *x->enabled = cJSON_IsTrue(cJSON_GetObjectItemCaseSensitive(j, "enabled"));
                }
            }
        }
    }
    return x;
}

cJSON * cJSON_CreateTuning(const struct Tuning * x) {
    cJSON * j = NULL;
    if (NULL != x) {
        if (NULL != (j = cJSON_CreateObject())) {
            if (NULL != x->address) {
                cJSON_AddStringToObject(j, "address", x->address);
            }
            if (NULL != x->enabled) {
                cJSON_AddBoolToObject(j, "enabled", *x->enabled);
            }
        }
    }
    return j;
}

char * cJSON_PrintTuning(const struct Tuning * x) {
    char * s = NULL;
    if (NULL != x) {
        cJSON * j = cJSON_CreateTuning(x);
        if (NULL != j) {
            s = cJSON_Print(j);
            cJSON_Delete(j);
        }
    }
    return s;
}

void cJSON_DeleteTuning(struct Tuning * x) {
    if (NULL != x) {
        if (NULL != x->address) {
            cJSON_free(x->address);
        }
        if (NULL != x->enabled) {
            cJSON_free(x->enabled);
        }
        cJSON_free(x);
    }
}

struct Service * cJSON_ParseService(const char * s) {
    struct Service * x = NULL;
    if (NULL != s) {
        cJSON * j = cJSON_Parse(s);
        if (NULL != j) {
            x = cJSON_GetServiceValue(j);
            cJSON_Delete(j);
        }
    }
    return x;
}

struct Service * cJSON_GetServiceValue(const cJSON * j) {
    struct Service * x = NULL;
    if (NULL != j) {
        if (NULL != (x = cJSON_malloc(sizeof(struct Service)))) {
            memset(x, 0, sizeof(struct Service));
            if (cJSON_HasObjectItem(j, "configuration")) {
                list_t * x1 = list_create(false, NULL);
                if (NULL != x1) {
                    cJSON * e1 = NULL;
                    cJSON * j1 = cJSON_GetObjectItemCaseSensitive(j, "configuration");
                    cJSON_ArrayForEach(e1, j1) {
                        list_add_tail(x1, cJSON_GetConfigurationValue(e1), sizeof(struct Configuration *));
                    }
                    x->configuration = x1;
                }
            }
            else {
                x->configuration = list_create(false, NULL);
            }
            if (cJSON_HasObjectItem(j, "inputs")) {
                list_t * x1 = list_create(false, NULL);
                if (NULL != x1) {
                    cJSON * e1 = NULL;
                    cJSON * j1 = cJSON_GetObjectItemCaseSensitive(j, "inputs");
                    cJSON_ArrayForEach(e1, j1) {
                        list_add_tail(x1, cJSON_GetInputValue(e1), sizeof(struct Input *));
                    }
                    x->inputs = x1;
                }
            }
            else {
                x->inputs = list_create(false, NULL);
            }
            if (cJSON_HasObjectItem(j, "name")) {
                x->name = strdup(cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(j, "name")));
            }
            if (cJSON_HasObjectItem(j, "outputs")) {
                list_t * x1 = list_create(false, NULL);
                if (NULL != x1) {
                    cJSON * e1 = NULL;
                    cJSON * j1 = cJSON_GetObjectItemCaseSensitive(j, "outputs");
                    cJSON_ArrayForEach(e1, j1) {
                        list_add_tail(x1, cJSON_GetOutputValue(e1), sizeof(struct Output *));
                    }
                    x->outputs = x1;
                }
            }
            else {
                x->outputs = list_create(false, NULL);
            }
            if (cJSON_HasObjectItem(j, "tuning")) {
                x->tuning = cJSON_GetTuningValue(cJSON_GetObjectItemCaseSensitive(j, "tuning"));
            }
            if (cJSON_HasObjectItem(j, "version")) {
                x->version = strdup(cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(j, "version")));
            }
            if (cJSON_HasObjectItem(j, "service")) {
                x->service = (void *)0xDEADBEEF;
            }
            else {
                x->service = (void *)0xDEADBEEF;
            }
        }
    }
    return x;
}

cJSON * cJSON_CreateService(const struct Service * x) {
    cJSON * j = NULL;
    if (NULL != x) {
        if (NULL != (j = cJSON_CreateObject())) {
            if (NULL != x->configuration) {
                cJSON * j1 = cJSON_AddArrayToObject(j, "configuration");
                if (NULL != j1) {
                    struct Configuration * x1 = list_get_head(x->configuration);
                    while (NULL != x1) {
                        cJSON_AddItemToArray(j1, cJSON_CreateConfiguration(x1));
                        x1 = list_get_next(x->configuration);
                    }
                }
            }
            if (NULL != x->inputs) {
                cJSON * j1 = cJSON_AddArrayToObject(j, "inputs");
                if (NULL != j1) {
                    struct Input * x1 = list_get_head(x->inputs);
                    while (NULL != x1) {
                        cJSON_AddItemToArray(j1, cJSON_CreateInput(x1));
                        x1 = list_get_next(x->inputs);
                    }
                }
            }
            if (NULL != x->name) {
                cJSON_AddStringToObject(j, "name", x->name);
            }
            if (NULL != x->outputs) {
                cJSON * j1 = cJSON_AddArrayToObject(j, "outputs");
                if (NULL != j1) {
                    struct Output * x1 = list_get_head(x->outputs);
                    while (NULL != x1) {
                        cJSON_AddItemToArray(j1, cJSON_CreateOutput(x1));
                        x1 = list_get_next(x->outputs);
                    }
                }
            }
            cJSON_AddItemToObject(j, "tuning", cJSON_CreateTuning(x->tuning));
            if (NULL != x->version) {
                cJSON_AddStringToObject(j, "version", x->version);
            }
        }
    }
    return j;
}

char * cJSON_PrintService(const struct Service * x) {
    char * s = NULL;
    if (NULL != x) {
        cJSON * j = cJSON_CreateService(x);
        if (NULL != j) {
            s = cJSON_Print(j);
            cJSON_Delete(j);
        }
    }
    return s;
}

void cJSON_DeleteService(struct Service * x) {
    if (NULL != x) {
        if (NULL != x->configuration) {
            struct Configuration * x1 = list_get_head(x->configuration);
            while (NULL != x1) {
                cJSON_DeleteConfiguration(x1);
                x1 = list_get_next(x->configuration);
            }
            list_release(x->configuration);
        }
        if (NULL != x->inputs) {
            struct Input * x1 = list_get_head(x->inputs);
            while (NULL != x1) {
                cJSON_DeleteInput(x1);
                x1 = list_get_next(x->inputs);
            }
            list_release(x->inputs);
        }
        if (NULL != x->name) {
            cJSON_free(x->name);
        }
        if (NULL != x->outputs) {
            struct Output * x1 = list_get_head(x->outputs);
            while (NULL != x1) {
                cJSON_DeleteOutput(x1);
                x1 = list_get_next(x->outputs);
            }
            list_release(x->outputs);
        }
        if (NULL != x->tuning) {
            cJSON_DeleteTuning(x->tuning);
        }
        if (NULL != x->version) {
            cJSON_free(x->version);
        }
        cJSON_free(x);
    }
}