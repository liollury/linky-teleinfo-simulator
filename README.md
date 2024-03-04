# Simulateur de trames Linky - Télé-information

Programme à ajouter sur une carte Arduino (ou équivalent) pour simuler des trames de télé-information pour un compteur Linky en mode **Historique**

[Specs compteur Linky](./Specs_Compteur_Linky.pdf)

## Simulateur

Compiler les sources du fichier [linky-teleinfo-simulator.ino](linky-teleinfo-simulator.ino) sur une carte Arduino.

Celle-ci va simuler des trames télé-info et envoyer des valeurs aléatoires pour les données suivantes:
* **BASE**: **+1kWh** par tour de boucle
* **IINST**: valeur aléatoire entre **1** et **20** **A**
* **PAPP**: valeur aléatoire entre **200** et **4000** **VA**

## ESP Home

Fichier [tele-information.yaml](tele-information.yaml) d'exemple pour lire les trames de télé-information sur un ESP32 à l'aide d'[ESPHome](https://esphome.io/index.html).

```sh
esphome run tele-information.yaml
```