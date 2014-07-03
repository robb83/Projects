// gcc -Wall -O2 -o rssfeeder main.c -lcurl -lxml2 -lhiredis -I/usr/include/libxml2
//TODOS:
//	refactors
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <curl/curl.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <hiredis/hiredis.h>

#define ISDIGIT(v) (v >= '0' && v <= '9')
#define XMLFREE(v) \
if (v) { \
	xmlFree(v); \
	v = NULL; \
}

char* buffer;
size_t buffer_length;
int buffer_offset;
redisContext *redis;
redisReply *reply;
char feedID[64];
char* feedurl;

int redisStringInteger(char* storage, size_t storage_size, redisReply *r) {
	if (r == NULL || r->type == REDIS_REPLY_NIL)
		return -1;
	
	if (r->type == REDIS_REPLY_INTEGER) {
		snprintf(storage, storage_size, "%lld", reply->integer);
		
		return 0;
	} else if (r->type == REDIS_REPLY_STRING) {
		strncpy(storage, r->str, storage_size);
		
		return 0;
	}
	
	return -1;
}

int storeChannel(char* title, char* link) {
	if (!title || !link)
		return -1;
		
	printf("Create Channel\n\ttitle: %s\n\tlink: %s\n\tfeed: %s\n", title, link, feedurl);
	
	// generate id
	reply = redisCommand(redis, "INCR id:feed");
	if (reply == NULL || (reply->type != REDIS_REPLY_INTEGER && reply->type != REDIS_REPLY_STRING)) {
		freeReplyObject(reply);
		return -1;
	}
	
	redisStringInteger(feedID, sizeof(feedID), reply);
	freeReplyObject(reply);

	// create hash
	reply = redisCommand(redis, "HMSET feed:%s feedid %s feed %s title %s link %s", 
			feedID, feedID, feedurl, title, link);
	freeReplyObject(reply);

	// create lookup
	reply = redisCommand(redis, "ZADD feedlookup %s %s", feedID, feedurl);
	freeReplyObject(reply);

	return 0;
}

int storeItem(char *id, char* title, char* link, time_t updated) {
	char storage1[64], storage2[64];

	if (!title || !link)
		return -1;
	
	if (!id) id = link;
	
	// check exists
	reply = redisCommand(redis,"ZSCORE itemlookup %s:%s", feedID, id);
	if (reply == NULL || reply->type != REDIS_REPLY_NIL) {
		freeReplyObject(reply);
		return -1;
	}
	freeReplyObject(reply);
	
	printf("Item: %s\n", title);
	
	// generate id
	reply = redisCommand(redis,"INCR id:item");
	if (reply == NULL || (reply->type != REDIS_REPLY_INTEGER && reply->type != REDIS_REPLY_STRING)) {
		freeReplyObject(reply);
		return -1;
	}
	redisStringInteger(storage1, sizeof(storage1), reply);
	freeReplyObject(reply);

	snprintf(storage2, sizeof(storage2), "%ld", updated);
	
	// create hash
	reply = redisCommand(redis,"HMSET item:%s itemid %s id %s link %s title %s updated %s", storage1, storage1, id, link, title, storage2);
	freeReplyObject(reply);
	
	// create lookup
	reply = redisCommand(redis,"ZADD feed:%s:items %s %s", feedID, storage2, storage1);
	freeReplyObject(reply);	
	
	reply = redisCommand(redis,"ZADD itemlookup %s %s:%s", storage1, feedID, id);
	freeReplyObject(reply);

	return 0;
}

