# Simulateur de trames Linky - Télé-information

Programme à ajouter sur une carte Arduino (ou équivalent) pour simuler des trames de télé-information pour un compteur Linky en mode **Historique**

[Specs compteur Linky](./Specs_Compteur_Linky.pdf)

## Simulateur

Choisir l'option tarifaire à simuler en modifier la variable **optionTarifaireConfig**

Compiler les sources du fichier [linky-teleinfo-simulator.ino](linky-teleinfo-simulator.ino) sur une carte Arduino.

Celle-ci va simuler des trames télé-info et envoyer des valeurs aléatoires pour les données suivantes:
* **BASE** / **HCHC** / **HCHP** / **EJPHN** / **EJPHPM** / **BBRHCJB** / **BBRHPJB** / **BBRHCJW** / **BBRHPJW** / **BBRHCJR** / **BBRHPJR**: **+1kWh** par tour de boucle
* **IINST**: valeur aléatoire entre **1** et **20** **A**
* **PAPP**: valeur aléatoire entre **200** et **4000** **VA**

## ESP Home

Un fichier d'exemple pour lire les trames de télé-information sur un ESP32 à l'aide d'[ESPHome](https://esphome.io/index.html) est fourni.

Choisir le fichier de configuration en fonction de son option tarifaire:
* **Base**: [tele-information-BASE.yaml](tele-information-BASE.yaml)
* **Heure Pleine / Heure Creuse**: [tele-information-HPHC.yaml](tele-information-HPHC.yaml)
* **EJP**: [tele-information-EJP.yaml](tele-information-EJP.yaml)
* **Tempo**: [tele-information-TEMPO.yaml](tele-information-TEMPO.yaml)

```sh
esphome run tele-information-BASE.yaml
```

### Serveur Web

Un serveur web est accessible à l'adresse IP de votre esp32. Pour que cela fonctionne, il faut configurer le WiFi dans le fichier ESPHome utilisé:

```yaml
wifi:
  # A MODIFIER dans le fichier yaml
  ssid: "SSID"
  # A MODIFIER dans le fichier yaml
  password: "SSID_PASSWORD"
```

### Home Assistant

Il est aussi possible de brancher ESPHome avec Home Assistant pour traiter les informations de manière plus avancée:

https://esphome.io/guides/getting_started_hassio.html