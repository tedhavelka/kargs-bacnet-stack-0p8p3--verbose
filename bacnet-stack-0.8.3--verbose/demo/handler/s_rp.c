/*********************************************************************

*
* Copyright (C) 2005 Steve Karg <skarg@users.sourceforge.net>
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
*********************************************************************/

#include <stddef.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include "config.h"
#include "txbuf.h"
#include "bacdef.h"
#include "bacdcode.h"
#include "address.h"
#include "tsm.h"
#include "npdu.h"
#include "apdu.h"
#include "device.h"
#include "datalink.h"
#include "dcc.h"
#include "rp.h"
/* some demo stuff needed */
#include "handlers.h"
#include "txbuf.h"
#include "client.h"



// 2017-04-13 - added by Ted:
#include <diagnostics.h>




/** @file s_rp.c  Send Read Property request. */

/** Sends a Read Property request
 * @ingroup DSRP
 *
 * @param dest [in] BACNET_ADDRESS of the destination device
 * @param max_apdu [in]
 * @param object_type [in]  Type of the object whose property is to be read.
 * @param object_instance [in] Instance # of the object to be read.
 * @param object_property [in] Property to be read, but not ALL, REQUIRED, or OPTIONAL.
 * @param array_index [in] Optional: if the Property is an array,
 *   - 0 for the array size
 *   - 1 to n for individual array members
 *   - BACNET_ARRAY_ALL (~0) for the full array to be read.
 * @return invoke id of outgoing message, or 0 if device is not bound or no tsm available
 */
uint8_t Send_Read_Property_Request_Address(
    BACNET_ADDRESS * dest,
    uint16_t max_apdu,
    BACNET_OBJECT_TYPE object_type,
    uint32_t object_instance,
    BACNET_PROPERTY_ID object_property,
    uint32_t array_index)
{
    BACNET_ADDRESS my_address;
    uint8_t invoke_id = 0;
    int len = 0;
    int pdu_len = 0;
    int bytes_sent = 0;
    BACNET_READ_PROPERTY_DATA data;
    BACNET_NPDU_DATA npdu_data;


// diagnostics:
    char lbuf[SIZE__DIAG_MESSAGE];
    memset(lbuf, 0, SIZE__DIAG_MESSAGE);

    char rname_plus[SIZE__ROUTINE_NAME];
    memset(rname_plus, 0, SIZE__ROUTINE_NAME);

//    unsigned int dflag_verbose = DIAGNOSTICS_ON;
    unsigned int dflag_announce = DIAGNOSTICS__SEND_READ_PROPERTY_REQUEST_ADDRESS;
//    unsigned int dflag_verbose = DIAGNOSTICS__SEND_READ_PROPERTY_REQUEST_ADDRESS;
    unsigned int dflag_verbose = DIAGNOSTICS_ON;

    DIAG__SET_ROUTINE_NAME("Send_Read_Property_Request_Address");



    show_diag(rname, "starting,", dflag_announce);

    if (!dcc_communication_enabled())
    {
        show_diag(rname, "dcc commmunication not enabled, returning zero to caller . . .", dflag_verbose);
        return 0;
    }

    if (!dest)
    {
        show_diag(rname, "BACnet address at *dest appears null, returning zero to caller . . .", dflag_verbose);
        return 0;
    }

    /* is there a tsm available? */
    invoke_id = tsm_next_free_invokeID();
    snprintf(lbuf, SIZE__DIAG_MESSAGE, "routine tsm_next_free_invokeID() return invoke ID of value %d,", invoke_id);
    show_diag(rname, lbuf, dflag_verbose);

    if (invoke_id)
    {
        /* encode the NPDU portion of the packet */
        datalink_get_my_address(&my_address);
        npdu_encode_npdu_data(&npdu_data, true, MESSAGE_PRIORITY_NORMAL);
        pdu_len =
            npdu_encode_pdu(&Handler_Transmit_Buffer[0], dest, &my_address,
            &npdu_data);

// 2017-05-17 - Ted attempting trace of Handler Transmit Buffer contents:
        snprintf(lbuf, SIZE__ROUTINE_NAME, "routine npdu_encode_pdu() returns encoded byte count of %d,", pdu_len);
        show_diag(rname, lbuf, dflag_verbose);

        snprintf(rname_plus, SIZE__ROUTINE_NAME, "%s - %s", rname, "after call to npdu_encode_pdu()");
        show_byte_array(rname_plus, (char*)Handler_Transmit_Buffer, 48, BYTE_ARRAY__DISPLAY_FORMAT__16_PER_LINE__GROUPS_OF_FOUR);


        /* encode the APDU portion of the packet */
        data.object_type = object_type;
        data.object_instance = object_instance;
        data.object_property = object_property;
        data.array_index = array_index;

        len = rp_encode_apdu(&Handler_Transmit_Buffer[pdu_len], invoke_id, &data);

// 2017-05-17 - Ted attempting trace of Handler Transmit Buffer contents:
        snprintf(lbuf, SIZE__ROUTINE_NAME, "routine rp_encode_apdu() returns encoded byte count of %d,", len);
        show_diag(rname, lbuf, dflag_verbose);

        snprintf(rname_plus, SIZE__ROUTINE_NAME, "%s - %s", rname, "after call to rp_encode_apdu()");
        show_byte_array(rname_plus, (char*)Handler_Transmit_Buffer, 48, BYTE_ARRAY__DISPLAY_FORMAT__16_PER_LINE__GROUPS_OF_FOUR);

        pdu_len += len;


        /* will it fit in the sender?
           note: if there is a bottleneck router in between
           us and the destination, we won't know unless
           we have a way to check for that and update the
           max_apdu in the address binding table. */
        if ((uint16_t) pdu_len < max_apdu)
        {
            tsm_set_confirmed_unsegmented_transaction(invoke_id, dest,
                &npdu_data, &Handler_Transmit_Buffer[0], (uint16_t) pdu_len);

// // 2017-05-15 - added by Ted:
//            show_diag(rname, "calling wrapper routine to Ringbuf_Count() . . .", dflag_verbose);
//            wrapper_to_ringbuf_count(rname);

            bytes_sent = datalink_send_pdu(dest, &npdu_data, &Handler_Transmit_Buffer[0], pdu_len);
// 2017-05-17 - Ted attempting trace of Handler Transmit Buffer contents:
            snprintf(rname_plus, SIZE__ROUTINE_NAME, "%s - %s", rname, "after call to dlmstp_send_pdu()");
            show_byte_array(rname_plus, (char*)Handler_Transmit_Buffer, 47, BYTE_ARRAY__DISPLAY_FORMAT__16_PER_LINE__GROUPS_OF_FOUR);



#if PRINT_ENABLED
            if (bytes_sent <= 0)
            {
                fprintf(stderr, "Failed to Send ReadProperty Request (%s)!\n", strerror(errno));
            }
#endif

        }
        else
        {
            tsm_free_invoke_id(invoke_id);
            invoke_id = 0;
#if PRINT_ENABLED
            fprintf(stderr, "Failed to Send ReadProperty Request " "(exceeds destination maximum APDU)!\n");
#endif

        }

    } // end IF-block testing whether invoke_id is true 

// 2017-05-08 - ELSE block added by Ted:
    else
    {
        show_diag(rname, "received an invoke ID of zero,", dflag_verbose);
        show_diag(rname, "taking this to mean no Transaction State Machine (tsm) available,", dflag_verbose);
    }


    snprintf(lbuf, SIZE__DIAG_MESSAGE, "returning invoke ID value of %d to caller . . .", invoke_id);
    show_diag(rname, lbuf, dflag_announce);

    return invoke_id;
}




