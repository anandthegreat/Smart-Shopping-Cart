
/* -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
*/
#include <SoftwareSerial.h>   
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>
#define RST_PIN         9           // Configurable, see typical pin layout above
#define SS_PIN          10          // Configurable, see typical pin layout above
int visited[] ={0,0,0};
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance
const int rs = 7, en = 6, d7 = 5, d6 = 4, d5 = 3, d4 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
byte x;

SoftwareSerial mySerial(14,15); 
int Sensorvalue;
int ledpin=16;
int Data=0;
int totalamt=0;
void setup() {
  mySerial.begin(9600);
   pinMode(ledpin,OUTPUT);
  Serial.begin(9600); 
  pinMode(5,OUTPUT); 
  digitalWrite(5,LOW);
  // Initialize serial communications with the PC
  SPI.begin();                                                  // Init SPI bus
  mfrc522.PCD_Init();                                              // Init MFRC522 card
//  Serial.println(F("Get information about a product:")); 
  //shows in serial that it is ready to read
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("noname SuperMall");

  lcd.setCursor(0,1);

  lcd.print("IIIT-DELHI"); 
}







void loop() {

  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
    MFRC522::MIFARE_Key key;
    digitalWrite(5,LOW);
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  byte block;
  byte len;
  MFRC522::StatusCode status;

  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

//  Serial.println(F("*Item Detected:*"));
//  mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)); //dump some details about the card

  //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));      //uncomment this to see all blocks in hex


  Serial.println(F("\n----------------------------"));
 //################################################################################################
//Serial.print("Status : ");
  
  byte buffer3[18]; //BUFFER2 IS FOR PRODUCT NAME
  block = 8;        //PRODUCT DETAILS ARE STORED IN BLOCK 1 OF RFID
    len = 18;
int amt=0;
int z=0;
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 8, &key, &(mfrc522.uid)); //line 834
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Authentication failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  status = mfrc522.MIFARE_Read(block, buffer3, &len);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Reading failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  int x=buffer3[0]-'0';
  
  if(x==0){
    digitalWrite(5,HIGH);
    delay(5000);
    Serial.print("You havent paid for some items!!!!!");
  
  }
  else{
    digitalWrite(5,LOW);
    }
  
  delay(100); //change value if you want to read cards faster

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

