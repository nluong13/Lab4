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
#define OPCODE (*(volatile unsigned int *)0b010000)
// Temperature sensor registers
#define Temp (*(volatile unsigned int *)0x00)

// pin configuration for LPC1769
void pinConfig() {	
	PINSEL1 |= (1<<22); 
	PINSEL1 &= ~(1<<23);
	PINSEL1 |= (1<<24); 
	PINSEL1 &= ~(1<<25);	
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

// start condition
void i2cStart() {
	I2C0CONSET = (1<<3); // set SI bit
	I2C0CONSET = (1<<5); // set STA bit
	I2C0CONCLR = (1<<3); // clear SI bit

	while ((I2C0CONSET >> 3) & 1 != 1) { // wait for SI bit to return to 0
	    I2C0CONCLR = (1<<5); // clear STA bit
	}
}

// stop condition
int i2cStop() {
	I2C0CONSET = (1<<4); // set STO bit
	I2C0CONCLR = (1<<3); // clear SI bit
	
	while ((I2C0CONSET >> 4) & 1 = 1) {} // wait for STO bit to return to 0
}

// write function
void i2cWrite(int data) {
	I2C0DAT = data; // assign data to I2C0DAT reg
	I2C0CONCLR = (1<<3); // clear SI bit

	while ((I2C0CONSET >> 3) & 1 != 1) { // wait for SI bit to return to 1
	}
}

// read function
int i2cRead(int read) {
	if (read) { // if final read, clear AA bit
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


int main() {

	pinConfig();
	
    while(1) {

    	// read 
    	start();
    	i2cWrite(i2cRead(1));
    	
      }
}