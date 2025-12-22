#ifndef __ELOOP_H__
#define __ELOOP_H__

#include <stdbool.h>

#define ELOOP_OK    1
#define ELOOP_ERR   0

/* Eloop mask flags. */
#define ELOOP_READABLE      1
#define ELOOP_WRITABLE      2
#define ELOOP_EXCEPTION     4

#define ELOOP_FILE_EVENTS   1
#define ELOOP_TIME_EVENTS   2
#define ELOOP_ALL_EVENTS    (ELOOP_FILE_EVENTS | ELOOP_TIME_EVENTS)
#define ELOOP_DONT_WAIT     4

struct EventLoop;

typedef void elFileProc(struct EventLoop *el, int fd, int mask, void *privdata);

/* Event loop control. */
typedef struct EventLoop {
    int maxfd;
    int numkeys;
    bool stop;
    struct FileEvent *fileEventHead;
} EventLoop;

/* File event structure. */
typedef struct FileEvent {
    int fd;
    int mask;
    elFileProc *proc;
    void *privdata;
    struct FileEvent *next;
} FileEvent;

int CreateFileEvent(EventLoop *el, int fd, int mask, elFileProc *proc, void *privdata);
void DeleteFileEvent(EventLoop *el, int fd, int mask);
void ProcessEvent(EventLoop *el, int flags);
void EloopMain(EventLoop *el);

#endif
