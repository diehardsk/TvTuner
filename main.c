/*
 * TvTuner.c
 *
 * SETUP TUA6039 MIXER AND TDA9885 DEMODULATOR TO RECEIVE WFM (AUDIO) SIGNAL
 *
 */ 
#define F_CPU 4000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <i2cmaster.h>

#define PD_1(x) PORTD |=  (1 << (x))
#define PD_0(x) PORTD &= ~(1 << (x))
#define LED_on	PD_0(4)
#define LED_off PD_1(4)

#define TUA6039_ADDR 0xC2
#define BB 0x01 //4=HIGH(UHF), 2=MID, 1=LOW, 0=POWER_DOWN
#define CB 0b10000000	//50kHz ref divider ratio
#define RF_FREQ 93300	//kHz
#define IF_FREQ 41250	//kHz (M3565D SAW sound carrier)
#define LO RF_FREQ + IF_FREQ
#define N ((LO)/50)

#define TDA9885_ADDR 0x86
#define B_DATA 0b11001101
#define deemphasis 1
#define C_DATA 0b01010000 | (deemphasis<<5)
#define E_DATA 0b00000100

int main(void)
{
	// Port D initialization
	// Func7=In Func6=In Func5=In Func4=Out Func3=In Func2=In Func1=In Func0=In
	// State7=T State6=T State5=T State4=1 State3=T State2=T State1=T State0=T
	PORTD=0b00010000;
	DDRD =0b00010000;

	i2c_init();
    while (1) 
    {
// MIXER
		uint8_t DB1 = N / 256;
		uint8_t DB2 = N % 256;
		if (i2c_start(TUA6039_ADDR + I2C_WRITE) == 0)
		{
			i2c_write(DB1);i2c_write(DB2);i2c_write(CB);i2c_write(BB);
		}
		i2c_stop();
		_delay_ms(100);
		if (i2c_start(TUA6039_ADDR + I2C_READ) == 0)
		{
			uint8_t STATUS = i2c_readNak();
			if (STATUS & (1<<6)) LED_on; _delay_ms(10); LED_off;	//PLL locked?
		}

// FM DEMODULATOR
		if (i2c_start(TDA9885_ADDR + I2C_WRITE) == 0)
		{
			i2c_write(0x00);i2c_write(B_DATA);i2c_write(C_DATA);i2c_write(E_DATA);
		}
		i2c_stop();


		_delay_ms(2000);
    }
}

