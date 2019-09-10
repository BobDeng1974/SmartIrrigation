
/*****************************************************************************
 *   ssp.h:  Header file for Philips LPC214x Family Microprocessors
 *
 *   Copyright(C) 2006, Philips Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2005.10.01  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#ifndef __SSP_H__
#define __SSP_H__
#define _BV(x) (1<<x)            /* Bit access support Macro */
#define SSPCR0_OFFSET 0x0
#define SSPCR0_SCR_MASK 0xFF00
#define SSPCR0_SCR_BIT 8
#define SSPCR0_CPHA_MASK 0x80
#define SSPCR0_CPHA 0x80
#define SSPCR0_CPHA_BIT 7
#define SSPCR0_CPOL_MASK 0x40
#define SSPCR0_CPOL 0x40
#define SSPCR0_CPOL_BIT 6
#define SSPCR0_FRF_MASK 0x30
#define SSPCR0_FRF_BIT 4
#define SSPCR0_DSS_MASK 0xF
#define SSPCR0_DSS_BIT 0

#define SSPCR1_OFFSET 0x4
#define SSPCR1_SOD_MASK 0x8
#define SSPCR1_SOD 0x8
#define SSPCR1_SOD_BIT 3
#define SSPCR1_MS_MASK 0x4
#define SSPCR1_MS 0x4
#define SSPCR1_MS_BIT 2
#define SSPCR1_SSE_MASK 0x2
#define SSPCR1_SSE 0x2
#define SSPCR1_SSE_BIT 1
#define SSPCR1_LBE_MASK 0x1
#define SSPCR1_LBE 0x1
#define SSPCR1_LBE_BIT 0

/*  Current status of the SSP controller.  */
#define SSP_SR_TFE_MASK      (0x00000001) // Transmit FIFO empty
#define SSP_SR_TFE_EMPTY     (0x00000001)
#define SSP_SR_TFE_NOTEMPTY  (0x00000000)
#define SSP_SR_TNF_MASK      (0x00000002) // Transmit FIFO not full
#define SSP_SR_TNF_NOTFULL   (0x00000002)
#define SSP_SR_TNF_FULL      (0x00000000)
#define SSP_SR_RNE_MASK      (0x00000004) // Receive FIFO not empty
#define SSP_SR_RNE_NOTEMPTY  (0x00000004)
#define SSP_SR_RNE_EMPTY     (0x00000000)
#define SSP_SR_RFF_MASK      (0x00000008) // Receive FIFO full
#define SSP_SR_RFF_FULL      (0x00000008)
#define SSP_SR_RFF_NOTFULL   (0x00000000)
#define SSP_SR_BSY_MASK      (0x00000010) // Busy Flag
#define SSP_SR_BSY_IDLE      (0x00000000)
#define SSP_SR_BSY_BUSY      (0x00000010)

#define SSPSR_OFFSET 0xC
#define SSPSR_BSY_MASK 0x10
#define SSPSR_BSY 0x10
#define SSPSR_BSY_BIT 4
#define SSPSR_RFF_MASK 0x8
#define SSPSR_RFF 0x8
#define SSPSR_RFF_BIT 3
#define SSPSR_RNE_MASK 0x4
#define SSPSR_RNE 0x4
#define SSPSR_RNE_BIT 2
#define SSPSR_TNF_MASK 0x2
#define SSPSR_TNF 0x2
#define SSPSR_TNF_BIT 1
#define SSPSR_TFE_MASK 0x1
#define SSPSR_TFE 0x1
#define SSPSR_TFE_BIT 0

#define SSPIMSC_RORIM	(1 << 0)
#define SSPIMSC_RTIM	(1 << 1)
/* SPI read and write buffer size */
#define BUFSIZE		256
#define FIFOSIZE	8

/* don't use CS, set CS as GPIO that you have total control
of the sequence */
#define USE_CS		0

/* SPI select pin */
#define SSP_PORT_DATA_SET_REG FIO0SET 
#define SSP_DSS_8_BIT 0x7
#define SSP_SCR 35 /* Note: Earlier this was set to 31 (bit rate = .9375 Mhz), somehow at this rate
                      we have observed data corruption over the SPI lines. However if we set the 
                      value to 35 (bit rate = .81 Mhz) the issue seems to disappear
                    */

#define SSP_FRF 0

/* Set DSS data to 8-bit, Frame format SPI, CPOL = 1, CPHA = 1, and SCR is 31 */
#define SSP_CONFIGURATION0 (SSP_SCR << SSPCR0_SCR_BIT) | _BV(SSPCR0_CPHA_BIT) | _BV(SSPCR0_CPOL_BIT) | SSP_DSS_8_BIT 
#define SSP_CONFIGURATION1 

uint32_t spi1_init(void);
void spi1_send(uint8_t *buf, uint32_t Length);
void spi1_receive(uint8_t *buf, uint32_t Length);
//void if_spiInit(void);

#endif  /* __SSP_H__ */





