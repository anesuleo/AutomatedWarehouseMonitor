void forward()
{
  Serial.println("Forward");
  ledcWrite(PWM_CHANNEL, dutyCycle);
  digitalWrite(MOTOR_R_PIN, LOW);
  digitalWrite(MOTOR_L_PIN, HIGH);
}

void left()
{
  Serial.println("Left");
  ledcWrite(PWM_CHANNEL, dutyCycle);
  digitalWrite(MOTOR_R_PIN, HIGH);
  digitalWrite(MOTOR_L_PIN, HIGH);
}

void stop()
{
  Serial.println("Stop");
  ledcWrite(PWM_CHANNEL, 0);
}

void right()
{
  Serial.println("Right");
  ledcWrite(PWM_CHANNEL, dutyCycle);
  digitalWrite(MOTOR_R_PIN, LOW);
  digitalWrite(MOTOR_L_PIN, LOW);
}

void reverse()
{
  Serial.println("Reverse");
  ledcWrite(MOTOR_R_PIN, HIGH);
  digitalWrite(MOTOR_R_PIN, HIGH);
  digitalWrite(MOTOR_L_PIN, LOW);
}