#define FIO0DIR (*(volatile unsigned int *)0x2009c000)
#define FIO0PIN (*(volatile unsigned int *)0x2009c014)
#define PINSEL1 (*(volatile unsigned int *)0x4002C004)
#define PCLKSEL0 (*(volatile unsigned int *)0x400FC1A8)
#define I2C0SCLH (*(volatile unsigned int *)0x4001C010)
#define I2C0SCLL (*(volatile unsigned int *)0x4001C014)
#define I2C0CONSET (*(volatile unsigned int *)0x4001C000)
#define I2C0CONCLR (*(volatile unsigned int *)0x4001C018)
#define I2C0DAT (*(volatile unsigned int *)0x4001C008)
#define I2C0STAT (*(volatile unsigned int *)0x4001C004)
#define IODIRA (*(volatile unsigned int *)0x00)
#define IODIRB (*(volatile unsigned int *)0x01)
#define GPIOA (*(volatile unsigned int *)0x12)
#define GPIOB (*(volatile unsigned int *)0x13)
#define Temp (*(volatile unsigned int *)0x00)



 void wait() {
	 
 }

void i2cStart() {

	I2C0CONSET = (1<<3);
	I2C0CONSET = (1<<5);
	I2C0CONCLR = (1<<3);

	while ((I2C0CONSET >> 3) & 1 != 1) {
		I2C0CONCLR = (1<<5);
	}

}

int i2cStop() {

	I2C0CONSET = (1<<4);
	I2C0CONCLR = (1<<3);
	
	while ((I2C0CONSET >> 4) & 1 = 1) {
	}

}

void i2cWrite(int data) {

	I2C0DAT = data;
	I2C0CONCLR = (1<<3);

	while ((I2C0CONSET >> 3) & 1 != 1) {
			I2C0CONCLR = (1<<5);
	}


}

int i2cRead(int read) {

	if (read) {
			I2C0CONCLR = (1<<2);
		} else {
			I2C0CONSET = (1<<2);
		}

	I2C0CONCLR = (1<<3);


	while ((I2C0CONSET >> 3) & 1 != 1) {
		I2C0CONCLR = (1<<5);
	}

	int save = I2C0DAT;

	return save;

}

void pinConfig() {
	
	PINSEL1 |= (1<<22); 
	PINSEL1 &= ~(1<<23);
	PINSEL1 |= (1<<24); 
	PINSEL1 &= ~(1<<25);
	

	
}

int main() {

	pinConfig();
	
    while(1) {

    	// read 
    	start();
    	i2cWrite(i2cRead(1));
    	
      }
}







































