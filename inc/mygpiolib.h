/**
 * my-gpio-lib - an abstraction over libopencm3/stm32/gpio
 * suted for Global Logic Started Kit
 */
#include <libopencm3/stm32/gpio.h>
#include <stdint.h>
#include <stdbool.h>

#define MGL_DEFAULT 0xff
/**
 * struct pin_s.
 * @gpio: Port from A to K.
 *        Size: 4 bits
 *        A = 0000, B = 0001, C = 0010, D = 0011, E = 0100,
 *        F = 0101, G = 0111, H = 1000, I = 1001, J = 1010,
 *        K = 1011, 1100 - 1111 are invalid codes
 * @pin: pin of port from 0 to 15
 *       Size: 4 bits
 * @reserved: reserved for future use
 *      Size: 7 bits
 *
 *
 * pin_s is a packed structure.
 */
 struct  __attribute__((packed, aligned(1))) mgl_pin {
     uint16_t port : 4;
     uint16_t pin : 4;
     uint16_t inverse : 1;
     uint16_t reserved : 7;
 };

typedef struct mgl_pin  mgl_pin;

 enum ports {
 	MGL_PORT_A = 0,
 	MGL_PORT_B = 1,
 	MGL_PORT_C = 2,
 	MGL_PORT_D = 3,
 	MGL_PORT_E = 4,
 	MGL_PORT_F = 5,
 	MGL_PORT_G = 6,
 	MGL_PORT_H = 7,
 	MGL_PORT_I = 8,
 	MGL_PORT_J = 9,
 	MGL_PORT_K = 10
 };

// some STM32F4DISCOVERY peripheral devices' pins
 extern const mgl_pin mgl_led_orange;
 extern const mgl_pin mgl_led_red;
 extern const mgl_pin mgl_led_green;
 extern const mgl_pin mgl_led_blue;
 extern const mgl_pin mgl_btn_usr;
// some GLSK peripheral devices' pins
 extern const mgl_pin mgl_btn_swt1;
 extern const mgl_pin mgl_btn_swt2;
 extern const mgl_pin mgl_btn_swt3;
 extern const mgl_pin mgl_btn_swt4 ;
 extern const mgl_pin mgl_btn_swt5;

 /**
  * An abstraction over basic GPIO functions.
  */
 void mgl_clear(mgl_pin peripheral_device);

 void mgl_set(mgl_pin peripheral_device);

 void mgl_toggle(mgl_pin peripheral_device);

 void mgl_set_value(mgl_pin periph, bool value);

 uint16_t mgl_port_read(mgl_pin periph);

 void mgl_port_write(mgl_pin periph, uint16_t data);

 void mgl_mode_setup_default(mgl_pin periph);

void mgl_mode_setup(mgl_pin periph, uint8_t mode, uint8_t pull_up_down);

/**
 * Functions for work with buttons.
 */
bool mgl_is_btn_pressed(mgl_pin btn);
