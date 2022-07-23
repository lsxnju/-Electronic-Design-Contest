/**********************************************************************
Discription:
行扫描法读取矩阵键盘，PG6 PG8 PC7 PC9作为列输入引脚共同触发中断函数
EXTI9_5_IRQHandler()，中断函数内实现键值读取，不占用主函数资源.

Date: 2021/02/08
Author: GYW
**********************************************************************/

#include "sys.h"
#include "keyboard.h"
#include "delay.h"
#include "lcd.h"

int main(void){
	delay_init(168);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	keyboard_init();
	LCD_Init();
	LCD_ShowString(30, 40, 480, 24, 24, "KeyBoard Test V1.0");
	while(1);
}

void EXTI9_5_IRQHandler() {
	u8 line, row, key;
	line = row = key = 0;
	delay_ms(10);
	if(!ROW1) row = 1;
		else if(!ROW2) row = 2;
			else if(!ROW3) row = 3;
				else if(!ROW4) row = 4;
					else return;	//这行很重要，用于避免按键松开时抖动产生的影响
	
	LINE1 = 0;	LINE2 = LINE3 = LINE4 = 1;
	delay_ms(2);
	if(!(ROW1 && ROW2 && ROW3 && ROW4)) line = 1;
	
	LINE2 = 0;	LINE1 = LINE3 = LINE4 = 1;
	delay_ms(2);
	if(!(ROW1 && ROW2 && ROW3 && ROW4)) line = 2;
	
	LINE3 = 0;	LINE1 = LINE2 = LINE4 = 1;
	delay_ms(2);
	if(!(ROW1 && ROW2 && ROW3 && ROW4)) line = 3;
	
	LINE4 = 0;	LINE1 = LINE2 = LINE3 = 1;
	delay_ms(2);
	if(!(ROW1 && ROW2 && ROW3 && ROW4)) line = 4;
	
	if(line >= 1)	key = (line - 1) * 4 + row;	else key = 0;
	if(!key) LCD_ShowString(30, 64, 480, 24, 24, "Input Error. ");
		else switch(key){
			case 1:	LCD_ShowString(30, 64, 480, 24, 24, "KeyNum = 1.  ");	break;
			case 2:	LCD_ShowString(30, 64, 480, 24, 24, "KeyNum = 2.  ");	break;
			case 3:	LCD_ShowString(30, 64, 480, 24, 24, "KeyNum = 3.  ");	break;
			case 4:	LCD_ShowString(30, 64, 480, 24, 24, "KeyNum = 4.  ");	break;
			case 5:	LCD_ShowString(30, 64, 480, 24, 24, "KeyNum = 5.  ");	break;
			case 6:	LCD_ShowString(30, 64, 480, 24, 24, "KeyNum = 6.  ");	break;
			case 7:	LCD_ShowString(30, 64, 480, 24, 24, "KeyNum = 7.  ");	break;
			case 8:	LCD_ShowString(30, 64, 480, 24, 24, "KeyNum = 8.  ");	break;
			case 9:	LCD_ShowString(30, 64, 480, 24, 24, "KeyNum = 9.  ");	break;
			case 10:	LCD_ShowString(30, 64, 480, 24, 24, "KeyNum = 10. ");	break;
			case 11:	LCD_ShowString(30, 64, 480, 24, 24, "KeyNum = 11. ");	break;
			case 12:	LCD_ShowString(30, 64, 480, 24, 24, "KeyNum = 12. ");	break;
			case 13:	LCD_ShowString(30, 64, 480, 24, 24, "KeyNum = 13. ");	break;
			case 14:	LCD_ShowString(30, 64, 480, 24, 24, "KeyNum = 14. ");	break;
			case 15:	LCD_ShowString(30, 64, 480, 24, 24, "KeyNum = 15. ");	break;
			case 16:	LCD_ShowString(30, 64, 480, 24, 24, "KeyNum = 16. ");	break;
		
			default:  LCD_ShowString(30, 64, 480, 24, 24, "KeyNum Error.");
	}

//	LCD_ShowString(30, 88, 480, 24, 24, "Test String.");

	LINE1 = LINE2 = LINE3 = LINE4 = 0;
	EXTI_ClearITPendingBit(EXTI_Line6 | EXTI_Line7 | EXTI_Line8 | EXTI_Line9);
}
