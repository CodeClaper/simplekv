#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "config.h"
#include "server.h"
#include "utils.h"

#define DEFAULT_CONF_FILE "config/simplekv.conf"

/* Read config file. */
static char *ReadConfig(const char *title, const char *key) {
    FILE *file = fopen(DEFAULT_CONF_FILE, "r");
    if (file == NULL) {
        fprintf(stderr, "Try to open config file in path '%s' fail.\n", DEFAULT_CONF_FILE);
        exit(1);
    }
    char buff[BUFF_SIZE];
    char *p;
    bool inBlock = false;
    while(!feof(file)) {
        char *ret = fgets(buff, BUFF_SIZE, file);
        if (ret == NULL) {
            perror("Read conf file error.");
            exit(1);
        }
        char *line = Trim(buff);
        /* Check if commenter line. */
        if (StartWith(line, "//") || StartWith(line, "#")) continue;
        /* Check if the title */
        if (StartWith(line, "[") && StartWith(line + 1, title)) {
            inBlock = true;    
            continue;
        }
        /* If not in the title block, skip it. */
        if (!inBlock) continue;
        /* Check if has symbol '='. */
        if (strchr(line, '=') == NULL) continue;
        /* Check if this line contains key. */
        if (!StartWith(line, key)) continue;
        p = strstr(line, key);
        if (p == NULL) {
            perror("Read configuration error.");
            exit(1);
        }
        p += strlen(key);
        Trim(p);
        /* skip '=' */
        p += 1;
        fclose(file);
        return strdup(Trim(p));
    }
    fprintf(stderr, "Not found [%s].[%s] in configuration file.\n", title, key);
    exit(EXIT_FAILURE);
}

/* Load configuration. */
void LoadConfigFile() {
    server.host = ReadConfig("server", "host");
    server.port = atoi(ReadConfig("server", "port"));
}
