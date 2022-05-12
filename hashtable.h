// Copyright 2022 Maria Sfiraiala (maria.sfiraiala@stud.acs.upb.ro)

#ifndef HASHTABLE_H__
#define HASHTABLE_H__

#define STRING_MAX_LEN 256
#define BOOK_NAME 40
#define HMAX 10
#define DEF_LEN 20

typedef struct ll_node_t
{
    void* data;
    struct ll_node_t* next;
} __attribute__((__packed__)) ll_node_t;

typedef struct linked_list_t
{
    ll_node_t* head;
    unsigned int data_size;
    unsigned int size;
} __attribute__((__packed__)) linked_list_t;

typedef struct info info;
struct info {
	void *key;
	void *value;
	unsigned int key_size;
	unsigned int value_size;
} __attribute__((__packed__));

typedef struct hashtable_t hashtable_t;
struct hashtable_t {
	linked_list_t **buckets;
	unsigned int size;
	unsigned int hmax;
	unsigned int (*hash_function)(void*);
	int (*compare_function)(void*, void*);
} __attribute__((__packed__));

typedef struct book_info_t book_info_t;
struct book_info_t {
	hashtable_t *book;
	char book_name[BOOK_NAME];
	double rating;
	int purchase;
	int is_borrowed;
} __attribute__((__packed__));

typedef struct user_info_t user_info_t;
struct user_info_t {
	int points;
	int is_banned;
	char book_name[BOOK_NAME];
	char user_name[DEF_LEN];
	int days_available;
} __attribute__((__packed__));

/*
* @brief -> creates a simply linked list
* @param -> data_size = the size of the values that are going to be stored
* @return -> returns the newly created list
*/
linked_list_t *ll_create(unsigned int data_size);

/*
* @brief -> inserts a new node in a simply linked list
* @param -> list = the list that needs to be updated
* @param -> n = the index at which the new node will be inserted
* @param -> data = the value for the entry
* @return -> none, we directly modify the list
*/
void ll_add_nth_node(linked_list_t* list, unsigned int n,
					const void* new_data);

/*
* @brief -> removes the nth node
* @param -> list = the list from which we delete nodes
* @param -> n = the index of the node that we want to be removed
* @return -> returns the now deleted node
*/
ll_node_t *ll_remove_nth_node(linked_list_t* list, unsigned int n);

/*
* @brief -> gets the size of a simply linked list
* @param -> list = the list for which we determine the size
* @return -> the size of the list
*/
unsigned int ll_get_size(linked_list_t* list);

/*
* @brief -> frees a whole list and its dinamically alocated values
* @param -> pp_list = the list that it's going to be freed
* @return -> none, the list is being persistently modified as it is sent as a
*            double pointer
*/
void ll_free(linked_list_t** pp_list);

/*
* @brief -> compares string values for the hashmap
* @param -> a = the first value
* @param -> b = the second value
* @return -> strcmp(a , b)
*/
int compare_function_strings(void *a, void *b);

/*
* @brief -> hashes the key
* @param -> a = the key we want to hash
* @return -> the value of the hashing algorithm
*/
unsigned int hash_function_string(void *a);

/*
* @brief -> creates a generic hashtable
* @param -> hmax = the maximum number of buckets
* @param -> hash_function = the function used for hashing
* @param -> compare_function = the function used for comparing values
* @return -> the newly created list
*/
hashtable_t *ht_create(unsigned int hmax, unsigned int (*hash_function)(void*),
					  int (*compare_function)(void*, void*));

/*
* @brief -> checks whether a bucket contains a certain key
* @param -> bucket = the bucket in which we searh for the key
* @param -> key = the key of we search for
* @param -> compare_function = used for detemring whether 2 keys are equal
* @brief -> returns the node that conatins the key we are searching for
*/
ll_node_t *check_index(linked_list_t *bucket, void *key,
					  int (*compare_function)(void *, void *));

/*
* @brief -> checking whether the hashtable contains the key at all
* @param -> ht = the hashtable in which we perform the search
* @param -> key = the key of the element we want
* @return -> 1 for true, 0 for false
*/
int ht_has_key(hashtable_t *ht, void *key);

/*
* @brief -> returns the value associated with a certain key
* @param -> ht = the hashtable in which we perform the search
* @param -> key = the key of the element for which we want the value
* @return -> pointer to data
*/
void *ht_get(hashtable_t *ht, void *key);

/*
* @brief -> doubles the initial size of the hashtable when needed
* @param -> ht = the hashtable we'll resize
* @param -> free_funtion = frees the previous, now obsolete hashtable
* @return -> the new hashtable
*/
hashtable_t *ht_resize(hashtable_t *ht, void (*free_function)(hashtable_t **));

/*
* @brief -> inserts a new value into the hashmap
* @param -> ht = the hashtable that gets the new value
* @param -> key = the key for the fresh entry
* @param -> key_size = size of the key
* @param -> value = the value we'll insert
* @return -> none, we'll modify the hashtable directly 
*/
void ht_put(hashtable_t *ht, void *key, unsigned int key_size,
	void *value, unsigned int value_size);

/*
* @brief -> removes an entry from the hashtable
* @param -> the hashtable on which we perform the deletion
* @param -> key = the identifier for the entry we'll remove
* @return -> the now removed entry
*/
ll_node_t *ht_remove_entry(hashtable_t *ht, void *key);

/*
* @brief -> frees a hashtable
* @param -> ht = the hashtable we'll free
* @return -> none, we'll simply free the data
*/
void ht_free(hashtable_t **ht);

/*
* @brief -> frees a hashtable full with other hashtables
* @param -> the compelx hashtable we'll free
* @return -> none, we'll simply free the data
*/
void free_ht_in_ht(hashtable_t **library);

#endif  // HASHTABLE_H__
