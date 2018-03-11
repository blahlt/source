/**
 * A Mirf example to test the latency between two Ardunio.
 *
 * Pins:
 * Hardware SPI:
 * MISO -> 12
 * MOSI -> 11
 * SCK -> 13
 *
 * Configurable:
 * CE -> 9
 * CSN -> 10
 *
 * Note: To see best case latency comment out all Serial.println
 * statements not displaying the result and load 
 * 'ping_server_interupt' on the server.
 */

#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

void setup(){
  Serial.begin(9600);

  Mirf.cePin = 9;
  Mirf.csnPin = 10;

  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.configRegister(EN_AA, 0x00);
  
  Mirf.setTADDR((byte *)"clie1");
  Mirf.payload = 1;
  Mirf.config();

  byte reg = 0;
  Mirf.readRegister( CONFIG, &reg, sizeof(reg) );
  Serial.print( "CONFIG = " );
  Serial.println(reg,HEX);
  Mirf.readRegister( SETUP_RETR, &reg, sizeof(reg) );
  Serial.print( "SETUP_RETR  = " );
  Serial.println(reg,HEX);
  Mirf.readRegister( SETUP_AW, &reg, sizeof(reg) );
  Serial.print( "SETUP_AW  = " );
  Serial.println(reg,HEX);
  Mirf.readRegister( RF_CH, &reg, sizeof(reg) );
  Serial.print( "RF_CH  = " );
  Serial.println(reg,HEX);
  Mirf.readRegister( RF_SETUP, &reg, sizeof(reg) );
  Serial.print( "RF_SETUP  = " );
  Serial.println(reg,HEX);
  Mirf.readRegister( RX_PW_P0, &reg, sizeof(reg) );
  Serial.print( "RX_PW_P0  = " );
  Serial.println(reg,HEX);
  Mirf.readRegister( EN_AA, &reg, sizeof(reg) );
  Serial.print( "EN_AA  = " );
  Serial.println(reg,HEX);
  uint8_t r[5];
  Mirf.readRegister( RX_ADDR_P0, r, sizeof(r) );
  Serial.print( "RX_ADDR_P0  = " );
  Serial.print((byte)r[0], HEX); Serial.print(" ");
  Serial.print((byte)r[1], HEX); Serial.print(" ");
  Serial.print((byte)r[2], HEX); Serial.print(" ");
  Serial.print((byte)r[3], HEX); Serial.print(" ");
  Serial.println((byte)r[4], HEX);
     
  Mirf.readRegister( TX_ADDR, r, sizeof(r) );
  Serial.print( "TX_ADDR  = " );
  Serial.print((byte)r[0], HEX); Serial.print(" ");
  Serial.print((byte)r[1], HEX); Serial.print(" ");
  Serial.print((byte)r[2], HEX); Serial.print(" ");
  Serial.print((byte)r[3], HEX); Serial.print(" ");
  Serial.println((byte)r[4], HEX);

  Mirf.readRegister( STATUS, &reg, sizeof(reg) );
  Serial.print( "STATUS  = " );
  Serial.println(reg,HEX);

  Serial.println("Beginning ... "); 
}
  
void loop(){
  while(!Mirf.dataReady()){
  }
  byte data;
  Mirf.getData((byte *) &data);
  
  Serial.print("Received: ");
  Serial.println(data);
} 
