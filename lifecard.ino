#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>
#include <String.h>

#define RST_PIN         9       
#define SS_PIN          10      
LiquidCrystal_I2C lcd(0x27, 20, 4);

byte BandTarj=0;
MFRC522 mfrc522(SS_PIN, RST_PIN);   

byte buffer[32];
String Simp1;
boolean band = false;


String CadenaAux,CadenaAuxS;

byte block;
byte len;


MFRC522::MIFARE_Key key;
MFRC522::StatusCode status;
int Direccion = 0;
int II,JJ;
String recibir1;
String s, s1;
int Valor;

void setup() {
  lcd.begin();
  lcd.backlight();
  Serial.begin(9600);
  SPI.begin();               
  mfrc522.PCD_Init();         
  letrero();
  Valor=0;
}


void letrero(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("TARJETA  INTELIGENTE"));
  lcd.setCursor(2, 2);
  lcd.print(F("L I F E   C A R D"));
 }





void ErrorLectura(){
    BandTarj=0;
    lcd.clear();
    lcd.setCursor(7, 0);
    lcd.print(F("ERROR "));
    lcd.setCursor(9, 1);
    lcd.print(F("DE "));
    lcd.setCursor(6, 2);
    lcd.print(F("LECTURA "));
}

void loop() {
  
  detectatarjeta();
  if (band) {
    letrero3();
    band = false;
  } 
      
}


void detectatarjeta() {
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;
  MFRC522::StatusCode status;
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  band = true;
  Serial.println(F("tarjeta detectada"));
}


void leercampo() {
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid)); 
  if (status != MFRC522::STATUS_OK) {
    ErrorLectura();
    letrero();
    return;
  }

  status = mfrc522.MIFARE_Read(block, buffer, &len);
  if (status != MFRC522::STATUS_OK) {
    ErrorLectura();
    letrero();
    
  }
  
  CadenaAux = "";
  for (uint8_t i = 0; i < 16; i++)
  {
    if (buffer[i] != 32)
    {
      if (buffer[i] != 46) {
        CadenaAux = CadenaAux + (buffer[i] - 48);

      }
      else if (buffer[i] == 46) {
        CadenaAux = CadenaAux + ".";
      }
    }
  }
 
}


void leercampo3() {
  
  for (byte r=0;r<32;r++)
   buffer[r]=32;
   
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid)); 
  if (status != MFRC522::STATUS_OK) {
    ErrorLectura();
    letrero();
    return;
  }
  len=18;
  status = mfrc522.MIFARE_Read(block, buffer, &len);
  if (status != MFRC522::STATUS_OK) {
    ErrorLectura();
    letrero();
  }  
}



void LecturaTarjeta() {
    byte cad[16];
    
  BandTarj=1;  
  
  for (byte j = len; j < 16; j++) cad[j] = 32;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
   cad[i]=mfrc522.uid.uidByte[i];
  }  
  
  CadenaAux=""; 
  block = 4;
  len = 18; 
  leercampo3();
  CadenaAux="";
  char kij;
  for(byte g=0;g<16;g++){
   kij=buffer[g];
   if (kij != " ") CadenaAux=CadenaAux+kij;
  }
  CadenaAuxS=CadenaAux; //Nombre Contacto

  CadenaAux=""; 
  block = 8;
  len = 18; 
  leercampo3();
  CadenaAux="";
  for(byte g=0;g<16;g++){
   kij=buffer[g];
   if (kij != " ") CadenaAux=CadenaAux+kij;
  }
  CadenaAuxS=CadenaAux; //Numero Contacto

  CadenaAux=""; 
  block = 12;
  len = 18; 
  leercampo3();
  CadenaAux="";
  for(byte g=0;g<16;g++){
   kij=buffer[g];
   if (kij != " ") CadenaAux=CadenaAux+kij;
  }
  CadenaAuxS=CadenaAux; //Tipo Sangre

  CadenaAux=""; 
  block = 16;
  len = 18; 
  leercampo3();
  CadenaAux="";
  for(byte g=0;g<16;g++){
   kij=buffer[g];
   if (kij != " ") CadenaAux=CadenaAux+kij;
  }
  CadenaAuxS=CadenaAux; //Alergias

  CadenaAux=""; 
  block = 20;
  len = 18; 
  leercampo3();
  CadenaAux="";
  for(byte g=0;g<16;g++){
   kij=buffer[g];
   if (kij != " ") CadenaAux=CadenaAux+kij;
  }
  CadenaAuxS=CadenaAux; //Padecimientos Cronicos

  CadenaAux=""; 
  block = 24;
  len = 18; 
  leercampo3();
  CadenaAux="";
  for(byte g=0;g<16;g++){
   kij=buffer[g];
   if (kij != " ") CadenaAux=CadenaAux+kij;
  }
  CadenaAuxS=CadenaAux; //Hopsital Aignado

  CadenaAux=""; 
  block = 26;
  len = 18; 
  leercampo3();
  CadenaAux="";
  for(byte g=0;g<16;g++){
   kij=buffer[g];
   if (kij != " ") CadenaAux=CadenaAux+kij;
  }
  CadenaAuxS=CadenaAux; //Donacion Organos

 
  
  mfrc522.PICC_HaltA(); 
  mfrc522.PCD_StopCrypto1(); 
                
}
