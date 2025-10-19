#include "stm32f10x.h"
#include "Serial.h"
#include "OLED.h"
#include "Delay.h"
#include <string.h>

#define PORT GPIOA
#define RESET GPIO_Pin_7
#define ENABLE GPIO_Pin_6

void wireless_gpio_init() {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = RESET | ENABLE;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PORT, &GPIO_InitStructure);
}

void enable_ESP() {
	GPIO_SetBits(PORT, ENABLE);
}

void disable_ESP() {
	GPIO_ResetBits(PORT, ENABLE);
}

void reset_ESP() {
	GPIO_ResetBits(PORT, RESET);
	Delay_ms(100);
	GPIO_SetBits(PORT, RESET);
}

void esp_init(void) {
	disable_ESP();   // EN拉低
	reset_ESP();     // RST复位
	Delay_ms(100);
	enable_ESP();    // EN拉高
	Delay_ms(2000);  // 等待ESP启动
}

void clean_esp_buff() {
	Preserved = 0;
	memset(RecvArray, 0, sizeof(RecvArray));
}

char test_AT() {
	clean_esp_buff();
	Serial_SendString("AT\r\n");
	Delay_ms(500);
	if (strstr((char *)RecvArray, "OK"))
		return 1;
	else
		return 0;
}

char link_internet(char *WIFI_name, char *password) {
	Serial_SendString("AT+CWMODE=1\r\n");
	Delay_ms(250);
	char link_wifi_cmd[64] = {0};
	sprintf(link_wifi_cmd, "AT+CWJAP=\"%s\",\"%s\"\r\n", WIFI_name, password);
	Serial_SendString(link_wifi_cmd);
	Delay_ms(250);
	for (char i = 0; i < 15; i++) {
		clean_esp_buff();
		Serial_SendString("AT+CIPSTATUS\r\n");
		Delay_ms(1000);
		if (!strstr((char *)RecvArray, "busy"))
			break;
	}
	OLED_ShowString(2,1,(char *)RecvArray);
	if (strstr((char *)RecvArray, "STATUS:2"))
		return 1;
	else
		return 0;
}

void invoke_website(char *location_name, char *request) {
    char link_website_cmd[64] = {0};
    char describe_msg_len[32];
    clean_esp_buff(); 
    sprintf(link_website_cmd, "AT+CIPSTART=\"TCP\",\"%s\",80\r\n", location_name);
    Serial_SendString(link_website_cmd);
    Delay_ms(3000);
    OLED_ShowString(1,1,(char *)RecvArray);
    clean_esp_buff(); 
    sprintf(describe_msg_len, "AT+CIPSEND=%d\r\n", strlen(request));
    Serial_SendString(describe_msg_len);
    Delay_ms(3000);
    clean_esp_buff(); 
		OLED_Clear();
    Serial_SendString(request);
    Delay_ms(5000);
    OLED_ShowString(1, 1, (char *)RecvArray);
}

void close_link(){
	Serial_SendString("AT+CIPCLOSE\r\n");
}

/*int main() {
	wireless_gpio_init();
	OLED_Init();
	Serial_Init();
	esp_init();
	test_AT();
	char x=link_internet("iQOO Z9 Turbo", "runc754scr79x9g");
	OLED_ShowNum(1,1,x,5);
	invoke_website("www.baidu.com","GET / HTTP/1.1\r\nHost: www.baidu.com\r\nConnection: close\r\n\r\n");
	close_link();
}*/
