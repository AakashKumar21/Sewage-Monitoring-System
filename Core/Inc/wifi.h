/******************************************************************************************
 * 
 * 
 *   @By: Aditya-Kumar-Mishra
 *   @Date: 30/Sep/2020
 * 
 * 
 * ****************************************************************************************/
#ifndef _wifi_h_
#define _wifi_h_
#define s_temp "field1"
#define s_hum   "field2"
#define s_depth "field3"
#define s_meth  "field4"
char wifi_temp[50];
char wifi_SSID[] = {"(A)ditya"};
char wifi_password[] = {"lololoXd"};
void wifi_setup();
void send_data(char *field,float data);
#endif /*_wifi_h_*/
