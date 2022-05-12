// Copyright 2022 Maria Sfiraiala (maria.sfiraiala@stud.acs.upb.ro)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./hashtable.h"
#include "./op_utils.h"

int main(void)
{
    char operation[STRING_MAX_LEN], buffer[STRING_MAX_LEN];

    // we create a hashtable for books
    hashtable_t *library = ht_create(HMAX, hash_function_string,
                                    compare_function_strings);
    // we create a hashtable for users
    hashtable_t *users = ht_create(HMAX, hash_function_string,
                                  compare_function_strings);

    do {
        // reading line by line from stdin
        fgets(buffer, STRING_MAX_LEN, stdin);

        // if the line is simply empty => junk
        if (!strcmp(buffer, "\n"))
            strncpy(buffer, "garbage", strlen("garbage"));

        // separating the op keyword from the rest of the input
        get_op(buffer, operation);

        // we execute the now decoded command
        apply_op(operation, buffer, &library, &users);
    } while (strcmp(operation, "EXIT"));


    return 0;
}
