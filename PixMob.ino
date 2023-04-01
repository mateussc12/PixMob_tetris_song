#include <Arduino.h>      // Carrega Bibliotecas 
#include <IRremote.hpp>


#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST 0

// change this to make the song slower or faster
int tempo=144; 

// change this to whichever pin you want to use
int buzzer = 8;

// notes of the moledy followed by the duration.
// a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
// !!negative numbers are used to represent dotted notes,
// so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!
int melody[] = {

  //Based on the arrangement at https://www.flutetunes.com/tunes.php?id=192
  
  NOTE_E5, 4,  NOTE_B4,8,  NOTE_C5,8,  NOTE_D5,4,  NOTE_C5,8,  NOTE_B4,8,
  NOTE_A4, 4,  NOTE_A4,8,  NOTE_C5,8,  NOTE_E5,4,  NOTE_D5,8,  NOTE_C5,8,
  NOTE_B4, -4,  NOTE_C5,8,  NOTE_D5,4,  NOTE_E5,4,
  NOTE_C5, 4,  NOTE_A4,4,  NOTE_A4,8,  NOTE_A4,4,  NOTE_B4,8,  NOTE_C5,8,

  NOTE_D5, -4,  NOTE_F5,8,  NOTE_A5,4,  NOTE_G5,8,  NOTE_F5,8,
  NOTE_E5, -4,  NOTE_C5,8,  NOTE_E5,4,  NOTE_D5,8,  NOTE_C5,8,
  NOTE_B4, 4,  NOTE_B4,8,  NOTE_C5,8,  NOTE_D5,4,  NOTE_E5,4,
  NOTE_C5, 4,  NOTE_A4,4,  NOTE_A4,4, REST, 4,

  NOTE_E5, 4,  NOTE_B4,8,  NOTE_C5,8,  NOTE_D5,4,  NOTE_C5,8,  NOTE_B4,8,
  NOTE_A4, 4,  NOTE_A4,8,  NOTE_C5,8,  NOTE_E5,4,  NOTE_D5,8,  NOTE_C5,8,
  NOTE_B4, -4,  NOTE_C5,8,  NOTE_D5,4,  NOTE_E5,4,
  NOTE_C5, 4,  NOTE_A4,4,  NOTE_A4,8,  NOTE_A4,4,  NOTE_B4,8,  NOTE_C5,8,

  NOTE_D5, -4,  NOTE_F5,8,  NOTE_A5,4,  NOTE_G5,8,  NOTE_F5,8,
  NOTE_E5, -4,  NOTE_C5,8,  NOTE_E5,4,  NOTE_D5,8,  NOTE_C5,8,
  NOTE_B4, 4,  NOTE_B4,8,  NOTE_C5,8,  NOTE_D5,4,  NOTE_E5,4,
  NOTE_C5, 4,  NOTE_A4,4,  NOTE_A4,4, REST, 4,
  

  NOTE_E5,2,  NOTE_C5,2,
  NOTE_D5,2,   NOTE_B4,2,
  NOTE_C5,2,   NOTE_A4,2,
  NOTE_GS4,2,  NOTE_B4,4,  REST,8, 
  NOTE_E5,2,   NOTE_C5,2,
  NOTE_D5,2,   NOTE_B4,2,
  NOTE_C5,4,   NOTE_E5,4,  NOTE_A5,2,
  NOTE_GS5,2,

};

// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes
int notes=sizeof(melody)/sizeof(melody[0])/2; 

// this calculates the duration of a whole note in ms (60s/tempo)*4 beats
int wholenote = (60000 * 4) / tempo;

int divider = 0, noteDuration = 0;

// Variáveis do programa
const uint16_t pin_IR_LED = 3; // numero do pino para ligar o transmissor IR
int tamanho;  // variavel para guardar o numero de bytes referente a cor enviada para a pulseira, nessesário para a função IRSender

