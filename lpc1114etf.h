/* 
 * Definicija imena pinova na ploci LPC1114ETF
 * 
 * Samim Konjicija, 25.3.2018. godine
 * 
 */

// Port sa LED diodama na LPC1114ETF
#define LED0        P0_0
#define LED1        P0_1
#define LED2        P0_2
#define LED3        P0_3
#define LED4        P0_4
#define LED5        P0_5
#define LED6        P0_6
#define LED7        P0_7

// LED disable LPC1114ETF - LED_ACT=0 - LED ukljucene, LED_ACT=1 - LED iskljucene
#define LED_ACT P1_5
    
// Tasteri LPC1114ETF
#define Taster_1    P0_8
#define Taster_2    P0_9

// Analogni ulazi na LPC1114ETF
#define AD0         P0_11
#define AD1         P1_0
#define AD2         P1_1
#define AD3         P1_2
#define AD5         P1_4

// PWM izlazi na LPC1114ETF
#define PWM0        P0_1
#define PWM1        P1_9
    
// Komunikacijski portovi na LPC1114ETF
#define I2C_SCL     P0_4
#define I2C_SDA     P0_5
#define SPI_SCK     P0_6
#define SPI_MISO    P0_8
#define SPI_MOSI    P0_9
#define TX          P1_7
#define RX          P1_6

// LPC1114ETF koristi P0_0 kao GPIO
#define dp23        P0_0
