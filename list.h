#ifndef list
#define list

    #include <stdlib.h>
    #include <string.h>

    typedef struct ListValue {
        void *value;
        size_t size;
    } ListValue;

    typedef struct List {
        ListValue **values;
        unsigned long long length;
    } List;

    List* create_list() {
        List* l = (List*) malloc(sizeof(List));
        l->length = 0;
        return l;
    }

    ListValue* create_list_value(void *value, size_t valueSize) {
        ListValue* listValue = (ListValue*) malloc(sizeof(ListValue));
        listValue->value = malloc(valueSize);
        strncpy((char*)listValue->value, (char*)value, valueSize);
        listValue->size = valueSize;
        return listValue;
    }

    void append_value(List *l, ListValue *value) {
        if (l->length == 0) {
            l->values = (ListValue**) malloc(sizeof(ListValue*));
            l->values[0] = value;
            l->length++;
        } else {
            l->length++;
            l->values = (ListValue**) realloc(l->values, l->length*sizeof(ListValue*));
            l->values[l->length-1] = value;
        }
    }

    void remove_value(List *l, unsigned long long index) {
        free(l->values[index]->value);
        free(l->values[index]);
        for (unsigned long long i=index; i<l->length-1; i++) {
            l->values[i] = l->values[i+1];
        }
        l->length--;
        l->values = (ListValue**) realloc(l->values, sizeof(ListValue*)*l->length);
    }

    void* pop_value(List *l, unsigned long long index) {
        void *value = malloc(l->values[index]->size);
        strncpy((char*)value, (char*)l->values[index]->value, l->values[index]->size);
        remove_value(l, index);
        return value;
    }

    void free_list(List *l) {
        for (unsigned long long i=0; i<l->length; i++) {
            free(l->values[i]->value);
            free(l->values[i]);
        }
        free(l->values);
        free(l);
    }

#endif