// Código de Cores que será enviado para a pulseira
uint16_t   vermelho[]      = {1400, 1400, 700, 700, 700, 1400, 700, 2800, 700, 2800, 700, 700, 700, 700, 700, 1400, 1400, 2800, 1400, 2800, 700};
uint16_t   verde[]         = {700, 700, 700, 700, 1400, 1400, 1400, 2800, 700, 1400, 700, 1400, 700, 1400, 700, 2100, 1400, 2100, 1400, 2800, 700};
uint16_t   azul[]          = {700, 700, 700, 2100, 1400, 1400, 700, 2100, 700, 1400, 700, 700, 700, 1400, 1400, 700, 700, 1400, 700, 700, 700, 700, 700, 700, 700, 2100, 700};
uint16_t   amarelo_verde[] = {700, 700, 700, 700, 1400, 1400, 1400, 2800, 700, 2100, 700, 700, 700, 1400, 700, 1400, 700, 700, 700, 2100, 1400, 2800, 700};
uint16_t   azul_claro[]    = {700, 700, 700, 2100, 1400, 700, 700, 2800, 700, 1400, 700, 700, 700, 1400, 700, 700, 700, 700, 700, 700, 700, 700, 1400, 1400, 700, 2100, 700};
uint16_t   magenta[]       = {700, 700, 700, 700, 1400, 1400, 1400, 2800, 700, 2100, 1400, 2100, 700, 700, 700, 700, 1400, 2100, 700, 700, 700, 2100, 700};
uint16_t   amarelo[]       = {1400, 1400, 700, 700, 700, 700, 1400, 2800, 700, 2100, 700, 700, 700, 1400, 700, 1400, 1400, 2800, 1400, 2800, 700};
uint16_t   rosa[]          = {700, 700, 700, 2100, 1400, 700, 700, 2800, 700, 1400, 700, 700, 700, 2800, 1400, 1400, 700, 2100, 700, 700, 700, 2100, 700};
uint16_t   laranja[]       = {700, 700, 700, 700, 1400, 1400, 1400, 2800, 700, 2100, 700, 700, 700, 1400, 700, 700, 700, 1400, 700, 2100, 1400, 2800, 700};
uint16_t   branco[]        = {700, 700, 700, 2100, 1400, 700, 700, 2800, 700, 1400, 700, 700, 700, 2800, 1400, 1400, 700, 2100, 700, 1400, 700, 1400, 700};
uint16_t   turquesa[]       = {700, 700, 700, 700, 1400, 1400, 1400, 2800, 700, 1400, 700, 700, 1400, 1400, 700, 1400, 1400, 2800, 700, 700, 700, 2100, 700};

long randNumber;

long int t1;
long int t2;

void setup() { // ajusta as configuraçãoes iniciais
  IrSender.begin(pin_IR_LED); // Inicializa o pino do Arduino que enviará os dados
  IrSender.enableIROut(38);   // Define a frequencia da portadora do sinal IR
} // fim das configuraçoes iniciais

