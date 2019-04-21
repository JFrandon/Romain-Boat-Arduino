#include <stdlib.h>
#include <stdio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>

#define DELAY 500
#define DATASIZE 100

Adafruit_MMA8451 mma = Adafruit_MMA8451();
int i;
File data;

void setup(void) {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial);


  //Init SD
  SPI.begin();
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  Serial.print("Initializing SD card... ");
  while (! SD.begin(4)) Serial.println("Initialization failed");
  Serial.println("Initialization done");
  

  //begin accelerometer
  Serial.println("Adafruit MMA8451 test!");
  while (! mma.begin()) Serial.println("Branche le connard");
  Serial.println("MMA8451 found!");
  mma.setRange(MMA8451_RANGE_2_G);
  Serial.print("Range = "); Serial.print(2 << mma.getRange());
  Serial.println("G");

  //Open file
  while(!(data = SD.open("data.csv", FILE_WRITE)))   Serial.println("Error during the opening phase - Something went wrong");
  Serial.println("SD Opened");


  //add header to the file
  data.println("X,Y,Z");
  data.flush();

}

void loop() {
  sensors_event_t event; 
  mma.getEvent(&event);

  //get acceleration
  float x=event.acceleration.x;
  float y=event.acceleration.y;
  float z=event.acceleration.z;

  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("X: \t"); Serial.print(x); Serial.print("\t");
  Serial.print("Y: \t"); Serial.print(y); Serial.print("\t");
  Serial.print("Z: \t"); Serial.print(z); Serial.print("\t");
  Serial.println("m/s^2 ");
  Serial.println();

  //delay and datasize are defined at the top of the file
  delay(DELAY);
  if (i < DATASIZE) {
    finalprint(x,y,z);
  }else{
    //closing the file and "terminating" the program
    data.close();
    Serial.println("File closed");
    while(1);
  }
  i++;
}

void finalprint(float x, float y, float z)  {
  //creates a string buffer of 50 characters
  char s [50];

  //convertis les float en int (m/s^2)->(mm/s^2)
  int xx = (int) (x*1000);
  int yy = (int) (y*1000);
  int zz = (int) (z*1000);
  
  //copie les valeurs dans le buffer
  sprintf(s,"%8d, %8d, %8d",xx,yy,zz);
 

  //ecris les donnees sur la carte SD
  data.println(s);
  data.flush();

  //imprime le message de succes
  Serial.print(s);
  Serial.println(" File updated with success");
  return;
}

/* CHANGELOG 1 : Changes by Jeremy
 * Amelioré la qualité du code 
 * Ajouté des commentaires
 * Ajouté data.flush() pour forcer l'écriture de la carte SD
 * Ajouté la fin du programmes avec le while true
 * Ajouté les define pour pouvoir rapidement changer ces valeurs
 * Remis le i++; du loop() au bon endroit (causait des bugs avec le  else if)
 * enlevé le else if
 * J'espere vraiment que ca va marcher 
*/ 

/* CHANGELOG 2 : Changes by Jeremy
 * Utilisation d'un string C au lieu d'un string c++ dans finalprint
*/ 




/* CATALEX MICRO SD ADAPTATOR
 *  +5V     VCC     ROUGE
 *  GND     GND     NOIR
 *  D12     MISO    VERT
 *  D11     MOSI    BLANC
 *  D13     SCK     MARRON
 *  D4      CS      JAUNE
 */
