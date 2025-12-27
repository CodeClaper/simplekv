#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "server.h"
#include "common.h"
#include "config.h"
#include "anet.h"
#include "eloop.h"
#include "err.h"
#include "mm.h"
#include "asciilog.h"
#include "command.h"
#include "slog.h"

SimpleKVServer server;
ServerShare shared;

/* Send to client process. */
void ClientSendProc(struct EventLoop *el, int fd, int mask, void *privdata) {
    int nsend = 0, sentlen = 0, len;

    UNUSED(el);
    UNUSED(mask);

    len = strlen(privdata);
    while (len > sentlen) {
        nsend = write(fd, ((char *)privdata) + sentlen, len - sentlen);
        if (nsend <= 0) break;
        sentlen += nsend;
        if (sentlen == len) break;;
    }

    if (nsend == -1) {
        slog(ERROR, "Error writting to client: %s", strerror(errno));
        return;
    }

    DeleteFileEvent(el, fd, ELOOP_WRITABLE);
}

/* Read from client process. */
static void ClientReadProc(struct EventLoop *el, int fd, int mask, void *privdata) {
    char buf[BUFF_SIZE];
    int bytes;
    
    memset(buf, 0, BUFF_SIZE);
    bytes = read(fd, buf, BUFF_SIZE);
    if (bytes == -1) {
        if (errno == EAGAIN) {
            bytes = 0;
        } else {
            slog(ERROR, "Read from client erro: %s", strerror(errno));
            return;
        }
    } else if (bytes == 0) {
        slog(INFO, "Client closed connection.");
        DeleteFileEvent(el,  fd, mask);
        return;
    }
        
    /* Do command. */
    doCommand(fd, buf);
}

/* Add reply. */
void AddReply(int fd, char *msg) {
    if (CreateFileEvent(server.el, fd, ELOOP_WRITABLE, ClientSendProc, msg) == ELOOP_ERR)
        ThrowErr("Create file event fail.");
}

/* Server socket accept process.*/
static void ServerAcceptProc(struct EventLoop *el, int fd, int mask, void *privdata) {
    int cfd, clientPort;
    char clientIp[128];

    UNUSED(el);
    UNUSED(mask);
    UNUSED(privdata);

    cfd = ServerAccept(fd, clientIp, &clientPort);
    if (cfd == ANET_ERR) ThrowErr("Accept fail."); 
    slog(INFO, "Accepted %s:%d.", clientIp, clientPort);
    if (CreateFileEvent(el, cfd, ELOOP_READABLE, ClientReadProc, NULL) == ELOOP_ERR) 
        ThrowErr("Create file event fail.");
}

/* Init the server. */
static void InitServer(void) {
    server.host = NULL;
    server.port = DEFAULT_PORT;
    server.version = VERSION;
    server.serverfd = -1;
    server.el = instance(EventLoop);
    server.el->numkeys = 0;
    server.el->maxfd = 0;
    server.el->fileEventHead = NULL;
    server.el->stop = false;
    server.llevel = INFO;
}

/* Init the shared. */
static void InitShared() {
    shared.ok = strdup("OK\r\n");
    shared.err = strdup("ERR\r\n");
    shared.pong = strdup("PONG\r\n");
}


/* Config the server. */
static void ConfigServer(void) {
    /* Load from config file. */
    LoadConfigFile();
}

/* Create some directorys.*/
static void CreateDirs() {
    if (mkdir(server.logDir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1) {
        if (errno == EEXIST) return;
        ThrowErr("Error createing log directorys");
    }  
}

/* Setup the server. */
static void SetupServer(void) {
    int fd, retval;

    fd = CreateTcpServer(server.host, server.port);
    if (fd == ANET_ERR) ThrowErr("Create tcp socket server fail");
    retval = CreateFileEvent(server.el, fd, ELOOP_READABLE, ServerAcceptProc,  NULL);
    if (retval == ANET_ERR) ThrowErr("Create file event fail");
    server.serverfd = fd;
}

static void SimpleKVAsciiArt(void) {
    slogRaw(asccii_logo, 
            server.version, server.host, server.port, getpid());   
}

/* The main entry of server.  */
int main(int argc, char *argv[]) {
    InitServer();
    InitShared();
    ConfigServer();
    CreateDirs();
    SetupServer();
    SimpleKVAsciiArt();
    EloopMain(server.el);
    return 0;
}
