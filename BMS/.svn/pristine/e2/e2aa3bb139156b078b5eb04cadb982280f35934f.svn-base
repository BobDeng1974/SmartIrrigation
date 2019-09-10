/*
 * @brief LWIP no-RTOS TCP Echo example
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2014
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

#include "lwip/init.h"
#include "lwip/opt.h"
#include "lwip/sys.h"
#include "lwip/memp.h"
#include "lwip/tcpip.h"
#include "lwip/ip_addr.h"
#include "lwip/netif.h"
#include "lwip/lwip_timers.h"
#include "netif/etharp.h"

#if LWIP_DHCP
#include "lwip/dhcp.h"
#endif
#include "LPC17xx.h"
#include "enet_17xx_40xx.h"

#include "lpc_phy.h"
#include "arch\lpc17xx_40xx_emac.h"
#include "arch\lpc_arch.h"
#include "echo.h"
#include "circular_buf.h"
#include "CDCTask.h"
/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/* NETIF data */
extern CIR_BUFFER Ethrxfifo;
static struct netif lpc_netif;
U8 mac_address[6];
xSemaphoreHandle xSemaphoreReceivelwIP;
/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/
void Ethernet_Pin_init()
{
	LPC_PINCON->PINSEL2 &= ~((3<<0));
	LPC_PINCON->PINSEL2 |=  ((1<<0));

	LPC_PINCON->PINSEL2 &= ~((3<<2));
	LPC_PINCON->PINSEL2 |=  ((1<<2));

	LPC_PINCON->PINSEL2 &= ~((3<<8));
	LPC_PINCON->PINSEL2 |=  ((1<<8));

	LPC_PINCON->PINSEL2 &= ~((3<<16));
	LPC_PINCON->PINSEL2 |=  ((1<<16));

	LPC_PINCON->PINSEL2 &= ~((3<<18));
	LPC_PINCON->PINSEL2 |=  ((1<<18));

	LPC_PINCON->PINSEL2 &= ~((3<<20));
	LPC_PINCON->PINSEL2 |=  ((1<<20));

	LPC_PINCON->PINSEL2 &= ~((3<<28));
	LPC_PINCON->PINSEL2 |=  ((1<<28));

	LPC_PINCON->PINSEL2 &= ~((3<<30));
	LPC_PINCON->PINSEL2 |=  ((1<<30));

	LPC_PINCON->PINSEL3 &= ~((3<<0));
	LPC_PINCON->PINSEL3 |=  ((1<<0));

	LPC_PINCON->PINSEL3 &= ~((3<<2));
	LPC_PINCON->PINSEL3 |=  ((1<<2));

	LPC_PINCON->PINSEL3 &= ~((3<<22));
	LPC_PINCON->PINSEL3 |=  ((1<<22));

	/*Select Pinmode here*/
	LPC_PINCON->PINMODE2 &= ~((3<<0));
	LPC_PINCON->PINMODE2 |=  ((1<<0));

	LPC_PINCON->PINMODE2 &= ~((3<<2));
	LPC_PINCON->PINMODE2 |=  ((1<<2));

	LPC_PINCON->PINMODE2 &= ~((3<<8));
	LPC_PINCON->PINMODE2 |=  ((1<<8));

	LPC_PINCON->PINMODE2 &= ~((3<<16));
	LPC_PINCON->PINMODE2 |=  ((1<<16));

	LPC_PINCON->PINMODE2 &= ~((3<<18));
	LPC_PINCON->PINMODE2 |=  ((1<<18));

	LPC_PINCON->PINMODE2 &= ~((3<<20));
	LPC_PINCON->PINMODE2 |=  ((1<<20));

	LPC_PINCON->PINMODE2 &= ~((3<<28));
	LPC_PINCON->PINMODE2 |=  ((1<<28));

	LPC_PINCON->PINMODE2 &= ~((3<<30));
	LPC_PINCON->PINMODE2 |=  ((1<<30));

	LPC_PINCON->PINMODE3 &= ~((3<<0));
	LPC_PINCON->PINMODE3 |=  ((1<<0));

	LPC_PINCON->PINMODE3 &= ~((3<<2));
	LPC_PINCON->PINMODE3 |=  ((1<<2));

	LPC_PINCON->PINMODE3 &= ~((3<<22));
	LPC_PINCON->PINMODE3 |=  ((1<<22));
}
#if 0
/* Sets up system hardware */
static void prvSetupHardware(void)
{
	SystemCoreClockUpdate();
//	Board_Init();

	/* LED0 is used for the link status, on = PHY cable detected */
	/* Initial LED state is off to show an unconnected cable state */
//	Board_LED_Set(0, false);

	/* Setup a 1mS sysTick for the primary time base */
	SysTick_Enable(1);
}
#endif
/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief	main routine for example_lwip_tcpecho_sa_17xx40xx
 * @return	Function should not exit.
 */
