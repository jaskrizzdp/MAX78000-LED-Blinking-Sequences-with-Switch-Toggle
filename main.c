//version2 - of led blinking file
/****************************************************************************** 
 * 8-LED Blinking Sequences with Switch Toggle (MAX78000)
 * Prints sequence to terminal and controls LEDs
 ******************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include "mxc_device.h"
#include "mxc_delay.h"
#include "gpio.h"
#include "gpio_reva.h"


/******************************************************************
 * GPIO Pin Definitions
 ******************************************************************/
#define LED_PORT    MXC_GPIO0
#define LED_PINS   (MXC_GPIO_PIN_5  | MXC_GPIO_PIN_6  | MXC_GPIO_PIN_7  | \
                    MXC_GPIO_PIN_8  | MXC_GPIO_PIN_9  | MXC_GPIO_PIN_11 | \
                    MXC_GPIO_PIN_16 | MXC_GPIO_PIN_17)

#define SWITCH_PORT MXC_GPIO1
#define SWITCH_PIN  MXC_GPIO_PIN_0

/******************************************************************
 * Writes an 8-bit pattern to the 8 LEDs
 ******************************************************************/
static void LED_Write(uint8_t value)
{
    uint32_t pin_mask = 0;

    if (value & 0x01) pin_mask |= MXC_GPIO_PIN_7;   // LED1
    if (value & 0x02) pin_mask |= MXC_GPIO_PIN_5;   // LED2
    if (value & 0x04) pin_mask |= MXC_GPIO_PIN_6;   // LED3
    if (value & 0x08) pin_mask |= MXC_GPIO_PIN_9;   // LED4
    if (value & 0x10) pin_mask |= MXC_GPIO_PIN_8;   // LED5
    if (value & 0x20) pin_mask |= MXC_GPIO_PIN_11;  // LED6
    if (value & 0x40) pin_mask |= MXC_GPIO_PIN_16;  // LED7
    if (value & 0x80) pin_mask |= MXC_GPIO_PIN_17;  // LED8

    MXC_GPIO_OutSet(LED_PORT, pin_mask);
    MXC_GPIO_OutClr(LED_PORT, LED_PINS & ~pin_mask);
}

/******************************************************************
 * Debounce Switch (returns 1 when pressed)
 ******************************************************************/
int SwitchPressed(void)
{
    if (MXC_GPIO_InGet(SWITCH_PORT, SWITCH_PIN) == 0) {
        MXC_Delay(20000);  // debounce delay
        if (MXC_GPIO_InGet(SWITCH_PORT, SWITCH_PIN) == 0)
            return 1;
    }
    return 0;
}

/******************************************************************
 * Converts byte to string of 0/1 for printing
 ******************************************************************/
void PrintPattern(uint8_t value)
{
    for (int i = 7; i >= 0; i--) {
        printf("%c", (value & (1 << i)) ? '1' : '0');
    }
    printf("\n");
}

/******************************************************************
 * MAIN PROGRAM
 ******************************************************************/
int main(void)
{
    printf("SYSTEM START\n");

    // Configure LED pins
    mxc_gpio_cfg_t led_cfg = {
        .port = LED_PORT,
        .mask = LED_PINS,
        .func = MXC_GPIO_FUNC_OUT,
        .pad  = MXC_GPIO_PAD_NONE,
        .vssel = MXC_GPIO_VSSEL_VDDIO
    };
    MXC_GPIO_Config(&led_cfg);

    // Configure Switch pin
    mxc_gpio_cfg_t sw_cfg = {
        .port = SWITCH_PORT,
        .mask = SWITCH_PIN,
        .func = MXC_GPIO_FUNC_IN,
        .pad  = MXC_GPIO_PAD_PULL_UP,
        .vssel = MXC_GPIO_VSSEL_VDDIO
    };
    MXC_GPIO_Config(&sw_cfg);

    // LED sequences
    uint8_t seq_edges_to_center[4]  = {0xFF, 0x7E, 0x3C, 0x18}; // Sequence 1
    uint8_t seq_center_to_edges[4]  = {0x18, 0x3C, 0x7E, 0xFF}; // Sequence 2

    int step = 0;
    int mode = 0;  // 0 = Sequence 1, 1 = Sequence 2

    while (1) {
        // Check switch to toggle mode
        if (SwitchPressed()) {
            mode ^= 1; // toggle mode
            printf("Switch pressed! Now in mode: %s\n",
                mode ? "Sequence 2 (Center→Edges)" : "Sequence 1 (Edges→Center)");

            // Wait until switch released
            while (MXC_GPIO_InGet(SWITCH_PORT, SWITCH_PIN) == 0);
            MXC_Delay(20000);
        }

        // Output LED pattern
        uint8_t pattern = (mode == 0) ? seq_edges_to_center[step] : seq_center_to_edges[step];
        LED_Write(pattern);
        PrintPattern(pattern);  // print to serial

        step = (step + 1) % 4;
        MXC_Delay(250000); // 250ms delay
    }

    return 0;
}
