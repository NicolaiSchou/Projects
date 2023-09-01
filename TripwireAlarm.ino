const int triggerPin = 2;    // Pin til at sende trigger-signal til HC-SR04
const int echoPin = 3;       // Pin til at modtage echo-signal fra HC-SR04
const int speakerPin = 4;    // Pin til tilslutning af højtaleren

int sensorThreshold = 50;    // Tærskelafstand for at aktivere tripwire (i centimeter)
boolean tripwireActive = false;  // Variabel til at spore tripwire-tilstanden

void setup() {
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(speakerPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  long duration, distance;
  
  // Sender ultralydssignal
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  
  // Måler tiden for ultralydssignalet til at returnere
  duration = pulseIn(echoPin, HIGH);
  
  // Beregner afstanden baseret på tiden
  distance = duration * 0.034 / 2;
  
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  if (distance > 0 && distance < sensorThreshold && !tripwireActive) {
    tripwireActive = true;
    playSound(1000, 500);  // Afspil en lyd med 1 kHz frekvens i 500 ms
    Serial.println("Tripwire activated!");
  }
  
  if (distance >= sensorThreshold) {
    tripwireActive = false;
  }
}

// Funktion til at afspille en lyd på højtaleren
void playSound(unsigned int frequency, unsigned long duration) {
  unsigned long period = 1000000 / frequency;  // Beregning af perioden for den ønskede frekvens
  unsigned long halfPeriod = period / 2;       // Halvperiode til toggling af højtalerens tilstand
  unsigned long numCycles = frequency * duration / 1000;  // Antal cyklusser baseret på ønsket varighed
  
  for (unsigned long i = 0; i < numCycles; i++) {
    digitalWrite(speakerPin, HIGH);    // Skifter højtalerens tilstand til HIGH
    delayMicroseconds(halfPeriod);     // Venter i halvdelen af perioden
    digitalWrite(speakerPin, LOW);     // Skifter højtalerens tilstand til LOW
    delayMicroseconds(halfPeriod);     // Venter i halvdelen af perioden
  }
}
