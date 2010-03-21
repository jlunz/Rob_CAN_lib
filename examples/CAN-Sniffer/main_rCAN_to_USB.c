#include <avr/io.h>
#include <util/delay.h>

#include "rob_can_config.h"

#include <avr/interrupt.h>


#include <stdlib.h>
#include <avr/pgmspace.h>
#include "uart.h"

#include "crc_can.h"


// Serial Data RX Buffer
static volatile unsigned char rx_buffer[14];
static volatile unsigned char *p_rx;
static volatile uint8_t num_rx;

int main(void)
{
  // unsigned char rx_buffer[14];
  // unsigned char *p_rx;
  // uint8_t num_rx;

   p_rx = rx_buffer;
   rx_buffer[0] = 0;
   num_rx = 0;


   CAN_init(BITRATE_100_KBPS);

   CAN_Config();

    // Init UART
    // Init UART-1 at 500k bauds
   uart1_init( UART_BAUD_SELECT(500000,F_CPU) );

   


// int on overflow
TIMSK0 = (1 << TOIE0);




   sei();
   //uart1_puts("Startup\n");

   CAN_MOb_action(MOb0, CMD_RX_ALL);

   unsigned int c;
   while(1)
   {
      c = uart1_getc();
      if((c & UART_NO_DATA))
      {
         
      }
      else
      {
         if((c == 0xAA) || (num_rx != 0))
         {
            // start serial watchdog, if 14 bytes aren't received within 16ms
            // normally it should take ~0.3ms
            // delete all received data
            // Prescaler: 1024
            TCCR0A = (1 << CS02) | (1 << CS00);

            
            if(num_rx > 0)
            {
               *p_rx = (unsigned char)c;
               p_rx++;
            }
            
            num_rx++;
            if(num_rx == 14)
            {
               // stop timer
               TCCR0A = 0;
               // reset timer
               TCNT0 = 0;
               

               // verify data with CRC
               uint16_t crc = 0;

               for(uint8_t i = 0; i < 14; i++)
               {  
                  crc = crc_can(crc, rx_buffer[i]);
               }

               if(crc != 0)
               {
                  // CRC-ERROR
                  //uart1_puts("CRC-Error!!!\n");
               }
               else
               {
                  //uart1_puts("CRC decoded :)!!!\n");
                  
                  CAN_msg msg;

                  msg.id      = (uint16_t)( (uint16_t)(rx_buffer[0] << 8) | (rx_buffer[1]) );
                  msg.length  = rx_buffer[2];
                  for(uint8_t i = 0; i < msg.length; i++)
                  {
                     msg.data[i] = rx_buffer[3 + i];
                  }

                  // Configure and send CAN Frame
                  CAN_Configure_MOb(MOb1, &msg);
                  CAN_MOb_action(MOb1, CMD_TX_DATA);

               }

               num_rx = 0;
               rx_buffer[0] = 0;
               // reset pointer to beginning of char[]
               p_rx = rx_buffer;
            }
            
         }

      }


      // Polling of CAN_MSG_RECEIVED -->actions for each MOb in rob_can_config
      if(CAN_MSG_RECEIVED != -1)
      {         
         CAN_Received_Action(CAN_MSG_RECEIVED);

         // Reset status
         CAN_MSG_RECEIVED = -1;
      }   

      
   }
   
   return 0;
}

ISR(TIMER0_OVF_vect)
{
   // stop timer
   TCCR0A = 0;
   // reset timer
   TCNT0 = 0;

   //uart1_puts("timeout!!!\n");

   num_rx = 0;
   rx_buffer[0] = 0;
   // reset pointer to beginning of char[]
   p_rx = rx_buffer;
}
