// Copyright 2022 Maria Sfiraiala (maria.sfiraiala@stud.acs.upb.ro)

#ifndef OP_UTILS_H__
#define OP_UTILS_H__

#include "hashtable.h"

typedef struct sorted_book_t sorted_book_t;
struct sorted_book_t {
	double rating;
    int purchase;
    char book_name[BOOK_NAME];
} __attribute__((__packed__));

/*
* @brief -> divides the input in 2, the operation keyword and everything else
* @param -> buffer = the line read from stdin, it needs to be interpreted
* @param -> op = stores the operation keyword
* @return -> none, the op will be modified inside the function
*/
void get_op(char *buffer, char *op);

/*
* @brief -> calculates the rating for each book, right before the sort
* @param -> library = the hashtable of books
* @return -> none, we modify the hashtable directly
*/
void calculate_rating(hashtable_t *library);

/*
* @brief -> compare function used for qsort() on the library
* @param -> a = the first value compared
* @param -> b = the second value compared
* return -> 1 for sort this, 0 for it's sorted
*/
int cmp_library(const void *a, const void *b);

/*
* @brief -> driver function for sorting and printing the books
* @param -> library = the hashtable of books
* @return -> none
*/
void top_books(hashtable_t *library);

/*
* @brief -> compare function used for qsort() on the users database
* @param -> a = the first value compared
* @param -> b = the second value compared
* return -> 1 for sort this, 0 for it's sorted
*/
int cmp_users(const void *a, const void *b);

/*
* @brief -> driver function for sorting and printing the users
* @param -> users = the hashtable of users
* @return -> none
*/
void top_users(hashtable_t *users);

/*
* @brief -> exits programme, not before deallocating memory
* @param -> library = the books database
* @param -> users = the users database
* @return -> none, we simply free dynamically allocated memory
*/
void exit_op(hashtable_t **library, hashtable_t **users);

/*
* @brief -> applies operations, the base function of the project
* @param -> operation = the keyword of the command
* @param -> buffer = the rest of the line, it contains vital parameters to the
*           operation
* @param -> library = the hashtable in which we store all the information
*           related to books
* @param -> users = the hashtable in which we store all the information
*           related to users
* @return -> none
*/
void apply_op(char *operation, char *buffer, hashtable_t **library,
             hashtable_t **users);

#endif  // OP_UTILS_H__
