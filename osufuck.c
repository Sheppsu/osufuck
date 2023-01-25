#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dict.h"
#include "list.h"


static char *osuCommands[] = {
    "wysi", "cookiezi", "727", "btmc",
    "pp", "sotarks", "playmore", 
    "peppyisinsideyourcomputer",
};
static short osuCommandsLength = 8; // easier than calculating lol
static short maxWordLength = 25;
static char translatedCommands[] = {
    '>', '<', '+', '-', '.', ',', '[', ']'
};


short match_command(char *commands, char* buf) {
    for (short w=0; w<osuCommandsLength; w++) {
        char *word = osuCommands[w];
        short i = 0;
        while (word[i] == commands[i]) {
            i++;
            if (i == strlen(word)) {
                *buf = translatedCommands[w];
                return i;
            }
        }
    }
    return 0;
}


char* translate_commands(char *commands) {
    char *translatedCommands = calloc(0, sizeof(char));
    unsigned long long i = 0;
    size_t commandLength = strlen(commands);
    while (i < commandLength) {
        char *buf = malloc(sizeof(char));
        short interval = match_command(&commands[i], buf);
        if (interval == 0) {
            printf("Failed to parse given commands due to translation error...\n");
            free(translatedCommands);
            free(buf);
            return NULL;
        }
        i += interval;
        translatedCommands = realloc(translatedCommands, strlen(translatedCommands)+1);
        strncat(translatedCommands, buf, 1);
        free(buf);
    }
    return translatedCommands;
}


Dictionary* build_bracemap(char *commands) {
    List *temp = create_list();
    Dictionary *map = create_dict();

    size_t commandLength = strlen(commands);
    for (unsigned long long i=0; i<commandLength; i++) {
        if (commands[i] == '[') {
            unsigned long long *copyI = malloc(sizeof(unsigned long long));
            *copyI = i;
            append_value(temp, create_list_value(copyI, sizeof i));
        }
        if (commands[i] == ']') {
            unsigned long long *copyI = malloc(sizeof(unsigned long long));
            *copyI = i;
            unsigned long long *start = (unsigned long long*)pop_value(temp, temp->length-1);
            append_item(map, create_item(start, copyI, sizeof i, sizeof i));
            append_item(map, create_item(copyI, start, sizeof i, sizeof i));
        }
    }
    return map;
}


void do_commands(char *commands) {
    char *translation = translate_commands(commands);
    if (translation == NULL) {
        return;
    }
    free(commands);
    Dictionary *bracemap = build_bracemap(translation);
    
    char *ptr = calloc(30000, sizeof(char));
    size_t commandLength = strlen(translation);
    for (unsigned long long i=0; i<commandLength; i++) {
        switch (translation[i]) {
            case '>' : ptr++;break;
            case '<' : ptr--;break;
            case '+' : ++*ptr;break;
            case '-' : --*ptr;break;
            case '.' : putchar(*ptr);break;
            case ',' : *ptr = getchar();break;
            case '[' : if (*ptr == 0) {i = *(unsigned long long*)get_value(bracemap, &i, sizeof i);}break;
            case ']' : if(*ptr !=0) {i = *(unsigned long long*)get_value(bracemap, &i, sizeof i);}break;
            default : printf("Invalid command was received after translation\n");return;
        }
    }
    
    free(translation);
    free(ptr);
    free_dict(bracemap);
}


char *read_commands_from_file() {
    int CHUNK_SIZE = 1024;
    FILE *f = fopen("commands.osufuck", "r");
    if (f == NULL) {
        printf("Failed to open commands.osufuck file\n");
        return NULL;
    }
    char *commands = calloc(0, sizeof(char));
    char buf[CHUNK_SIZE];
   size_t size = 0;
    while (fgets(buf, CHUNK_SIZE, f) != NULL) {
        commands = realloc(commands, size + CHUNK_SIZE);
        strcat(commands, buf);
        size += CHUNK_SIZE;
    }
    if (!feof(f)) {
        printf("Failed to read contents of file...\n");
        return NULL;
    }
    fclose(f);
    return commands;
}


int main(int argc, char **argv) {
    if (argc > 2) {
        printf("Either supply no arguments or only one.");
    } else if (argc == 2) {
        do_commands(argv[1]);
    } else {
        char *commands = read_commands_from_file();
        if (commands == NULL) {
            return 0;
        }
        do_commands(commands);
    }
    return 0;
}
