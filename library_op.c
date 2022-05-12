// Copyright 2022 Maria Sfiraiala (maria.sfiraiala@stud.acs.upb.ro)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./library_op.h"
#include "./utils.h"

book_info_t *create_book(char *book_name)
{
    // creating a new book which also contains a hashtable for definitions
    book_info_t *new_entry = (book_info_t *)malloc(sizeof(*new_entry));
    DIE(!new_entry, "malloc() failed\n");

    new_entry->book = ht_create(HMAX, hash_function_string,
                               compare_function_strings);
    // initializing the new book with standard values
    new_entry->rating = new_entry->purchase = new_entry->is_borrowed = 0;

    // getting the name of the book without the quotation marks
    char new_book_name[BOOK_NAME];
    size_t i;
    for (i = 1; i < strlen(book_name) - 1; ++i)
        new_book_name[i - 1] = book_name[i];
    new_book_name[i - 1] = '\0';

    // copying the name into the new entry
    memcpy(new_entry->book_name, new_book_name, strlen(book_name) + 1);

    return new_entry;
}

void get_name(char *buffer, char *name)
{
    // increment j until we find the first quotation mark
    unsigned int j = 0;
    while (j < strlen(buffer) && buffer[j] != '"')
        ++j;

    // we start to copy the name in the afferent string
    unsigned int i;
    name[0] = '"';
    for (i = 1; i < strlen(buffer) && buffer[++j] != '"'; ++i)
        name[i] = buffer[j];

    name[i++] = '"';
    name[i] = '\0';  // don't forget about the string terminator
}

hashtable_t *add_book(char *buffer, hashtable_t *library)
{
    char book_name[BOOK_NAME + 1];
    int def_number;

    // get the name from the buffer and afterwards, we get rid off of the
    // name from the buffer, we don't need it there anymore
    get_name(buffer, book_name);
    buffer += strlen(book_name);
    // we now read the number of definitions
    sscanf(buffer, "%d", &def_number);

    // we create a separate book which we'll be inserted in the hashmap we
    // created for the library
    book_info_t *new_entry = create_book(book_name);

    for (int i = 0; i < def_number; ++i) {
        // reading line by line; from this line we'll get the 2 words
        // that make up a definition
        char line[STRING_MAX_LEN];
        fgets(line, STRING_MAX_LEN, stdin);

        char def_key[DEF_LEN + 1], def_val[DEF_LEN + 1];
        sscanf(line, "%s %s", def_key, def_val);

        ht_put(new_entry->book, def_key, strlen(def_key) + 1, def_val,
              strlen(def_val) + 1);
        if (new_entry->book->size > new_entry->book->hmax)
            new_entry->book = ht_resize(new_entry->book, ht_free);
    }

    // if there's already a book with the same name in the library, we replace
    // it
    if (ht_has_key(library, book_name)) {
        ll_node_t *book = ht_remove_entry(library, book_name);
        free(((info *)book->data)->key);

        ht_free((hashtable_t **)(((info *)book->data)->value));

	    free(((info *)book->data)->value);
        free(book->data);
        free(book);
    }

    // inserting the new book into the library
    ht_put(library, book_name, strlen(book_name) + 1, new_entry,
          sizeof(*new_entry));

    // if the hashtable needs resizing, we do so
    if (library->size > library->hmax)
        library = ht_resize(library, ht_free);
    free(new_entry);

    return library;
}

void get_book(char *buffer, hashtable_t *library)
{
    char book_name[BOOK_NAME];

    get_name(buffer, book_name);
    buffer += strlen(book_name);

    if (!ht_get(library, book_name)) {
        printf("The book is not in the library.\n");
        return;
    }

    // getting the book for which we'll print information
    book_info_t *book = (book_info_t *)ht_get(library, book_name);

    // calculating the temporary rating
    double tmp = 0;
    if (book->purchase)
        tmp = (double)book->rating / book->purchase;

    printf("Name:%s Rating:%.3f Purchases:%d\n", book->book_name, tmp,
          book->purchase);
}

void rmv_book(char *buffer, hashtable_t *library)
{
    char book_name[BOOK_NAME];

    get_name(buffer, book_name);
    buffer += strlen(book_name);

    if (!ht_has_key(library, book_name)) {
        printf("The book is not in the library.\n");
        return;
    }

    // removing the entry with the given book name
    ll_node_t *book = ht_remove_entry(library, book_name);
    free(((info *)book->data)->key);

    ht_free((hashtable_t **)(((info *)book->data)->value));

	free(((info *)book->data)->value);
    free(book->data);
    free(book);
}

hashtable_t *add_def(char *buffer, hashtable_t *library)
{
    char book_name[BOOK_NAME], def_key[DEF_LEN], def_val[DEF_LEN];

    get_name(buffer, book_name);
    buffer += strlen(book_name);
    sscanf(buffer, "%s %s", def_key, def_val);

    if (!ht_has_key(library, book_name)) {
        printf("The book is not in the library.\n");
        return library;
    }

    // getting the book which will be updated
    book_info_t *book = (book_info_t *)ht_get(library, book_name);

    ht_put(book->book, def_key, strlen(def_key) + 1, def_val,
          strlen(def_val) + 1);

    // after inserting the new defintion into the book, we check whether we
    // need to resize
    if (book->book->size > book->book->hmax)
        book->book = ht_resize(book->book, ht_free);
    return library;
}

void get_def(char *buffer, hashtable_t *library)
{
    char book_name[BOOK_NAME], def_key[DEF_LEN];

    // parsing the name of the book and the definition key
    get_name(buffer, book_name);
    buffer += strlen(book_name);
    sscanf(buffer, "%s", def_key);

    if (!ht_has_key(library, book_name)) {
        printf("The book is not in the library.\n");
        return;
    }

    book_info_t *book = (book_info_t *)ht_get(library, book_name);

    if (!ht_has_key(book->book, def_key)) {
        printf("The definition is not in the book.\n");
        return;
    }

    // printing the defintion after checking whether the book and the
    // definition key exist
    char *def = ht_get(book->book, def_key);
    printf("%s\n", def);
}

void rmv_def(char *buffer, hashtable_t *library)
{
    char book_name[BOOK_NAME], def_key[DEF_LEN];

    get_name(buffer, book_name);
    buffer += strlen(book_name);
    sscanf(buffer, "%s", def_key);

    if (!ht_has_key(library, book_name)) {
        printf("The book is not in the library.\n");
        return;
    }

    book_info_t *book = (book_info_t *)ht_get(library, book_name);

    if (!ht_has_key(book->book, def_key)) {
        printf("The definition is not in the book.\n");
        return;
    }

    // remove the defintion and free its space
    ll_node_t *tmp = ht_remove_entry(book->book, def_key);
    free(((info *)tmp->data)->key);

	free(((info *)tmp->data)->value);
	free(tmp->data);
	free(tmp);
}