/** Sends a Read Property request.
 * @ingroup DSRP
 *
 * @param device_id [in] ID of the destination device
 * @param object_type [in]  Type of the object whose property is to be read.
 * @param object_instance [in] Instance # of the object to be read.
 * @param object_property [in] Property to be read, but not ALL, REQUIRED, or OPTIONAL.
 * @param array_index [in] Optional: if the Property is an array,
 *   - 0 for the array size
 *   - 1 to n for individual array members
 *   - BACNET_ARRAY_ALL (~0) for the full array to be read.
 * @return invoke id of outgoing message, or 0 if device is not bound or no tsm available
 */

uint8_t Send_Read_Property_Request(
    uint32_t device_id, /* destination device */
    BACNET_OBJECT_TYPE object_type,
    uint32_t object_instance,
    BACNET_PROPERTY_ID object_property,
    uint32_t array_index)
{
    BACNET_ADDRESS dest = { 0 };
    unsigned max_apdu = 0;
    uint8_t invoke_id = 0;
    bool status = false;

// diagnostics:
    char lbuf[SIZE__DIAG_MESSAGE];

//    unsigned int dflag_verbose = DIAGNOSTICS_ON;
    unsigned int dflag_verbose = DIAGNOSTICS__SEND_READ_PROPERTY_REQUEST;

    DIAG__SET_ROUTINE_NAME("Send_Read_Property_Request()");


    show_diag(rname, "starting,", dflag_verbose);


    /* is the device bound? */
//    snprintf(lbuf, SIZE__DIAG_MESSAGE, "calling Kargs' BACnet routine address_get_by_device(%u, %u, %u),",
//      (unsigned int)device_id, max_apdu, (unsigned int)dest);
    snprintf(lbuf, SIZE__DIAG_MESSAGE, "calling Kargs' BACnet routine address_get_by_device(%u, %u, BACNET_ADDRESS dest),", device_id, max_apdu);
    show_diag(rname, lbuf, dflag_verbose);

    show_diag(rname, "where arguments are 'device id', address of 'max ADPU', and pointer to BACNET_ADDRESS 'dest',",
      dflag_verbose);
    show_diag(rname, "calling . . .", dflag_verbose);

    status = address_get_by_device(device_id, &max_apdu, &dest);

    show_diag(rname, "checking return value of latest routing called,", dflag_verbose);
    if (status)
    {
        show_diag(rname, "calling routine Send_Read_Property_Request_Address() . . .", dflag_verbose);
        invoke_id =
            Send_Read_Property_Request_Address(&dest, max_apdu, object_type,
            object_instance, object_property, array_index);

        show_diag(rname, "back from Send_Read_Property_Request_Address(),", dflag_verbose);
    }

    snprintf(lbuf, SIZE__DIAG_MESSAGE, "returning invoke_id = %d to calling code . . .", invoke_id);
    show_diag(rname, lbuf, dflag_verbose);

    return invoke_id;
}





// --- EOF ---
