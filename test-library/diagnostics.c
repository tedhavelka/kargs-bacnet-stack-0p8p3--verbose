//
//
//
//
//





#include "diagnostics.h"




void show_diag(const char* caller, const char *message, unsigned int format)
{
//----------------------------------------------------------------------
//  PURPOSE:  to provide a simple and general purpose diagnostics
//   displaying routine with a few simple formatting options.
//----------------------------------------------------------------------


    char lbuf[SIZE__DIAG_MESSAGE];

    DIAG__SET_ROUTINE_NAME("show_diag");



    if ( format == DIAGNOSTICS_OFF )
    {   
        return;
    }   


// STEP - merge and format calling code name, and message, into one string:

    switch ( format )
    {   
        case DIAGNOSTICS__SHOW_MESSAGE_ONLY:
        {
            snprintf(lbuf, SIZE__DIAG_MESSAGE, "%s", message);
            break;
        }

        case DIAGNOSTICS__FORMAT_MESSAGE_WITH_COLON_SEPARATOR:
        {
            snprintf(lbuf, SIZE__DIAG_MESSAGE, "%s:  %s\n", caller, message);
            break;
        }

        case DIAGNOSTICS__FORMAT_MESSAGE_WITH_DASH_SEPARATOR:
        default:
        {
            snprintf(lbuf, SIZE__DIAG_MESSAGE, "%s - %s\n", caller, message);
            break;
        }

    } // end switch on parameter 'format'


    {
        printf("%s", lbuf);
    }




}





// --- EOF ---
