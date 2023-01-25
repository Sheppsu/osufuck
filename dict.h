 #ifndef dict
 #define dict

    #include <stdlib.h>
    #include <string.h>

    typedef struct DictItem {
        void *key;
        void *value;
    } DictItem;


    typedef struct Dictionary {
        DictItem **items;
        unsigned long length;
    } Dictionary;


    Dictionary* create_dict() {
        Dictionary* dictionary = (Dictionary*) malloc(sizeof(Dictionary));
        dictionary->length = 0;
        return dictionary;
    }

    DictItem* create_item(void *key, void *value, size_t keySize, size_t valueSize) {
        DictItem *item = (DictItem*) malloc(sizeof(DictItem));
        item->key = malloc(keySize);
        item->value = malloc(valueSize);
        strncpy((char*)item->key, (char*)key, keySize);
        strncpy((char*)item->value, (char*)value, valueSize);
        return item;
    }

    void append_item(Dictionary *dictionary, DictItem *item) {
        if (dictionary->length == 0) {
            dictionary->items = (DictItem**) malloc(sizeof(DictItem*));
            dictionary->items[0] = item;
            dictionary->length++;
        } else {
            dictionary->length++;
            dictionary->items = (DictItem**) realloc(dictionary->items, dictionary->length*sizeof(DictItem*));
            dictionary->items[dictionary->length-1] = item;
        }
    }

    void free_dict(Dictionary *dictionary) {
        for (unsigned long i=0; i<dictionary->length; i++) {
            free(dictionary->items[i]->key);
            free(dictionary->items[i]->value);
            free(dictionary->items[i]);
        }
        free(dictionary->items);
        free(dictionary);
    }

    void* get_value(Dictionary* dictionary, void *key, size_t keySize) {
        char *byteKey = (char*) malloc(keySize);
        strncpy(byteKey, (char*) key, keySize);
        for (unsigned long long i=0; i<dictionary->length; i++) {
            char *compare = (char*) malloc(sizeof(char)*keySize);
            strncpy(compare, (char*)dictionary->items[i]->key, keySize);

            if (strcmp(byteKey, compare) == 0) {
                return dictionary->items[i]->value;
            }

            free(compare);
        }
        free(byteKey);
        return NULL;
    }

#endif
