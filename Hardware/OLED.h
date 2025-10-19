#ifndef __OLED_H
#define __OLED_H

void OLED_Init(void);
void OLED_Clear(void);
void OLED_CleanLine(short line);
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowChar_Reverse(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
void OLED_ShowString_Limit(uint8_t Line, uint8_t Column, char *String,uint8_t len);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
void OLED_ShowSignedDecimal(uint8_t Line, uint8_t Column, float Number, uint8_t IntegerLength, uint8_t DecimalLength);
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);

#endif
