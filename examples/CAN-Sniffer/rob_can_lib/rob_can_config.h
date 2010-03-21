//******************************************************************************
//! @file rob_can_config.h
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

#ifndef ROB_CAN_CONFIG_H
#define ROB_CAN_CONFIG_H

//_____ I N C L U D E S ________________________________________________________
#include "rob_can_public.h"

//_____ D E C L A R A T I O N S ________________________________________________

//------------------------------------------------------------------------------
//  @fn CAN_Config
//!
//! Initialises the CAN Controller MObs
//!
//! @warning none
//!
//! @param  none
//!
//! @return none
//!
void CAN_Config(void);

//------------------------------------------------------------------------------
//  @fn CAN_Received_Action
//!
//! This functions contains all the actions to react to CAN Frames
//!
//! @warning none
//!
//! @param  CAN_mob mob
//!
//! @return none
//!
void CAN_Received_Action(CAN_mob mob);

#endif //ROB_CAN_CONFIG_H
