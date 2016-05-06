
/**
   Explore the use of interrupts and timers
*/

int loopCount = 0;
#define pot0 A6
#define pot1 A7
#define pot2 A8
#define pot3 A9

#define sw0 5
#define button0 10

ISR(TIMER1_COMPA_vect) {
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  
  pinMode(pot0, INPUT);
  pinMode(pot1, INPUT);
  pinMode(pot2, INPUT);
  pinMode(pot3, INPUT);
  
  pinMode(sw0, INPUT);
  pinMode(button0, INPUT);

  // initialize Timer1
  cli();          // disable global interrupts
  TCCR1A = 0;     // set entire TCCR1A register to 0
  TCCR1B = 0;     // same for TCCR1B

  // set compare match register to desired timer count:
  OCR1A = 15624;
  // turn on CTC mode:
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler:
  TCCR1B |= (1 << CS10);
  TCCR1B |= (1 << CS12);
  // enable timer compare interrupt:
  TIMSK1 |= (1 << OCIE1A);
  sei();          // enable global interrupts
  }

  void loop() {
    delay(500);

    int vB0 = digitalRead(button0);
    int vP0 = analogRead(pot0);    // read the input pin
    int vP2 = analogRead(pot2);    // read the input pin

    Serial.print(loopCount);
    Serial.print(", button0: ");
    Serial.print(vB0);

    Serial.print(", pot0: ");
    Serial.print(vP0);
    Serial.print(", pot2: ");
    Serial.println(vP2);

    delay(500);

    loopCount += 1;
  }
