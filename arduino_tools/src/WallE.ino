#include <Servo.h>

Servo Head, Right_Eye, Left_Eye;

const int metalDetectorPin = A0; // Pin analogique où le détecteur est connecté
const int metalDetectorPin2 = A1; // Pin analogique où le détecteur est connecté

const int signalLevel = 200; // Seuil de détection
const int consecutiveThreshold = 4; // Nombre de lectures consécutives nécessaires
int consecutiveCount = 0; // Compteur de lectures consécutives
bool isMetalDetected = false; // État de détection

// Définition des angles
const int HEAD_CENTER = 85;
const int HEAD_RIGHT = 20;
const int HEAD_LEFT = 120;

const int RIGHT_EYE_UP = 180;
const int RIGHT_EYE_DOWN = 140;

const int LEFT_EYE_UP = 30;
const int LEFT_EYE_DOWN = 70;

void initServos() {
  Head.attach(8);        // Broche du servo de la tête
  Right_Eye.attach(9);  // Broche du servo de l'œil droit
  Left_Eye.attach(10);   // Broche du servo de l'œil gauche

  // Position par défaut : tête centrée, yeux baissés
  //setHeadCenter();
  //setEyesDown();
}

void setHeadCenter() {
  Head.write(HEAD_CENTER);
}

void setEyesUp() {
  Right_Eye.write(RIGHT_EYE_UP);
  Left_Eye.write(LEFT_EYE_UP);
}

void performDetectionAnimation() {
  // 1. Hausse les yeux
  /*Right_Eye.write(RIGHT_EYE_UP);
  delay(1000);
  Left_Eye.write(LEFT_EYE_UP);
  delay(1000); // Pause pour l'effet visuel*/

  // 2. Tourne la tête à droite, puis à gauche
  Head.write(HEAD_RIGHT);
  delay(500);
  Head.write(HEAD_LEFT);
  delay(500);
  setHeadCenter(); // Retour au centre
  delay(500);

  // 3. Baisse l'œil droit puis le remonte
  /*Right_Eye.write(RIGHT_EYE_DOWN);
  delay(300);
  Right_Eye.write(RIGHT_EYE_UP);
  delay(300);

  // 4. Baisse l'œil gauche puis le remonte
  Left_Eye.write(LEFT_EYE_DOWN);
  delay(300);
  Left_Eye.write(LEFT_EYE_UP);
  delay(300);

  // 5. Retour à la position normale
  setEyesUp();*/
}

void setup(){
  initServos();
  Serial.begin(9600);
  Serial.println("Détecteur de métaux prêt !");
}

void loop(){  
   
  int signalValue1 = analogRead(metalDetectorPin);
  int signalValue2 = analogRead(metalDetectorPin2);

  // Calcul de la différence entre les deux valeurs
  int signalValue = signalValue2 - signalValue1;

  // Vérification si le signal dépasse le seuil
  if (signalValue >= signalLevel) {
    consecutiveCount++; // Incrémentation du compteur
    if (consecutiveCount >= consecutiveThreshold) {
      if (!isMetalDetected) { // Si métal non détecté précédemment
        isMetalDetected = true;
        Serial.println("Métal détecté !");
        performDetectionAnimation(); // Déclencher l'animation
      }
    }
  } else {
    consecutiveCount = 0; // Réinitialisation du compteur si le signal est en dessous du seuil
    isMetalDetected = false; // Réinitialisation de l'état de détection
  }

  // Affichage de la donnée correspondant au niveau de signal
  Serial.print("Signal détecté : ");
  Serial.println(signalValue);

  // Pause avant la prochaine lecture
  delay(200); // Ajuster le délai selon les besoins
}