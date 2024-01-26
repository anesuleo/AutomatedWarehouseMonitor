//Left Motor
const int MOTOR_L_PIN = 33;
const int ENABLE_L_PIN = 32;

//Right Motor
const int MOTOR_R_PIN = 27;
const int ENABLE_R_PIN = 14;

//Setting PWM properties
const int FREQ = 30000;
const int PWM_CHANNEL = 0;
const int RESOLUTION = 8;
int dutyCycle = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(MOTOR_R_PIN, OUTPUT);
  pinMode(MOTOR_L_PIN, OUTPUT);
  ledcSetup(PWM_CHANNEL, FREQ, RESOLUTION);
  ledcAttachPin(ENABLE_R_PIN, PWM_CHANNEL);
  ledcAttachPin(ENABLE_L_PIN, PWM_CHANNEL);
  ledcWrite(PWM_CHANNEL, dutyCycle);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  dutyCycle = 200;
  ledcWrite(PWM_CHANNEL, dutyCycle);
  reverse();
  
}