#include "http.h"
#include<stdio.h>
#include<string.h>

/**
  * @brief   HTTP GET
  * @param   pkt 
  * @param   key
  * @param   data
  */
U32 HTTP_GetPkt(char *pkt,const char* url, const char *key, const char *ip,const char *port)
{
	char text[100];
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
	char text[100];
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
	snprintf(text,sizeof(text),"Content-Length:%d\r\n",strlen(json));
	strcat(pkt, text);
	
	memset(text,0,sizeof(text));
	snprintf(text,sizeof(text),"\r\n");
	strcat(pkt, text);
	
	strcat(pkt, json);
	
  return strlen(pkt);
}


U8 Json_to_data(char * json, json_param *data)
{
	cJSON *root = cJSON_Parse(json);
	if(!root) {
		printf("get root faild !\n");
		return 1;
	}
	cJSON *item = cJSON_GetObjectItem(root,"errno");
	if(item != NULL)
	{
		data->errno = item->valueint;
	}
	item = cJSON_GetObjectItem(root,"error");
	if(item != NULL)
	{
		memcpy(data->error,item->valuestring,strlen(item->valuestring));
	}
	cJSON *object=cJSON_GetObjectItem(root,"data");  
	if(object==NULL)  
	{
		printf("Error before: [%s]\n",cJSON_GetErrorPtr());  
		cJSON_Delete(root);  
		return 1;  
	}
	else
	{
		item=cJSON_GetObjectItem(object,"protocol");  
		if(item!=NULL)  
		{
			memcpy(data->data.protocol,item->valuestring,strlen(item->valuestring));  
		}
		item=cJSON_GetObjectItem(object,"create_time");  
		if(item!=NULL)  
		{
			memcpy(data->data.create_time,item->valuestring,strlen(item->valuestring));  
		}
		item=cJSON_GetObjectItem(object,"id");  
		if(item!=NULL)  
		{  
			memcpy(data->data.id,item->valuestring,strlen(item->valuestring));  
		}
		item=cJSON_GetObjectItem(object,"auth_info");  
		if(item!=NULL)  
		{
			memcpy(data->data.auth_info,item->valuestring,strlen(item->valuestring));  
		}
		item=cJSON_GetObjectItem(object,"title");  
		if(item!=NULL)  
		{ 
			memcpy(data->data.title,item->valuestring,strlen(item->valuestring));  
		}
		
		item=cJSON_GetObjectItem(object,"private");  
		if(item!=NULL)
		{  
			data->data.priv = cJSON_IsTrue(item) ? 1 : 0;;  
		}
		
		item=cJSON_GetObjectItem(object,"online");  
		if(item!=NULL)
		{ 
			data->data.online = cJSON_IsTrue(item) ? 1 : 0;;  
		}
		
		item=cJSON_GetObjectItem(object,"datastreams");  
		
		cJSON *arrayItem,*item1;
		
		if(item!=NULL)  
		{		
			int size=cJSON_GetArraySize(item);  
			for(U8 i=0;i<size;i++)  
			{
				arrayItem = cJSON_GetArrayItem(item,i);
				item1=cJSON_GetObjectItem(arrayItem,"unit");  
				if(item!=NULL)  
				{
					memcpy(data->data.datastreams[i].unit,item1->valuestring,strlen(item1->valuestring));  
				}
				
				item1=cJSON_GetObjectItem(arrayItem,"id");  
				if(item!=NULL)  
				{
					memcpy(data->data.datastreams[i].id,item1->valuestring,strlen(item1->valuestring));  
				}
				
				item1=cJSON_GetObjectItem(arrayItem,"unit_symbol");  
				if(item!=NULL)  
				{
					memcpy(data->data.datastreams[i].unit_symbol,item1->valuestring,strlen(item1->valuestring));  
				}
				
				item1=cJSON_GetObjectItem(arrayItem,"create_time");  
				if(item!=NULL)  
				{
					memcpy(data->data.datastreams[i].create_time,item1->valuestring,strlen(item1->valuestring));  
				}
			}
		}
	
	}
	cJSON_Delete(root);
	return 0;
}

U8 data_to_Json(U8 *Json)
{
	cJSON *root, *js_body, *js_list,*js_sub_body;
	root = cJSON_CreateObject();
	cJSON_AddItemToObject(root,"datastreams", js_body = cJSON_CreateArray());
	cJSON_AddItemToArray(js_body, js_list = cJSON_CreateObject());
	cJSON_AddStringToObject(js_list,"id","switch");
	cJSON_AddItemToObject(js_list,"datapoints", js_sub_body = cJSON_CreateArray());
	cJSON_AddItemToArray(js_sub_body, js_list = cJSON_CreateObject());
	cJSON_AddNumberToObject(js_list,"value",1);
	char *s = cJSON_PrintUnformatted(root);
	memcpy(Json,s,strlen(s));
	if(root)
		cJSON_Delete(root);
	return 0;
}



