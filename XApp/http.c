#include "http.h"

char text[100];
char temp[10];

/**
  * @brief   HTTP GET
  * @param   pkt 
  * @param   key
  * @param   data
  */
U32 HTTP_GetPkt(char *pkt,const char* url, const char *key, const char *ip,const char *port)
{
  *pkt = 0;
	memset(text,0,sizeof(text));
	snprintf(text,sizeof(text),"GET /%s HTTP/1.1\r\n",url);
	strcat(pkt, text);
	
	memset(text,0,sizeof(text));
	snprintf(text,sizeof(text),"api-key: %s\r\n",key);
	strcat(pkt, text);
	
	memset(text,0,sizeof(text));
	snprintf(text,sizeof(text),"Host:%s:%s\r\n",ip,port);
	strcat(pkt, text);
	
	memset(text,0,sizeof(text));
	snprintf(text,sizeof(text),"\r\n\r\n");
	strcat(pkt, text);
	
  return strlen(pkt);
}

U32 HTTP_PostPkt(char *pkt,const char* url, const char *key, const char *ip,const char *port,char *json)
{
  *pkt = 0;
	memset(text,0,sizeof(text));
	snprintf(text,sizeof(text),"POST /%s HTTP/1.1\r\n",url);
	strcat(pkt, text);
	
	memset(text,0,sizeof(text));
	snprintf(text,sizeof(text),"api-key: %s\r\n",key);
	strcat(pkt, text);
	
	memset(text,0,sizeof(text));
	snprintf(text,sizeof(text),"Host:%s:%s\r\n",ip,port);
	strcat(pkt, text);
	
	memset(text,0,sizeof(text));
	snprintf(text,sizeof(text),"Content-Length:%s\r\n",strlen(json));
	strcat(pkt, text);
	
	memset(text,0,sizeof(text));
	snprintf(text,sizeof(text),"\r\n");
	strcat(pkt, text);
	
	strcat(pkt, json);
	
  return strlen(pkt);
}