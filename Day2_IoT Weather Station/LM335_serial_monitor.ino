const int Temp_Pin = 26; //This code records temp from LM335 and displays on serial monitor
float Raw_Val;
float Voltage_Out;

float temp_C, temp_F, temp_K;

void setup() {
  pinMode(Temp_Pin, INPUT);
  Serial.begin(115200);
}

void loop() {
  Raw_Val = analogRead(Temp_Pin);
  Voltage_Out = (Raw_Val * 3300) / (4096);

  // calculate temperature for LM35 (LM35DZ)
  //temp_C = Voltage_Out/ 10;
  //temp_F = (temp_C * 1.8) + 32;

  // calculate temperature for LM335
  temp_K = Voltage_Out / 10;
  temp_C = temp_K - 273;
  temp_F = (temp_C * 1.8) + 32;

  Serial.print("Temperature(ºC): ");
  Serial.print(temp_C);
  Serial.print("  Temperature(ºF): ");
  Serial.print(temp_F);
  Serial.print("  Voltage(mV): ");
  Serial.println(Voltage_Out);
  delay(1000);
} 
