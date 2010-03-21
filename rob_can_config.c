//******************************************************************************
//! @file rob_can_config.c
//!
//!
//! @brief  This file contains functions for initial config of MObs and
//!         actions for received CAN Frames
//!
//! @version $Revision: 0.10 $ $Name: Julian Lunz $
//!
//! @todo 
//! @bug
//******************************************************************************

#include "rob_can_config.h"

void CAN_Config()
{
   /*
      Example Config, prepare three MObs for sending
      Call 
            CAN_MOb_action(MObX, CMD_TX_DATA);
      to transmit MObX via CAN
   */
/*
   CAN_msg msg2, msg3, msg4;



   // >>>msg2
   msg2.id      = 0x735;

   msg2.length  = 8;

   msg2.data[0] = 2;
   msg2.data[1] = 3;
   msg2.data[2] = 4;
   msg2.data[3] = 5;
   msg2.data[4] = 6;
   msg2.data[5] = 7;
   msg2.data[6] = 8;
   msg2.data[7] = 9;
   // msg2<<<

   // >>>msg3
   msg3.id      = 0x333;

   msg3.length  = 3;

   msg3.data[0] = 4;
   msg3.data[1] = 5;
   msg3.data[2] = 6;
   // msg3<<<

   // >>>msg4
   msg4.id      = 0x444;

   msg4.length  = 4;

   msg4.data[0] = 7;
   msg4.data[1] = 8;
   msg4.data[2] = 9;
   msg4.data[3] = 0xA;
   // msg4<<<
   

   CAN_Configure_MOb(MOb2, &msg2);   
   CAN_Configure_MOb(MOb3, &msg3);   
   CAN_Configure_MOb(MOb4, &msg4); 

*/

// Your code here...
   
}

void CAN_Received_Action(CAN_mob mob)
{
   CAN_msg msg_received;

   // Action for each CAN Frame here

   //get received message
   CAN_Copy_MOb(mob, &msg_received);

/*
// Action for each CAN Frame here
   if(msg_received.id == 0x123)
   {
      //action
   }
*/

}
