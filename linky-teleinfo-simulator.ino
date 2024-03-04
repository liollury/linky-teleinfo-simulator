/**
 * ==========================================================================
 *   Simulateur de trames de teleinfo (Compteur Linky)
 * ==========================================================================
 * + Simule des trames teleinfo en mode Historique
 * + Donne des valeurs aléatoire pour les données IINST & PAPP
 * + Incrémente BASE
 */
int currentBase = 10000;

// Passer à TRUE pour debugger les trames
#define debug false

void setup() {
  if (!debug) {
    Serial.begin(1200, SERIAL_7E1);
  } else {
    Serial.begin(1200);
  }
}

void loop() {
  writeFrame();

  delay(1000);
}

/**
 * Une trame est un groupe de données qui commence par un caractère STX (Start of Text) et fini par un caractère ETX (End of Text)
 */
void writeFrame() {
  #define START_OF_TEXT 0x02
  #define END_OF_TEXT 0x03

  // Début de la trame
  Serial.write(START_OF_TEXT);

  // Adresse du compteur // Caractères: 12
  writeData("ADCO", "123456789012");

  // Option tarifaire choisie // Caractères: 4
  writeData("OPTARIF", "BASE");

  // Intensité souscrite // Caractères: 2 / Unité: A
  writeData("ISOUSC", "30");

  // Consommation totale // Caractères: 9 / Unité: Wh
  currentBase += 1000;
  writeData("BASE", prefixInt(currentBase, 9, "0"));

  // Période Tarifaire en cours // Caractères: 4 
  writeData("PTEC", "TH..");

  // Intensité Instantanée // Caractères: 3 / Unité: A
  int iinst = random(1, 20);
  writeData("IINST", prefixInt(iinst, 3, "0"));

  // Intensité maximale appelée // Caractères: 3 / Unité: A
  writeData("IMAX", "090");

  // Puissance apparente // Caractères: 5 / Unité: VA
  int papp = random(200, 4000);
  writeData("PAPP", prefixInt(papp, 5, "0"));

  // Mot d'état du compteur / Caractères: 6
  writeData("MOTDETAT", "000000");

  // Autres donées pouvant être ajoutée en fonction du compteur:
  // + HCHC: Heures Creuses // Caractères: 9 / Unité: Wh
  // + HCHP: Heures Pleines // Caractères: 9 / Unité: Wh
  // + EJPHN: Heures Normales // Caractères: 9 / Unité: Wh
  // + EJPHPM: Heures de Pointe Mobile // Caractères: 9 / Unité: Wh
  // + BBRHCJB: Heures Creuses Jours Bleus // Caractères: 9 / Unité: Wh
  // + BBRHPJB: Heures Pleines Jours Bleus // Caractères: 9 / Unité: Wh
  // + BBRHCJW: Heures Creuses Jours Blancs // Caractères: 9 / Unité: Wh
  // + BBRHPJW: Heures Pleines Jours Blancs // Caractères: 9 / Unité: Wh
  // + BBRHCJR: Heures Creuses Jours Rouges // Caractères: 9 / Unité: Wh
  // + BBRHPJR: Heures Pleines Jours Rouges // Caractères: 9 / Unité: Wh
  // + PEJP: Préavis Début EJP (30 min) // Caractères: 2 / Unité: min
  // + DEMAIN: Couleur du lendemain // Caractères: 4
  // + ADPS: Avertissement de Dépassement De Puissance Souscrite // Caractères: 3 / Unité: A
  // + HHPHC: Horaire Heures Pleines Heures Creuses // Caractères: 1

  // Fin de la trame
  Serial.write(END_OF_TEXT);
}

/**
 * Une ligne de donnée est représentée comme suit:
 * 
 * <LF> Etiquette <SP> Donnée <SP> Checksum <CR>
 * 0x0A           0x20        0x20          0x0D
 *
 * Exemple
 * <LF>ADCO<SP>123456789012<SP>P<CR>
 */
void writeData(String label, String data) {
  #define LF 0x0A
  #define SP 0x20
  #define CR 0x0D

  Serial.write(LF);
  Serial.print(label);
  Serial.write(SP);
  Serial.print(data);
  Serial.write(SP);
  Serial.print(getChecksum(label, data));
  Serial.write(CR);
}

/**
 * Le checksum valide les valeurs de la ligne de donnée (Etiquette + Donnée)
 * Il est calculé comme suit:
 * + calcul de la somme (S1) de tous les caractères allant du début du champ "Etiquette" jusqu'à la fin du champ "Donnée"
 *   => Dans le cas du mode standard, il faut ajouter le délimiteur entre les champs "Donnée" et "Checksum"
 * + tronque la somme déduite (S1) sur 6 bits
 * + ajoute "0x20" au résultat de la somme précédente
 */
char getChecksum(String label, String data) {
  int labelLength = label.length() + 1;
  char labelChar[labelLength];
  label.toCharArray(labelChar, labelLength);

  int dataLength = data.length() + 1;
  char dataChar[dataLength];
  data.toCharArray(dataChar, dataLength);

  // Calculate checksum
  char line[strlen(labelChar)+strlen(dataChar)+1];
  strcpy(line, labelChar);
  strcat(line, " ");
  strcat(line, dataChar);
  char checksum = 0;
  for(int i = 0; i < strlen(line); i++ ) {
    char c = line[i];
    checksum += c;
  }
  checksum &= 0x3F;
  checksum += 0x20;

  return checksum;
}

/**
 * Créé une chaine de caractères d'une certaine taille à partir d'un nombre et
 * préfixe cette chaine si elle est trop petite
 *
 * @param number Nombre à convertir en String et à préfixer
 * @param size Taille finale que doit faire la chaine de caractères
 * @param prefix Prefix à utiliser si la chaine de caractères est trop petite
 */
String prefixInt(int number, int size, String prefix) {
  String numberStr = "";
  numberStr += number;
  String strPrefix = "";

  for (int i = 0; i < size - numberStr.length(); i++) {
    strPrefix += prefix;
  }

  return strPrefix + numberStr;
}