// Copyright 2022 Maria Sfiraiala (maria.sfiraiala@stud.acs.upb.ro)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./hashtable.h"
#include "./library_op.h"
#include "./users_op.h"
#include "./utils.h"
#include "./op_utils.h"

void get_op(char *buffer, char *op)
{
    char tmp[STRING_MAX_LEN], *p;

    sscanf(buffer, "%s", op);

    p = buffer + strlen(op) + 1;
    strncpy(tmp, p, strlen(p) + 1);
    strncpy(buffer, tmp, strlen(tmp) + 1);
}

void calculate_rating(hashtable_t *library)
{
    for (unsigned int i = 0; i < library->hmax; ++i) {
        // iterating through every bucket
        ll_node_t *node = library->buckets[i]->head;

        // for each node, we get the value and we calculate the rating
        while (node) {
            book_info_t *book = ((info *)node->data)->value;

            if (book->purchase)
                book->rating = (double)book->rating / book->purchase;

            node = node->next;
        }
    }
}

int cmp_library(const void *a, const void *b)
{
    sorted_book_t *book1 = (sorted_book_t *)a, *book2 = (sorted_book_t *)b;

    if (book1->rating < book2->rating) {
            return 1;
    } else if (book1->rating == book2->rating){
            if (book1->purchase < book2->purchase) {
                return 1;
            } else if (book1->purchase == book2->purchase) {
                if (strcmp(book1->book_name, book2->book_name) > 0)
                    return 1;
                return 0;
            }
            return 0;
    }
    return 0;
}

void top_books(hashtable_t *library)
{
    // we create an array of structs, containing the info we want to sort and
    // print
    sorted_book_t *v = malloc(library->size * sizeof(*v));
    DIE(!v, "malloc() failed\n");

    calculate_rating(library);

    int j = 0;
    for (unsigned int i = 0; i < library->hmax; ++i) {
        ll_node_t *node = library->buckets[i]->head;

        while (node) {
            book_info_t *book = ((info *)node->data)->value;

            // we insert the book info into the array of structs
            memcpy(v[j].book_name, book->book_name, strlen(book->book_name)
                  + 1);
            v[j].rating = book->rating;
            v[j].purchase = book->purchase;

            ++j;

            node = node->next;
        }
    }

    // sorting the books and then printing the top books
    qsort(v, j, sizeof(*v), cmp_library);

    printf("Books ranking:\n");
    for (int i = 0; i < j; ++i)
        printf("%d. Name:%s Rating:%.3f Purchases:%d\n", i + 1, v[i].book_name,
              v[i].rating, v[i].purchase);

    free(v);
}

int cmp_users(const void *a, const void *b)
{
    user_info_t *user1 = (user_info_t *)a, *user2 = (user_info_t *)b;

    if (user1->points < user2->points) {
        return 1;
    } else if (user1->points == user2->points) {
        if (strcmp(user1->user_name, user2->user_name) > 0)
            return 1;
        return 0;
    }
    return 0;
}

void top_users(hashtable_t *users)
{
    // we create an array of structs, containing the info we want to sort and
    // print
    user_info_t *v = malloc(users->size * sizeof(*v));
    DIE(!v, "malloc() failed\n");

    int j = 0;
    for (unsigned int i = 0; i < users->hmax; ++i) {
        ll_node_t *node = users->buckets[i]->head;

        while (node) {
            user_info_t *user = ((info *)node->data)->value;

            // inserting the users info into the soon to be sorted array
            memcpy(v[j].user_name, user->user_name, strlen(user->user_name)
                  + 1);
            v[j].points = user->points;

            ++j;

            node = node->next;
        }
    }

    // sorting the array and print the result
    qsort(v, j, sizeof(*v), cmp_users);

    printf("Users ranking:\n");
    for (int i = 0; i < j; ++i)
        if (v[i].points >= 0)
            printf("%d. Name:%s Points:%d\n", i + 1, v[i].user_name,
                  v[i].points);

    free(v);
}

void exit_op(hashtable_t **library, hashtable_t **users)
{
    top_books(*library);
    top_users(*users);

    free_ht_in_ht(library);

    ht_free(users);
}

void apply_op(char *operation, char *buffer, hashtable_t **library,
             hashtable_t **users)
{
    if (!strcmp(operation, "ADD_BOOK"))
        *library = add_book(buffer, *library);

    if (!strcmp(operation, "GET_BOOK"))
        get_book(buffer, *library);

    if (!strcmp(operation, "RMV_BOOK"))
        rmv_book(buffer, *library);

    if (!strcmp(operation, "ADD_DEF"))
        *library = add_def(buffer, *library);

    if (!strcmp(operation, "GET_DEF"))
        get_def(buffer, *library);

    if (!strcmp(operation, "RMV_DEF"))
        rmv_def(buffer, *library);

    if (!strcmp(operation, "ADD_USER"))
        *users = add_user(buffer, *users);

    if (!strcmp(operation, "BORROW"))
        borrow(buffer, *library, *users);

    if (!strcmp(operation, "RETURN"))
        return_book(buffer, *library, *users);

    if (!strcmp(operation, "LOST"))
        lost(buffer, *library, *users);

    if (!strcmp(operation, "EXIT"))
        exit_op(library, users);
}
