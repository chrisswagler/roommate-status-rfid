# roommate-status-rfid
The purpose of this project was to design a system for the apartment using university IDs. Using an RFID tag reader and an LCD screen, data could be stored regarding the last time each roommate had tapped their card. 

# Background
The design was inspired as a method to keep track of who was present in the apartment. The initial system was simply hanging the IDs by the door, so that when someone left, their ID would not be on the wall. However, the system deteriorated when roommates forgot to hang their ID or did not use their ID lanyard anymore. By using an RFID reader, it would eliminate the need to have the physical ID hanging. The first concept for this project was to use the reader and LCD to indicate when someone was in or out of the apartment, essentially flipping the bit for their status. This would rely heavily on the human interaction and leave room for error if an ID was not tapped before entering/exiting or if an ID was tapped multiple times. To remove this error, the system was instead designed to output when the last tap was. While this would not explicitly indicate if someone was in/out, it would be a more accurate indicator for when a roommate was last seen. 

# Components Used
<ul>
  <li>Arduino Uno</li>
  <li>Breadboard</li>
  <li>LCD Display</li>
  <li>MFRC522 RFID Reader</li>
  <li>330 Ohm Resistor</li>
</ul>

# Libraries Used
<ul>
  <li>LiquidCrystal</li>
  <li>MFRC522</li>
</ul>

# Functionality
After initializing a roommate with a name and ID (obtained with the DumpInfo sketch in the MFRC522 library), this program keeps track of how long ago a roommate's ID was tapped. Every two seconds, the display updates to show each roommate's status. If an ID is tapped to the reader, a welcome message immediately pops up to welcome the user, and their time info is reset. The welcome message remains on screen for two seconds, and then the roommate status loop picks up where it last left off. If an unknown ID is tapped, the display indicates that the ID is unknown.

As a note, the string IDs in the file have been changed from the DumpInfo data.

To modify the program for different numbers of roommates, a simple change could be made to add/remove roommate objects along with the array size and numOfRoommate variable adjusted accordingly. Additionally, the delay time was set to two seconds but could easily be adjusted as well. 

# Limitations
This program was designed precisely as it was envisioned. Its primary limitation was in the fact that it still didn't provide a concrete answer for if someone left in the apartment. 

An alternative design is to also have a boolean field to indicate if a roommate left. Tapping in would flip the status for leaving/entering, and the time component could remain. However, knowing my roommates, I know this system would not work for us and would just lead to false information displayed. 
