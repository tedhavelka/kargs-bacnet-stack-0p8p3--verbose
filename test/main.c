


#include <stdlib.h>
#include <stdio.h>

#include <functions-string.h>



#define SIZE__BUFFER (1024)




int main(int argc, char** argv)
{


    char* string_ptr = NULL;

    char lbuf[SIZE__BUFFER];


    printf("main():  starting,\n");

    printf("main():  calling test library routine to combine two strings . . .\n");
    string_ptr = combine_two_strings("main", "FIRST_STRING_IS_UPPER_CASE  ", "second_string_is_lower_case_1234");

    snprintf(lbuf, SIZE__BUFFER, "main():  back from test library, string pointer holds '%s',\n", string_ptr);
    printf("%s", lbuf);


    printf("main():  done.\n\n");

    return 0;

}
