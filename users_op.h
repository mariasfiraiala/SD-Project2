// Copyright 2022 Maria Sfiraiala (maria.sfiraiala@stud.acs.upb.ro)

#ifndef USERS_OP_H__
#define USERS_OP_H__

/*
* @brief -> adds user to the users data base
* @param -> buffer = the rest of the line, it contains vital parameters to the
*           operation
* @param -> users = the hashtable containing info about users (the data base)
* @return -> the now updated users data base
*/
hashtable_t *add_user(char *buffer, hashtable_t *users);

/*
* @brief -> performs a borrow operation, the user gets the book, which now
*           becomes unavailable to other users
* @param -> buffer = the rest of the line, it contains vital parameters to the
*           operation
* @param -> library = books data_base, will be updated
* @param -> users = users data_base, will be updated
* @return -> none
*/
void borrow(char *buffer, hashtable_t *library, hashtable_t *users);

/*
* @brief -> performs a return operation, the user returns the book, which now
*           becomes available to other users
* @param -> buffer = the rest of the line, it contains vital parameters to the
*           operation
* @param -> library = books data_base, will be updated
* @param -> users = users data_base, will be updated
* @return -> none
*/
void return_book(char *buffer, hashtable_t *library, hashtable_t *users);

/*
* @brief -> performs a lost operation, the user has lost the book, which now
*           becomes forever unavailable to all users, by being deleted
* @param -> buffer = the rest of the line, it contains vital parameters to the
*           operation
* @param -> library = books data_base, will be updated
* @param -> users = users data_base, will be updated
* @return -> none
*/
void lost(char *buffer, hashtable_t *library, hashtable_t *users);

#endif  // USERS_OP_H__
