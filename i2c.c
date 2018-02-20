#include <avr/io.h>
#include <util/delay.h>
#include <compat/twi.h>
#include <Arduino.h>
#define start_sucess 1
#define start_fail 2
#define start_stuck 3
uint8_t val = 90;

void i2cInit(void)
{
  Serial.println("Hi");
  TWSR = 0x00; 
  TWBR = 0x07; 
  TWCR = (1<<TWEN); 
}

void i2cStart()
{
  TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
  while(TWCR &(1<<TWINT)==0);
}

void i2cwrite(uint8_t data)
{
   TWDR = data;
   TWCR = (1<<TWINT)|(1<<TWEN);
   while(TWCR &(1<<TWINT)==0);
}

void i2c_location(uint8_t beta)
{
  TWDR = (beta >> 8); 
   TWCR = (1<<TWINT)|(1<<TWEN);
   while(!(TWCR &(1<<TWINT)));

   
  TWDR =  beta; 
   TWCR = (1<<TWINT)|(1<<TWEN);
   while(!(TWCR &(1<<TWINT)));
}

void i2c_data(uint8_t ceta)
{
  TWDR = ceta;
   TWCR = (1<<TWINT)|(1<<TWEN);
   while(!(TWCR &(1<<TWINT)));
}

int write_check()
{
  delay(500);
  if((TWSR & 0xF8) == TW_MT_SLA_NACK)
    return start_sucess;

  else if((TWSR & 0xF8) == TW_MT_ARB_LOST)
    return start_stuck;
  
  else
    return start_fail;
}

int data_check()
{
  if((TWSR & 0xF8) == TW_MT_DATA_NACK)
    return start_sucess;
  
  else if((TWSR & 0xF8) == TW_MT_ARB_LOST)
    return start_stuck;
    
  else
    return start_fail;
}

void i2cStop()
{
  TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
  _delay_ms(1);
}

int i2cCheck()
{ 
  delay(500);
  if((TWSR & 0xF8) == TW_START)
    return start_sucess;
    
  else
    return start_fail;
}


void setup()
{
  digitalWrite(SDA, 1);
  digitalWrite(SCL, 1);
  Serial.begin(9600);
  
  i2cInit();
  i2cStart();  
  int x = i2cCheck();
  Serial.println(x);
  
  i2cwrite(0x50);
  int y = write_check();
  Serial.println(y);
  
  i2c_location(5);
  int z = data_check();
  Serial.println(z);
  
  i2c_data(val);  
  int w = data_check();
  Serial.println(w);
  
  i2cStop();      

  i2cStart();
  int a = i2cCheck();
  Serial.println(a);
  
  i2cwrite(0x50);
  int b = write_check();
  Serial.println(b);
  
  i2c_location(5);
  int c = data_check();
  Serial.println(c);
  
  i2cStart();
  int d = i2cCheck();
  Serial.println(d);
  
  i2cwrite(0x50);
  int e = write_check();
  Serial.println(e);

  TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
  while(TWCR &(1<<TWINT)==0);
    
  int data = TWDR;
  Serial.println(data);
}

void loop()
{
  
}
