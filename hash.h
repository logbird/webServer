#ifndef INC_HASH
#define INC_HASH 1

typedef struct _hashItem{
    char *key;
    char *value;
    struct _hashItem *next;
    struct _hashItem *prev;
}hashItem;

typedef struct _hashStr{
    unsigned int length;
    hashItem *first;
    hashItem *end;
}hashStr;

hashStr *create_hash();
char *get_hash_by_key(hashStr *hs, char *key);
int set_hash_by_key(hashStr *hs, char *key, char *value);
hashItem *find_hash_by_key(hashStr *hs, char *key);
int remove_hash_by_key(hashStr *hs, char *key);
int free_hash(hashStr *hs);
int free_item(hashItem *hs);

#endif


