// Copyright 2022 Maria Sfiraiala (maria.sfiraiala@stud.acs.upb.ro)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "./utils.h"
#include "./hashtable.h"

linked_list_t *ll_create(unsigned int data_size)
{
    linked_list_t* ll;

    ll = malloc(sizeof(*ll));
    DIE(ll == NULL, "linked_list malloc");

    ll->head = NULL;
    ll->data_size = data_size;
    ll->size = 0;

    return ll;
}

void ll_add_nth_node(linked_list_t* list, unsigned int n, const void* new_data)
{
    ll_node_t *prev, *curr;
    ll_node_t* new_node;

    if (!list) {
        return;
    }

    /* n >= list->size inseamna adaugarea unui nou nod la finalul listei. */
    if (n > list->size) {
        n = list->size;
    }

    curr = list->head;
    prev = NULL;
    while (n > 0) {
        prev = curr;
        curr = curr->next;
        --n;
    }

    new_node = malloc(sizeof(*new_node));
    DIE(new_node == NULL, "new_node malloc");
    new_node->data = malloc(list->data_size);
    DIE(new_node->data == NULL, "new_node->data malloc");
    memcpy(new_node->data, new_data, list->data_size);

    new_node->next = curr;
    if (prev == NULL) {
        /* Adica n == 0. */
        list->head = new_node;
    } else {
        prev->next = new_node;
    }

    list->size++;
}

ll_node_t *ll_remove_nth_node(linked_list_t* list, unsigned int n)
{
    ll_node_t *prev, *curr;

    if (!list || !list->head) {
        return NULL;
    }

    /* n >= list->size - 1 inseamna eliminarea nodului de la finalul listei. */
    if (n > list->size - 1) {
        n = list->size - 1;
    }

    curr = list->head;
    prev = NULL;
    while (n > 0) {
        prev = curr;
        curr = curr->next;
        --n;
    }

    if (prev == NULL) {
        /* Adica n == 0. */
        list->head = curr->next;
    } else {
        prev->next = curr->next;
    }

    list->size--;

    return curr;
}

unsigned int ll_get_size(linked_list_t* list)
{
     if (!list) {
        return -1;
    }

    return list->size;
}

void ll_free(linked_list_t** pp_list)
{
    ll_node_t* currNode;

    if (!pp_list || !*pp_list) {
        return;
    }

    while (ll_get_size(*pp_list) > 0) {
        currNode = ll_remove_nth_node(*pp_list, 0);
        free(currNode->data);
        currNode->data = NULL;
        free(currNode);
        currNode = NULL;
    }

    free(*pp_list);
    *pp_list = NULL;
}

int compare_function_strings(void *a, void *b)
{
	char *str_a = (char *)a;
	char *str_b = (char *)b;

	return strcmp(str_a, str_b);
}

unsigned int hash_function_string(void *a)
{
	/*
	 * Credits: http://www.cse.yorku.ca/~oz/hash.html
	 */
	unsigned char *puchar_a = (unsigned char*) a;
	int64_t hash = 5381;
	int c;

	while ((c = *puchar_a++))
		hash = ((hash << 5u) + hash) + c; /* hash * 33 + c */

	return hash;
}

hashtable_t * ht_create(unsigned int hmax, unsigned int (*hash_function)(void*),
		int (*compare_function)(void*, void*))
{
	hashtable_t *table = (hashtable_t *)malloc(sizeof(*table));
	DIE(!table, "malloc() faieled\n");

	linked_list_t **buckets = (linked_list_t **)malloc(hmax * sizeof(*buckets));
	DIE(!buckets, "malloc failed\n");

	for (unsigned int i = 0; i < hmax; ++i)
		buckets[i] = ll_create(sizeof(info));

	table->buckets = buckets;
	table->size = 0;
	table->hmax = hmax;
	table->hash_function = hash_function;
	table->compare_function = compare_function;

	return table;
}

