#include "mxc_device.h"
#include "mxc_delay.h"
#include "gpio.h"
#include <stdio.h>

// ==============================================================================
// Configuration
// ==============================================================================
#define NUM_LEDS 8
#define DELAY_MS 250  // ~250ms per step

// DIP Switch
#define SWITCH_PORT MXC_GPIO2
#define SWITCH_PIN  MXC_GPIO_PIN_3
#define SWITCH_MASK (MXC_GPIO_PIN_3)

// ==============================================================================
// LED pin mapping
// ==============================================================================
typedef struct {
    mxc_gpio_regs_t *port;
    uint32_t mask;
} gpio_map_t;

const gpio_map_t led_pins[NUM_LEDS] = {
    { MXC_GPIO1, MXC_GPIO_PIN_6  },
    { MXC_GPIO0, MXC_GPIO_PIN_9  },
    { MXC_GPIO0, MXC_GPIO_PIN_8  },
    { MXC_GPIO0, MXC_GPIO_PIN_11 },
    { MXC_GPIO0, MXC_GPIO_PIN_19 },
    { MXC_GPIO3, MXC_GPIO_PIN_1  },
    { MXC_GPIO0, MXC_GPIO_PIN_16 },
    { MXC_GPIO0, MXC_GPIO_PIN_17 }
};

// ==============================================================================
// Global Sequence State
// ==============================================================================
int step = 0;
int mode = 0; // 0 = Sequence 1 (Edges→Center), 1 = Sequence 2 (Center→Edges)
int last_switch = 1;

// ==============================================================================
// Function Prototypes
// ==============================================================================
void init_leds(void);
void init_switch(void);
int  read_switch(void);
void update_leds(uint8_t pattern);
void print_pattern(uint8_t pattern);
void reset_sequences(void);

// ==============================================================================
// LED sequences
// ==============================================================================
const uint8_t seq_edges_to_center[4] = {0xFF, 0x7E, 0x3C, 0x18}; // 11111111 → 00011000
const uint8_t seq_center_to_edges[4] = {0x18, 0x3C, 0x7E, 0xFF}; // 00011000 → 11111111

// ==============================================================================
// MAIN
// ==============================================================================
int main(void)
{
    printf("SYSTEM START\n");

    init_leds();
    init_switch();
    reset_sequences();

    while (1) {
        int sw_state = read_switch();

        // toggle sequence on switch press (active low)
        if (sw_state == 0 && last_switch == 1) {
            mode ^= 1;  // toggle sequence
            step = 0;
            printf("SWITCH PRESSED: SEQUENCE %d\n", mode + 1);

            // wait until release
            while (read_switch() == 0);
            MXC_Delay(20000);
        }
        last_switch = sw_state;

        // update LEDs
        uint8_t pattern = (mode == 0) ? seq_edges_to_center[step] : seq_center_to_edges[step];
        update_leds(pattern);
        print_pattern(pattern);

        step = (step + 1) % 4;
        MXC_Delay(2000000); // ~250ms
    }

    return 0;
}

// ==============================================================================
// GPIO Initialization
// ==============================================================================
void init_leds(void)
{
    mxc_gpio_cfg_t cfg;
    for (int i = 0; i < NUM_LEDS; i++) {
        cfg.port  = led_pins[i].port;
        cfg.mask  = led_pins[i].mask;
        cfg.func  = MXC_GPIO_FUNC_OUT;
        cfg.pad   = MXC_GPIO_PAD_NONE;
        cfg.vssel = MXC_GPIO_VSSEL_VDDIOH;

        MXC_GPIO_Config(&cfg);
        MXC_GPIO_OutClr(led_pins[i].port, led_pins[i].mask);
    }
}

void init_switch(void)
{
    mxc_gpio_cfg_t sw;
    sw.port  = SWITCH_PORT;
    sw.mask  = SWITCH_MASK;
    sw.func  = MXC_GPIO_FUNC_IN;
    sw.pad   = MXC_GPIO_PAD_PULL_UP;
    sw.vssel = MXC_GPIO_VSSEL_VDDIO;

    MXC_GPIO_Config(&sw);
}

// ==============================================================================
// Switch Read
// ==============================================================================
int read_switch(void)
{
    return (SWITCH_PORT->in & SWITCH_MASK) ? 1 : 0;
}

// ==============================================================================
// LED Output
// ==============================================================================
void update_leds(uint8_t pattern)
{
    for (int i = 0; i < NUM_LEDS; i++) {
        if (pattern & (1 << i))
            MXC_GPIO_OutSet(led_pins[i].port, led_pins[i].mask);
        else
            MXC_GPIO_OutClr(led_pins[i].port, led_pins[i].mask);
    }
}

void print_pattern(uint8_t pattern)
{
    for (int i = 7; i >= 0; i--) {
        printf("%c", (pattern & (1 << i)) ? '1' : '0');
    }
    printf("\n");
}

// ==============================================================================
// Reset Sequence
// ==============================================================================
void reset_sequences(void)
{
    step = 0;
    mode = 0;
    for (int i = 0; i < NUM_LEDS; i++)
        MXC_GPIO_OutClr(led_pins[i].port, led_pins[i].mask);
}