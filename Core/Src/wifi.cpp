/******************************************************************************************
 * 
 * 
 *   @By: Aditya-Kumar-Mishra
 *   @Date: 30/Sep/2020
 * 
 * 
 * ****************************************************************************************/
#include "wifi.h"
void wifi_setup(){
      // commands to test the Wi-Fi startup connection
  HAL_UART_Transmit(&huart1, (uint8_t *)"AT\r\n",sizeof("AT\r\n"), 100);
  HAL_UART_Receive(&huart1, (uint8_t *)Response,sizeof(Response), 200);
  memset(Response,0,sizeof(Response));
  HAL_Delay(1000);

  // command to set the operating mode of Wi-Fi mosule
  HAL_UART_Transmit(&huart1, (uint8_t *)"AT+CWMODE=1\r\n",sizeof("AT+CWMODE=1\r\n"), 200);
  HAL_UART_Receive(&huart1, (uint8_t *)Response,sizeof(Response), 200);
  memset(Response,0,sizeof(Response));
  HAL_Delay(1000);
    sprintf(wifi_temp,"AT+CWJAP=\"%s\",\"%s\"\r\n",wifi_SSID,wifi_password);
  // command to connect the nearest access point using SSID & Password
  HAL_UART_Transmit(&huart1, (uint8_t *)wifi_temp,sizeof(wifi_temp), 200);
  HAL_UART_Receive(&huart1, (uint8_t *)Response,sizeof(Response), 1000);
  memset(Response,0,sizeof(Response));
  HAL_Delay(1000);
}
void send_data(char *field,float data){
      /*command to update data over thingspeak.com*/
  char get_cmd[50];
  sprintf(get_cmd,"%s%s=%f",api,field,data);
  uint16_t cmd_length = strlen(get_cmd);

  /*Initiating the TCP/IP connection for thingspeak.com over http port*/
  HAL_UART_Transmit(&huart1, (uint8_t *)"AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n", strlen("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n"), 1000);
  HAL_UART_Receive(&huart1, (uint8_t *)Response,sizeof(Response), 1000);
  memset(Response,0,sizeof(Response));
  HAL_Delay(1500);

  /* sending the data size to be inserted in the update command*/
  HAL_UART_Transmit(&huart1, (uint8_t *) "AT+CIPSEND=51\r\n\r\n", strlen("AT+CIPSEND=51\r\n\r\n"), 1000);
  HAL_UART_Receive(&huart1, (uint8_t *)Response,sizeof(Response), 1000);

  /* sending the update command over thingspeak server*/
  HAL_UART_Transmit(&huart1, (uint8_t *) &get_cmd, cmd_length, 1000);
  HAL_UART_Transmit(&huart1, (uint8_t *) "\r\n", strlen("\r\n"), 1000);
  HAL_UART_Receive(&huart1, (uint8_t *)Response,sizeof(Response), 1000);
  HAL_Delay(1500);
  memset(Response,0,sizeof(Response));

  /*closing the TCP/IP connection after sending the data*/
  HAL_UART_Transmit(&huart1, (uint8_t *) "AT+CIPCLOSE\r\n", strlen("AT+CIPCLOSE\r\n"), 1000);
  HAL_UART_Receive(&huart1, (uint8_t *)Response,sizeof(Response), 1000);
    HAL_Delay(1500);
    memset(Response,0,sizeof(Response));
}