ll_node_t *check_index(linked_list_t *bucket, void *key,
					  int (*compare_function)(void *, void *))
{
	ll_node_t *node = bucket->head;
	for (unsigned int i = 0; i < bucket->size; ++i) {
		if (!compare_function(key, ((info *)node->data)->key))
			return node;
		node = node->next;
	}
	return NULL;
}

int ht_has_key(hashtable_t *ht, void *key)
{
	int index = ht->hash_function(key) % ht->hmax;
	if (check_index(ht->buckets[index], key, ht->compare_function))
		return 1;
	return 0;
}

void *ht_get(hashtable_t *ht, void *key)
{
	int index = ht->hash_function(key) % ht->hmax;
	ll_node_t *node = check_index(ht->buckets[index], key, ht->compare_function);

	if (node)
		return ((info *)node->data)->value;
	return NULL;
}

hashtable_t *ht_resize(hashtable_t *ht, void (*free_function)(hashtable_t **))
{
	hashtable_t *tmp = ht_create(2 * ht->hmax, ht->hash_function,
								ht->compare_function);

	for (unsigned int i = 0; i < ht->hmax; ++i) {
		ll_node_t *node = ht->buckets[i]->head;

		while(node) {
			info *entry = (info *)node->data;
			ht_put(tmp, entry->key, entry->key_size, entry->value,
				  entry->value_size);

			node = node->next;
		}
	}

	free_function(&ht);

	return tmp;
}

void ht_put(hashtable_t *ht, void *key, unsigned int key_size, void *value,
		   unsigned int value_size)
{
	info new_entry;
	int index = ht->hash_function(key) % ht->hmax;

	ll_node_t *node = check_index(ht->buckets[index], key, ht->compare_function);

	if (!node) {
		new_entry.key = malloc(key_size);
		DIE(!new_entry.key, "malloc failed\n");

		new_entry.value = malloc(value_size);
		DIE(!new_entry.value, "malloc() failed\n");

		new_entry.key_size = key_size;
		memcpy(new_entry.key, key, key_size);

		new_entry.value_size = value_size;
		memcpy(new_entry.value, value, value_size);

		ll_add_nth_node(ht->buckets[index], 0, &new_entry);
		++(ht->size);
	} else {
		free(((info *)node->data)->value);
		((info *)node->data)->value = malloc(value_size);
		DIE(!((info *)node->data)->value, "malloc() failed\n");

		((info *)node->data)->value_size = value_size;
		memcpy(((info *)node->data)->value, value, value_size);
	}
}

ll_node_t * ht_remove_entry(hashtable_t *ht, void *key)
{
	int index = ht->hash_function(key) % ht->hmax;

	ll_node_t *tmp = ht->buckets[index]->head;
	int pos = -1;
	for (unsigned int i = 0; i < ht->buckets[index]->size && pos == -1; ++i) {
		if (!ht->compare_function(key, ((info *)tmp->data)->key))
			pos = i;
		tmp = tmp->next;
	}
	tmp = ll_remove_nth_node(ht->buckets[index], pos);

	return tmp;
}

void ht_free(hashtable_t **ht)
{
    for (unsigned int i = 0; i < (*ht)->hmax; ++i) {
		while ((*ht)->buckets[i]->size) {
			ll_node_t *node = ll_remove_nth_node((*ht)->buckets[i], 0);

			free(((info *)node->data)->key);
			free(((info *)node->data)->value);
			free(node->data);
			free(node);
		}
		free((*ht)->buckets[i]);
	}
	free((*ht)->buckets);
	free(*ht);
}

void free_ht_in_ht(hashtable_t **library)
{
    for (unsigned int i = 0; i < (*library)->hmax; ++i) {
        while ((*library)->buckets[i]->size) {
            ll_node_t *book = ll_remove_nth_node((*library)->buckets[i], 0);

			free(((info *)book->data)->key);

            ht_free((hashtable_t **)(((info *)book->data)->value));

			free(((info *)book->data)->value);
            free(book->data);
            free(book);
        }
        free((*library)->buckets[i]);
    }
    free((*library)->buckets);
    free((*library));
}
