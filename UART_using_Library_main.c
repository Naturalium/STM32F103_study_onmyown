/*

USART �� PA09(Tx), PA10(Rx)		USART�� 8��Ʈ ������ ����̹Ƿ� 12��Ʈ¥�� ADC���� �����ϱ� ���ؼ��� �ι��� �Է��� �޾� ���� �����ؾ� �Ұ�
					Ȥ�� 12��Ʈ¥�� ADC���� 8��Ʈ�� ���̴� ����� ����(�� ���� ���ϴ� ���� ���ڸ� ��õ)
*/

#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_usart.h>		//stm32f10x_usart.c, stm32f10x_usart.h ���� �ʿ�

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
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		// �������� �Ȱ��Ƽ� ���� (�ٸ� ��� �ٽ� �ۼ�)
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
//(����), ���� ����� ����ü ������ ������
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}


// �Ʒ� �Լ��� ���ڸ� �����Ҷ� ����� (ǥ�� ��ű԰� ASKII�ڵ带 �̿���)

void SerialPutChar(uint8_t c)					// 8bit, 1byte ��� �Լ� ---- 1byte �� ���� 1���� �����ͷ�(�ѱ��� ��� 2byte, ASKII�ڵ忡�� �������� �ʾƼ� �����ȵ�)
{
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
  USART_SendData(USART1, c);
}

void Serial_PutString(uint8_t *s)				// nbyte ��� �Լ� ----- ���� ��� ����("����"�� �ƽ�Ű�ڵ带 ����� ��)
{
  while(*s != '\0')
  {
    SerialPutChar(*s);
    s++;
  }
}


void main(void) 
{
  int UDRVal;							// �Է��� ���� �� �̸��� �ڱ⸾���, but �������ְ�
  RCC_Configuration();
  USART_Configuration();
  GPIO_Configuration();
  

  /*
  �� ���̿� ������ UDRVal���� ���� ��.
  12��Ʈ ������ ���·� ���� �����Ϸ��� ��� �Ʒ� �Լ��鵵 �ٸ��� ����ؾ� �Ұ�.







  */

/////////////////////////////////////
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	// ����� ���� ����� �ϴ� Tx buffer(�������)�� ������ִ����� Ȯ���ϴ� ���� �÷���
  USART_SendData(USART1, UDRVal);				// Tx buffer�� UDRVal ���� �Է��ϴ� �Լ�, ���۱��� �� �˾Ƽ� �ȴ�.

  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
  USART_SendData(USART1, '\n');					// ���� ����ǥ, ���ڸ� ��Ÿ�� (char)������ �����, ASKII�ڵ�� �ڵ� ��ȯ
								// \n�� �ٹٲ��� �˸��� ����
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
  USART_SendData(USART1, '\r');					// \r�� Ŀ����ġ�� �ʱ�ȭ ��Ű�� ����
//////////////////////////////////////
}


