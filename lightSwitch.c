/*
 * lightSwitch.c
 * Author : Zachary Mineiko Dawson Crowley
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#define PWM_TOP (39999u) // value for setting input capture reg
#define SEVRO_MIN (1999u) // minimum servo value
#define SEVRO_MAX  (3999u) // max servo value

bool isUp = true; // variable for state of switch
const uint8_t tolerance = 100; // for signal processing
volatile static bool PWMState = false;

uint16_t readIRSignal(void); // returns values of IR sensor
void initServo(void); // initializes servo
void blink(void);//strobes led
void pwm_init(void); // sets up fast pwm mode
static void update_pwm(uint16_t i); // updates registers
void flickUp(void); // rotates servo cw
void flickDown(void); // rotates servo cw
void servo_set(uint16_t deg,uint16_t max_deg); // sets servo to a certain degree (target degree, max degree)

int main(void)
{
	pwm_init(); // config fast PWM mode
	sei(); // enable interrupts
	servo_set(0,180); // set servo to home position
	int16_t i = 0; 
    DDRB |=(1<<PB4); // Set output
     
	_delay_ms(1000);
	DDRD &= ~(1 << PD3); // set sensor pin (D3) as input 
	blink(); // strobe leb indicating loop starts
	while (1)
	{
		uint16_t sig = readIRSignal(); // get value of sensor
		if(sig>tolerance){ // if signal exceeds tolerance 
			PORTB |= (1 << PB4); // Turn LED on
			if(isUp){
				flickDown(); // rotate servo
				isUp = false;
			}else{
				flickUp();	
				isUp = true;
			}
			_delay_ms(1000);
			PORTB &= ~(1 << PB4); // turn LED off
		}
    }
}
// returns pulse of the IR signal
uint16_t readIRSignal(void) {
	while (PIND & (1 << PD3));// wait for a falling edge (IR burst start)
	uint16_t dur = 0;
	while (!(PIND & (1 << PD3))) { // increment duration of pulse
		_delay_us(50); 
		dur++;
	}
	return dur;
}

void set_servo_position(uint16_t pw) {
	OCR1A = pw; // set the compare value
}

//strobes led
void blink(void){
	_delay_ms(500);
	PORTB |= (1 << PB4); // pull pin D12 low, Turn LED ON
	_delay_ms(500);
	PORTB &= ~(1 << PB4);
}

//==================timer1/servo stuff=======================

// interupt to update state for PWM
ISR(TIMER1_OVF_vect){
	PWMState = false;
}
// setup fast pwm mode, set compare values
void pwm_init(void){
	DDRB |= (1 << DDB1);
	TCCR1A = (0b10 << COM1A0) | (0b00 << COM1B0) | (0b10 << WGM10); // config pwm fast mode
	TIMSK1 = (1 << TOIE1) | (1 << OCIE1A); // config timer interrupt
	
	// set output register compare values
	OCR1AH = (SEVRO_MIN & 0xFF00) >> 8; 
	OCR1AL = (SEVRO_MIN & 0x00FF);
	// set input capture registers
	ICR1H = (PWM_TOP & 0xFF00) >> 8;
	ICR1L = (PWM_TOP & 0x00FF);
	TCCR1B = (0b11 << WGM12) | (0b010 << CS10);
	
}
// update servo
static void update_pwm(uint16_t i){
	PWMState = true;
	while(PWMState != false);
	OCR1AH = (i & 0xFF00) >> 8;
	OCR1AL = (i & 0x00FF);
}
// rotate servo ~45 deg cw
void flickUp(void){
	uint16_t i = 0;
	for(i = SEVRO_MIN; i <= SEVRO_MAX-300;i+=50){
		update_pwm(i);
		_delay_ms(40);
	}
}
// rotate servo ~45 deg ccw
void flickDown(void){
	uint16_t i = 0;
	for(i = SEVRO_MAX-300; i >= SEVRO_MIN;i-=50){
		update_pwm(i);
		_delay_ms(40);
	}
}
// set servo to given deg
void servo_set(uint16_t deg,uint16_t max_deg){
	float set = (float)deg / (float)max_deg;
	set = (((float)SEVRO_MAX-(float)SEVRO_MIN)*set) + (float)SEVRO_MIN; // calculate point for servo
	uint16_t point = (uint16_t)set;
	update_pwm(point);
	
}
