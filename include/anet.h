#ifndef __ANET_H__
#define __ANET_H__

#define ANET_OK   1
#define ANET_ERR  0

int CreateTcpServer(char *host, int port);
int ServerAccept(int serversocket, char *clientIp, int *clientPort);

#endif