time_t simple_rfc3339(char* string) {
	int dy, dm, dd;
	int th, tm, ts;
	int oh, om, osign;
	char current;
	
	if (!string)
		return (time_t)0;
	
	// date
	if (sscanf(string, "%04d-%02d-%02d", &dy, &dm, &dd) == 3) {
		string += 10;
		
		if (*string++ != 'T')
			return (time_t)0;
		
		// time
		if (sscanf(string, "%02d:%02d:%02d", &th, &tm, &ts) == 3) {
			string += 8;
			
			current = *string;
			
			// optional: second fraction
			if (current == '.') {
				++string;
				while(ISDIGIT(*string))
					++string;
					
				current = *string;
			}
			
			if (current == 'Z') {
				oh = om = 0;
				osign = 1;
			} else if (current == '-') {
				++string;
				if (sscanf(string, "%02d:%02d", &oh, &om) != 2)
					return (time_t)0;
				osign = -1;
			} else if (current == '+') {
				++string; 
				if (sscanf(string, "%02d:%02d", &oh, &om) != 2)
					return (time_t)0;
				osign = 1;
			} else {
				return (time_t)0;
			}
			
			struct tm timeinfo;
			timeinfo.tm_wday = timeinfo.tm_yday = 0;
			timeinfo.tm_zone = NULL;
			timeinfo.tm_isdst = -1;
			
			timeinfo.tm_year = dy - 1900;
			timeinfo.tm_mon = dm - 1;
			timeinfo.tm_mday = dd;
			
			timeinfo.tm_hour = th;
			timeinfo.tm_min = tm;
			timeinfo.tm_sec = ts;
			
			// convert to utc
			return timegm(&timeinfo) - (((oh * 60 * 60) + (om * 60)) * osign);
		}
	}
	
	return (time_t)0;
}

size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
	size_t realsize = size * nmemb;
	
	if (buffer_offset + realsize > buffer_length) {
		int amount = 16 * 1024;
		if (amount < realsize) {
			amount = realsize;
		}
		
		buffer = realloc(buffer, buffer_length + amount);
		
		if (buffer == NULL) {
			return 0; // Not enough memory
		}
		
		buffer_length = buffer_length + amount;
	}
	
	memcpy(buffer + buffer_offset, ptr, realsize);
	buffer_offset += realsize;
	
	return realsize;
}

int downloadContentToBuffer(char* url) {
	CURL *curl;
	CURLcode res;
	
	if (!url) {
		return -1;
	}
	
	if (curl_global_init(CURL_GLOBAL_DEFAULT) != 0) {
		printf("Error while curl_global_init.\n");
		return -1;
	}
	
	curl = curl_easy_init();
	if (!curl) {
		printf("Error while init curl.\n");
		curl_global_cleanup();
		return -1;
	}
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 3L);
	curl_easy_setopt(curl,CURLOPT_URL, url);
	
	res = curl_easy_perform(curl);
	if (res != CURLE_OK) {
		printf("Error: %s\n", curl_easy_strerror(res));
		curl_easy_cleanup(curl);
		curl_global_cleanup();
		
		return -1;
	}
	
	curl_easy_cleanup(curl);
	curl_global_cleanup();
	
	return 0;
}

