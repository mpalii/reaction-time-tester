#ifndef _GPIO_H_
#define _GPIO_H_

/**
 * @author Maksym Palii
 * @brief GPIO mapping
 * @version 1.0
 * @date 13-July-2023
 * 
 * @details
 * General purpose IO pins assignment and handling
 * ATmega328P has 23 general purpose IO lines
 * Two 8bit ports (B, D) and one 7bit port (C)
 */

/**
 * @brief GPIO initialization function prototype.
 * @see gpio.c for implementation 
 * @param -
 * @return -
 */
void gpio_init(void);

#include <avr/io.h>

/**
 * @brief Generic pin map table.
 */
#define _PB0    0U
#define _PB1    1U
#define _PB2    2U
#define _PB3    3U
#define _PB4    4U
#define _PB5    5U
#define _PB6    6U
#define _PB7    7U

#define _PC0    8U
#define _PC1    9U
#define _PC2    10U
#define _PC3    11U
#define _PC4    12U
#define _PC5    13U
#define _PC6    14U
// skip 15U value because PC7 pin doesn't exist physically

#define _PD0    16U
#define _PD1    17U
#define _PD2    18U
#define _PD3    19U
#define _PD4    20U
#define _PD5    21U
#define _PD6    22U
#define _PD7    23U

/**
 * @brief Physical hardware pin mapping
 */
#define _GPIO_UNUSED_01   _PB0
#define BUTTON            _PB1
#define LED               _PB2
#define BUZZER            _PB3
#define _GPIO_UNUSED_05   _PB4
#define _GPIO_UNUSED_06   _PB5
#define XTAL1             _PB6 // ignore in gpio.c (crystal oscillator)
#define XTAL2             _PB7 // ignore in gpio.c (crystal oscillator)

#define LCD_1602_RS       _PC0
#define LCD_1602_E        _PC1
#define _GPIO_UNUSED_11   _PC2
#define _GPIO_UNUSED_12   _PC3
#define _GPIO_UNUSED_13   _PC4
#define _GPIO_UNUSED_14   _PC5
#define RESET             _PC6

#define UART_RX           _PD0 // ignore in gpio.c (serial RX)
#define UART_TX           _PD1 // ignore in gpio.c (serial TX)
#define _GPIO_UNUSED_19   _PD2
#define _GPIO_UNUSED_20   _PD3
#define LCD_1602_PD4      _PD4
#define LCD_1602_PD5      _PD5
#define LCD_1602_PD6      _PD6
#define LCD_1602_PD7      _PD7

/**
 * Addresses of DDRx, PORTx and PINx registers have the same offset and equals to 3.
 * It is useful for determining a real register address during a bit manipulation
 * 
 * DDRB:0x04;  DDRC:0x07;  DDRD:0x0a     -> offset = 3U
 * PORTB:0x05; PORTC:0x08; PORTD:0x0b    -> offset = 3U
 * PINB:Ox03;  PINC:0x06;  PIND:0x09     -> offset = 3U
 * 
 * Port B will be a starting point for determining adresses of direction, port, and pin registers for other ports (C, and D)
 */
#define SFR_OFFSET        (3U)
#define INIT_DDR_ADDRESS  (&DDRB)
#define INIT_PORT_ADDRESS (&PORTB)
#define INIT_PIN_ADDRESS  (&PINB)

/**
 * The main idea is that every value from generic pin map table represents: 
 * pin number (0b00000111 a.k.a 7U mask); 
 * port number (0b00011000 a.k.a. 3U << 3U, or 3U << GPIO_PORT_OFFSET).
 * 
 * Port number * SFR_OFFSET gives offset for determining the real register address. 
 * It depends on initial DDR, PORT, and PIN addresses.
 */
#define GPIO_PORT_OFFSET  (3U)
#define GPIO_PORT_MASK    (3U << GPIO_PORT_OFFSET)
#define GPIO_PIN_MASK     (7U)

/**
 * @brief for extracting the physical (hardware) bit value from generic pin value
 */
#define GPIO_PIN_BIT(GPIO) (1 << (GPIO & GPIO_PIN_MASK))

/**
 * @brief for extracting the port number from the generic pin value
 */
#define GPIO_PORT(GPIO) ((GPIO & GPIO_PORT_MASK) >> GPIO_PORT_OFFSET)

#define DDR_ADDRESS(GPIO) (INIT_DDR_ADDRESS + (GPIO_PORT(GPIO) * SFR_OFFSET))
#define PORT_ADDRESS(GPIO) (INIT_PORT_ADDRESS + (GPIO_PORT(GPIO) * SFR_OFFSET))
#define PIN_ADDRESS(GPIO) (INIT_PIN_ADDRESS + (GPIO_PORT(GPIO) * SFR_OFFSET))

// GPIO state manipulation macros
#define GPIO_SET_OUTPUT(GPIO) (*DDR_ADDRESS(GPIO) |= GPIO_PIN_BIT(GPIO))
#define GPIO_SET_INPUT(GPIO) (*DDR_ADDRESS(GPIO) &= ~GPIO_PIN_BIT(GPIO))
#define GPIO_HIGH(GPIO) (*PORT_ADDRESS(GPIO) |= GPIO_PIN_BIT(GPIO))
#define GPIO_LOW(GPIO) (*PORT_ADDRESS(GPIO) &= ~GPIO_PIN_BIT(GPIO))
#define GPIO_GET_INPUT(GPIO) (*PIN_ADDRESS(GPIO) & GPIO_PIN_BIT(GPIO))

#endif /* _GPIO_H_ */
