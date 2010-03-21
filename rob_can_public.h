//******************************************************************************
//! @file rob_can_public.h
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

#ifndef ROB_CAN_PUBLIC_H
#define ROB_CAN_PUBLIC_H

//_____ I N C L U D E S ________________________________________________________
#include <avr/io.h>
#include "rob_can_private.h"

//_____ D E F I N E S __________________________________________________________
#define RECEIVE_ONLY_THIS_ID  0x07FF
#define RECEIVE_ALL           0x0000

// ----------
//! @brief This module global variable signals a received CAN message
//!        -1   : no message
//!        0-14 : MOb number of new message
volatile int8_t CAN_MSG_RECEIVED;

// ----------
//! @brief This enumeration is used to set the bitrate of the CAN controller
typedef enum {
   BITRATE_10_KBPS   = 0,
   BITRATE_20_KBPS   = 1,
   BITRATE_50_KBPS   = 2,
   BITRATE_100_KBPS  = 3,
   BITRATE_125_KBPS  = 4,
   BITRATE_250_KBPS  = 5,
   BITRATE_500_KBPS  = 6,
   BITRATE_1_MBPS    = 7,
} CAN_bitrate;

// ----------
//! @brief This array contains the settings for Bitrates @ 16MHz system clock
extern const uint8_t AT90CAN_Bitrates[8][3];

// ----------
//! @brief This struct represents a CAN message
typedef struct
{
   uint16_t id;      //!< CAN ID (11 Bit)
   uint16_t msk;     //!< CAN MSK (11 Bit)
   uint8_t length;   //!< CAN DLC
   uint8_t data[8];  //!< CAN Data
} CAN_msg;

// ----------
//! @brief This enumeration is used to enhance readability
typedef enum
{ 
      MOb0, MOb1, MOb2, MOb3, MOb4, MOb5, MOb6, MOb7, MOb8, MOb9, MOb10, MOb11,
      MOb12, MOb13, MOb14 
} CAN_mob;

// ----------
//! @brief This enumeration is used to select an action for a specific message
typedef enum
{
  CMD_NONE,

  CMD_DISABLED,

  CMD_TX_DATA,
  CMD_TX_REMOTE,

  CMD_RX_ALL,
  CMD_RX_MASKED,

  CMD_RX_REMOTE,
  CMD_RX_REMOTE_MASKED,

  CMD_REPLY,
  CMD_REPLY_MASKED

} CAN_action;

//_____ D E C L A R A T I O N S ________________________________________________

//------------------------------------------------------------------------------
//  @fn CAN_init
//!
//! Initialises the CAN controller with given bitrate
//!
//! @warning none
//!
//! @param  CAN_bitrate bitrate : Bitrate, range 10k - 1M
//!
//! @return 1 : success
//!         0 : fail
//!
extern uint8_t CAN_init(CAN_bitrate bitrate);

//------------------------------------------------------------------------------
//  @fn CAN_Configure_MOb
//!
//! Configures a MOb
//!
//! @warning none
//!
//! @param  CAN_mob mob  : Which MOb to configure, MOb0 - MOb14
//!         CAN_msg *msg : pointer to a CAN message
//!
//! @return 1 : success
//!         0 : fail
//!
extern uint8_t CAN_Configure_MOb(CAN_mob mob, CAN_msg *msg);

//------------------------------------------------------------------------------
//  @fn CAN_MOb_action
//!
//! Action to perform for given MOb
//!
//! @warning This functions assumes the MOb being properly configured!
//!
//! @param  CAN_mob mob       : Action for which MOb, MOb0 - MOb14
//!         CAN_action action :    CMD_NONE,
//!
//!                                CMD_DISABLED,
//!
//!                                CMD_TX_DATA,
//!                                CMD_TX_REMOTE,
//!
//!                                CMD_RX_ALL,
//!                                CMD_RX_MASKED,
//!
//!                                CMD_RX_REMOTE,
//!                                CMD_RX_REMOTE_MASKED,
//!
//!                                CMD_REPLY,
//!                                CMD_REPLY_MASKED
//!
//! @return 1 : success
//!         0 : fail
//!
extern uint8_t CAN_MOb_action(CAN_mob mob, CAN_action action);

//------------------------------------------------------------------------------
//  @fn CAN_Get_MOb
//!
//! Copies MOb to CAN_mob
//!
//! @warning none
//!
//! @param  CAN_mob mob  : Which MOb to copy, MOb0 - MOb14
//!         CAN_msg *msg : pointer to a CAN message
//!
//! @return none
//!
extern void CAN_Copy_MOb(CAN_mob mob, CAN_msg *msg);

//------------------------------------------------------------------------------
//  @fn CAN_Reply_Ready
//!
//! Restores the Automatic Reply Mode of given MOb, this has to be done
//! manually, e.g. if you have to set valid data to the MOb first
//!
//! @warning none
//!
//! @param  CAN_mob mob  : Which MOb to restore
//!
//! @return none
//!
extern void CAN_Reply_Ready(CAN_mob mob);


#endif //ROB_CAN_PUBLIC_H
