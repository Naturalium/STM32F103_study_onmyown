/*

USART 핀 PA09(Tx), PA10(Rx)		USART는 8비트 데이터 통신이므로 12비트짜리 ADC값을 전달하기 위해서는 두번의 입력을 받아 값을 전달해야 할것
					혹은 12비트짜리 ADC값을 8비트로 줄이는 방법도 있음(잘 알지 못하는 경우는 후자를 추천)
*/

#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_usart.h>		//stm32f10x_usart.c, stm32f10x_usart.h 파일 필요

void RCC_Configuration(void)
{ 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
}



void USART_Configuration()
{
  USART_InitTypeDef USART_InitStructure;
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1, &USART_InitStructure);
  USART_Cmd(USART1, ENABLE);
}


void GPIO_Configuration()
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		// 다음에도 똑같아서 생략 (다른 경우 다시 작성)
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
//(생략), 이전 선언된 구조체 변수로 설정됨
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}


// 아래 함수는 글자를 전송할때 사용함 (표준 통신규격 ASKII코드를 이용함)

void SerialPutChar(uint8_t c)					// 8bit, 1byte 출력 함수 ---- 1byte 는 글자 1개의 데이터량(한글의 경우 2byte, ASKII코드에는 존재하지 않아서 지원안됨)
{
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
  USART_SendData(USART1, c);
}

void Serial_PutString(uint8_t *s)				// nbyte 출력 함수 ----- 문장 출력 가능("글자"는 아스키코드를 따라야 함)
{
  while(*s != '\0')
  {
    SerialPutChar(*s);
    s++;
  }
}


void main(void) 
{
  int UDRVal;							// 입력을 받을 값 이름은 자기맘대로, but 가독성있게
  RCC_Configuration();
  USART_Configuration();
  GPIO_Configuration();
  

  /*
  이 사이에 뭔가가 UDRVal으로 들어가야 함.
  12비트 온전한 상태로 값을 전달하려는 경우 아래 함수들도 다르게 사용해야 할것.







  */

/////////////////////////////////////
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	// 통신을 통해 출력을 하는 Tx buffer(가상공간)이 비워져있는지를 확인하는 상태 플래그
  USART_SendData(USART1, UDRVal);				// Tx buffer에 UDRVal 값을 입력하는 함수, 전송까지 다 알아서 된다.

  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
  USART_SendData(USART1, '\n');					// 작은 따옴표, 문자를 나타냄 (char)형으로 출력함, ASKII코드로 자동 변환
								// \n은 줄바꿈을 알리는 문자
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
  USART_SendData(USART1, '\r');					// \r은 커서위치를 초기화 시키는 문자
//////////////////////////////////////
}


