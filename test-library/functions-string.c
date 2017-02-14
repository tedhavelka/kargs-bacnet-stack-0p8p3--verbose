//----------------------------------------------------------------------
//
//  PROJECT:  Test library writting in C
//
//
//  FILE:  functions-string.c
//
//
//  STARTED:  2017-02-10 FRI
//
//
//  PURPOSE:  To learn and understand how to compile object files,
//    relocatable C object files, shared objects, and kernel type 
//    object files as part of larger C projects and mixed language
//    projects.
//
//    Inspired by difficulties getting Steve Kargs' BACnet stack
//    sources to correctly usefully link to a C based program which
//    would implement and act as a BACnet master node on an MSTP
//    RS-485 based network.  - TMH
//
//
//
//  COMPILE STEPS:
//
//    $ gcc -o libtestlib0.o -c functions-string.c
//
//    $ gcc -shared -o libtestlib0.su libtestlib0.o
//
//
//
//
//----------------------------------------------------------------------



#include <stdlib.h>    // provides malloc(),
#include <stdio.h>     // provides printf(),
#include <string.h>    // provides strlen(),


#include "functions-string.h"



#define LOCAL_SIZE__DIAG_MESSAGE (1024)
#define LOCAL_SIZE__TOKEN (256)

#define LOCAL_SIZE__MEMORY_TO_ALLOCATE_FOR_COMBINED_STRINGS (1024)




// char** combine_two_strings(const char* caller, const char* string_1, const char* string_2)
char* combine_two_strings(const char* caller, const char* string_1, const char* string_2)
{


//    int length_string_1 = 0;
//    int length_string_2 = 0;
    int bytes_to_request = LOCAL_SIZE__MEMORY_TO_ALLOCATE_FOR_COMBINED_STRINGS;

    char* string_ptr = NULL;

// diagnostics:
    char lbuf[LOCAL_SIZE__DIAG_MESSAGE];




    printf("starting,\n");

    snprintf(lbuf, LOCAL_SIZE__DIAG_MESSAGE, "called by '%s',\n", caller);
    printf("%s", lbuf);

    snprintf(lbuf, LOCAL_SIZE__DIAG_MESSAGE, "with string 1 holding '%s',\n", string_1);
    printf("%s", lbuf);

    snprintf(lbuf, LOCAL_SIZE__DIAG_MESSAGE, "and string 2 holding '%s',\n", string_2);
    printf("%s", lbuf);



//    length_string_1 = (sizeof)string_1; 
//    length_string_2 = (sizeof)string_2; 

//    bytes_to_request = ( length_string_1 + length_string_2 + 1 );

    string_ptr = malloc(bytes_to_request);

    if ( string_ptr == NULL )
    {
        printf("-- WARNING -- unable to allocate %d bytes for combined string!\n", bytes_to_request);
        printf("-- WARNING -- returning null string pointer to caller . . .\n");
    }
    else
    {
        printf("-- NOTE -- allocated %d bytes to hold combined strings from calling code,\n", bytes_to_request);
        printf("-- NOTE -- it is up to to caller to free this memory when program no longer needs it.\n");
    }

    printf("returning to caller . . .\n");

    return string_ptr;

}





// 2017-02-14 TUE - Ted adding routine:

int combine_two_strings_v2(const char* caller, const char* string_1, const char* string_2, char* pointer_to_caller_memory)
{
    int number_of_bytes_caller_sends;

// diagnostics:
    char lbuf[LOCAL_SIZE__DIAG_MESSAGE];

    char rname[LOCAL_SIZE__TOKEN];
    snprintf(rname, LOCAL_SIZE__TOKEN, "%s", "combine_two_strings_v2");


    printf("starting,\n");

    snprintf(lbuf, LOCAL_SIZE__DIAG_MESSAGE, "called by '%s',\n", caller);
    printf("%s:  %s", rname, lbuf);
    snprintf(lbuf, LOCAL_SIZE__DIAG_MESSAGE, "with string 1 holding '%s',\n", string_1);
    printf("%s:  %s", rname, lbuf);
    snprintf(lbuf, LOCAL_SIZE__DIAG_MESSAGE, "and string 2 holding '%s',\n", string_2);
    printf("%s:  %s", rname, lbuf);

    number_of_bytes_caller_sends = strlen(pointer_to_caller_memory);
    snprintf(lbuf, LOCAL_SIZE__DIAG_MESSAGE, "last argument is pointer to block of %d bytes of calling-code-allocated memory,\n",
      number_of_bytes_caller_sends);
    printf("%s:  %s", rname, lbuf);

    printf("%s:  %s", rname, "combining these strings in caller's memory . . .");
    snprintf(pointer_to_caller_memory, number_of_bytes_caller_sends, "%s%s", string_1, string_2);

    printf("%s:  done for now, returning . . .\n", rname);
}




// --- EOF ---
