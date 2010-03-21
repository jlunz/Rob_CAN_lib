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

//for uart
#include "uart.h"


void CAN_Config()
{
   /*
      Example Config, preparethree MObs for sending
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
   CAN_msg rec_all;

   // >>>msg rec_all
   rec_all.id  = 0x000;
   rec_all.msk = RECEIVE_ALL;
   // msg rec_all<<<

   CAN_Configure_MOb(MOb0, &rec_all);
   
}

void CAN_Received_Action(CAN_mob mob)
{
   CAN_msg msg_received;

   // Action for each CAN Frame here

   //get received message
   CAN_Copy_MOb(mob, &msg_received);



   // prepare UART Message
   unsigned char buffer[15];

   // no initialisation because of runtime issues ;)

   buffer[0] = 0xAA; // SOF
   buffer[1] = 0xAA; // SOF
   buffer[2] = 0xAA; // SOF
   buffer[3] = 0xAA; // SOF

   buffer[4] = ((uint8_t) (msg_received.id >> 8));    // High Byte of ID
   buffer[5] = ((uint8_t) (msg_received.id & 0xFF));  // Low Byte of ID
   buffer[6] = msg_received.length;                   // DLC

   for(uint8_t i = 0; i < msg_received.length; i++)
   {
      buffer[7 + i] = msg_received.data[i];
   }

   for(uint8_t i = 0; i < 15; i++)
   {
      // Transmit UART Message
      uart1_putc(buffer[i]);
   }   
       
}
