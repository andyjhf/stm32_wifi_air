#include "MUtility.h"

U32 HTTP_GetPkt(char *pkt,const char* url, const char *key, const char *ip,const char *port);
U32 HTTP_PostPkt(char *pkt,const char* url, const char *key, const char *ip,const char *port,char *json);