portTASK_FUNCTION( vLWIPTask, pvParameters)
//int main(void)
{
	uint32_t physts;
	ip_addr_t ipaddr, netmask, gw;
	U8 IpAddress[4];
	Ethernet_Pin_init();
//	prvSetupHardware();

	/* Initialize LWIP */
	lwip_init();

	LWIP_DEBUGF(LWIP_DBG_ON, ("Starting LWIP TCP echo server...\n"));
	memset(IpAddress,0,sizeof(IpAddress));
	memset(mac_address,0,sizeof(mac_address));
	//ChangeIP_Temp();// To set default IP Address to 192.168.1.123
	I2cEepromRead(IpAddress,4,EEPROMIPADDRESS);
	I2cEepromRead(mac_address, sizeof(mac_address), EEPROMMACADDRESS);

	/* Static IP assignment */
#if LWIP_DHCP
	IP4_ADDR(&gw, 0, 0, 0, 0);
	IP4_ADDR(&ipaddr, 0, 0, 0, 0);
	IP4_ADDR(&netmask, 0, 0, 0, 0);
#else
	IP4_ADDR(&gw, IpAddress[0], IpAddress[1], IpAddress[2], 1);
	IP4_ADDR(&ipaddr, IpAddress[0], IpAddress[1], IpAddress[2], IpAddress[3]);
	//IP4_ADDR(&gw, 192, 168, 1, 1);
	//IP4_ADDR(&ipaddr, 192, 168, 1, 123);
	IP4_ADDR(&netmask, 255, 255, 255, 0);
//	APP_PRINT_IP(&ipaddr);
#endif
	/* Add netif interface for lpc17xx_8x */
	netif_add(&lpc_netif, &ipaddr, &netmask, &gw, NULL, lpc_enetif_init,
			  ethernet_input);
	netif_set_default(&lpc_netif);
	netif_set_up(&lpc_netif);
#if LWIP_DHCP
	dhcp_start(&lpc_netif);
#endif
	/* Initialize and start application */
	echo_init();

	NVIC_SetPriority(ENET_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY+1);
	NVIC_EnableIRQ(ENET_IRQn);
	/* This could be done in the sysTick ISR, but may stay in IRQ context
	   too long, so do this stuff with a background loop. */
	while (1) {
		/* lpc_rx_queue will re-qeueu receive buffers. This normally occurs
		   automatically, but in systems were memory is constrained, pbufs
		   may not always be able to get allocated, so this function can be
		   optionally enabled to re-queue receive buffers. */
#if 0
		while (lpc_rx_queue(&lpc_netif)) {}
#endif
		/* Call the PHY status update state machine once in a while
		   to keep the link status up-to-date */
		physts = lpcPHYStsPoll();

		/* Only check for connection state when the PHY status has changed */
		if (physts & PHY_LINK_CHANGED) {
			if (physts & PHY_LINK_CONNECTED) {
				/* Set interface speed and duplex */
				if (physts & PHY_LINK_SPEED100) {
					Chip_ENET_Set100Mbps(LPC_ETHERNET);
					NETIF_INIT_SNMP(&lpc_netif, snmp_ifType_ethernet_csmacd, 100000000);
				}
				else {
					Chip_ENET_Set10Mbps(LPC_ETHERNET);
					NETIF_INIT_SNMP(&lpc_netif, snmp_ifType_ethernet_csmacd, 10000000);
				}
				if (physts & PHY_LINK_FULLDUPLX) {
					Chip_ENET_SetFullDuplex(LPC_ETHERNET);
				}
				else {
					Chip_ENET_SetHalfDuplex(LPC_ETHERNET);
				}

				netif_set_link_up(&lpc_netif);
			}
			else {
				netif_set_link_down(&lpc_netif);
			}

			DEBUGOUT("Link connect status: %d\r\n", ((physts & PHY_LINK_CONNECTED) != 0));
		}
	}
}

portTASK_FUNCTION( vReceiveLWIPTask, pvParameters)
{
	xSemaphoreReceivelwIP = xSemaphoreCreateBinary();
//	vTaskDelay(1000);
	while(1)
	{
		xSemaphoreTake( xSemaphoreReceivelwIP, portMAX_DELAY);
#if 1
		while (!Chip_ENET_IsRxEmpty(LPC_ETHERNET)) {
			lpc_enetif_input(&lpc_netif);
			/* Free TX buffers that are done sending */
			lpc_tx_reclaim(&lpc_netif);

			/* LWIP timers - ARP, DHCP, TCP, etc. */
			sys_check_timeouts();
		}
		NVIC_EnableIRQ(ENET_IRQn);
#endif
	}
}
/**
 * @}
 */
