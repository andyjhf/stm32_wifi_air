#include "MUtility.h"
#include <stdbool.h>
#include "cJSON.h"

typedef struct _json_loc
{
	U32 lat;
	U32 lon;	
}json_loc;

typedef struct _json_str
{
	U8 unit[30];
	U8 id[30];	
	U8 unit_symbol[30];	
	U8 create_time[30];	
}json_str;

typedef struct _json_data
{
	U8  priv;
	U8 	protocol[30];
	U8	create_time[30];
	U8 	online;
	json_loc  location;
	U8 id[30];
	U8 auth_info[30];
	json_str datastreams[5];
	U8 title[30];
	U8 tags[30];
}json_data;

typedef struct _json_param
{
	U32 errno;
	json_data data;
	U8 error[30];
}json_param;

U32 HTTP_GetPkt(char *pkt,const char* url, const char *key, const char *ip,const char *port);
U32 HTTP_PostPkt(char *pkt,const char* url, const char *key, const char *ip,const char *port,char *json);
U8 Json_to_data(char * json, json_param *data);
U8 data_to_Json(U8 *Json);
