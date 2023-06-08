#include <LiquidCrystal.h>
#include <math.h>
#include <Wire.h>
LiquidCrystal lcd ( 2, 3, 4, 5, 6, 7 );

//F.P
float pulsewidth = 0;
float powerfactor = 0;
float phase = 0;

//Voltaje
float max_v = 0;
float maxx_v = 0;
float v = 0;
float r = 0;

//Corriente
const int analogchannel = 0; //
int sensitivity = 66; // 
float adcvalue= 0;
int offsetvoltage = 2500; 
double Voltage = 0; //
double ecurrent = 0;// 
unsigned int temp=0;
float maxpoint = 0;

//Potencia Activa
float pa = 0;

//Potencia Reactiva
float q = 0;

//Potencia Aparente Modulo
float s = 0;

//Voltaje de fuente
int v2=100;

void setup()
{
  Serial.begin(9600);
  lcd.begin (20,4);
}
void loop() 
{
    //F.P
    pulsewidth = pulseIn (8,HIGH);
    phase = 2 * 180 * 60 * (pulsewidth/1000000); //"60" valor de frecuencia de las ondas
    powerfactor = cos ( phase * PI / 180 );
    lcd.setCursor ( 0 , 0 );
    lcd.print ( "<:" );
    lcd.setCursor ( 2 , 0 );
    lcd.print ( phase );
    lcd.setCursor ( 0 , 1 );
    lcd.print ( "F.P:" );
    lcd.setCursor ( 4 , 1 );
    lcd.print ( powerfactor );
  //VOLTAJE
    for(float i = 0; i < 100; i++)
    {
      //Voltaje
      r = analogRead(A1);  
      if(max_v < r)
      {
        max_v = r;
        maxx_v = r;
      }
      //Corriente
      temp = analogRead(analogchannel);
      if( temp > maxpoint )
      {
        maxpoint = temp;
      }
    }
    v = maxx_v;// amplitud
    v = v * 5000/1024;// voltaje (ADC referencia = 5V)
    v /= sqrt(2);// calcular valor rms ( = peak/√2 )
    lcd.setCursor(0, 2);
    lcd.print("V:");
    lcd.setCursor(2, 2);
    lcd.print(v);
  //Corriente
    adcvalue = maxpoint; 
    Voltage = (adcvalue / 1024.0) * 5000; //  mV
    ecurrent = ((Voltage - offsetvoltage) / sensitivity);
    ecurrent = ( ecurrent ) / ( sqrt(2) );
    lcd.setCursor(0, 3);
    lcd.print("I:");
    lcd.setCursor(2, 3);
    lcd.print(ecurrent,3);
    //POTENCIA ACTIVA
    pa = v2 * ecurrent * powerfactor;
    lcd.setCursor ( 9 , 0 );
    lcd.print ( "PA:" );
    lcd.setCursor ( 13 , 0 );
    lcd.print ( pa );
    //POTENCIA REACTIVA
    q = sqrt(pow(pa/(powerfactor),2)-pow(pa,2));
    lcd.setCursor ( 9 , 1 );
    lcd.print ( "PRA:" );
    lcd.setCursor ( 13 , 1 );
    lcd.print ( q );
    //POTENCIA APARENTE MODULO
    s = sqrt(pow(q,2)+pow(pa,2));
    lcd.setCursor ( 9 , 2 );
    lcd.print ( "|S|:" );
    lcd.setCursor ( 13 , 2 );
    lcd.print ( s );
    //TIPO DE CIRCUITO
    if(powerfactor < 0.99)
    {
      if (digitalRead(9)==LOW)
      {
        lcd.setCursor ( 9 , 3 );
        lcd.print ( "Capacitivo" );
      }
      if (digitalRead(9)==HIGH)
      {
        lcd.setCursor ( 9 , 3 );
        lcd.print ( "Inductivo" );
      }
    }else 
    {
      lcd.setCursor ( 9 , 3 );
      lcd.print ( "Resistivo" );
    }
    //delay(100);
}
