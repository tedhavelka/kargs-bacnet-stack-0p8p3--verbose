//======================================================================
//
//  FILE:  main.c, stub code to exercise C language shared object file
//
//
//
//  COMPILE STEPS:
//
//    $ gcc main.c -I/usr/local/include/testlib0 -ltestlib0
//
//
//
//======================================================================


#include <stdlib.h>

#include <stdio.h>

#include <string.h>    // provides strncpy(),



// Test libraries, locally authored:

#include <functions-string.h>

#include <diagnostics.h>




#define SIZE__BUFFER (1024)
#define SIZE__TOKEN (256)

#ifndef false
#define false (0)
#endif

#ifndef true 
#define true (1)
#endif

#define STRING_SIZE__3_BYTES (3)
#define STRING_SIZE__64_BYTES (64)
#define STRING_SIZE__147_BYTES (147)
#define STRING_SIZE__288_BYTES (288)





// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  --- SECTION ---
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int combine_two_strings__local(const char* caller, const char* string_1, const char* string_2, char* string_ptr)
{

//    char combined_string[] = malloc(SIZE__BUFFER);
    char* combined_string = malloc(SIZE__BUFFER);

    int success_flag = false;


    if ( combined_string != NULL )
    {
        snprintf(combined_string, SIZE__BUFFER, "%s%s", string_1, string_2);
        success_flag = true;
        printf("combine_two_strings__local():  combined string holds '%s',\n", combined_string);
    }
    else
    {
        printf("- WARNING - in local combine strings function, failed to allocate %d bytes for combined string.\n",
          SIZE__BUFFER);
    }

    string_ptr = combined_string;

    return success_flag;

}




int main(int argc, char** argv)
{


    char* string_ptr = NULL;

    int result = 0;


// diagnostics:

    char lbuf[SIZE__BUFFER];

    unsigned int dflag_announce = DIAGNOSTICS_ON;
    unsigned int dflag_verbose = DIAGNOSTICS_ON;

    DIAG__SET_ROUTINE_NAME("main");



    show_diag(rname, "starting,", dflag_announce);

    show_diag(rname, "- TEST 1 -", dflag_verbose);

//    printf("main():  calling test library routine to combine two strings . . .\n");
    show_diag(rname, "calling test library routine to combine two strings . . .", dflag_verbose);
    string_ptr = combine_two_strings("main", "FIRST_STRING_IS_UPPER_CASE  ", "second_string_is_lower_case_1234");

    snprintf(lbuf, SIZE__BUFFER, "back from test library, string pointer holds '%s',", string_ptr);
    show_diag(rname, lbuf, dflag_verbose);



    snprintf(lbuf, SIZE__BUFFER, "%s:  - TEST 2 -\n", rname);
    printf("%s", lbuf);
    snprintf(lbuf, SIZE__BUFFER, "%s:  callling local function to combine two strings . . .\n", rname);
    printf("%s", lbuf);
    result = combine_two_strings__local(rname, "String one.  ", "String two.  ", string_ptr);

    snprintf(lbuf, SIZE__BUFFER, "%s:  back from local function and string pointer holds '%s',\n", rname, string_ptr);
    printf("%s", lbuf);



    snprintf(lbuf, SIZE__BUFFER, "%s:  - TEST 3 -\n", rname);
    printf("%s", lbuf);

    string_ptr = malloc(STRING_SIZE__64_BYTES);

    memset(string_ptr, 65, STRING_SIZE__64_BYTES);
    string_ptr[(STRING_SIZE__64_BYTES - 1)] = 0;

    if ( string_ptr != NULL )
    {
        result = combine_two_strings_v2(rname, "A B C D 1 2 3 4 - ", "e f g h 5 6 7 8 - ", string_ptr);
        snprintf(lbuf, SIZE__BUFFER, "%s:  back from library function v2, string pointer holds '%s',\n", rname, string_ptr);
        printf("%s", lbuf);

        free(string_ptr);
    }
    else
    {
        snprintf(lbuf, SIZE__BUFFER, "%s:  - WARNING - trouble allocating %d bytes for string test,\n", rname, STRING_SIZE__64_BYTES);
        printf("%s", lbuf);
    }




    printf("main():  done.\n\n");

    return 0;

}