void loop() {
  // iterate over the notes of the melody. 
  // Remember, the array is twice the number of notes (notes + durations)
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(buzzer, melody[thisNote], noteDuration*0.9);

    // Define qual cor vai ser colocada na pulseira
    // Generate a random number from 0 to 11
    randNumber = random(0, 12);
    
    if (randNumber == 0) {
      t1 = millis();
      // pisca LEDs Cor Vermelho  
      tamanho = sizeof vermelho / sizeof *vermelho; // conta quantos bytes serão enviados da cor vermelha 
      IrSender.sendRaw(vermelho, tamanho, 38);    // envia os dados para o tranmissor IR
      t2 = millis();
      delay(noteDuration - (t2 - t1));
    } 
    else if ((randNumber == 1)){
      t1 = millis();
      // pisca LEDs Cor Verde  
      tamanho = sizeof verde / sizeof *verde; // conta quantos bytes serão enviados da cor verde 
      IrSender.sendRaw(verde, tamanho, 38);    // envia os dados para o tranmissor IR
      t2 = millis();
      delay(noteDuration - (t2 - t1));
    }
    else if ((randNumber == 2)){
      t1 = millis();
      // pisca LEDs Cor amarelo_verde 
      tamanho = sizeof amarelo_verde / sizeof *amarelo_verde; // conta quantos bytes serão enviados da cor amarelo_verde
      IrSender.sendRaw(amarelo_verde, tamanho, 38);    // envia os dados para o tranmissor IR
      t2 = millis();
      delay(noteDuration - (t2 - t1));
    }
    else if ((randNumber == 3)){
      t1 = millis();
      // pisca LEDs Cor azul_claro
      tamanho = sizeof azul_claro / sizeof *azul_claro; // conta quantos bytes serão enviados da cor azul_claro
      IrSender.sendRaw(azul_claro, tamanho, 38);    // envia os dados para o tranmissor IR
      t2 = millis();
      delay(noteDuration - (t2 - t1));
    }
    else if ((randNumber == 4)){
      t1 = millis();
      // pisca LEDs Cor magenta
      tamanho = sizeof magenta / sizeof *magenta; // conta quantos bytes serão enviados da cor magenta
      IrSender.sendRaw(magenta, tamanho, 38);    // envia os dados para o tranmissor IR
      t2 = millis();
      delay(noteDuration - (t2 - t1));
    }
    else if ((randNumber == 5)){
      t1 = millis();
      // pisca LEDs Cor amarelo
      tamanho = sizeof amarelo / sizeof *amarelo; // conta quantos bytes serão enviados da cor amarelo
      IrSender.sendRaw(amarelo, tamanho, 38);    // envia os dados para o tranmissor IR
      t2 = millis();
      delay(noteDuration - (t2 - t1));
    }
    else if ((randNumber == 6)){
      t1 = millis();
      // pisca LEDs Cor rosa
      tamanho = sizeof rosa / sizeof *rosa; // conta quantos bytes serão enviados da cor rosa
      IrSender.sendRaw(rosa, tamanho, 38);    // envia os dados para o tranmissor IR
      t2 = millis();
      delay(noteDuration - (t2 - t1));
    }
    else if ((randNumber == 7)){
      t1 = millis();
      // pisca LEDs Cor laranja
      tamanho = sizeof laranja / sizeof *laranja; // conta quantos bytes serão enviados da cor laranja
      IrSender.sendRaw(laranja, tamanho, 38);    // envia os dados para o tranmissor IR
      t2 = millis();
      delay(noteDuration - (t2 - t1));
    }
    else if ((randNumber == 8)){
      t1 = millis();
      // pisca LEDs Cor branco
      tamanho = sizeof branco / sizeof *branco; // conta quantos bytes serão enviados da cor branco
      IrSender.sendRaw(branco, tamanho, 38);    // envia os dados para o tranmissor IR
      t2 = millis();
      delay(noteDuration - (t2 - t1));
    }
    else if ((randNumber == 9)){
      t1 = millis();
      // pisca LEDs Cor turquesa
      tamanho = sizeof turquesa / sizeof *turquesa; // conta quantos bytes serão enviados da cor turquesa
      IrSender.sendRaw(turquesa, tamanho, 38);    // envia os dados para o tranmissor IR
      t2 = millis();
      delay(noteDuration - (t2 - t1));
    }
    else if ((randNumber == 10)){
      t1 = millis();
      // pisca LEDs Cor Azul  
      tamanho = sizeof azul / sizeof *azul; // conta quantos bytes serão enviados da cor azul 
      IrSender.sendRaw(azul, tamanho, 38);    // envia os dados para o tranmissor IR
      t2 = millis();
      delay(noteDuration - (t2 - t1));
    }

    // stop the waveform generation before the next note.
    noTone(buzzer);
    
  }
}
