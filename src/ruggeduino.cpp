#include <Arduino.h>

// We communicate, by default, with the power board at 9200 baud.
void setup() {
  Serial.begin(9600);
}

/* Read a pin ID from the serial port and return it.
   We use readBytes here for the blocking semantics - a loop going by without
   a command is fine so we use read() there, but here we need to wait until
   the pin is received. */
int read_pin() {
  char inputByte;
  Serial.readBytes(&inputByte, 1);
  // Because pin 0 is encoded to the byte 0 and so on, we can just upcast
  return (int)(inputByte - 'a');
}

void command_read() {
  int pin = read_pin();
  // Read from the expected pin.
  int level = digitalRead(pin);
  // Send back the result indicator.
  if (level == HIGH) {
    Serial.write('h');
  } else {
    Serial.write('l');
  }
}

void command_write(int level) {
  int pin = read_pin();
  digitalWrite(pin, level);
}

void command_mode(int mode) {
  int pin = read_pin();
  pinMode(pin, mode);
}

void loop() {
  // Fetch all commands that are in the buffer
  while (Serial.available()) {
    int selected_command = Serial.read();
    // Do something different based on what we got:
    switch (selected_command) {
      case 'r':
        command_read();
        break;
      case 'l':
        command_write(LOW);
        break;
      case 'h':
        command_write(HIGH);
        break;
      case 'i':
        command_mode(INPUT);
        break;
      case 'o':
        command_mode(OUTPUT);
        break;
      case 'p':
        command_mode(INPUT_PULLUP);
        break;
      case 'a':
        Serial.print("Hello");
        break;
      default:
        // A problem here: we do not know how to handle the command!
        // Just ignore this for now.
        break;
    }
  }
  // Pause briefly
  delay(1);
}