void processRssConent(xmlNode *node) {
	xmlNode *cur_node = NULL;
	xmlNode *cur_channel = NULL;
	xmlNode *cur_item = NULL;
	
	char *item_title = NULL, *item_date = NULL, *item_link = NULL, *item_id = NULL;
	char *channel_title = NULL, *channel_link = NULL;
	time_t temp_ticks;
	
	xmlChar xmlCharChannel[] = "channel";
	xmlChar xmlCharTitle[] = "title";
	xmlChar xmlCharLink[] = "link";
	// xmlChar xmlCharDescription[] = "description";
	xmlChar xmlCharItem[] = "item";
	xmlChar xmlCharPubDate[] = "pubDate";
	xmlChar xmlCharGuid[] = "guid";
	
	// rss
	for (cur_node = node; cur_node; cur_node = cur_node->next) {
		if (cur_node->type == XML_ELEMENT_NODE) {
			if (xmlStrcmp(cur_node->name, xmlCharChannel) == 0) {
				
				// channel
				for (cur_channel = cur_node->children; cur_channel; cur_channel = cur_channel->next) {
					if (cur_channel->type == XML_ELEMENT_NODE) {
						if (xmlStrcmp(cur_channel->name, xmlCharTitle) == 0) {
							
							XMLFREE(channel_title)
							channel_title = (char*)xmlNodeGetContent(cur_channel);
							
						} else if (xmlStrcmp(cur_channel->name, xmlCharLink) == 0) {							
							
							XMLFREE(channel_link)
							channel_link = (char*)xmlNodeGetContent(cur_channel);
							
						} else if (xmlStrcmp(cur_channel->name, xmlCharItem) == 0) {
																		
							if (feedID[0] == '\0') {
								if (storeChannel(channel_title, channel_link) == -1) {
									break;
								}
							}
							
							// item
							for (cur_item = cur_channel->children; cur_item; cur_item = cur_item->next) {
								if (cur_item->type == XML_ELEMENT_NODE) {
									if (xmlStrcmp(cur_item->name, xmlCharTitle) == 0) {
										
										XMLFREE(item_title)
										item_title = (char*)xmlNodeGetContent(cur_item);
									
									} else if (xmlStrcmp(cur_item->name, xmlCharLink) == 0) {
										
										XMLFREE(item_link)
										item_link = (char*)xmlNodeGetContent(cur_item);
										
									} else if (xmlStrcmp(cur_item->name, xmlCharGuid) == 0) {
										
										XMLFREE(item_id)
										item_id = (char*)xmlNodeGetContent(cur_item);
										
									} else if (xmlStrcmp(cur_item->name, xmlCharPubDate) == 0) {
										
										XMLFREE(item_date)
										item_date = (char*)xmlNodeGetContent(cur_item);
										
									}
								}
							}
							
							// rfc822 to seconds
							temp_ticks = curl_getdate(item_date, NULL);
							
							storeItem(item_id, item_title, item_link, temp_ticks);
							
							XMLFREE(item_title)
							XMLFREE(item_link)
							XMLFREE(item_id)
							XMLFREE(item_date)
						}
					}
				}
				
				XMLFREE(channel_title)
				XMLFREE(channel_link)
			}
		}
	}
}

void processAtomContent(xmlNode *node) {
	xmlNode *cur_channel = NULL;
	xmlNode *cur_item = NULL;
	
	char *item_title = NULL, *item_date = NULL, *item_link = NULL, *item_id = NULL;
	char *channel_title = NULL, *channel_link = NULL;
	
	xmlChar xmlCharTitle[] = "title";
	xmlChar xmlCharLink[] = "link";
	xmlChar xmlCharEntry[] = "entry";
	xmlChar xmlCharUpdated[] = "updated";
	xmlChar xmlCharHref[] = "href";
	xmlChar xmlCharRel[] = "rel";
	xmlChar xmlCharAlternate[] = "alternate";
	xmlChar xmlCharId[] = "id";
	xmlChar *temp;
	time_t temp_ticks;
	
	// feed
	for (cur_channel = node; cur_channel; cur_channel = cur_channel->next) {
		if (cur_channel->type == XML_ELEMENT_NODE) {
			if (xmlStrcmp(cur_channel->name, xmlCharTitle) == 0) {
				
				XMLFREE(channel_title)
				channel_title = (char*)xmlNodeGetContent(cur_channel);
				
			} else if (xmlStrcmp(cur_channel->name, xmlCharLink) == 0) {
				
				
				temp = xmlGetProp(cur_channel, xmlCharRel);
				if (temp == NULL || xmlStrcmp(temp, xmlCharAlternate)) {
					XMLFREE(channel_link)
					channel_link = (char*)xmlGetProp(cur_channel, xmlCharHref);
				}
				XMLFREE(temp)
		
			} else if (xmlStrcmp(cur_channel->name, xmlCharEntry) == 0) {
				
				if (feedID[0] == '\0') {
					if (storeChannel(channel_title, channel_link) == -1) {
						break;
					}
				}
				
				// entry
				for (cur_item = cur_channel->children; cur_item; cur_item = cur_item->next) {
					if (cur_item->type == XML_ELEMENT_NODE) {
						if (xmlStrcmp(cur_item->name, xmlCharTitle) == 0) {
							
							XMLFREE(item_title)
							item_title = (char*)xmlNodeGetContent(cur_item);
							
						} else if (xmlStrcmp(cur_item->name, xmlCharLink) == 0) {
							
							temp = xmlGetProp(cur_item, xmlCharRel);
							if (temp == NULL || xmlStrcmp(temp, xmlCharAlternate)) {
								XMLFREE(item_link)
								item_link = (char*)xmlGetProp(cur_item, xmlCharHref);
							}
							XMLFREE(temp)
							
						} else if (xmlStrcmp(cur_item->name, xmlCharUpdated) == 0) {
							
							XMLFREE(item_date)
							item_date = (char*)xmlNodeGetContent(cur_item);
							
						} else if (xmlStrcmp(cur_item->name, xmlCharId) == 0) {
							
							XMLFREE(item_id)
							item_id = (char*)xmlNodeGetContent(cur_item);
							
						}
					}
				}
			
				temp_ticks = simple_rfc3339(item_date);
				
				storeItem(item_id, item_title, item_link, temp_ticks);
				
				XMLFREE(item_title)
				XMLFREE(item_link)
				XMLFREE(item_date)
				XMLFREE(item_id)
			}
		}
	}
	
	XMLFREE(channel_title)
	XMLFREE(channel_link)
}

