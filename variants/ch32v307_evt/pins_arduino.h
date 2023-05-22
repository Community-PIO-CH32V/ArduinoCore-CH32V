#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#define INT_PORT_A 0
#define INT_PORT_B 1
#define INT_PORT_C 2
#define INT_PORT_D 3

#define PC0 ((INT_PORT_C << 4u) | 0)
#define PC1 ((INT_PORT_C << 4u) | 1)

/* nothing yet. really */

// #define PIN_SPI_MISO  (12)
// #define PIN_SPI_SCK   (13)
// #define PIN_SPI_MOSI  (11)
// #define PIN_SPI_SS    (8)

// static const uint8_t SS   = PIN_SPI_SS;
// static const uint8_t MOSI = PIN_SPI_MOSI;
// static const uint8_t MISO = PIN_SPI_MISO;
// static const uint8_t SCK  = PIN_SPI_SCK;

// #define PIN_WIRE_SDA  (22)
// #define PIN_WIRE_SCL  (23)

// static const uint8_t SDA = PIN_WIRE_SDA;
// static const uint8_t SCL = PIN_WIRE_SCL;

#endif