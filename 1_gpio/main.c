#include <stdint.h>

#define GPIO ((NRF_GPIO_REGS*)0x50000000)

typedef struct {
	volatile uint32_t RESERVED0[321];
	volatile uint32_t OUT;
	volatile uint32_t OUTSET;
	volatile uint32_t OUTCLR;
	volatile uint32_t IN;
	volatile uint32_t DIR;
	volatile uint32_t DIRSET;
	volatile uint32_t DIRCLR;
	volatile uint32_t LATCH;
	volatile uint32_t DETECTMODE;
	volatile uint32_t RESERVED1[118];
	volatile uint32_t PIN_CNF[32];
} NRF_GPIO_REGS;

void led_on(){
    // Using OUTCLR register to clear bits (turn on LED if active low)
	GPIO->OUTCLR |= (1 << 17) | (1 << 18) | (1 << 19) | (1 << 20);
    // Alternative: directly clear bits in OUT register using bitwise AND with inverted mask.
    // This assumes active low logic and that modifying OUT directly is safe.
    // GPIO->OUT = GPIO->OUT & ~((1 << 17) | (1 << 18) | (1 << 19) | (1 << 20));  // Clear bits via AND with ~mask
};

void led_off(){
    // Using OUTSET register to set bits (turn off LED if active low)
	GPIO->OUTSET |= (1 << 17) | (1 << 18) | (1 << 19) | (1 << 20);
    // Alternative: directly set bits in OUT register using OR.
    // GPIO->OUT = GPIO->OUT | ((1 << 17) | (1 << 18) | (1 << 19) | (1 << 20));  // Set bits via OR with mask
};

void button_init(){ 
	// Configure buttons with a given setting (3 << 2 sets bits 2 and 3, for example)
	GPIO->PIN_CNF[13] = (3 << 2);  // Sets button 1 configuration
	GPIO->PIN_CNF[14] = (3 << 2);  // Sets button 2 configuration
	// Alternative: if different configuration is needed, adjust the mask.
	// For instance, if only a single bit is needed: GPIO->PIN_CNF[13] = (1 << 2); 
	// (This depends on the hardware design in the datasheet.)
};

int main(){
	// Configure LED Matrix pins as outputs and clear them initially.
	for(int i = 17; i <= 20; i++){
		// Set direction of pin i to output.
		GPIO->DIRSET = (1 << i);  
		// Clear output for pin i.
		GPIO->OUTCLR = (1 << i);
        // Alternative: setting direction using a single OR operation:
        // GPIO->DIR = GPIO->DIR | (1 << i);  // This would be equivalent if DIR were modifiable directly.
	}

	// Configure buttons -> see button_init()
	button_init();

	int sleep = 0;
	while(1){

		/* Check if button 1 is pressed;
		 * turn on LED matrix if it is.
         * Using bitwise AND to check a bit: */
		if(!(GPIO->IN & (1 << 13))){
			led_on();
		};

		/* Check if button 2 is pressed;
		 * turn off LED matrix if it is.
         * Again using bitwise AND to check the bit: */
		if(!(GPIO->IN & (1 << 14))){
			led_off();
		};

		// Simple delay loop.
		sleep = 10000;
		while(--sleep);
	}
	return 0;
}
