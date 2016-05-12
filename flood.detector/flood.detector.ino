/** 
  *
  *  Compiled and coded by;
  *  Gerard James B. Paglingayen
  *  
  *  Gizduino (Arduino Clone)
  *  GSM Shield
  *  Water Level sensor
  *  
  *  Note;
  *    Read the Gizduino Manual especially the Pin configuration.
  *    
 **/

/**  DO NOT CHANGE the values below
  *  Variables for GSM Shield
**/
char Rx_data[50];
unsigned char Rx_index = 0;
int i = 0;
char msg[160];
int sig;

/**  You may change the default pins.
  *  This will depend on your connections.
**/ 

int statusLevel = 0;

int ledPin1 = 2; // LED for level 1
int ledPin2 = 3; // LED for level 2
int ledPin3 = 4; // LED for level 3
int ledPin4 = 5; // LED for the buzzer

int sensor1Pin = 6; // Sensor for level 1
int sensor2Pin = 7; // Sensor for level 2
int sensor3Pin = 8; // Sensor for level 3

char msgLVL0[] = "Water Level Status: Mornal";
char msgLVL1[] = "Water Level Status: Level 1";
char msgLVL2[] = "Water Level Status: Level 2";
char msgLVL3[] = "Water Level Status: Level 3";

char* regNumbers[]={"09150000000", "09360000000"};
int numNumbers = 2;


void setup() {


  pinMode(ledPin1, OUTPUT);  //  LED for level 1
  pinMode(ledPin2, OUTPUT);  //  LED for level 2
  pinMode(ledPin3, OUTPUT);  //  LED for level 3
  pinMode(ledPin4, OUTPUT);  //  LED for the buzzer
  
  pinMode(sensor1Pin, INPUT); // Sensor for level 1
  pinMode(sensor2Pin, INPUT); // Sensor for level 2
  pinMode(sensor3Pin, INPUT); // Sensor for level 3
  
  Serial.begin(38400);
  
  //initGSM(); // for debugging purpose only DO NOT REMOVE
  //send_msg("09154370656", "GSM module started successfully."); // for debugging purpose only DO NOT REMOVE
  initGSM();
  sendMulti("GSM module started successfully.");
  //Serial.println(msgLVL1);

}

void loop(){

  if (digitalRead(sensor3Pin) == 1 && digitalRead(sensor2Pin) == 1 && digitalRead(sensor1Pin) == 1) {
    if (statusLevel != 3)
    {
      statusLevel = 3;
      Serial.println("Level 3");
      
      digitalWrite(2, LOW);
      digitalWrite(3, LOW);
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      
      sendMulti(msgLVL3);
    }
  }

  else if (digitalRead(sensor3Pin) == 0 && digitalRead(sensor2Pin) == 1 && digitalRead(sensor1Pin) == 1){
    if (statusLevel != 2)
    {
      statusLevel = 2;
      Serial.println("Level 2");
      
      digitalWrite(2, LOW);
      digitalWrite(3, HIGH);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      
      sendMulti(msgLVL2);
    }
  }
  
  else if (digitalRead(sensor3Pin) == 0 && digitalRead(sensor2Pin) == 0 && digitalRead(sensor1Pin) == 1){
    if (statusLevel != 1)
    {
      statusLevel = 1;
      Serial.println("Level 1");
      
      digitalWrite(2, HIGH);
      digitalWrite(3, LOW);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      
      sendMulti(msgLVL1);
    }
  }

  else {
    if (statusLevel != 0)
    {
      statusLevel = 0;
      Serial.println("Normal Level");
      
      digitalWrite(2, LOW);
      digitalWrite(3, LOW);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      
      sendMulti(msgLVL0);
    }
  }


  delay(50);
}

void sendMulti(char *msgMulti){

  for (int i = 0; i < numNumbers; i++){
    //Serial.println(regNumbers[i]);

    send_msg(regNumbers[i], msgMulti);
    delay(5000);
  }
  

}


void send_msg(char *number, char *msg)
{
  char at_cmgs_cmd[30] = {'\0'};
  char msg1[160] = {'\0'};
  char ctl_z = 0x1A;

  sprintf(msg1, "%s%c", msg, ctl_z);
  sprintf(at_cmgs_cmd, "AT+CMGS=\"%s\"\r\n",number);
  
  sendGSM(at_cmgs_cmd);
  delay(100);
  delay(100);
  delay(100);
  sendGSM(msg1);
  delay(100);
}

void sendGSM(char *string){
  Serial.write(string);
  delay(90);
}

void clearString(char *strArray) {
  int j;
  for (j = 100; j > 0; j--)
    strArray[j] = 0x00;
}

void send_cmd(char *at_cmd, char clr){
  char *stat = '\0';
  while(!stat){
    sendGSM(at_cmd);
    delay(90);
    readSerialString(Rx_data);
    
    stat = strstr(Rx_data, "OK");
  }
  if (clr){
    clearString(Rx_data);
    delay(200);
    stat = '\0';
  }
}

void initGSM(){
  
  send_cmd("AT\r\n",1);						
//  send_cmd("ATE0\r\n",1); // Turn off automatic echo of the GSM Module	
	
  send_cmd("AT+CMGF=1\r\n",1);			// Set message format to text mode
  //Sucess
  
  Serial.println("Success");
	
  delay(1000);
  delay(1000);
  delay(1000);
}

void readSerialString (char *strArray) {
  
  if(!Serial.available()) {
    return;
  }
  
  while(Serial.available()) {
    strArray[i] = Serial.read();
    i++;
  }
}
