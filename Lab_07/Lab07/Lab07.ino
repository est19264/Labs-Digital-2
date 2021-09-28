/* 
 *  NOMBRE: Diego Estrada - 19264
 *  PROYECTO: Laboratorio 7 - Memoria SD
 *  FECHA: 27/09/2021
 */

// Librerías 
#include <SPI.h>
#include <SD.h>

// Variables a usar dentro de la libreria
Sd2Card card;
SdVolume volume;
SdFile root;
char get;

const int chipSelect = PA_3; //cs PIN
String palabra;            //variable en la q ira la palabra

File dataFile;

// Configuración principal
void setup() {

  Serial.begin(9600);       // Baud Rate
  while (!Serial) {         //
    ;
  }
  SPI.setModule(0);         // Para poder comunicarse con la SD
  delay(2500);              // Abre la terminal serial
  Serial.print("\nInitializing SD card...\n");
  pinMode(PA_3, OUTPUT);     // pin 12/ para utilizar como cs PIN


  if (!card.init(SPI_HALF_SPEED, chipSelect)) {     // Configuración para prueba de estado de la SD
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card is inserted?");
    Serial.println("* Is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    return;
  } else {
    Serial.println("Wiring is correct and a card is present.\n");
  }

  if (!volume.init(card)) {       // Se verifica los formatos permitidos
    Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card\n");
    return;
  }

  Serial.println("\nFiles found on the card (name, date and size in bytes): ");
  root.openRoot(volume);              // Muestra la infromacion en la SD
  root.ls(LS_R | LS_DATE | LS_SIZE);  // Muestra los datos en un orden
  delay(1000);                        
  root.close();                       // Cierra la root


  if (!SD.begin(chipSelect)) {        // Inicio de la comunicacion a la SD en read y write
    Serial.println("Card failed, or not present");
    return;
  }
  Serial.println("\nCard initialized as read/write\n.");

  Serial.println("Envie el numero del documento que desea ver:");
  Serial.println("(1) si quiere mostrar la imagen de ejemplo 1");
  Serial.println("(2) si quiere mostrar la imagen de ejemplo 2");
  Serial.println("(3) si quiere mostrar la imagen de ejemplo 3");
}

void loop() {
  get = Serial.read();    // Asigna la lectura del serial a la variable
  //char  a = (get, DEC);
  switch (get) {      // Utiliza el valor leido del serial para mostrar la imagen en ascii.txt
    case '1':
      dataFile = SD.open("uno.txt");
      if (dataFile) {

        // read from the file until there's nothing else in it:
        while (dataFile.available()) {
          Serial.write(dataFile.read());
        }
        // close the file:
        dataFile.close();
      } else {
        // if the file didn't open, print an error:
        Serial.println("error opening pelota.txt");
      }
      Serial.println("Envie el numero del documento que desea ver:");
      Serial.println("(1) si quiere mostrar la imagen de ejemplo 1");
      Serial.println("(2) si quiere mostrar la imagen de ejemplo 2");
      Serial.println("(3) si quiere mostrar la imagen de ejemplo 3");
      break;

    case '2':
      dataFile = SD.open("dos.txt");
      if (dataFile) {

        // read from the file until there's nothing else in it:
        while (dataFile.available()) {
          Serial.write(dataFile.read());
        }
        // close the file:
        dataFile.close();
      } else {
        // if the file didn't open, print an error:
        Serial.println("error opening madrid.txt");
      }
      Serial.println("Envie el numero del documento que desea ver:");
      Serial.println("(1) si quiere mostrar la imagen de ejemplo 1");
      Serial.println("(2) si quiere mostrar la imagen de ejemplo 2");
      Serial.println("(3) si quiere mostrar la imagen de ejemplo 3");
      break;

    case '3':
      dataFile = SD.open("tres.txt");
      if (dataFile) {

        // read from the file until there's nothing else in it:
        while (dataFile.available()) {
          Serial.write(dataFile.read());
        }
        // close the file:
        dataFile.close();
      } else {
        // if the file didn't open, print an error:
        Serial.println("error opening liver.txt");
      }
      Serial.println("Envie el numero del documento que desea ver:");
      Serial.println("(1) si quiere mostrar la imagen de ejemplo 1");
      Serial.println("(2) si quiere mostrar la imagen de ejemplo 2");
      Serial.println("(3) si quiere mostrar la imagen de ejemplo 3");
      break;
  }
}
