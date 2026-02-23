Berço iteligente 

#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <Servo.h>

// --- Definições de pinos e limites ---
#define DHTPIN        2           // DHT11
#define DHTTYPE       DHT11
#define TEMP_LIMITE   30           // °C
#define PINO_VENT     8           // Ventilador
#define PINO_SOM      3           // Sensor de som digital
#define PINO_PIR      4           // PIR (INPUT_PULLUP)
#define PINO_UMID     A0          // Sensor de umidade
#define UMID_LIMITE   400
#define PINO_SERVO    11          // Servo motor

// --- Constantes para debounce/histerese ---
const unsigned long PIR_DEBOUNCE     = 200;
const unsigned long UMID_ALERT_INTV  = 10000;
const unsigned long SERVO_DURATION  = 30000;

// --- Objetos ---
DHT                dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C  lcd(0x27, 16, 2);
SoftwareSerial     mp3Serial(12, 13);
DFRobotDFPlayerMini dfplayer;
Servo              motorServo;

// --- Estados e tempos ---
float  temperatura    = 0;
bool   ventiladorOn   = false;
bool   musicaOn       = false;
bool   motorOn        = false;
bool   pirState       = false;
int    micVal         = 0;
int    umidVal        = 0;

unsigned long tempoSomSel   = 0;
unsigned long tempoMotorSel = 0;
unsigned long lastPirTime   = 0;
unsigned long lastUmidAlert = 0;

void setup() {
  pinMode(PINO_VENT, OUTPUT);
  pinMode(PINO_SOM,  INPUT);
  pinMode(PINO_PIR,  INPUT_PULLUP);
  pinMode(PINO_UMID, INPUT);

  Serial.begin(9600);
  dht.begin();
  lcd.init(); lcd.backlight();

  mp3Serial.begin(9600);
  if (!dfplayer.begin(mp3Serial)) {
    Serial.println("Erro DFPlayer");
    while (true) delay(100);
  }

  motorServo.detach();

  lcd.setCursor(0,0);
  lcd.print("Sistema ON");
  lcd.setCursor(0,1);
  lcd.print("Iniciando...");
  delay(2000);
  lcd.clear();

  enviarSMS("Sistema iniciado.");
}

void loop() {
  unsigned long now = millis();
  temperatura = dht.readTemperature();
  bool somDet = digitalRead(PINO_SOM);
  umidVal     = analogRead(PINO_UMID);
  bool umidAl = (umidVal < UMID_LIMITE);
  bool pirRaw = (digitalRead(PINO_PIR) == LOW);

  // Ventilador com histerese
  if (!isnan(temperatura)) {
    if (temperatura > TEMP_LIMITE && !ventiladorOn) {
      digitalWrite(PINO_VENT, HIGH);
      ventiladorOn = true;
      enviarSMS("Temp alta: ventilador ON");
    } else if (temperatura < (TEMP_LIMITE - 2) && ventiladorOn) {
      digitalWrite(PINO_VENT, LOW);
      ventiladorOn = false;
      enviarSMS("Temp ok: ventilador OFF");
    }
  }

  // Alerta de umidade com intervalo
  if (umidAl && (now - lastUmidAlert > UMID_ALERT_INTV)) {
    enviarSMS("Alerta: umidade alta!");
    lastUmidAlert = now;
  }

  // Som detectado -> toca música 30s
  if (somDet && !musicaOn) {
    dfplayer.play(1);
    musicaOn     = true;
    tempoSomSel  = now;
    enviarSMS("Som detectado: musica ON");
  }
  if (musicaOn && (now - tempoSomSel >= 30000)) {
    dfplayer.stop();
    musicaOn = false;
    enviarSMS("Musica OFF");
  }

  // PIR debounce e servo
  if (pirRaw != pirState && (now - lastPirTime) > PIR_DEBOUNCE) {
    pirState     = pirRaw;
    lastPirTime  = now;
    if (pirState && !motorOn) {
      motorServo.attach(PINO_SERVO);
      motorOn      = true;
      tempoMotorSel= now;
      enviarSMS("Movimento: servo ON");
    }
  }

  // Movimento do servo até 30s
  if (motorOn) {
    if (now - tempoMotorSel <= SERVO_DURATION) {
      motorServo.write(0);
      delay(400);
      motorServo.write(90);
      delay(400);
    } else {
      motorServo.detach();
      motorOn = false;
      enviarSMS("Servo OFF pos ciclo");
    }
  }

  checarComandosSMS();
  atualizarLCD(umidAl);
}

void enviarSMS(const String &msg) {
  Serial.println("AT+CMGF=1"); delay(200);
  Serial.println("AT+CMGS=\"+244939621311\""); delay(200);
  Serial.print(msg);
  Serial.write(26);
  delay(1000);
}

void checarComandosSMS() {
  if (!Serial.available()) return;
  String sms = Serial.readString(); sms.toUpperCase();
  if (sms.indexOf("DESLIGA VENTILADOR") >= 0) {
    digitalWrite(PINO_VENT, LOW);
    ventiladorOn = false;
    enviarSMS("CMD: Vent OFF");
  }
  if (sms.indexOf("DESLIGA SERVO") >= 0) {
    motorServo.detach();
    motorOn = false;
    enviarSMS("CMD: Servo OFF");
  }
}

void atualizarLCD(bool umidAlta) {
  lcd.setCursor(0,0);
  lcd.print("T:");
  if (!isnan(temperatura)) lcd.print(temperatura,1);
  else lcd.print("--");
  lcd.print("C ");
  lcd.print(umidAlta ? "Molhado" : "Seco   ");

  lcd.setCursor(0,1);
  lcd.print("V:"); lcd.print(ventiladorOn?"On ":"Off");
  lcd.print(" M:"); lcd.print(musicaOn?"On ":"Off");
  lcd.print(" B:"); lcd.print(motorOn?"On":"Off");
}
