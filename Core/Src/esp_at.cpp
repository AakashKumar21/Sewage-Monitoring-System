#include "esp_at.h"
#include "helper.h"

extern UART_HandleTypeDef huart3;

struct Commands{
    char *const WifiMode;
    char *const ConnectWifi;
    char *const Reset;
    char *const DisconnectWifi;
    char *const AT;
    char *const TCP_No_of_conn;
    char *const StartConn;
    char *const TcpConn;
    char *const SendData;
    char *const EndConn;
};

struct ThingspeakApi{
    char *const Part1; // GET /update?api_key=
    char *const Part2; // &field
};

Commands Cmd{
    .WifiMode =         "CWMODE_CUR=",       // Wifi Mode
    .ConnectWifi =      "CWJAP_CUR=",    // curr AP
    .Reset =            "RST",                 // Reset
    .DisconnectWifi =   "CWQAP",       // Disconnect
    .AT =               "AT+",                      // AT+
    .TCP_No_of_conn =   "CIPMUX=",     // no. of tcp conn
    .StartConn =        "CIPSTART=",          // Start conn
    .TcpConn =          "\"TCP\"",           // conn mode tcp
    .SendData =         "CIPSEND=",
    .EndConn =          "CIPEND"
};

ThingspeakApi Api{
    "GET /update?api_key=",
    "&field"
};

void ESP_AT::_setSingleConn(){
    // Set single connection
    serialPrint(Cmd.AT);                // AT+
    serialPrint(Cmd.TCP_No_of_conn);    // CIPMUX=
    serialPrint(0);
    serialPrint("\n");
}

ESP_AT::ESP_AT(UART_HandleTypeDef* uart):
_uart(uart),
_host{"\"api.thingspeak.com\""}
{
    _setSingleConn();
}

ESP_AT::ESP_AT(UART_HandleTypeDef* uart, char* ssid, char*pass, char* apiKey):
_uart(uart),
_ssid(ssid),
_pass(pass),
_apiKey(apiKey),
_host{"\"api.thingspeak.com\""}
{
    _setSingleConn();
}

void ESP_AT::setApiKey(char *key){
    _apiKey = key;
}


bool ESP_AT::WifiConnect(char* ssid, char *pass){
    _ssid = ssid;
    _ssid = pass;
    serialPrint(Cmd.AT);
    serialPrint(Cmd.ConnectWifi);

    serialPrint("\"");
    serialPrint(ssid);
    serialPrint("\"");
    serialPrint(",");

    serialPrint("\"");
    serialPrint(pass);
    serialPrint("\"");
    serialPrint("\n");

    return 0;
}

bool ESP_AT::updateValue(uint8_t field, int16_t data){
    if(field>8) return 0;
    uint8_t total_len = 53; // hardcoded for now, TODO
    // Connect to Thingspeak
    serialPrint(Cmd.AT);        // AT+
    serialPrint(Cmd.StartConn); // CIPSTART=
    serialPrint(Cmd.TcpConn);   // "TCP"
    serialPrint(",");           // ,
    serialPrint(_host);         // "api.thingspeak.com"
    serialPrint(",");           // ,
    serialPrint(80);           // 80
    serialPrint("\n");

    //Start sending data
    serialPrint(Cmd.AT);        // AT+
    serialPrint(Cmd.SendData);  // CIPSEND=
    serialPrint(total_len);           // int: len
    serialPrint("\n");

    // Update value
    serialPrint(Api.Part1);     // GET /update?api_key=
    serialPrint(_apiKey);       // XXXXXXXXXXXXXXXX
    serialPrint(Api.Part2);     // &field
    serialPrint(field);         // #
    serialPrint("=");           // =
    serialPrint(data);          // #
    serialPrint("\n");          // \n

    // End Connection
    serialPrint(Cmd.AT);        // AT+
    serialPrint(Cmd.EndConn);   // CIPEND
    serialPrint("\n");          // \n
}
bool disconnect(){return 0;};
bool restart(){return 0;};