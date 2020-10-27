
#include <stm32f10x_rcc.h>              //RCC 관련한 라이브러리를 사용하기 위한 헤더
#include <stm32f10x_gpio.h>             //GPIO 관련한 라이브러리를 사용하기 위한 헤더


void RCC_Configuration(void)             // RCC 설정 함수 선언
{                                        // GPIOB, AFIO 기능을 위해 클럭을 제공
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}


void GPIO_Configuration()               // GPIOB의 3번핀의 설정 함수 선언
{
  GPIO_InitTypeDef GPIO_InitStructure;                  //설정해주는 키워드 이름정의
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;             //3번 핀을 사용함을 작성
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //해당 핀에서 나오는 신호주기
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //모드 고르기(출력, push-pull)
  GPIO_Init(GPIOB, &GPIO_InitStructure);                //설정 적용
}

static void delay_int_count(volatile unsigned int nTime)  //줄처리 소요 시간을
{                                                         //지연(delay)시키는 함수
  for(; nTime > 0; nTime--);
}



void main(void) 
{
  RCC_Configuration();                  // 클럭을 제공하는 함수 사용

  
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);  // REMAP PB03핀의 기능 변경
  GPIO_Configuration();                 // B포트의 PB03번 핀 설정 함수 사용
  while(1)
  {
    GPIO_ResetBits(GPIOB, GPIO_Pin_3);  // B포트의 PB03번 핀의 출력을 0으로(LOW)
    delay_int_count(806596);            // 806596번의 코드 처리 = 약 1초
    GPIO_SetBits(GPIOB, GPIO_Pin_3);    // B포트의 PB03번 핀의 출력을 1로(HIGH)
    delay_int_count(806596);            // 806596번의 코드 처리 = 약 1초
  }  
}
 