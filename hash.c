#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash.h"
#include "util.h"

hashStr *create_hash()
{
    hashStr *root;
    root = malloc(sizeof(hashStr));
    if(root == NULL)
    {
        return NULL;
    }
    memset(root, 0, sizeof(hashStr));
    root->length = 0;
    root->first = NULL;
    root->end = NULL;
    return root;
}
char *get_hash_by_key(hashStr *hs, char *key)
{
    hashItem *item;
    item = find_hash_by_key(hs, key);
    if(item == NULL)
    {
        return NULL;
    }else
    {
        return item->value;
    }
}

int set_hash_by_key(hashStr *hs, char *key, char *value)
{
    hashItem *item, *newitem;
    if(hs == NULL)
    {
        return 0;
    }
    item = find_hash_by_key(hs, key);
    if(item == NULL)
    {
        newitem = malloc(sizeof(hashItem));
        if(newitem == NULL)
        {
            return 0;
        }
        memset(newitem, 0, sizeof(hashItem));
        newitem->key = key;
        newitem->value = value;
        newitem->next = NULL;
        newitem->prev = hs->end;
        if(hs->length == 0)
        {
            hs->first = newitem;
        }else
        {
            hs->end->next = newitem;
        }
        hs->end = newitem;
        hs->length++;
    }else
    {
        item->value = value;
    }
    return 1;
}

hashItem *find_hash_by_key(hashStr *hs, char *key)
{
    hashItem *item;
    if(hs == NULL || hs->length == 0)
    {
        return NULL;
    }
    item = hs->first;
    while(item != NULL)
    {
        if(strcmp(key, item->key) == 0)
        {
            return item;
        }
        item = item->next;
    }
    return NULL;
}

int remove_hash_by_key(hashStr *hs, char *key)
{
    hashItem *item;
    item = find_hash_by_key(hs, key);
    if(item->prev != NULL)
    {
        item->prev->next = item->next;
    }else
    {
        hs->first = item->next;
    }
    if(item->next != NULL)
    {
        item->next->prev = item->prev;
    }else
    {
        hs->end = item->prev;
    }
    hs->length--;
    return free_item(item);
}

int free_hash(hashStr *hs)
{
    hashItem *item, *cur;
    if(hs == NULL || hs->length == 0)
    {
        return 0;
    }
    item = hs->first;
    while(item != NULL)
    {
        cur = item;
        item = item->next;
        free(cur);
    }
    free(hs);
    return 1;
}

int free_item(hashItem *item)
{
    if(item == NULL)
    {
        return 0;
    }
    free(item);
    return 1;
}

