#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11);
String msg , isc;


void setup()
{
  mySerial.begin(9600);   // Setting the baud rate of GSM Module
  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
  Serial.println("GSM SIM900A BEGIN");

  pinMode(2, INPUT_PULLUP);  // ignition
  pinMode(3, INPUT_PULLUP);   // engine
  pinMode(6, OUTPUT);
  digitalWrite(6, !LOW);
  delay(100);

}


void loop()
{
  input_status();
  if (mySerial.available())
  {
    Serial.println("Serial Available");
    msg = mySerial.readString();
    String buffer = msg;
    buffer.remove(0, 51);
    int len = buffer.length();
    // Remove \r\n from tail
    buffer.remove(len - 2, 2);
    Serial.println(buffer);
    msg = buffer;

      if ( msg == "MENU" || msg == "menu" )
      {
        Serial.println("Serial Available");
        SendMessageMenu() ;
      }

      else if ( msg == "STATUS" || msg == "status" )
      {
        SendMessageStatus();
      }
      else if ( msg == "LOCATION" || msg == "location" )
      {
        SendMessageLocation();
      }
      else if ( msg == "RESUME" || msg == "resume" )
      {
        SendMessageResume();
      }

      else if ( msg == "STOP" || msg == "stop")
      {
        SendMessageStop();
      }
  }
}


void sendSMS(String message) {
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  Serial.println("AT+CMGF=1");
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+917205308022\"\r"); // Replace x with mobile number
  Serial.println("AT+CMGS=\"+917205308022");
  delay(1000);
  mySerial.println(message);
  delay(100);
  mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}

String input_status()
{

  int eng = digitalRead(2);
  int ing = digitalRead(3);
  if ( eng == LOW && ing == LOW)
  {
    return String("Ignition ON");
  }
  else if ( eng == LOW && ing == HIGH)
  {
    return String("Acc ON");
  }
  else if (eng == HIGH && ing == HIGH)
  {
    return String("Ignition & Acc off");
  }
  else if (eng == HIGH && ing == LOW )
  {
    hotWriring(String("hot wiring"));
  }
}

void hotWriring(String message) {
  while (1)
  {
    Serial.println("HOT Wiring");
    sendSMS(message);

    for (int i = 0 ; i < 30 ; i++)
    {
      delay(1000);
    }
  }

}
void SendMessageMenu()
{
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  Serial.println("AT+CMGF=1");
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+917205308022\"\r"); // Replace x with mobile number
  Serial.println("AT+CMGS=\"+917205308022");
  delay(1000);
  mySerial.println("1.Status");
  Serial.println("1.Status");
  delay(100);
  mySerial.println("2.Location");
  Serial.println("2.Location");
  delay(100);
  mySerial.println("3.Resume");
  Serial.println("3.Resume");
  delay(100);
  mySerial.println("4.Stop");
  Serial.println("4.Stop");
  delay(100);
  mySerial.println("Type your request");
  Serial.println("Type your request");
  delay(100);
  mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}

void SendMessageStatus()
{
  sendSMS(input_status());
}

void SendMessageLocation()
{

}

void SendMessageResume()
{
  if (input_status() == String("Acc ON"))
  {
    digitalWrite(6, LOW);
    sendSMS("Ignition ON");
  }
  else
  {
    digitalWrite(6, !LOW);
    sendSMS("Check for ACC ON");
  }
}

void SendMessageStop()
{
  digitalWrite(6, !LOW);
  sendSMS("Ignition OFF");
}

void wrong_input()
{
  sendSMS("Wrong input. Type 'menu' to get the list.");
}
