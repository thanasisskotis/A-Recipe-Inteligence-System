/*
  test_RTC1302.ino
  Il programma verifica il funzionamento del modulo MH-Real-Time
  Clock Module 2 mostrando data e ora su Monitor seriale o
  Display LCD 16x2 collegato tramite interfaccia I2C
  Basato su libreria di Henning Karlsen web: http://www.RinkyDinkElectronics.com/

  Sono utilizzati i seguenti pin
  Pin +5V           -> Alimentazione modulo display e RTC
  Pin GND           -> GND modulo modulo display e RTC
  Pin Digital 2     -> Pin CE - RST modulo RTC
  Pin Digital 3     -> Pin I/O - DAT modulo RTC
  Pin Digital 4     -> Pin CLCK - CLK modulo RTC
  Pin Analogic A4   -> Pin SDA display
  Pin Analogic A5   -> Pin SCL display

  Creato il 15/12/2016 da Adriano Gandolfo
  Sito web http://www.adrirobot.it
  Blog http://it.emcelettronica.com/author/adrirobot
  Pagina Facebook https://www.facebook.com/Adrirobot-318949048122955
  Istagram https://www.instagram.com/adrirobot/
  This example code is in the public domain.
*/

#include <DS1302.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Creazione oggetto RTC
DS1302 rtc(2, 3, 4);

// Creazione dell'oggetto LCD
LiquidCrystal_I2C lcd(0x27, 16, 2); // Imposta l'indirizzo LCD 0x27 per un display a 16 caratteri e 2 linee

void setup()
{
  // Imposta RTC in run-mode e disabilita' la protezione da scrittura
  rtc.halt(false);
  rtc.writeProtect(false);

  // Imposta la comunicazione seriale
  Serial.begin(9600);

  // inizializza il display
  lcd.init();
  lcd.backlight();//accende la retroilluminazione

  // Le seguenti linee possono essere commentate per utilizzare i valori già memorizzati nel DS1302
  //rtc.setDOW(SUNDAY);        // Imposta il giorno della settimana a SUNDAY
  //rtc.setTime(11, 32, 0);     // Imposta l'ora come 11:32:00 (Formato 24hr)
  //rtc.setDate(12, 2, 2017);   // Imposta la data cone 12 febbraio 2017
}

void loop()
{
  // Invia giorno della settimana
  Serial.print(rtc.getDOWStr());
  Serial.print(" ");

  // Invia data
  Serial.print(rtc.getDateStr());
  Serial.print(" -- ");

  // Invia ora
  Serial.println(rtc.getTimeStr());

  // Scrive i valori sul display LCD
  lcd.clear();//cancella precedenti valori
  data (); // Scrive su LCD la data
  ora (); // Scrive su LCD l'ora

// Aspetta un secondo prima di ripetere
  delay (1000);
}

void data ()
{
  lcd.setCursor(0, 0);
  lcd.print(rtc.getDOWStr());
  lcd.print(" ");
  lcd.print(rtc.getDateStr());
}

void ora()
{
  lcd.setCursor(0, 1);
  lcd.print(rtc.getTimeStr());
}
