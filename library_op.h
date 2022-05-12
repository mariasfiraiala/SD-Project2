// Copyright 2022 Maria Sfiraiala (maria.sfiraiala@stud.acs.upb.ro)

#ifndef LIBRARY_OP_H__
#define LIBRARY_OP_H__

#include "hashtable.h"

/*
* @brief -> gets a name from a buffer, stored between ""
* @param -> buffer = the strig from which we get the name
* @param -> name = string which stores the name
* @return -> none
*/
void get_name(char *buffer, char *name);

/*
* @brief -> creates a book by allocating space for it and initializing fields
* @param -> book_name = the name of the book, used for initialization
* @return -> the newly created book 
*/
book_info_t *create_book(char *book_name);

/*
* @brief -> adds book to library
* @param -> buffer = the rest of the line, it contains vital parameters to the
*           operation
* @param -> library = hashtable that stores books
* @return -> the now modified library
*/
hashtable_t *add_book(char *buffer, hashtable_t *library);

/*
* @brief -> gets book from library, printing its information
* @param -> buffer = the rest of the line, it contains vital parameters to the
*           operation
* @param -> library = hashtable that stores books
* @return -> none
*/
void get_book(char *buffer, hashtable_t *library);

/*
* @brief -> removes book from library
* @param -> buffer = the rest of the line, it contains vital parameters to the
*           operation
* @param -> library = hashtable that stores books
* @return -> none
*/
void rmv_book(char *buffer, hashtable_t *library);

/*
* @brief -> adds definition to a book
* @param -> buffer = the rest of the line, it contains vital parameters to the
*           operation
* @param -> library = hashtable that stores books
* @return -> the now modified library
*/
hashtable_t *add_def(char *buffer, hashtable_t *library);

/*
* @brief -> gets definition from a book, printing it
* @param -> buffer = the rest of the line, it contains vital parameters to the
*           operation
* @param -> library = hashtable that stores books
* @return -> none
*/
void get_def(char *buffer, hashtable_t *library);

/*
* @brief -> removes definition from a book
* @param -> buffer = the rest of the line, it contains vital parameters to the
*           operation
* @param -> library = hashtable that stores books
* @return -> none
*/
void rmv_def(char *buffer, hashtable_t *library);

#endif  // LIBRARY_OP_H__
