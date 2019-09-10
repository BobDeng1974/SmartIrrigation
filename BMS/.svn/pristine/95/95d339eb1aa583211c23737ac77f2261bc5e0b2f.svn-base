#include "lpc17xx_utils.h"
#include "lpc17xx_timer.h"
#ifdef LPC1830_XPLORER_TEST 
#include "tcpip.h"

#endif
//timer init
TIM_TIMERCFG_Type TIM_ConfigStruct;
TIM_MATCHCFG_Type TIM_MatchConfigStruct;


/*********************************************************************//**
 * @brief		Main TIMER program body
 * @param[in]	None
 * @return 		int
 **********************************************************************/
int timer_delay_us( int cnt)
{


	// Initialize timer 0, prescale count time of 1uS
	TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_USVAL;
	TIM_ConfigStruct.PrescaleValue	= 20;

	// use channel 0, MR0
	TIM_MatchConfigStruct.MatchChannel = 3;
	// Disable interrupt when MR0 matches the value in TC register
	TIM_MatchConfigStruct.IntOnMatch   = TRUE;
	//Enable reset on MR0: TIMER will reset if MR0 matches it
	TIM_MatchConfigStruct.ResetOnMatch = TRUE;
	//Stop on MR0 if MR0 matches it
	TIM_MatchConfigStruct.StopOnMatch  = TRUE;

	TIM_MatchConfigStruct.ExtMatchOutputType =TIM_EXTMATCH_NOTHING;
	
	TIM_MatchConfigStruct.MatchValue   = cnt;

	// Set configuration for Tim_config and Tim_MatchConfig
	TIM_Init(LPC_TIM3, TIM_TIMER_MODE,&TIM_ConfigStruct);
	TIM_ConfigMatch(LPC_TIM3,&TIM_MatchConfigStruct);
	TIM_Cmd(LPC_TIM3,ENABLE);

	while (!(TIM_GetIntStatus(LPC_TIM3,TIM_MR3_INT)));
	TIM_ClearIntPending(LPC_TIM3,(TIM_INT_TYPE)3);

  return 0;
}

/*********************************************************************//**
 * @brief		Main TIMER program body
 * @param[in]	None
 * @return 		int
 **********************************************************************/
int timer_delay_ms( int cnt)
{

	// Initialize timer 0, prescale count time of 1uS
	TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_USVAL;
	TIM_ConfigStruct.PrescaleValue	= 1000;

	// use channel 0, MR0
	TIM_MatchConfigStruct.MatchChannel = 3;
	// Disable interrupt when MR0 matches the value in TC register
	TIM_MatchConfigStruct.IntOnMatch   = TRUE;
	//Enable reset on MR0: TIMER will reset if MR0 matches it
	TIM_MatchConfigStruct.ResetOnMatch = TRUE;
	//Stop on MR0 if MR0 matches it
	TIM_MatchConfigStruct.StopOnMatch  = TRUE;

	TIM_MatchConfigStruct.ExtMatchOutputType =TIM_EXTMATCH_NOTHING;
	
	TIM_MatchConfigStruct.MatchValue   = cnt;

	// Set configuration for Tim_config and Tim_MatchConfig
	TIM_Init(LPC_TIM3, TIM_TIMER_MODE,&TIM_ConfigStruct);
	TIM_ConfigMatch(LPC_TIM3,&TIM_MatchConfigStruct);
	TIM_Cmd(LPC_TIM3,ENABLE);

	while (!(TIM_GetIntStatus(LPC_TIM3,TIM_MR3_INT)));
	TIM_ClearIntPending(LPC_TIM3,(TIM_INT_TYPE)3);

  return 0;
}

