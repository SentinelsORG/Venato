#define mySerial Serial2

#define DEBUG true

String data[5];
String dataDummy[5];
String commandFromWeb;
char Received;
short sendok = -1;
String nam = "";
char d;
String comd[20];

String state, timegps, latitude, longitude;



void setup() {

  Serial.begin(115200);
  mySerial.begin(9600);
  pinMode(5, OUTPUT);
  mySerial.print("AT + CSMP = 17, 167, 0, 0");  // set this parameter if empty SMS received
  delay(100);
  mySerial.print("AT + CMGF = 1\r");
  delay(400);

  sendData("AT + CGNSPWR = 1", 1000, DEBUG);
  delay(50);
  sendData("AT + CGNSSEQ = RMC", 1000, DEBUG);
  delay(150);
}

void loop() {

  // if (Serial.available() > 0)
  //   switch (Serial.read()) {
  //     case 's':
  //       call();
  //       break;
  //     case 'r':
  //       SendMessage();
  //       break;
  //   }

  getGPS();
  delay(6000);
  // SendMessage("http://maps.google.com/maps?q=loc:");
  // delay(1000);
  SubmitHttpRequest(latitude, longitude);


  delay(60000);
  // mySerial.flush();
}


void SendMessage(String string) {
  mySerial.println("AT+CMGF=1");                   //Sets the GSM Module in Text Mode
  delay(1000);                                     // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+94779675524\"\r");  // Replace x with mobile number
  delay(1000);
  mySerial.println("Hi, I'm Venato. Find me at:");
  mySerial.print(string);
  mySerial.print(latitude);
  mySerial.print(",");
  mySerial.print(longitude);  // The SMS text you want to send
  delay(100);
  mySerial.println((char)26);  // ASCII code of CTRL+Z
  delay(1000);
}




void RecieveMessage() {
  mySerial.println("AT+CNMI=2,2,0,0,0");  // AT Command to receive a live SMS
  delay(1000);
}

void call() {
  mySerial.println("ATD+94779675524;");  // AT Command to receive a live SMS
  Serial.println("Calling.......");
  delay(1000);
}


void getGPS() {
  sendTabData("AT+CGNSINF", 1000, DEBUG);
}



void sendTabData(String command, const int timeout, boolean debug) {

  mySerial.println(command);
  long int time = millis();
  int i = 0;

  while ((time + timeout) > millis()) {
    while (mySerial.available()) {
      char c = mySerial.read();
      if (c != ',') {
        data[i] += c;
        delay(100);
      } else {
        i++;
      }
      if (i == 5) {
        delay(100);
        goto exitL;
      }
    }
  }
exitL:
  if (debug) {

    if (data[3].length() > 2 and data[4].length() > 2) {
      state = data[1];
      timegps = data[2];
      latitude = data[3];
      longitude = data[4];

      dataDummy[1] = data[1];
      dataDummy[2] = data[2];
      dataDummy[3] = data[3];
      dataDummy[4] = data[4];
    } else {
      state = dataDummy[1];
      timegps = dataDummy[2];
      latitude = dataDummy[3];
      longitude = dataDummy[4];
    }

    data[1] = "";
    data[2] = "";
    data[3] = "";
    data[4] = "";
  }
}

String sendData(String command, const int timeout, boolean debug) {
  String response = "";
  mySerial.println(command);
  long int time = millis();
  int i = 0;

  while ((time + timeout) > millis()) {
    while (mySerial.available()) {
      char c = mySerial.read();
      response += c;
    }
  }

  return response;
}

void SubmitHttpRequest(String la, String lo) {

  mySerial.println("AT+CGATT=1");
  delay(100);
  printSerialData();
  mySerial.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
  delay(1000);
  printSerialData();
  mySerial.println("AT+SAPBR=3,1,\"APN\",\"\"");
  delay(5000);
  printSerialData();
  mySerial.println("AT+SAPBR=0,1");
  delay(15000);
  mySerial.println("AT+SAPBR=1,1");
  delay(15000);
  // printSerialData();
  mySerial.println("AT+HTTPINIT");
  delay(2000);
  printSerialData();
  String url = "AT+HTTPPARA=\"URL\",\"http://220.253.238.2/venato/location.php?loc=" + la + "," + lo + "\"";
  const char* urlChar = url.c_str();
  mySerial.println(urlChar);

  delay(2000);
  printSerialData();
  mySerial.println("AT+HTTPACTION=0");  //submit the request
  delay(8000);
  printSerialData();
  // SetAlarm();
  // printSerialData();
  getSerialData();
  // commandFromWeb = getSerialData();


  mySerial.println("AT+HTTPTERM");  //submit the request
}

void printSerialData() {
  while (mySerial.available() != 0)
    Serial.write(mySerial.read());
}

void getSerialData() {

  mySerial.println("AT+HTTPREAD=0,200");  //submit the request

  long int t = millis();
  int i = 0;
  const int tout = 1000;
  boolean debugg = true;


  while ((t + tout) > millis()) {
    while (mySerial.available()) {
      char ch = mySerial.read();

      if (ch != '\n') {
        comd[i] += ch;

        delay(100);
      } else {
        i++;
      }
      if (i == 3) {
        delay(100);
        goto exitK;
      }
    }
  }
exitK:
  if (debugg) {

    if (comd[2].substring(1, 2) == "1") {

      // ring();
      delay(2000);
      Serial.println(comd[2]);
      comd[0] = "";
      comd[1] = "";
      comd[2] = "";


    } else {
      comd[0] = "";
      comd[1] = "";
      comd[2] = "";
    }
  }
}


void ring() {
  tone(5, 4186, 500);  //C Note
  delay(1000);
  tone(5, 5274, 500);  //E Note
  delay(1000);
}