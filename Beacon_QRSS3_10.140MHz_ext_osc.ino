//Beacon QRSS3 by IA5ITY
//version 1.0.11
//operating frequency -> 10,139,900
//uses the external 20MHz oscillator i the X20 PLL inside the DDS

//calculation tunning word
//FTW = f_out * (2³² / SYSCLK) -> SYSCLK = 400MHz, 2^32 = 4294967296

#include <SPI.h>
#include <AD9951.h>

//define DDS registers
#define cfr1_reg  0x00
#define cfr2_reg  0x01
#define asf_reg   0x02
#define arr_reg   0x03
#define ftw_reg   0x04
#define pow_reg   0x05
#define read_reg  0x00
#define write_reg 0x01

//constant DDS I/O pins
const unsigned int oreset = A1;
const unsigned int ioupdate = 9;
const unsigned int clkmodesel = 8; //non usato
const unsigned int pwrdwnctrl = A0;
const unsigned int osk = 2;
const unsigned int iosync = 3;
const unsigned int cs = 10;

//define I/O di sistema
#define SYSLED  A3      //status LED
#define PTT     A2      //enabling transmission
#define CWLED   2       //displays the sent CW character

#define DOTLEN  3000
#define DASHLEN 9000

//prototypes
void sendMsg(char*);
void Msg(char);
void dash(void);
void dot(void);

//Variable
char str[230]; // = "";
uint8_t i = 0;

//AD9951
AD9951 VFO(cs, oreset, ioupdate, clkmodesel, pwrdwnctrl, osk, iosync);

//setup
void setup() {
  //initialisation of system I/O pins, system LEDs
  pinMode(SYSLED, OUTPUT);
  digitalWrite(SYSLED, HIGH);
  //transmission authorisation
  pinMode(PTT, OUTPUT);
  digitalWrite(PTT, LOW);
  //echo character morse
  pinMode(CWLED, OUTPUT);
  digitalWrite(CWLED, LOW);
  //DDS initialisation, Reset
  pinMode(oreset, OUTPUT);
  digitalWrite(oreset, LOW);
  //I/O Update
  pinMode(ioupdate, OUTPUT);
  digitalWrite(ioupdate, LOW);
  //chip select
  pinMode(cs, OUTPUT);
  digitalWrite(cs, HIGH);
  //powerdown
  pinMode(pwrdwnctrl, OUTPUT);
  digitalWrite(pwrdwnctrl, LOW);
  //reset pulse and I/O sync
  VFO.toggleReset();
  VFO.toggleIOSync();
  //Configuration register 1
  VFO.cfr1[0] = 0x00;
  VFO.cfr1[1] = 0x02;
  VFO.cfr1[2] = 0x00;
  VFO.cfr1[3] = 0x00;
  VFO.SendReg(cfr1_reg, write_reg);
  //Configuration register 2
  VFO.cfr2[0] = 0xa4;
  VFO.cfr2[1] = 0x02;
  VFO.cfr2[2] = 0x18;
  VFO.SendReg(cfr2_reg, write_reg);
  //register "Amplitude scale factor"
  VFO.asf[0] = 0xff;
  VFO.asf[1] = 0x3f;
  VFO.SendReg(asf_reg, write_reg);
  //register "Amplitude Ramp Rate"
  VFO.arr = 0xff;
  VFO.SendReg(arr_reg, write_reg);
  //register "Tunning word" 10.139.900HMz
  VFO.ftw[0] = 0x3b;
  VFO.ftw[1] = 0x52;
  VFO.ftw[2] = 0x7d;
  VFO.ftw[3] = 0x06;
  VFO.SendReg(ftw_reg, write_reg);
  //register "phase offset word"
  VFO.pow[0] = 0x00;
  VFO.pow[1] = 0x00;
  VFO.SendReg(pow_reg, write_reg);
  //delay di 250mS
  delay(250);
}//end setup

