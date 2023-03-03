// Lab 4



/*
* ECE 3223 Assignment 5
* Nhi Luong 4/22/23
*
*/

#include "mbed.h"

DigitalIn L1up(p24,PullDown); // active high
DigitalIn L1down(p23,PullDown); // active high
DigitalIn L2up(p22,PullDown); // active high
DigitalIn L2down(p21,PullDown); // active high

I2C i2c(p9,p10); // SDA,SCL

int i2cWrite(int addr, int data) {
    int ack;
    
    i2c.start();
    ack = i2c.write(0b10100000); // address: 1010XXX, r/w = 0 (write)

    // check 1st ack
    if (!ack) {
        return 0;
    }

    i2c.write(addr); // address inside the chip
    i2c.write(data); // data to store
    i2c.stop();
    
    wait_ms(7); // wait for write to complete (wait cycle: 5ms)
    
    return 1;
}

int i2cRead(int addr) {
    int ack, data;
    
    i2c.start();
    ack = i2c.write(0b10100000);

    // check ack after intial selecting
    if (!ack) { 
        return -1;
    }

    i2c.write(addr); // select location to read from
    i2c.start();
    i2c.write(0b10100001); // address: 1010XXX, r/w = 1 (read)
    data = i2c.read(0);
    i2c.stop();
    
    return data;
}

int main() {
  
    int L1U; // switch states
   
    while(1) {        
        // read switch states only once per loop
        L1U = L1up; L1D = L1down;
        L2U = L2up; L2D = L2down;
        
        // L1up: increases LED1 brightness (0%-100% in steps of 20% per press)
        // L1down: decreases LED1 brightness (100%-0% in steps of 20% per press)
        if (L1U == 1 && b1 < MAX_BRIGHTNESS) {
            b1 += 1;
            led1 = b1 * STEP;
            i2cWrite(0, b1); 
            wait(0.5);
        } else if (L1D == 1 && b1 > MIN_BRIGHTNESS) {
            b1 -= 1;
            led1 = b1 * STEP;
            i2cWrite(0, b1);
            wait(0.5);
        }
      }
}
