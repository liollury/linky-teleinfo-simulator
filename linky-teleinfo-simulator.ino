/**
 * ==========================================================================
 *   Simulateur de trames de teleinfo (Compteur Linky)
 * ==========================================================================
 * + Simule des trames teleinfo en mode Historique
 * + Choix de l'option tarifaire à simuler
 * + Donne des valeurs aléatoire pour les données IINST & PAPP
 * + Incrémente BASE / HCHC / HCHP / EJPHN / EJPHPM / BBRHCJB / BBRHPJB / BBRHCJW / BBRHPJW / BBRHCJR / BBRHPJR
 */

// Option tarifaire choisie pour la simulation des trames
// Valeurs possibles:
//   + BASE: Option classique
//   + HC..: Option Heure Pleine / Heure Creuse
//   + EJP.: Option EJP
//   + BBRx: Option Tempo
#define optionTarifaireConfig "BASE"

// Passer à TRUE pour debugger les trames
#define debug false

long currentBASE = 10000;

long currentHCHC = 20000;
long currentHCHP = 10000;

long currentEJPHN = 20000;
long currentEJPHPM = 10000;

long currentBBRHCJB = 20000;
long currentBBRHPJB = 10000;
long currentBBRHCJW = 20000;
long currentBBRHPJW = 10000;
long currentBBRHCJR = 20000;
long currentBBRHPJR = 10000;

void setup() {
  if (!debug) {
    Serial.begin(1200, SERIAL_7E1);
  } else {
    Serial.begin(1200);
  }
}

void loop() {
  writeFrame(optionTarifaireConfig);

  delay(1000);
}

/**
 * Ecrit une trame télé-information pour l'option tarifaire choisie
 * Une trame est un groupe de données qui commence par un caractère STX (Start of Text) et fini par un caractère ETX (End of Text)
 *
 * @param optionTarifaire Option tarifaire choisie. BASE / HC.. / EJP. / BBRx
 */
void writeFrame(String optionTarifaire) {
  #define START_OF_TEXT 0x02
  #define END_OF_TEXT 0x03

  // Début de la trame
  Serial.write(START_OF_TEXT);

  // Adresse du compteur // Caractères: 12
  writeData("ADCO", "123456789012");

  // Option tarifaire choisie // Caractères: 4
  writeData("OPTARIF", optionTarifaire);

  // Intensité souscrite // Caractères: 2 / Unité: A
  writeData("ISOUSC", "30");

  String ptec = "";
  
  // Option Base
  if (optionTarifaire == "BASE") {
    // TH..: Toutes les Heures
    ptec = "TH..";

    // Consommation totale // Caractères: 9 / Unité: Wh
    currentBASE += random(100, 2000);
    writeData("BASE", prefixInt(currentBASE, 9, "0"));

  // Option Heure Pleine / Heure Creuse
  } else if (optionTarifaire == "HC..") {
    // HC..: Heures Creuses
    // HP..: Heures Pleines
    ptec = "HC..";

    // HHPHC: Horaire Heures Pleines Heures Creuses (Lettre qui définie la plage horaire souscrite dans le contrat) // Caractères: 1
    writeData("HHPHC", "A");

    // Heures Creuses // Caractères: 9 / Unité: Wh
    currentHCHC += random(100, 2000);
    writeData("HCHC", prefixInt(currentHCHC, 9, "0"));

    // Heures Pleines // Caractères: 9 / Unité: Wh
    currentHCHP += 1000;
    writeData("HCHP", prefixInt(currentHCHP, 9, "0"));

  // Option EJP
  } else if (optionTarifaire == "EJP.") {
    // HN..: Heures Normales
    // PM..: Heures de Pointe Mobile
    ptec = "HN..";

    // Heures Normales // Caractères: 9 / Unité: Wh
    currentEJPHN += random(100, 2000);
    writeData("EJPHN", prefixInt(currentEJPHN, 9, "0"));

    // Heures de Pointe Mobile // Caractères: 9 / Unité: Wh
    currentEJPHPM += random(100, 2000);
    writeData("EJPHPM", prefixInt(currentEJPHPM, 9, "0"));

    // Préavis Début EJP (30 min) // Caractères: 2 / Unité: min
    int pejp = random(1, 60);
    writeData("PEJP", prefixInt(pejp, 2, "0"));

  // Option Tempo
  } else if (optionTarifaire == "BBRx") {
    // HCJB: Heures Creuses Jours Bleus
    // HCJW: Heures Creuses Jours Blancs
    // HCJR: Heures Creuses Jours Rouges
    // HPJB: Heures Pleines Jours Bleus
    // HPJW: Heures Pleines Jours Blancs
    // HPJR: Heures Pleines Jours Rouges
    ptec = "HCJB";

    // Heures Creuses Jours Bleus // Caractères: 9 / Unité: Wh
    currentBBRHCJB += random(100, 2000);
    writeData("BBRHCJB", prefixInt(currentBBRHCJB, 9, "0"));

    // Heures Pleines Jours Bleus // Caractères: 9 / Unité: Wh
    currentBBRHPJB += random(100, 2000);
    writeData("BBRHPJB", prefixInt(currentBBRHPJB, 9, "0"));

    // Heures Creuses Jours Blancs // Caractères: 9 / Unité: Wh
    currentBBRHCJW += random(100, 2000);
    writeData("BBRHCJW", prefixInt(currentBBRHCJW, 9, "0"));

    // Heures Pleines Jours Blancs // Caractères: 9 / Unité: Wh
    currentBBRHPJW += random(100, 2000);
    writeData("BBRHPJW", prefixInt(currentBBRHPJW, 9, "0"));

    // Heures Creuses Jours Rouges // Caractères: 9 / Unité: Wh
    currentBBRHCJR += random(100, 2000);
    writeData("BBRHCJR", prefixInt(currentBBRHCJR, 9, "0"));

    // Heures Pleines Jours Rouges // Caractères: 9 / Unité: Wh
    currentBBRHPJR += random(100, 2000);
    writeData("BBRHPJR", prefixInt(currentBBRHPJR, 9, "0"));

    // + DEMAIN: Couleur du lendemain (BLEU / BLAN / ROUG) // Caractères: 4
    writeData("DEMAIN", "BLEU");

  }

  // Période Tarifaire en cours // Caractères: 4 
  writeData("PTEC", ptec);

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

  // Avertissement de Dépassement De Puissance Souscrite // Caractères: 3 / Unité: A
  writeData("ADPS", "100");

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
 * 
 * @param label Etiquette de la ligne
 * @param data Donnée de la ligne
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
 *
 * @param label Etiquette de la ligne
 * @param data Donnée de la ligne
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