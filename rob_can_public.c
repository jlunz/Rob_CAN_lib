//******************************************************************************
//! @file rob_can_public.c
//!
//!
//! @brief  This file contains functions for use with 
//!         the CAN-module of AT90CAN128/64/32
//!
//! @version $Revision: 0.10 $ $Name: Julian Lunz $
//!
//! @todo
//! @bug
//******************************************************************************
#include "rob_can_public.h"

const uint8_t AT90CAN_Bitrates[8][3] = 
{
   // 10 kbps
   {  0x7E,
      0x6E,
      0x7F
   },
   // 20 kbps
   {  0x62,
      0x0C,
      0x37
   },
   // 50 kbps
   {  0x26,
      0x0C,
      0x37
   },
   // 100 kbps
   {  0x12,
      0x0C,
      0x37
   },
   // 125 kbps
   {  0x0E,
      0x0C,
      0x37
   },
   // 250 kbps
   {  0x06,
      0x0C,
      0x37
   },
   // 500 kbps
   {  0x02,
      0x0C,
      0x37
   },
   // 1 Mbps
   {  0x00,
      0x0C,
      0x37
   }
};

uint8_t CAN_init(CAN_bitrate bitrate)
{
   uint8_t mob;
   
   // set to -1, no message received yet
   CAN_MSG_RECEIVED = -1;
   
   if (bitrate >= 8)
      return 0;
   
   // switch CAN controller to reset mode
   CANGCON |= (1 << SWRES);
   
   // reset all interrupt registers
   CANSIT2 = 0x00;
   CANSIT1 = 0x00;
   CANGIT  = 0x00;
   CANGIE  = 0x00;
   CANEN1  = 0x00;
   CANEN2  = 0x00;
   CANIE1  = 0x00;
   CANIE2  = 0x00;
   
   // reset all MObs
  for (mob = 0; mob < 15; mob++)
  { 
      CAN_get_mob(mob);
      CANIDT1 = 0x00;  //  reset ID-Tag
      CANIDT2 = 0x00;
      CANIDT3 = 0x00;
      CANIDT4 = 0x00;

      CANIDM1 = 0x00;  //  reset ID-Mask
      CANIDM2 = 0x00;
      CANIDM3 = 0x00;
      CANIDM4 = 0x00;
      
      // Disable MOb interrupt
      CAN_disable_mob_interrupt(mob); 

      CANSTMOB = 0x00;  //  reset MOb status
      CANCDMOB = 0x00;  //  disable MOb
  }
   
   
   // set CAN Bit Timing
   CANBT1 = AT90CAN_Bitrates[bitrate][0];
   CANBT2 = AT90CAN_Bitrates[bitrate][1];
   CANBT3 = AT90CAN_Bitrates[bitrate][2];
   
   // activate CAN transmit- and receive-interrupt
   CANGIE = ((1 << ENIT) | (1 << ENRX) | (1 << ENTX));
   
   // activate CAN controller
   CANGCON = (1 << ENASTB);

     //  wait for EnableFlag
  while (!(CANGSTA & (1<<ENFG))); 
   
   return 1;
}

uint8_t CAN_Configure_MOb(CAN_mob mob, CAN_msg *msg)
{
	// check if MOb is already in use --> return 0
   if (mob < 8)
	{
      if(CANEN2 & (1 << mob))
			return 0;
	}
   else
	{
      if(CANEN1 & (1 << (mob - 8)))
			return 0;
	}
	
   // select MOb
   CAN_get_mob(mob);
   // Set DLC (Data Length Code) and data
   CAN_set_data(msg->length, msg->data);
   // set ID
   CAN_set_id(msg->id);
   // Set Mask
   CAN_set_id_mask(msg->msk);

   // Enable interrupts for MOb
   CAN_enable_mob_interrupt(mob);

   return 1;
}


