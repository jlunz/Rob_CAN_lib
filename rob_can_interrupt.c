//******************************************************************************
//! @file rob_can_private.h
//!
//!
//! @brief  This file contains the ISR for the CAN-module of AT90CAN128/64/32
//!
//! @version $Revision: 0.20 $ $Name: Julian Lunz $
//!
//! @todo
//! @bug
//******************************************************************************

#include <avr/io.h>
#include <avr/interrupt.h>
#include "rob_can_public.h"

#include <util/atomic.h>
/*
ATOMIC_RESTORESTATE:
When used, it will cause the ATOMIC_BLOCK to restore the previous state of the 
SREG register, saved before the Global Interrupt Status flag bit was disabled. 
The net effect of this is to make the ATOMIC_BLOCK's contents guaranteed atomic,
without changing the state of the Global Interrupt Status flag when execution 
of the block completes.
*/
#define  ENTER_CRITICAL_SECTION ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
#define  LEAVE_CRITICAL_SECTION }

ISR(CANIT_vect)
{
   // Begin atomic block here
   ENTER_CRITICAL_SECTION
   
   uint8_t canpage = 0;
   uint8_t MOb;
   //CAN_msg msg_received;

   // save MOb page register
   canpage = CANPAGE;
   
   // If CANSIT = 0 (no MOb), the return value is 0xF --> General Interrupt
   if ((CANHPMOB & 0xF0) != 0xF0)
   {
      
      // select MOb page with the highest priority
      CANPAGE = (CANHPMOB & 0xF0);
      MOb = (CANHPMOB >> 4);
      
      //------Data Length Code Warning------------------------------------------
      if(CANSTMOB & (1 << DLCW))
      {
         CANSTMOB &= ~(1 << DLCW);
      }
      //------Transmit OK-------------------------------------------------------
      if(CANSTMOB & (1 << TXOK))
      {
         CANSTMOB &= ~(1 << TXOK);
      }
      //------Receive OK--------------------------------------------------------
      if(CANSTMOB & (1 << RXOK))
      {
         CANSTMOB &= ~(1 << RXOK);
         
         // signal a new message
         CAN_MSG_RECEIVED = MOb;
         
         // re-enable receive operating mode
         CANCDMOB |= (1<<CONMOB1); 
      }
      //------Bit Error (Only in Transmission)----------------------------------
      if(CANSTMOB & (1 << BERR))
      {
         CANSTMOB &= ~(1 << BERR);
      }
      //------Stuff Error-------------------------------------------------------
      if(CANSTMOB & (1 << SERR))
      {
         CANSTMOB &= ~(1 << SERR);
      }
      //------CRC Error---------------------------------------------------------
      if(CANSTMOB & (1 << CERR))
      {
         CANSTMOB &= ~(1 << CERR);
      }
      //------Form Error--------------------------------------------------------
      if(CANSTMOB & (1 << FERR))
      {
         CANSTMOB &= ~(1 << FERR);
      }
      //------Acknowledgment Error----------------------------------------------
      if(CANSTMOB & (1 << AERR))
      {
         CANSTMOB &= ~(1 << AERR);
      }
   }
   else
   {
      // no MOb matches with the interrupt => general interrupt
      
      // check CAN General Interrupt Registers
      
      //------Bus Off Interrupt Flag--------------------------------------------
      if(CANGIT & (1 << BOFFIT))
      {
         // reset flag, write a logical one
         CANGIT |= (1 << BOFFIT);
      }
      //------Overrun CAN Timer-------------------------------------------------
      if(CANGIT & (1 << OVRTIM))
      {
         // reset flag, write a logical one
         CANGIT |= (1 << OVRTIM);
      }
      //------Frame Buffer Receive Interrupt------------------------------------
      if(CANGIT & (1 << BXOK))
      {
         // reset flag, write a logical one
         CANGIT |= (1 << BXOK);
      }
      //------Stuff Error General-----------------------------------------------
      if(CANGIT & (1 << SERG))
      {
         // reset flag, write a logical one
         CANGIT |= (1 << SERG);
      }
      //------CRC Error General-------------------------------------------------
      if(CANGIT & (1 << CERG))
      {
         // reset flag, write a logical one
         CANGIT |= (1 << CERG);
      }
      //------Form Error General------------------------------------------------
      if(CANGIT & (1 << FERG))
      {
         // reset flag, write a logical one
         CANGIT |= (1 << FERG);
      }
      //------Acknowledgment Error General--------------------------------------
      if(CANGIT & (1 << AERG))
      {
         // reset flag, write a logical one
         CANGIT |= (1 << AERG);
      }
      
   }
   // restore MOb page register
   CANPAGE = canpage;
   
   // end atomic block here
   LEAVE_CRITICAL_SECTION
   
}

