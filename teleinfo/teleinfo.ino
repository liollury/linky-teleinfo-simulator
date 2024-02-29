void setup() {
  Serial.begin(1200, SERIAL_7E1);
}

void loop() {
  Serial.write(0x02);
  writeLine("ADCO", "123456789012");
  writeLine("OPTARIF", "BASE");
  writeLine("ISOUSC", "30");
  writeLine("BASE", "002565285");
  writeLine("PTEC", "TH..");
  writeLine("IINST", "002");
  writeLine("IMAX", "030");
  writeLine("PAPP", "00420");
  writeLine("MOTDETAT", "000000");
  Serial.write(0x03);
  delay(1000);
}

void writeLine(char* symb, char* data) {
  char line[strlen(symb)+strlen(data)+1];
  strcpy(line, symb);
  strcat(line, " ");
  strcat(line, data);
  char checksum = 0;
  for(int i = 0; i < strlen(line); i++ ) {
    char c = line[i];
    checksum += c;
  }
  checksum &= 0x3F;
  checksum += 0x20;

  Serial.write(0x0A);
  Serial.print(symb);
  Serial.write(0x20);
  Serial.print(data);
  Serial.write(0x20);
  Serial.print(checksum);
  Serial.write(0x0D);
}