/*
|------------------------------------------------------------------|
|CONMOB1 CONMOB0     |Reply Valid| RTR Tag   | Operating Mode      |
|--------------------|-----------|-----------|---------------------|
|0         0         |   x       |   x       | Disabled            |
|--------------------|-----------|-----------|---------------------|
|0         1         |   x       |   0       | Tx Data Frame       |
|0         1         |   x       |   1       | Tx Remote Frame     |
|--------------------|-----------|-----------|---------------------|
|1         0         |   x       |   0       | Rx Data Frame       |
|1         0         |   0       |   1       | Rx Remote Frame     |
|1         0         |   1       |   1       | Rx Remote Frame,    |
|                    |           |           | then Tx Data Frame  |
|                    |           |           | (reply mode)        |
|------------------------------------------------------------------|
*/
// This functions assumes the MOb being properly configured!
uint8_t CAN_MOb_action(CAN_mob mob, CAN_action action)
{
   // select MOb
   CAN_get_mob(mob);
   
   switch(action)
   {
     case CMD_DISABLED:
       CANCDMOB = 0;
       CANIDT4  &= ~(1<<RTRTAG);
       CANIDM4  &= ~(1<<RTRMSK);
     break;
     //------------   

     case CMD_TX_DATA:
       // Disable MOb
       CANCDMOB &= ~((1<<CONMOB1)|(1<<CONMOB0));

       CANIDT4  &= ~(1<<RTRTAG);
       CANCDMOB &= ~(1<<RPLV);
       // Configure as TX
       CANCDMOB |= (1  << CONMOB0);
       break;
     //------------   
        
     case CMD_TX_REMOTE:       
       // Set RTR
       CANIDT4  |= (1  << RTRTAG);
       
       // Disable MOb
       CANCDMOB &= ~((1<<CONMOB1)|(1<<CONMOB0));
       // Configure as TX
       CANCDMOB |= (1  << CONMOB0);
       break;
     //------------  
            
     case CMD_RX_ALL:
       CANIDM1   = 0;
       CANIDM2   = 0;
       CANIDM4  |=  (1 << IDEMSK); // to receive only standard frames
       CANIDT4  &= ~(1 << RTRTAG);
       
       // Disable MOb
       CANCDMOB &= ~((1<<CONMOB1)|(1<<CONMOB0));
       // Configure as RX
       CANCDMOB |= (1  <<CONMOB1);
       break;
     //------------

     case CMD_RX_MASKED:
       CANIDM4  |=  (1 << IDEMSK); // to receive only standard frames
       CANIDT4  &= ~(1 << RTRTAG);
       
       // Disable MOb
       CANCDMOB &= ~((1<<CONMOB1)|(1<<CONMOB0));
       // Configure as RX
       CANCDMOB |= (1  <<CONMOB1);
       break;
     //------------  
          
     case CMD_RX_REMOTE:
       // Disable MOb
       CANCDMOB &= ~((1<<CONMOB1)|(1<<CONMOB0));

        CANIDM4  |= (1<<RTRMSK);
        CANIDT4  |= (1  << RTRTAG);
        CANCDMOB &= ~(1<<RPLV);
        CANIDM4  |=  (1 << IDEMSK); // to receive only standard frames
       

       // Configure as RX
       CANCDMOB |= (1  <<CONMOB1);
       break;
     //------------    
           
     case CMD_RX_REMOTE_MASKED:
       // Disable MOb
       CANCDMOB &= ~((1<<CONMOB1)|(1<<CONMOB0));

       CANIDM4  |= (1<<RTRMSK);

       CANCDMOB |= (1<<RPLV);
       CANIDM4  |=  (1 << IDEMSK); // to receive only standard frames
       
       // Configure as RX
       CANCDMOB |= (1  <<CONMOB1);
       break;
     //------------   
        
     case CMD_REPLY:
       // Disable MOb
       CANCDMOB &= ~((1<<CONMOB1)|(1<<CONMOB0));
       // Configure as RX
       CANCDMOB |= (1  <<CONMOB1);
       
       CANCDMOB |= (1<<RPLV);
       CANIDT4  |= (1<<RTRTAG);
       CANIDM4  |= (1<<RTRMSK); 
       CANIDM4  |= (1 << IDEMSK); // to receive only standard frames
       
       break;
     //------------    
       
     case CMD_REPLY_MASKED:
       // Disable MOb
       CANCDMOB &= ~((1<<CONMOB1)|(1<<CONMOB0));
       // Configure as RX
       CANCDMOB |= (1  <<CONMOB1);

       CANCDMOB |= (1<<RPLV);
       CANIDT4  |= (1<<RTRTAG);
       CANIDM4  |= (1<<RTRMSK); 
       CANIDM4  |= (1 << IDEMSK); // to receive only standard frames
       
       break;
     //------------     
      
     default:
       // not implemented command
       return 0;
       break;
     //------------
   }
   return 1;
}

void CAN_Copy_MOb(CAN_mob mob, CAN_msg *msg)
{
   uint8_t i;

   // select MOb
   CAN_get_mob(mob);

   msg->id     = CAN_get_id();
   msg->length = CAN_get_dlc();

   // clear first
	for(i = 0; i < 8; i++)	
	{	
	  msg->data[i] = 0;
	}

   for(i = 0; i < msg->length; i++)
   {
      msg->data[i] = CANMSG;
   }
}

void CAN_Reply_Ready(CAN_mob mob)
{
   // select MOb
   CAN_get_mob(mob);

   // Restore RTRTAG and RPLV to 1, Auto Reply Mode
   CANIDT4  |= (1<<RTRTAG);
   CANCDMOB |= (1<<RPLV);
   
}







