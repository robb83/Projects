#include <stdio.h>
#include <string.h>
#include "list_ops.h"

list_t *new_list(size_t length, list_element_t elements[])
{
    list_t *result = (list_t*)malloc(sizeof(list_t) + sizeof(list_element_t) * length);
    if (result != NULL)
    {
        result->length = length;
        memcpy(result->elements, elements, length * sizeof(list_element_t));
    }

    return result;
}

list_t *append_list(list_t *list1, list_t *list2)
{
    if (list1 == NULL && list2 == NULL)
    {
        return new_list(0, NULL);
    }

    if (list1 == NULL)
    {
        list1 = list2;
        list2 = NULL;
    }

    size_t total_size = sizeof(list_t) + (sizeof(list_element_t) * list1->length);
    size_t total_count = list1->length;

    if (list2 != NULL)
    {
        total_size += sizeof(list_element_t) * list2->length;
        total_count += list2->length;
    }

    list_t *result = (list_t*)malloc(total_size);
    if (result != NULL)
    {
        result->length = total_count;
        memcpy(result->elements, list1->elements, list1->length * sizeof(list_element_t));

        if (list2 != NULL)
        {
            memcpy(result->elements + list1->length, list2->elements, list2->length * sizeof(list_element_t));
        }
    }

    return result;
}

void delete_list(list_t *list)
{
    if (list == NULL) return;
    free(list);
}

list_t *reverse_list(list_t *list)
{
    if (list == NULL)
    {
        return new_list(0, NULL);
    }

    list_t *result = (list_t*)malloc(sizeof(list_t) + sizeof(list_element_t) * list->length);
    if (result != NULL)
    {
        result->length = list->length;
        for (size_t i = 0, j = list->length - 1; i < list->length; ++i, --j)
        {
            result->elements[j] = list->elements[i];
        }
    }

    return result;
}

size_t length_list(list_t *list)
{
    if (list == NULL) return 0;
    return list->length;
}

list_t *map_list(list_t *list, list_element_t (*map)(list_element_t))
{
    if (list == NULL)
    {
        return new_list(0, NULL);
    }

    list_t *result = (list_t*)malloc(sizeof(list_t) + (sizeof(list_element_t) * list->length));
    if (result != NULL)
    {
        result->length = list->length;
        for (size_t i = 0; i < list->length; ++i)
        {
            result->elements[i] = map(list->elements[i]);
        }
    }

    return result;
}

list_t *filter_list(list_t *list, bool (*filter)(list_element_t))
{
    if (list == NULL)
    {
        return new_list(0, NULL);
    }

    list_t *result = (list_t*)malloc(sizeof(list_t) + sizeof(list_element_t) * 0);
    if (result != NULL)
    {
        result->length = 0;

        for (size_t i = 0; i < list->length; ++i)
        {
            if (filter(list->elements[i]))
            {
                result = (list_t*)realloc(result, sizeof(list_t) + sizeof(list_element_t) * (result->length + 1));
                if (result == NULL) { return NULL; }
                result->elements[result->length] = list->elements[i];
                ++result->length;
            }
        }
    }

    return result;
}

list_element_t foldl_list(list_t *list, list_element_t initial, list_element_t (*foldl)(list_element_t, list_element_t))
{
    if (list == NULL)
    {
        return initial;
    }

    for (size_t i = 0; i < list->length; ++i)
    {
        initial = foldl(initial, list->elements[i]);
    }

    return initial;
}

list_element_t foldr_list(list_t *list, list_element_t initial, list_element_t (*foldr)(list_element_t, list_element_t))
{
    if (list == NULL)
    {
        return initial;
    }

    for (size_t i = list->length; i > 0; --i)
    {
        initial = foldr(list->elements[i - 1], initial);
    }

    return initial;
}