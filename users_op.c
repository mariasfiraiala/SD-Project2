// Copyright 2022 Maria Sfiraiala (maria.sfiraiala@stud.acs.upb.ro)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./hashtable.h"
#include "./library_op.h"
#include "./users_op.h"

hashtable_t *add_user(char *buffer, hashtable_t *users)
{
    char user_name[DEF_LEN];
    sscanf(buffer, "%s", user_name);

    // checking whether the user is already registered
    if (ht_has_key(users, user_name)) {
        printf("User is already registered.\n");
        return users;
    }

    // create a new user instance and initialize the info about it
    user_info_t *new_entry = (user_info_t *)malloc(sizeof(*new_entry));
    new_entry->points = 100;
    new_entry->is_banned = 0;
    strncpy(new_entry->book_name, "\n", 1);
    memcpy(new_entry->user_name, user_name, strlen(user_name) + 1);
    new_entry->days_available = -1;

    // insert the newly created user
    ht_put(users, user_name, strlen(user_name) + 1, new_entry,
          sizeof(*new_entry));

    // check if we need to resize the hashtable of users
    if (users->size > users->hmax)
        users = ht_resize(users, ht_free);
    free(new_entry);

    return users;
}

void borrow(char *buffer, hashtable_t *library, hashtable_t *users)
{
    char user_name[DEF_LEN], book_name[BOOK_NAME];
    int days_available;

    sscanf(buffer, "%s ", user_name);
    get_name(buffer, book_name);
    buffer += strlen(user_name) + strlen(book_name) + 2;
    sscanf(buffer, "%d", &days_available);

    if (!ht_has_key(users, user_name)) {
        printf("You are not registered yet.\n");
        return;
    }

    user_info_t *user = (user_info_t *)ht_get(users, user_name);

    if (user->is_banned) {
        printf("You are banned from this library.\n");
        return;
    }

    book_info_t *book = (book_info_t *)ht_get(library, book_name);

    if (user->days_available != -1) {
        printf("You have already borrowed a book.\n");
        return;
    }
    if (!ht_get(library, book_name)) {
        printf("The book is not in the library.\n");
        return;
    }
    if (book->is_borrowed) {
        printf("The book is borrowed.\n");
        return;
    }

    // if the borrowing process is valid, we mark the book as borrowed
    // and we update the user info
    book->is_borrowed = 1;
    user->days_available = days_available;
    memcpy(user->book_name, book_name, strlen(book_name) + 1);
}

void return_book(char *buffer, hashtable_t *library, hashtable_t *users)
{
    char user_name[DEF_LEN], book_name[BOOK_NAME];
    int days_since_borrow, rating;

    sscanf(buffer, "%s ", user_name);
    get_name(buffer, book_name);
    buffer += strlen(user_name) + strlen(book_name) + 2;
    sscanf(buffer, "%d %d", &days_since_borrow, &rating);

    user_info_t *user = (user_info_t *)ht_get(users, user_name);

    if (!user)
        return;

    if (user->is_banned) {
        printf("You are banned from this library.\n");
        return;
    }

    if (strcmp(user->book_name, book_name) || !strcmp(user->book_name, "\n")) {
        printf("You didn't borrow this book.\n");
        return;
    }

    // substracting points if the user brings back the book late
    // otherwise adding points
    if (days_since_borrow > user->days_available)
        user->points -= 2 * (days_since_borrow - user->days_available);
    else
        user->points += user->days_available - days_since_borrow;

    if (user->points < 0) {
        user->is_banned = 1;
        printf("The user %s has been banned from this library.\n", user_name);
    }

    book_info_t *book = (book_info_t *)ht_get(library, book_name);

    // set the book back to being not borrowed
    if (book) {
        ++(book->purchase);
        book->rating += rating;
        book->is_borrowed = 0;
        user->days_available = -1;
    }
}

void lost(char *buffer, hashtable_t *library, hashtable_t *users)
{
    char user_name[DEF_LEN], book_name[BOOK_NAME];

    sscanf(buffer, "%s", user_name);

    get_name(buffer, book_name);
    buffer += strlen(book_name);

    if (!ht_has_key(users, user_name)) {
        printf("You are not registered yet.\n");
        return;
    }

    user_info_t *user = (user_info_t *)ht_get(users, user_name);

    if (user->is_banned) {
        printf("You are banned from this library.\n");
        return;
    }

    // when the book is lost, we delete it from the library
    ll_node_t *book = ht_remove_entry(library, book_name);
    free(((info *)book->data)->key);

    ht_free((hashtable_t **)(((info *)book->data)->value));

	free(((info *)book->data)->value);
    free(book->data);
    free(book);

    // penalty for the user: deduct 50 points
    user->points -= 50;
    user->days_available = -1;

    if (user->points < 0) {
        user->is_banned = 1;
        printf("The user %s has been banned from this library.\n", user_name);
    }
}
