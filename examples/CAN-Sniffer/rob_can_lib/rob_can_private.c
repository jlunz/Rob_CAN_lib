//******************************************************************************
//! @file rob_can_private.c
//!
//!
//! @brief  This file contains functions for use with 
//!         the CAN-module of AT90CAN128/64/32
//!
//! @version $Revision: 0.40 $ $Name: Julian Lunz $
//!
//! @todo
//! @bug
//******************************************************************************
#include "rob_can_private.h"

void CAN_get_mob(uint8_t mob)
{
   //Only MObs from 0-14 allowed
   if(mob < 15)
   {
      CANPAGE = (mob << 4);
   }
}

void CAN_enable_mob_interrupt(uint8_t mob)
{
   if (mob < 8)
      CANIE2 |= (1 << mob);
   else
      CANIE1 |= (1 << (mob - 8));
}

void CAN_disable_mob_interrupt(uint8_t mob)
{
   if (mob < 8)
      CANIE2 &= ~(1 << mob);
   else
      CANIE1 &= ~(1 << (mob - 8));
}

void CAN_set_id(uint16_t id)
{
   // only 11-bit IDs
      CANIDT4 = 0;
      CANIDT3 = 0;
      CANIDT2 = (uint8_t) id << 5;
      CANIDT1 = (uint16_t)id >> 3;
      
      // no IDE
      CANCDMOB &= (~(1<<IDE));
}

uint16_t CAN_get_id()
{
   uint16_t id;
   id = 0;

   id  = (uint8_t)CANIDT2  >> 5;
   id |= (uint16_t)CANIDT1 << 3;

   return id;
}

void CAN_set_id_mask(uint16_t idm)
{
   // only 11-bit IDs
   CANIDM4 = (1 << IDEMSK); //receive only standard frames
   CANIDM3 = 0;
   CANIDM2 = (uint8_t) idm << 5; 
   CANIDM1 = (uint16_t)idm >> 3;
}

void CAN_set_dlc(uint8_t dlc)
{
	// delete old CANCDMOB
	CANCDMOB = 0x00;
   // write DLC (Data Length Code)
   CANCDMOB |= dlc;
}

uint8_t CAN_get_dlc()
{
   uint8_t dlc;

   dlc = (CANCDMOB & 0x0F);

   return dlc;
}

void CAN_set_data(uint8_t dlc, uint8_t *data)
{
   uint8_t i;
   
   CAN_set_dlc(dlc);
   
   for(i = 0; i < dlc; i++)
   {
      CANMSG = data[i];
   }
}
