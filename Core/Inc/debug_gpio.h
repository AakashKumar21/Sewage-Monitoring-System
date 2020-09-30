// Comment/uncomment below line to turn off/on debugging
#define DEBUG_BY_GPIO
#if defined(DEBUG_BY_GPIO)
#define DGPIO(x) HAL_GPIO_WritePin(DPORT, DPIN, x); HAL_Delay(100);
#endif
#if !defined(DEBUG_BY_GPIO)
#define DGPIO(x)
#endif

#define DPORT LED_GPIO_Port
#define DPIN LED_Pin
#define High GPIO_PIN_SET
#define Low GPIO_PIN_RESET