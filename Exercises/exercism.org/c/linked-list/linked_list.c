#include "linked_list.h"
#include <stdlib.h>

struct list_node {
   struct list_node *prev, *next;
   ll_data_t data;
};

struct list {
   struct list_node *first, *last;
};

struct list *list_create(void)
{
    struct list* result = (struct list*)malloc(sizeof(struct list));
    result->first = result->last = NULL;
    return result;
}

void list_destroy(struct list *list)
{
    if (list)
    {
        struct list_node* current = list->first;
        struct list_node* next;

        while (current != NULL)
        {
            next = current->next;
            free(current);
            current = next;
        }

        free(list);
    }
}

size_t list_count(const struct list *list)
{
    size_t result = 0;
    
    if (list)
    {
        struct list_node* current = list->first;
        while (current != NULL)
        {
            ++result;
            current = current->next;
        }
    }

    return result;
}

void list_push(struct list *list, ll_data_t item_data)
{
    if (list)
    {
        struct list_node* node = (struct list_node*)malloc(sizeof(struct list_node));
        node->prev = list->last;
        node->next = NULL;
        node->data = item_data;

        if (list->last)
        {
            list->last->next = node;
            list->last = node;
        } 
        else 
        {
            list->first = node;
            list->last = node;
        }
    }
}

ll_data_t list_pop(struct list *list)
{
    if (list && list->last)
    {
        struct list_node* node = list->last;
        ll_data_t result = node->data;

        list->last = node->prev;
        if (list->last)
        {
            list->last->next = NULL;
        } 
        else
        {
            list->first = NULL;
            list->last = NULL;
        }
        
        free(node);
        return result;
    }

    return -1;
}

void list_unshift(struct list *list, ll_data_t item_data)
{
    if (list)
    {
        struct list_node* node = (struct list_node*)malloc(sizeof(struct list_node));
        node->prev = NULL;
        node->next = list->first;
        node->data = item_data;

        if (list->first)
        {
            list->first->prev = node;
            list->first = node;
        } 
        else 
        {
            list->first = node;
            list->last = node;
        }
    }
}

ll_data_t list_shift(struct list *list)
{
    if (list && list->first)
    {
        struct list_node* node = list->first;
        ll_data_t result = node->data;

        list->first = node->next;
        if (list->first)
        {
            list->first->prev = NULL;
        }
        else
        {
            list->first = NULL;
            list->last = NULL;
        }
        
        free(node);
        return result;
    }

    return -1;
}

void list_delete(struct list *list, ll_data_t data)
{
    if (list)
    {
        struct list_node* current = list->first;
        
        while(current != NULL)
        {
            if (current->data == data)
            {
                if (current->prev)
                {
                    current->prev->next = current->next;
                }
                else
                {
                    list->first = current->next;
                }

                if (current->next)
                {
                    current->next->prev = current->prev;
                }
                else 
                {
                    list->last = current->prev;
                }

                free(current);
                return;
            }
            else
            {
                current = current->next;
            }
        }
    }
}