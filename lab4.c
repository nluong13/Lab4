// Lab 4
// Port 0 registers
#define PINSEL1 (*(volatile unsigned int *)0x4002C004)
// I2C registers
#define PCLKSEL0 (*(volatile unsigned int *)0x400FC1A8)
#define I2C0SCLH (*(volatile unsigned int *)0x4001C010)
#define I2C0SCLL (*(volatile unsigned int *)0x4001C014)
#define I2C0CONSET (*(volatile unsigned int *)0x4001C000)
#define I2C0CONCLR (*(volatile unsigned int *)0x4001C018)
#define I2C0DAT (*(volatile unsigned int *)0x4001C008)
#define I2C0STAT (*(volatile unsigned int *)0x4001C004)
#define T0TCR (*(volatile unsigned int *)0x40004004)
#define T0TC (*(volatile unsigned int *)0x40004008)
// MCP23017 registers
#define IODIRA (*(volatile unsigned int *)0x00)
#define IODIRB (*(volatile unsigned int *)0x01)
#define GPIOA (*(volatile unsigned int *)0x12)
#define GPIOB (*(volatile unsigned int *)0x13)
#define MCP_OPCODE (*(volatile unsigned int *)0b0100000)
// Temperature sensor registers
#define PCT_TEMP (*(volatile unsigned int *)0x00)
#define PCT_ADDR (*(volatile unsigned int *)0b1001000)
// global variables -- MIGHT NEED TO CHANGE TO PREPROCESSOR DIRECTIVES IF NOT WORKING
const volatile int WRITE = 0;
const volatile int READ = 1;

// configure LPC1769
void configLPC() {
	// configure SDA0	
	PINSEL1 |= (1<<22); 
	PINSEL1 &= ~(1<<23);
	// configure SCL0
	PINSEL1 |= (1<<24); 
	PINSEL1 &= ~(1<<25);	

	PCLKSEL0 &= ~(1<<14) & ~(1<<15);
	I2C0SCLL = 5;
	I2C0SCLH = I2C0SCLL + 1;

	I2C0CONCLR = (1<<6); // clear enable bit
    I2C0CONSET = (1<<6); // set enable bit
}

// configure MCP23017
void configMCP() {
	// set IODIRA and IODIRB as outputs (0=output)
	IODIRA &= ~(1<<1) & ~(1<<2) & ~(1<<3) & ~(1<<4) & (1<<5) & ~(1<<6) & ~(1<<7);
	IODIRB &= ~(1<<1) & ~(1<<2) & ~(1<<3) & ~(1<<4) & (1<<5) & ~(1<<6) & ~(1<<7);
}

// wait function
 void wait_us(int us) {
  int start = T0TC;  // note starting time
  T0TCR |= (1<<0);   // start timer
  while ((T0TC-start)<us) {} // wait for time to pass
}
void wait(float sec) {
  wait_us(sec*1000000); // convert seconds to microseconds
}

// I2C start condition
void i2cStart() {
	I2C0CONSET = (1<<3); // set SI bit
	I2C0CONSET = (1<<5); // set STA bit
	I2C0CONCLR = (1<<3); // clear SI bit

	while ((I2C0CONSET >> 3) & 1 != 1) { // wait for SI bit to return to 1
		I2C0CONCLR = (1<<5); // clear STA bit
	}
}

// I2C stop condition
int i2cStop() {
	I2C0CONSET = (1<<4); // set STO bit
	I2C0CONCLR = (1<<3); // clear SI bit
	
	while ((I2C0CONSET >> 4) & 1 = 1) {} // wait for STO bit to return to 0
}

// I2C write function
void i2cWrite(int data) {
	I2C0DAT = data; // assign data to I2C0DAT reg
	I2C0CONCLR = (1<<3); // clear SI bit

	while ((I2C0CONSET >> 3) & 1 != 1) { // wait for SI bit to return to 1
	}
}

// I2C read function
int i2cRead(int finalRead) {
	if (finalRead) { // if final read of transaction, clear AA bit
		I2C0CONCLR = (1<<2);
	} 
    else { // otherwise, set it
		I2C0CONSET = (1<<2);
	}

	I2C0CONCLR = (1<<3); // clear SI bit

	while ((I2C0CONSET >> 3) & 1 != 1) {} // wait for SI bit to return to 1

	int save = I2C0DAT; // asign value in I2C0DAT to another variable
	return save;
}

// read temperature
int readTemp(int sw_toggle) {
	int tempC;
	int tempF;

	i2cStart(); // start
	i2cWrite((PCT_ADDR<<1) | WRITE); // address, r/w = 0;
	i2cWrite(PCT_TEMP); // pointer byte -- COULD BE WRONG POINTER BYTE
	i2cStop(); // stop
	i2cStart(); // restart
	i2cWrite((PCT_ADDR<<1) | READ); // address, r/w = 1;
	tempC = i2cRead(1); // data byte
	i2cStop(); // stop

	// convert from C to F if switch toggled
	if (sw_toggle) {
		tempF = tempConvert(tempC);
		return tempF;
	}
	else{
		return tempC;
	}
}

// convert temperature from C to F
int tempConvert(int tempC) {
	int tempF = tempC * 1.8 + 32;
	return tempF;
}

// read switch
int readSW() {
	int sw;

	i2cStart(); // start
	i2cWrite((MCP_OPCODE<<1) | WRITE); // opcode, r/w = 0;
	i2cWrite(GPIOB); // address
	i2cStop(); // stop
	i2cStart(); // restart
	i2cWrite((MCP_OPCODE<<1) | READ); // opcode, r/w = 1;
	sw = (i2cRead(1)>>7); // Dout stored in sw (switch connected to Port B bit 7)
	i2cStop(); // stop

	return sw;
}

// display numbers function
int disp(int number) {
	switch(number) {
		case 0: return 0b1000000;
        case 1: return 0b1111001;
        case 2: return 0b0100100;
        case 3: return 0b0110000;
        case 4: return 0b0011001;
        case 5: return 0b0010010;
        case 6: return 0b0000010;
        case 7: return 0b1111000;
        case 8: return 0b0000000;
        case 9: return 0b0011000;
	}
	return 0b1000000;
}

// display temperature function
void dispTemp(int temp) {
	// write ones place
	i2cStart(); // start
	i2cWrite((MCP_OPCODE<<1) | WRITE); // opcode, r/w = 0;
	i2cWrite(GPIOB); // addr (port B)
	i2cWrite((disp(temp%10))); // Din (ones digit)
	i2cStop(); // stop

	// write tens place
	i2cStart(); // start
	i2cWrite((MCP_OPCODE<<1) | WRITE); // opcode, r/w = 0;
	i2cWrite(GPIOA); // addr (port A)
	i2cWrite((disp(temp/10))); // Din (tens digit)
	i2cStop(); // stop
} 



int main() {
	// configure LPC and MCP
	configLPC();
	configMCP();
	
	int sw;
	int temp;
	int tempC = 1; // starts in celsius

    while(1) {
		// read switch input
		sw = readSW();
		// if sw pressed, toggle temperature unit
		if (sw == 1) tempC != tempC;
		// read data from temperature sensor
		temp = readTemp(tempC);
		// write data to display
		dispTemp(temp);
		// wait
		wait(0.2);
    	
    }
}