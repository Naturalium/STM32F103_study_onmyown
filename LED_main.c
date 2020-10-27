
#include <stm32f10x_rcc.h>              //RCC ������ ���̺귯���� ����ϱ� ���� ���
#include <stm32f10x_gpio.h>             //GPIO ������ ���̺귯���� ����ϱ� ���� ���


void RCC_Configuration(void)             // RCC ���� �Լ� ����
{                                        // GPIOB, AFIO ����� ���� Ŭ���� ����
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}


void GPIO_Configuration()               // GPIOB�� 3������ ���� �Լ� ����
{
  GPIO_InitTypeDef GPIO_InitStructure;                  //�������ִ� Ű���� �̸�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;             //3�� ���� ������� �ۼ�
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //�ش� �ɿ��� ������ ��ȣ�ֱ�
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //��� ����(���, push-pull)
  GPIO_Init(GPIOB, &GPIO_InitStructure);                //���� ����
}

static void delay_int_count(volatile unsigned int nTime)  //��ó�� �ҿ� �ð���
{                                                         //����(delay)��Ű�� �Լ�
  for(; nTime > 0; nTime--);
}



void main(void) 
{
  RCC_Configuration();                  // Ŭ���� �����ϴ� �Լ� ���

  
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);  // REMAP PB03���� ��� ����
  GPIO_Configuration();                 // B��Ʈ�� PB03�� �� ���� �Լ� ���
  while(1)
  {
    GPIO_ResetBits(GPIOB, GPIO_Pin_3);  // B��Ʈ�� PB03�� ���� ����� 0����(LOW)
    delay_int_count(806596);            // 806596���� �ڵ� ó�� = �� 1��
    GPIO_SetBits(GPIOB, GPIO_Pin_3);    // B��Ʈ�� PB03�� ���� ����� 1��(HIGH)
    delay_int_count(806596);            // 806596���� �ڵ� ó�� = �� 1��
  }  
}
 