//main loop
void loop() {
  //text message to be transmitted (respect spaces = pause)
  strcat (str, "IA5ITY "); 
  //message to be sent
  sendMsg( str ); 
  //deactivates the system LED
  digitalWrite(SYSLED, LOW);
  //60 second delay between messages
  delay(60000);
}//end loop
//send string
void sendMsg(char *str){
  //local variable
  int i;
  //activates the carrier
  digitalWrite(PTT, HIGH);
  delay(50);
  //encodes the string with morse signals
  for(i = 0; i < strlen(str); i++){
    switch (str[i]){
      case 'A':
        dot(); dash(); break;
      case 'B':
        dash(); dot(); dot(); dot(); break;
      case 'C':
        dash(); dot(); dash(); dot(); break;
      case 'D':
        dash(); dot(); dot(); break;
      case 'E':
        dot(); break;
      case 'F':
        dot(); dot(); dash(); dot(); break;
      case 'G':
        dash(); dash(); dot(); break;
      case 'H':
        dot(); dot(); dot(); dot(); break;
      case 'I':
        dot(); dot(); break;
      case 'J':
        dot(); dash(); dash(); dash(); break;
      case 'K':
        dash(); dot(); dash(); break;
      case 'L':
        dot(); dash(); dot(); dot(); break;
      case 'M':
        dash(); dash(); break;
      case 'N':
        dash(); dot(); break;
      case 'O':
        dash(); dash(); dash(); break;
      case 'P':
        dot(); dash(); dash(); dot(); break;
      case 'Q':
        dash(); dash(); dot(); dash(); break;
      case 'R':
        dot(); dash(); dot(); break;
      case 'S':
        dot(); dot(); dot(); break;
      case 'T':
        dash(); break;
      case 'U':
        dot(); dot(); dash(); break;
      case 'V':
        dot(); dot(); dot(); dash(); break;
      case 'W':
        dot(); dash(); dash(); break;
      case 'X':
        dash(); dot(); dot(); dash(); break;
      case 'Y':
        dash(); dot(); dash(); dash(); break;
      case 'Z':
        dash(); dash(); dot(); dot(); break;
      case ' ':
        delay(DOTLEN * 5); break;
      case '.':
        dot(); dash(); dot(); dash(); dot(); dash(); break;
      case ',':
        dash(); dash(); dot(); dot(); dash(); dash(); break;
      case ':':
        dash(); dash(); dash(); dot(); dot(); break;
      case '?':
        dot(); dot(); dash(); dash(); dot() ;dot(); break;
      case '\'':
        dot(); dash(); dash(); dash(); dash(); dot(); break;
      case '-':
        dash(); dot(); dot(); dot(); dot(); dash(); break;
      case '/':
        dash(); dot(); dot(); dash(); dot(); break;
      case '(':
      case ')':
        dash(); dot(); dash(); dash(); dot(); dash(); break;
      case '\"':
        dot(); dash(); dot(); dot(); dash(); dot(); break;
      case '@':
        dot(); dash(); dash(); dot(); dash(); dot(); break;
      case '=':
        dash(); dot(); dot(); dot(); dash(); break;
      case '0':
        dash(); dash(); dash(); dash(); dash(); break;
      case '1':
        dot(); dash(); dash(); dash(); dash(); break;
      case '2':
        dot(); dot(); dash(); dash(); dash(); break;
      case '3':
        dot(); dot(); dot(); dash(); dash(); break;
      case '4':
        dot(); dot(); dot(); dot(); dash(); break;
      case '5':
        dot(); dot(); dot(); dot(); dot(); break;
      case '6':
        dash(); dot(); dot(); dot(); dot(); break;
      case '7':
        dash(); dash(); dot(); dot(); dot(); break;
      case '8':
        dash(); dash(); dash(); dot(); dot(); break;
      case '9':
        dash(); dash(); dash(); dash(); dot(); break;
    }
    delay(2 * DOTLEN);
  }
  //cleans the string with all end-of-line terminators
  memset( str, '\0', strlen( str ) );
  //disattiva la portante
  delay(50);
  digitalWrite(PTT, LOW);
}
//trasmette la linea
void dash(){
  //eco
  digitalWrite(CWLED, HIGH);
  digitalWrite(SYSLED, HIGH);
  //FSK, imposta la frequenza base 10.139.900 + 5Hz
  VFO.ftw[0] = 0x71;
  VFO.ftw[1] = 0x52;
  VFO.ftw[2] = 0x7d;
  VFO.ftw[3] = 0x06;
  VFO.SendReg(ftw_reg, write_reg);
  //delay
  delay(DASHLEN);
  //eco
  digitalWrite(CWLED, LOW);
  digitalWrite(SYSLED, LOW);
  //sets the base frequency 10.139.900
  VFO.ftw[0] = 0x3b;
  VFO.ftw[1] = 0x52;
  VFO.ftw[2] = 0x7d;
  VFO.ftw[3] = 0x06;
  VFO.SendReg(ftw_reg, write_reg);
  //delay
  delay(DOTLEN);
}
//transmits the dot
void dot(){
  //eco
  digitalWrite(CWLED, HIGH);
  digitalWrite(SYSLED, HIGH);
  //FSK, sets the base frequency 10.139.900 + 5Hz
  VFO.ftw[0] = 0x71;
  VFO.ftw[1] = 0x52;
  VFO.ftw[2] = 0x7d;
  VFO.ftw[3] = 0x06;
  VFO.SendReg(ftw_reg, write_reg);
  //delay
  delay(DOTLEN);
  //eco
  digitalWrite(CWLED, LOW);
  digitalWrite(SYSLED, LOW);
  //sets the base frequency 10.139.900
  VFO.ftw[0] = 0x3b;
  VFO.ftw[1] = 0x52;
  VFO.ftw[2] = 0x7d;
  VFO.ftw[3] = 0x06;
  VFO.SendReg(ftw_reg, write_reg);
  //ritardo
  delay(DOTLEN);
}
//end
