// gcc -Wall -O2 -o rssfeeder main.c -lcurl -lxml2 -I/usr/include/libxml2
//TODOS:
//	xmlFree xmlChar
//  rfc3339 date parser
//  storage for items (redis)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <curl/curl.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

char* buffer;
size_t buffer_length;
int buffer_offset;

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
	
	char *item_title = NULL, *item_date = NULL, *item_link = NULL, *item_description = NULL;
	char *channel_title = NULL, *channel_date = NULL, *channel_link = NULL, *channel_description = NULL;
	time_t temp_ticks;
	
	xmlChar *xmlCharChannel = (xmlChar *)"channel";
	xmlChar *xmlCharTitle = (xmlChar *)"title";
	xmlChar *xmlCharLink = (xmlChar *)"link";
	xmlChar *xmlCharDescription = (xmlChar *)"description";
	xmlChar *xmlCharItem = (xmlChar *)"item";
	xmlChar *xmlCharPubDate = (xmlChar *)"pubDate";
	
	// rss
	for (cur_node = node; cur_node; cur_node = cur_node->next) {
		if (cur_node->type == XML_ELEMENT_NODE) {
			if (xmlStrcmp(cur_node->name, xmlCharChannel) == 0) {
				
				// channel
				for (cur_channel = cur_node->children; cur_channel; cur_channel = cur_channel->next) {
					if (cur_channel->type == XML_ELEMENT_NODE) {
						if (xmlStrcmp(cur_channel->name, xmlCharTitle) == 0) {
							channel_title = (char*)xmlNodeGetContent(cur_channel);
						} else if (xmlStrcmp(cur_channel->name, xmlCharLink) == 0) {							
							channel_link = (char*)xmlNodeGetContent(cur_channel);
						} else if (xmlStrcmp(cur_channel->name, xmlCharDescription) == 0) {
							channel_description = (char*)xmlNodeGetContent(cur_channel);
						} else if (xmlStrcmp(cur_channel->name, xmlCharPubDate) == 0) {
							channel_date = (char*)xmlNodeGetContent(cur_channel);
						} else if (xmlStrcmp(cur_channel->name, xmlCharItem) == 0) {
							
							item_title = item_link = item_description = item_date = NULL;
							
							// item
							for (cur_item = cur_channel->children; cur_item; cur_item = cur_item->next) {
								if (cur_item->type == XML_ELEMENT_NODE) {
									if (xmlStrcmp(cur_item->name, xmlCharTitle) == 0) {
										item_title = (char*)xmlNodeGetContent(cur_item);
									} else if (xmlStrcmp(cur_item->name, xmlCharLink) == 0) {
										item_link = (char*)xmlNodeGetContent(cur_item);
									} else if (xmlStrcmp(cur_item->name, xmlCharDescription) == 0) {
										item_description = (char*)xmlNodeGetContent(cur_item);
									} else if (xmlStrcmp(cur_item->name, xmlCharPubDate) == 0) {
										item_date = (char*)xmlNodeGetContent(cur_item);
									}
								}
							}
							
							// rfc822 to seconds
							temp_ticks = curl_getdate(item_date, NULL);
							
							printf("\ttitle: %s\n" , item_title);
							printf("\tlink: %s\n" , item_link);
							printf("\tpubDate: %d\n" , (int)temp_ticks);
							printf("\tdescription: %s\n" , item_description);
						}
					}
				}
				
				printf("\nChannel info:\n");
							
				if (channel_title) {
					printf("title: %s\n" , channel_title);
				}
				
				if (channel_link) {
					printf("link: %s\n" , channel_link);
				}
				
				if (channel_date) {
					// rfc822 to seconds
					temp_ticks = curl_getdate(channel_date, NULL);
				
					printf("pubDate: %d\n" , (int)temp_ticks);
				}
				
				if (channel_description) {
					printf("description: %s\n" , channel_description);
				}
			}
		}
	}
}

void processAtomContent(xmlNode *node) {
	xmlNode *cur_channel = NULL;
	xmlNode *cur_item = NULL;
	
	char *item_title = NULL, *item_date = NULL, *item_link = NULL;
	char *channel_title = NULL, *channel_date = NULL, *channel_link = NULL;
	
	xmlChar *xmlCharTitle = (xmlChar *)"title";
	xmlChar *xmlCharLink = (xmlChar *)"link";
	xmlChar *xmlCharEntry = (xmlChar *)"entry";
	xmlChar *xmlCharUpdated = (xmlChar *)"updated";
	xmlChar *xmlCharHref = (xmlChar *)"href";
	xmlChar *xmlCharRel = (xmlChar *)"rel";
	xmlChar *xmlCharAlternate = (xmlChar *)"alternate";
	xmlChar *temp;
	
	// feed
	for (cur_channel = node; cur_channel; cur_channel = cur_channel->next) {
		if (cur_channel->type == XML_ELEMENT_NODE) {
			if (xmlStrcmp(cur_channel->name, xmlCharTitle) == 0) {
				channel_title = (char*)xmlNodeGetContent(cur_channel);
			} else if (xmlStrcmp(cur_channel->name, xmlCharLink) == 0) {
				temp = xmlGetProp(cur_channel, xmlCharRel);
				if (temp == NULL || xmlStrcmp(temp, xmlCharAlternate)) {
					channel_link = (char*)xmlGetProp(cur_channel, xmlCharHref);
				}
				xmlFree(temp);
			} else if (xmlStrcmp(cur_channel->name, xmlCharUpdated) == 0) {
				channel_date = (char*)xmlNodeGetContent(cur_channel);
			} else if (xmlStrcmp(cur_channel->name, xmlCharEntry) == 0) {
				
				item_title = item_link = item_date = NULL;
				
				// entry
				for (cur_item = cur_channel->children; cur_item; cur_item = cur_item->next) {
					if (cur_item->type == XML_ELEMENT_NODE) {
						if (xmlStrcmp(cur_item->name, xmlCharTitle) == 0) {
							item_title = (char*)xmlNodeGetContent(cur_item);
						} else if (xmlStrcmp(cur_item->name, xmlCharLink) == 0) {
							temp = xmlGetProp(cur_item, xmlCharRel);
							if (temp == NULL || xmlStrcmp(temp, xmlCharAlternate)) {
								item_link = (char*)xmlGetProp(cur_item, xmlCharHref);
							}
							xmlFree(temp);
						} else if (xmlStrcmp(cur_item->name, xmlCharUpdated) == 0) {
							item_date = (char*)xmlNodeGetContent(cur_item);
						}
					}
				}
			
				printf("\ttitle: %s\n" , item_title);
				printf("\tlink: %s\n" , item_link);
				printf("\tupdated: %s\n" , item_date);
			}
		}
	}
	
	printf("\nChannel info:\n");
				
	if (channel_title) {
		printf("title: %s\n" , channel_title);
	}
	
	if (channel_link) {
		printf("link: %s\n" , channel_link);
	}
	
	if (channel_date) {
		printf("updated: %s\n" , channel_date);
	}
}

int processBufferContent() {
	xmlDoc *doc = NULL;	
	xmlNode *root = NULL;
	xmlChar *xmlCharRss = (xmlChar *)"rss";
	xmlChar *xmlCharAtom = (xmlChar *)"feed";
	
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
	if (downloadContentToBuffer(argv[1]) != 0) {
		printf("Download failed\n");
		return -1;
	}

	// processing content with libxml2
	return processBufferContent();
}
