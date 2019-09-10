#if 1

/*
  i2c support for LPC21XX

  (c) 2004  Yuri Tiomkin (yuri@tnkernel.com)


  Includes drivers:

   - EEPROM 24XX series (from 24XX04 to 24XX512)
   - Real Time Clock DS1307
   - I/O Extender PCA9555
   - Temperature sensor LM75

  Supports uCOS-II and TNKernel

THIS SOFTWARE IS PROVIDED BY THE YURI TIOMKIN AND CONTRIBUTORS ``AS IS'' AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL YURI TIOMKIN OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
SUCH DAMAGE.

*/

//#define __UCOS_          1  //-- if uCOS-II is used
//#define __TNKERNEL_    1    //-- if TNKernel is used

#include "lpc17xx.h"
#include "i2c.h"

#ifndef NULL
#define NULL 0
#endif

#if defined(__UCOS_)
extern OS_EVENT  * gpSemI2Cop;
#elif defined(__TNKERNEL_)
extern TN_SEM  gSemI2Cop;
#endif

#define SCHL 75
//===========================================================================
//================ Common routines ==========================================
//===========================================================================
void i2c_lpc_init(int Mode)
{
 
  LPC_PINCON->PINSEL0 &= ~0x0000000F;  // I2C1,P0.0, P0.1.
  LPC_PINCON->PINSEL0 |= 0x0000000F;		
 
   if(Mode == I2C_SPEED_400)
   {
  //--- I2C Timing for 58 MHz (t = 16.954 ns) ---
      LPC_I2C1->I2SCLH = SCHL;  //-- more then 0.6 us  - 0.8
      LPC_I2C1->I2SCLL = SCHL;  //-- more then 1.3 us  - 1.4
   }
   else //Slow
   {
      LPC_I2C1->I2SCLH = SCHL*4;
      LPC_I2C1->I2SCLL = SCHL*4;
   }

   LPC_I2C1->I2CONCLR = 0xFF;           //-- Clear all flags
   LPC_I2C1->I2CONSET = 0x40;           //-- Set Master Mode
   LPC_I2C1->I2CONSET |= I2C_FLAG_I2EN; //--- Enable I2C
}

//---------------------------------------------------------------------------
 void i2c_lpc_wr_byte(int byte)
{
   LPC_I2C1->I2DAT = byte;
   LPC_I2C1->I2CONCLR = I2C_FLAG_SI;                //-- Clear SI
   while(!(LPC_I2C1->I2CONSET & I2C_FLAG_SI));      //-- End wr POINT
}

//---------------------------------------------------------------------------
 void i2c_lpc_stop()
{
    //-- Set STOP condition
   LPC_I2C1->I2CONCLR = I2C_FLAG_SI;                  //-- Clear SI
   LPC_I2C1->I2CONSET |=  I2C_FLAG_AA | I2C_FLAG_STO; //-- Clear NO ASK
}

//---------------------------------------------------------------------------
 int i2c_lpc_ctrl(int ctrl)
{
   int chk;
   //-- Set START
   LPC_I2C1->I2CONCLR = 0xFF; // Clear all bits
   LPC_I2C1->I2CONSET |= I2C_FLAG_I2EN | I2C_FLAG_STA;
   while(!(LPC_I2C1->I2CONSET & I2C_FLAG_SI));      //--- End START
   //-- Set ADDRESS
   LPC_I2C1->I2DAT = ctrl;
   LPC_I2C1->I2CONCLR = I2C_FLAG_STA | I2C_FLAG_SI; //-- Clear START & SI
   if(ctrl & 1) //-- RD
      chk = 0x40; //-- 40H - SLA+R has been transmitted; ACK has been received
   else
      chk = 0x18; //-- 18H - SLA+W has been transmitted; ACK has been received
   while(!(LPC_I2C1->I2CONSET & I2C_FLAG_SI));      //-- End CTRL
   if(LPC_I2C1->I2STAT != chk)
   {
      i2c_lpc_stop();
      return I2C_ERR_NO_RESPONSE;
   }
   return I2C_NO_ERR;
}

//---------------------------------------------------------------------------
int i2c_lpc_rx_to_buf(char * buf,int num)
{
   int rc;

   if(buf == NULL)
      return I2C_ERR_WRONG_PARAM;

   rc = num;
   if(rc > 1)
   {
      LPC_I2C1->I2CONCLR = I2C_FLAG_SI;
      LPC_I2C1->I2CONSET |= I2C_FLAG_AA;
      for(;;)
      {
         while(!(LPC_I2C1->I2CONSET & I2C_FLAG_SI));  //-- End Data from slave;
         *buf++ = (unsigned char)LPC_I2C1->I2DAT;
         rc--;
         if(rc <= 0)
            break;
         else if(rc == 1)
            LPC_I2C1->I2CONCLR = I2C_FLAG_AA | I2C_FLAG_SI;  //-- After next will NO ASK
         else
         {
            LPC_I2C1->I2CONCLR = I2C_FLAG_SI;
            LPC_I2C1->I2CONSET |= I2C_FLAG_AA;
         }
      }
   }
   else if(rc == 1)
   {
      LPC_I2C1->I2CONCLR = I2C_FLAG_AA | I2C_FLAG_SI;  //-- After next will NO ASK
      while(!(LPC_I2C1->I2CONSET & I2C_FLAG_SI));  //-- End Data from slave;
      *buf = (unsigned char)LPC_I2C1->I2DAT;
   }
   else //err
      return I2C_ERR_WRONG_PARAM;

   return I2C_NO_ERR;
}
//----------------------------------------------------------------------
int i2c_lpc_ask_polling_op(int ctrl)  //-- wait until write finished
{
   int rc;
   int i;

#if defined(__UCOS_)
   for(i=0;i < 15; i++)
#elif defined(__TNKERNEL_)
   for(i=0;i < 15; i++)
#else
   for(i=0;i < I2C_WR_24XX_TIMEOUT; i++) //-- actually wr = ~110 but timeout =10000
#endif
   {
      LPC_I2C1->I2CONSET = I2C_FLAG_STA;
      LPC_I2C1->I2CONCLR = I2C_FLAG_SI;  //-- Here - clear only SI (not all LPC_I2C1->I2CONCLR)
      while(!(LPC_I2C1->I2CONSET & I2C_FLAG_SI));        //wait the ACK

      LPC_I2C1->I2DAT = ctrl & 0xFE;; // R/WI = 0
      LPC_I2C1->I2CONCLR = I2C_FLAG_SI | I2C_FLAG_STA; //-- Clear START & SI
      while(!(LPC_I2C1->I2CONSET & I2C_FLAG_SI));//wait the ACK
      rc = LPC_I2C1->I2STAT;
      if(rc == 0x18) //-- got ACK after CLA + W
         break;
      else
      {
#if defined(__UCOS_)
         OSTimeDly(2);      //-- 2 mS
#elif defined(__TNKERNEL_)
         tn_task_sleep(2);  //-- 2 mS
#endif
      }
   }
   if(i == I2C_WR_24XX_TIMEOUT)
      return I2C_ERR_24XX_WR_TIMEOUT;
   return I2C_NO_ERR;
}
#endif
