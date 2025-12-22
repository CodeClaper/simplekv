#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <time.h>
#include <float.h>
#include <regex.h>
#include "utils.h"
#include "server.h"

/* Left trim. 
 * Notice: not use the s directly, 
 * rather than the returning vlaue. */
char *LeftTrim(char *s) {
    while(isspace(*s)) s++;
    return s;
}

/* Right trim. 
 * Notice: although you can use s directly, 
 * but not recommand that. */
char *RightTrim(char *s) {
    char* back = s + strlen(s);
    while(isspace(*--back));
    *(back+1) = '\0';
    return s;
}

/* Trim */
char *Trim(char *s) {
    return RightTrim(LeftTrim(s)); 
}

/* Check if a string contains substring.*/
bool Contains(char* str, char *substr) {
    if (!str || !substr)
        return false;
    char *s = strstr(str, substr);
    return s != NULL;
}

/* Check if a file has prefix. */
bool StartWith(char *str, const char *prefix) {
    if (!str || !prefix)
        return false;
    ssize_t str_len = strlen(str);
    ssize_t pre_size = strlen(prefix);
    if (pre_size > str_len)
        return false;
    return strncmp(str, prefix, pre_size) == 0;
}

/* check if a file has suffix. */
bool EndWith(char *str, char *suffix) {
    if (!str || !suffix)
        return false;
    ssize_t str_len = strlen(str);
    ssize_t suffix_size = strlen(suffix);
    if (suffix_size > str_len)
        return false;
    return strcmp(str + str_len - suffix_size, suffix) == 0;
}

/* substring */
char *SubStr(char *str, uint32_t start, uint32_t end) {
    if (!str)
        return NULL;
    ssize_t str_size = strlen(str);
    if (start >= str_size || end >= str_size)
        return NULL;
    char *substr = malloc(end - start + 1);
    uint index = 0;
    for (uint32_t i = 0; i <str_size; i++) {
        if (start <= i && i <= end) {
            *(substr + index) = *(str + i);
            index++;
        }
    }
    *(substr + index) = '\0';
    return substr;
}

/* replace onece */
char *ReplaceOnce(char *str, const char *old_str, const char *new_str) {
    if (!str || !old_str)
        return NULL;
    ssize_t str_size = strlen(str);
    ssize_t old_size = strlen(old_str);
    if (old_size > str_size)
        return NULL;
    if (new_str == NULL)
        new_str = "";
    ssize_t new_size = strlen(new_str);
    char *ret = malloc(str_size - old_size + new_size + 1);

    uint32_t index;
    for (index = 0; index < str_size; index++) {
        if (strncmp(str + index, old_str, old_size) == 0) {
            memcpy(ret + index , new_str, new_size); 
            strcpy(ret + index + new_size, str + index + old_size);
            *(ret + str_size - old_size + new_size) = '\0';
            return ret;
        }
        *(ret + index) = *(str + index);
    }
    free(ret);
    return NULL;
}

/* Replace all. */
char *ReplaceAll(char *str, char *old_str, char *new_str) {
    if (!str || !old_str)
        return NULL;
    ssize_t str_size = strlen(str);
    ssize_t old_size = strlen(old_str);
    if (old_size > str_size)
        return NULL;
    if (new_str == NULL)
        new_str = "";
    ssize_t new_size = strlen(new_str);
    
    ssize_t size = str_size + 1;
    char* ret = malloc(size);
    uint32_t i, j;
    for (i = 0, j = 0; i < str_size; i++, j++) {
        if (strncmp(str + i, old_str, old_size) == 0) {
            if (j + new_size - old_size + 1 > size) {
                size = j + new_size -old_size + 1;
                ret = realloc(ret, size);
            }
            memcpy(ret + j, new_str, new_size);
            i += old_size;
            j += new_size;
        } 
        *(ret + j) = *(str + i);
    }

    *(ret + j) = '\0';
    
    return ret;
}

/* Check if empty string. */
bool StrIsEmpty(char *s) {
    if (s == NULL) 
        return true;
    size_t size = strlen(s);
    if (size == 0)
        return true;
    for(int i = 0; i < size; i++) {
        if (*(s + i) != ' ')
            return false;
    }
    return true;
}

/* Check if str is date format. */
bool StrIsDate(char *str) {
    if (StrIsEmpty(str))
        return false;

    regex_t reegex;
    int comp_result, exe_result;

    /* Visit `https://www.regular-expressions.info/gnu.html`, and notice there`s not "\\b". */
    comp_result = regcomp(&reegex, 
                          "^([0-9]{4})-(0[1-9]|1[0-2])-(0[1-9]|[12][0-9]|3[01])$", 
                          REG_EXTENDED);
    if (comp_result != 0)
        perror("Regex compile fail.");

    exe_result = regexec(&reegex, str, 0, NULL, 0);
    regfree(&reegex);

    return exe_result == REG_NOERROR;
}

/* Check if str is timestamp format. */
bool StrIsTimestamp(char *str) {
    if (StrIsEmpty(str))
        return false;

    regex_t reegex;
    int comp_result, exe_result;

    /* Visit `https://www.regular-expressions.info/gnu.html` and notice there`s not "\\b". */
    comp_result = regcomp(&reegex, 
                          "^([0-9]{4})-(0[1-9]|1[0-2])-(0[1-9]|[12][0-9]|3[01])\\s(0[0-9]|1[0-9]|2[0-3]):([0-5][0-9]):([0-5][0-9])(\\.[0-9]{1,3})?$", 
                          REG_EXTENDED);
    if (comp_result != 0)
        perror("Regex compile fail.");

    exe_result = regexec(&reegex, str, 0, NULL, 0);
    regfree(&reegex);

    return exe_result == REG_NOERROR;
}

/* Format String and return. */
char *FormatStr(char *format, ...) {
    char message[BUFF_SIZE];
    va_list ap;
    va_start(ap, format);
    vsprintf(message, format, ap);
    char *ret = strdup(message);
    va_end(ap);
    return ret;
}

/* Check if two strings are equal. 
 * Any of strings is NULL, return false. */
bool StrEq(char *str1, char *str2) {
    return str1 && str2 && strcmp(str1, str2) == 0;
}

/* Check if two strings are equal, ignoring case. 
 * Any of strings is NULL, return false. */
bool StrNoCaseEq(char *str1, char *str2) {
    return str1 && str2 && strcasecmp(str1, str2) == 0;
}

size_t StrLen(char *str) {
    return strlen(str);
}

/* Check if two strings are equal, 
 * if both are null, also return true. */
bool StrEqOrNull(char *str1, char *str2) {
    if (str1 && str2)
        return strcmp(str1, str2) == 0;
    else if (!str1 && !str2)
        return true;
    else
        return false;
}