int processBufferContent() {
	xmlDoc *doc = NULL;	
	xmlNode *root = NULL;
	xmlChar xmlCharRss[] = "rss";
	xmlChar xmlCharAtom[] = "feed";
	
	doc = xmlReadMemory(buffer, buffer_offset, NULL, NULL, 0);
	if (doc == NULL) {
		xmlCleanupParser();
		return -1;
	}
	
	root = xmlDocGetRootElement(doc);
	if (root) {
		if (xmlStrcmp(root->name, xmlCharRss) == 0) {
			processRssConent(root->children);
		} else if (xmlStrcmp(root->name, xmlCharAtom) == 0) {
			processAtomContent(root->children);
		}
	}
	
	xmlFreeDoc(doc);
	xmlCleanupParser();
	
	return 0;
}

int main(int argc, char** argv) {

	// check arguments
	if (argc < 2) {
		printf("Usage: httpurl\n");
		return -1;
	}
	
	feedurl = argv[1];

	// initialize content buffer
	buffer_offset = 0;
	buffer_length = sizeof(char) * 16 * 1024;
	buffer = (char*)malloc(buffer_length);
	
	// check memory
	if (buffer == NULL) {
		printf("Not enought memory\n");
		return -1;
	}
	
	// download content with curl
	if (downloadContentToBuffer(feedurl) != 0) {
		printf("Download failed\n");
		return -1;
	}

	struct timeval timeout = { 1, 500000 };
	redis = redisConnectWithTimeout("localhost", 6379, timeout);
	if (redis == NULL || redis->err) {
		printf("Redis connection failed\n");	
		return -1;
	}
	
	// check exists
	reply = redisCommand(redis,"ZSCORE feedlookup %s", feedurl);
	if (reply == NULL || (reply->type != REDIS_REPLY_NIL && reply->type != REDIS_REPLY_INTEGER && reply->type != REDIS_REPLY_STRING)) {
		printf("Redis unknow error\n");
		freeReplyObject(reply);
		redisFree(redis);
		
		return -1;
	} else if (reply->type != REDIS_REPLY_NIL) {
		redisStringInteger(feedID, sizeof(feedID), reply);
	} else {
		feedID[0] = '\0';
	}
	
	freeReplyObject(reply);

	// processing content with libxml2
	int response = processBufferContent();
	
	redisFree(redis);
	
	return response;
}
