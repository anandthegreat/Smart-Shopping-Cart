#include <SoftwareSerial.h>   
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>

#define RST_PIN         9           // Configurable, see typical pin layout above
#define SS_PIN          10          // Configurable, see typical pin layout above
int visited[] ={0,0,0,0,0};

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance
const int rs = 7, en = 6, d7 = 5, d6 = 4, d5 = 3, d4 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
byte x;

SoftwareSerial mySerial(A0,A1); 
int Sensorvalue;
int ledpin=16;
int Data=0;
int totalamt=0;
void setup() {
  mySerial.begin(9600);
   pinMode(ledpin,OUTPUT);
  Serial.begin(9600);  
  
  // Initialize serial communications with the PC
  SPI.begin();                                                  // Init SPI bus
  mfrc522.PCD_Init();                                              // Init MFRC522 card
  Serial.println(F("Get information about a product:")); 
  //shows in serial that it is ready to read
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("noname SuperMall");

  lcd.setCursor(0,1);

  lcd.print("IIIT-DELHI"); 
}







void loop() {

//########################################################################################
//FOLLOWER CODE HERE!!!!!!!!!!!!!!

//#########################################################################################
  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
    MFRC522::MIFARE_Key key;
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

  Serial.println(F("*Item Detected:*"));
  mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)); //dump some details about the card

  //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));      //uncomment this to see all blocks in hex

  byte buffer[34];
  len=1;
  int a=byte(1);
  buffer[0]=a;
//len = Serial.readBytesUntil('#', (char *) buffer, 20) ; // read first name from serial
  for (byte i = len; i < 20; i++) buffer[i] = ' ';     // pad with spaces
  block = 8;
  
  //Serial.println(F("Authenticating using key A..."));
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
//    Serial.print(F("PCD_Authenticate() failed: "));
//    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  // Write block
  status = mfrc522.MIFARE_Write(block, buffer, 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Write() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else Serial.println(F("MIFARE_Write() success: "));

  block = 9;
  //Serial.println(F("Authenticating using key A..."));
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  // Write block
  status = mfrc522.MIFARE_Write(block, &buffer[16], 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Write() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
//  else Serial.println(F("MIFARE_Write() success: "));



  Serial.println(F("\n----------------------------"));


  Serial.print(F("Product Name: "));
 //#######################################################################################################

  byte buffer2[18]; //BUFFER2 IS FOR PRODUCT NAME
  block = 1;        //PRODUCT DETAILS ARE STORED IN BLOCK 1 OF RFID
    len = 18;

  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 1, &key, &(mfrc522.uid)); //line 834
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Authentication failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  status = mfrc522.MIFARE_Read(block, buffer2, &len);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Reading failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  lcd.setCursor(0,0);
  lcd.clear();    //CLEAR LCD

  
  
digitalWrite(ledpin,HIGH);
  delay(500);
  digitalWrite(ledpin,LOW);
  //PRINT PRODUCT NAME
  for (uint8_t i = 0; i < 16; i++) {
    Serial.write(buffer2[i]);
    lcd.write(buffer2[i]);
   // lcd.setCursor(i,0);
  }


//############################################################################################################################
// PRINT PRODUCT PRICE WHICH IS STORED IN BLOCK 4 OF RFID

  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 4, &key, &(mfrc522.uid)); //line 834 of MFRC522.cpp file
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Authentication failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
 byte buffer1[18];
  block = 4;
  len=18;
  status = mfrc522.MIFARE_Read(block, buffer1, &len);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Reading failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  int z=0;
  int amt=0;
   lcd.setCursor(0,1);
  
  
  Serial.println();
  Serial.write("Price: ");
  lcd.print("Price: Rs.");  
  
  for (uint8_t i = 0; i < 16; i++)
  {
    if (buffer1[i] != 32)
    {
      Serial.write(buffer1[i]);
      lcd.write(buffer1[i]);
      z++;
    }
  }
  z=z-1;
  int z2=z;
  for (uint8_t i = 0; i < 16; i++)
  {
    if (buffer1[i] != 32)
    {
      int x=buffer1[i]-'0';
      amt+=x*pow(10,z);
      z--;
    }
  }
  if(z2>1){
    amt+=1;
  }

  if(amt==85 && visited[0]==0){
    totalamt+=amt;
    visited[0]=1;
    delay(4000);
    lcd.clear();
    lcd.print("Total : ");
    lcd.print(totalamt);
  }
  else if(amt==85 && visited[0]==1){
    lcd.clear();
    lcd.print("Item Removed.");
    totalamt-=amt;
    visited[0]=0;
    delay(4000);
    lcd.clear();
    lcd.print("Total : ");
    lcd.print(totalamt);
  }
  else if(amt==120 && visited[1]==0){
    totalamt+=amt;
    visited[1]=1;
    delay(4000);
    lcd.clear();
    lcd.print("Total : ");
    lcd.print(totalamt);
  }
  else if(amt==120 && visited[1]==1){
    lcd.clear();
    lcd.print("Item Removed.");
    totalamt-=amt;
    visited[1]=0;
    delay(4000);
    lcd.clear();
    lcd.print("Total : ");
    lcd.print(totalamt);
  }
  else if(amt==200 && visited[2]==0){
    
    totalamt+=amt;
    visited[2]=1;
    delay(4000);
    lcd.clear();
    lcd.print("Total : ");
    lcd.print(totalamt);  }
  else if(amt==200 && visited[2]==1){
    lcd.clear();
    lcd.print("Item Removed.");
    totalamt-=amt;
    visited[2]=0;
    delay(4000);
    lcd.clear();
    lcd.print("Total : ");
    lcd.print(totalamt);
  }
  else if(amt==100 && visited[3]==0){
    totalamt+=amt;
    visited[3]=1;
    delay(4000);
    lcd.clear();
    lcd.print("Total:");
    lcd.print(totalamt);
  }
  else if(amt==100 && visited[3]==1){
    lcd.clear();
    lcd.print("Item Removed.");
    totalamt-=amt;
    visited[3]=0;
    delay(4000);
    lcd.clear();
    lcd.print("Total:");
    lcd.print(totalamt);
  }
  else if(amt==510 && visited[4]==0){
    totalamt+=amt;
    visited[4]=1;
    delay(4000);
    lcd.clear();
    lcd.print("Total:");
    lcd.print(totalamt);
  }
  else if(amt==510 && visited[4]==1){
    lcd.clear();
    lcd.print("Item Removed.");
    totalamt-=amt;
    visited[4]=0;
    delay(4000);
    lcd.clear();
    lcd.print("Total:");
    lcd.print(totalamt);
  }
  
  Serial.println(" ");
  Serial.print("Total amt is Rs. ");
  Serial.println(totalamt);
  mySerial.print(totalamt);


  Serial.println(F("\n----------------------------"));

 //################################################################################################
Serial.print("Status : ");
  
  byte buffer3[18]; //BUFFER2 IS FOR PRODUCT NAME
  block = 8;        //PRODUCT DETAILS ARE STORED IN BLOCK 1 OF RFID
    len = 18;

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
   for (uint8_t i = 0; i < 16; i++) {
    Serial.write(buffer3[i]);
//    lcd.write(buffer2[i]);
//    lcd.setCursor(i,0);
  }
  delay(100); //change value if you want to read cards faster

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

