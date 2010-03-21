//******************************************************************************
//! @file rob_can_private.h
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

#ifndef ROB_CAN_PRIVATE_H
#define ROB_CAN_PRIVATE_H

#ifndef F_CPU
#define F_CPU    16000000UL
#endif

//_____ I N C L U D E S ________________________________________________________
#include <avr/io.h>
#include <stdint.h>

//_____ D E C L A R A T I O N S ________________________________________________

//------------------------------------------------------------------------------
//  @fn CAN_get_mob
//!
//! Sets CANPAGE to given MOb
//!
//! @warning none
//!
//! @param  uint8_t mob : Number of MOb page, range 0-14
//!
//! @return none
//!
void CAN_get_mob(uint8_t mob);

//------------------------------------------------------------------------------
//  @fn CAN_enable_mob_interrupt
//!
//! Enables MOb interrupts
//!
//! @warning This function assumes that the right MOb is selected before
//!
//! @param  uint8_t mob : Number of MOb page, range 0-14
//!
//! @return none
//!
void CAN_enable_mob_interrupt(uint8_t mob);

//------------------------------------------------------------------------------
//  @fn CAN_disable_mob_interrupt
//!
//! Disables MOb interrupts
//!
//! @warning This function assumes that the right MOb is selected before
//!
//! @param  uint8_t mob : Number of MOb page, range 0-14
//!
//! @return none
//!
void CAN_disable_mob_interrupt(uint8_t mob);

//------------------------------------------------------------------------------
//  @fn CAN_set_id
//!
//! Sets the Identifier of given MOb in CAN Identifier Tag Registers
//! CANIDT1 - CANIDT4
//!
//! @warning This function assumes that the right MOb is selected before
//!
//! @param  uint16_t id : CAN Identifier Tag(V2.0 part A, 11-bit)
//!
//! @return none
//!
void CAN_set_id(uint16_t id);

//------------------------------------------------------------------------------
//  @fn CAN_get_id
//!
//! Gets the Identifier of given MOb in CAN Identifier Tag Registers
//! CANIDT1 - CANIDT4
//!
//! @warning This function assumes that the right MOb is selected before
//!
//! @param  none
//!
//! @return uint16_t    : ID of selected MOb
//!
uint16_t CAN_get_id(void);

//------------------------------------------------------------------------------
//  @fn CAN_set_id_mask
//!
//! Sets the Mask of given MOb in CAN Identifier Mask Registers
//! CANIDM1 - CANIDM4
//!
//! @warning This function assumes that the right MOb is selected before
//!
//! @param  uint16_t idm : CAN Identifier Mask(V2.0 part A, 11-bit)
//!
//! @return none
//!
void CAN_set_id_mask(uint16_t idm);

//------------------------------------------------------------------------------
//  @fn CAN_set_dlc
//!
//! Sets DLC(Data Length Code) in CANCDMOB
//!
//! @warning This function assumes that the right MOb is selected before
//!
//! @param  uint8_t dlc : The range of DLC is from 0 up to 8. If DLC field >8
//!                       then effective DLC=8.
//!
//! @return none
//!
void CAN_set_dlc(uint8_t dlc);

//------------------------------------------------------------------------------
//  @fn CAN_get_dlc
//!
//! Returns DLC(Data Length Code) in CANCDMOB
//!
//! @warning This function assumes that the right MOb is selected before
//!
//! @param  none
//!
//! @return uint8_t     : DLC of selected MOb
//!
uint8_t CAN_get_dlc(void);

//------------------------------------------------------------------------------
//  @fn CAN_set_data
//!
//! Sets Data in CANMSG
//!
//! @warning This function assumes that the right MOb is selected before
//!
//! @param  uint8_t dlc   : DLC of data
//!         uint8_t *data : pointer to array of Message Data
//!
//! @return none
//!
void CAN_set_data(uint8_t dlc, uint8_t *data);

#endif //ROB_CAN_PRIVATE_H
