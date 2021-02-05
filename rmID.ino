#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>

#define SS_PIN 10
#define RST_PIN 9

//represents a roommate
class Roommate {
  public:
    String user;
    String id;
    int hours;
    int mins;
    unsigned long prev;

    //constructor using the user name and id
    Roommate(String user, String id) {
      this->user = user;
      this->id = id;
      this->hours = 0;
      this->mins = 0;
      this->prev = 0;
    }

    //calculates the time passed based on the current millis (_curr_)
    void timePassed(unsigned long curr) {
      int minutes = ceil((curr - this->prev) / 60000);
      this->hours = floor(minutes / 60);
      this->mins = minutes % 60;
    }

    //clears the time and sets this user's previous millis (_prev_)
    //to current millis (curr)
    void clearTime(unsigned long curr) {
      this->hours = 0;
      this->mins = 0;
      this->prev = curr;
    }
};

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

//set up LCD display based on pins connected to
const int rs = 2, en = 4, d4 = 3, d5 = 5, d6 = 8, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int contrastLevel = 90; //viable contrast level for this particular LCD
unsigned long curr; //to store the current millis
int disp = 0; //to change which roommate is displayed
int delayTime = 2000;

//IDs as Strings, obtained with DumpInfo sketch
String cid = "00 00 00 00 00 00 00";
String eid = "11 11 11 11 11 11 11";
String nid = "22 22 22 22 22 22 22";
String gid = "33 33 33 33 33 33 33";
String sid = "44 44 44 44 44 44 44";

//create roommate objects with a name and id
const int numOfRoommates = 5;
Roommate chris = Roommate("Chris", cid);
Roommate eshan = Roommate("Eshan", eid);
Roommate nick = Roommate("Nick", nid);
Roommate george = Roommate("George", gid);
Roommate sam = Roommate("Sam", sid);
//create roommate array to cycle through
Roommate rm[5] = {chris, eshan, nick, george, sam};

void setup()
{
  pinMode(6, OUTPUT);
  analogWrite(6, contrastLevel); //use the contrast level with resistor for LCD
  lcd.begin(16, 2);
  lcd.clear();
  
  Serial.begin(9600);   // Initiate a serial communication
  
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  
  //set all the initial previous millis values
  for (int i = 0; i < numOfRoommates; i++) {
    rm[i].prev = millis();
  }
}

void loop()
{
  //when a new card isn't present
  while (! mfrc522.PICC_IsNewCardPresent()) {
    disp = disp % numOfRoommates;
    curr = millis();
    displayInfo(curr,disp);
    
    //remain on screen for 2 sec or until a new card is detected
    while (millis() < curr + delayTime) {
      
      //if a new card is present, skip out of the loop and move on
      if (mfrc522.PICC_IsNewCardPresent()) {
        goto nextSequence;
      }
    }

    //display the next roommate in sequence
    disp++;
  }

nextSequence:

  //select one of the cards
  if (! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  //get the content of the ID and welcome the user
  String content = "";
  obtainID(content);
  welcomeUser(content);
}

//displays the current roommate info
void displayInfo(unsigned long curr, int disp) {
  rm[disp].timePassed(curr);
  Serial.println((String) rm[disp].user + " last seen");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print((String) rm[disp].user + " last seen");

  Serial.println((String) rm[disp].hours + " h " + rm[disp].mins + " m ago");
  lcd.setCursor(0, 1);
  lcd.print((String) rm[disp].hours + " h " + rm[disp].mins + " m ago");
}

//obtains the ID and concats to _content_
void obtainID(String& content) {

  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  content = content.substring(1);
}

//welcomes the user based on the provided content
void welcomeUser(String content) {
  //loop through the array +1 more time than the _numOfRoommates_
  for (int j = 0; j < (numOfRoommates + 1); j++) {
    
    //if the index has reached the _numOfRoommates_, there wasn't an ID match
    if (j == numOfRoommates) {
      Serial.println("User not recognized");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("User not recognized");
      break;
    }
    
    //otherwise check which ID it matches to
    else if (content == rm[j].id) {
      Serial.println((String) "Welcome back " + rm[j].user);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Welcome back");
      lcd.setCursor(0, 1);
      lcd.print((String) rm[j].user);

      //clear the time for this user
      rm[j].clearTime(millis());
      break;
    }
  }

  //keep this info on screen for 2 sec
  curr = millis();
  while (millis() < curr + delayTime);
}
