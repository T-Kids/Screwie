#include <SoftwareSerial.h>               //Llibreria que permet establit comunicació entre altres pins
#include <TimerOne.h>                     //Llibreria per a les interrupcions
 

SoftwareSerial BT(10,11);                 //10 RX, 11 TX. // Aqui conectem els pins RXD,TDX del módul Bluetooth
 
volatile unsigned int  foratsDreta=0;     //Declarem les Variables dels forats de la dreta com a volatil, degut a que el seu valor variara
volatile unsigned int  foratsEsquerra=0;  //Declarem les Variables dels forats de l'esquerra com a volatil,tambè,degut a que el seu valor variara.
char cadena[25];                          //Posem una cadena d'un màxim de 25 posicions
int i=0;                                  //Mida de la cadena actual
int j=0;
int cl=0;                                 //Variable per rentar la cadena
int dato=0;                               //Variable de dato
int velocitatD = 170;                     //Variable per a la velocitat del motor dret 
int velocitatE = 145;                     //Variable per a la velocitta del motor esquerra
int E1 = 5;                               // Velocitat Motor 1 
int M1 = 4;                               // Sentit Motor 1
int E2 = 6;                               //Velocitat Motor 2 
int M2 = 7;                               // Sentit Motor 2
#define pasosgir 160                       // Variable per als forats
#define pasosD 80                         //Variable per als pasos per girar
#define pasosE 80


 
void setup()                              // El Setup es el que s'executara una sola vegada quan s'inici el programa                          
{
  BT.begin(9600);                         // S'inicia el Bluetooth                        
  Serial.begin(9600);
  pinMode(13,OUTPUT);                     //Declarem la sortida 13 com a OUTPUT(Sortida d'informació de l'ordinador)
  attachInterrupt(1, FE, RISING);         //Interrupcions per als forats de l'esquerra
  attachInterrupt(0, FD, RISING);         //Interrupcions per als forats de la dreta
}

void FD() 
{
foratsDreta++;
}            //Comptador del sensor de velocitat.Incrementa en 1 el comptador de la dreta


void FE()                                 //Comptador del sensor de velocitat
{
  foratsEsquerra++;                       //Incrementa en 1 el comptador de l'esquerra
} 
 
void clean()                              //Això netejara la cadena
{
  for (cl=0; cl<=i; cl++)                 //Quan això sigui cert, la cadena valdrà 0
  {
    cadena[cl]=0;                         //Posem cadena a 0
  }
  i=0;
  digitalWrite(13,0);                     //led 13(verd)     apagat no hi ha programa
  
}


 
void executamoviment()                  //Això executara les ordres enviades desde el dispositiu mòbil.
{
  char dato=cadena[j];            //Hi posa a dato les ordres emmagatzemades a cadena  
  velocitatD == 190;
  velocitatE == 175;  
  switch(dato)
      {   
          case '1':
            {
              clean;                 //Rentar la cadena
              break;
            }
          case '3':                 //Si dato es 3, realitzara l'operació endavant
             {
               digitalWrite(13,1);
               foratsDreta = 0;
               foratsEsquerra = 0;
               while ( foratsDreta<pasosD && foratsEsquerra<pasosE)
               {
                  
                  if (foratsDreta>foratsEsquerra)
                  {
                        velocitatD==velocitatD*0.95;
                        velocitatE==velocitatE*1.5;
                  }
                  if (foratsDreta<foratsEsquerra)
                  {
                        velocitatD==velocitatD*1.5;
                        velocitatE==velocitatE*0.95;
                  }                                  
                   if (foratsDreta==foratsEsquerra)
                   {
                    velocitatD == 190;
                    velocitatE == 120;                  
                   }
                   analogWrite  (E1,velocitatD);
                   digitalWrite (M1,1);
                   analogWrite  (E2, 150);
                   digitalWrite (M2,1);
               } 
                   analogWrite  (E1,velocitatD);      //retrocés per frenar
                   digitalWrite (M1,0);
                   analogWrite  (E2, velocitatE);
                   digitalWrite (M2,0);
                   delay(50);
                   analogWrite  (E1,0);
                   digitalWrite (M1,1);
                   analogWrite  (E2, 0);
                   digitalWrite (M2,1);                                   
                   delay(1000);
                   break;
                      
             } 

          case '4':                                   //Si dato es 4, gira a la dreta
             {
               foratsDreta = 0;
               foratsEsquerra = 0;
               while (foratsEsquerra<19)
                  {
                     analogWrite  (E2, velocitatE);   //Encen motor
                     digitalWrite (M2,1);
                     analogWrite  (E1, velocitatD);   //Encen motor
                     digitalWrite (M1,0);
                  } 
               analogWrite  (E2, 0);                  //Atura motor
               digitalWrite (M2,1);
               analogWrite  (E1, 0);                  //Atura motor
               digitalWrite (M1,1);
               Serial.print(j);
               Serial.println(dato);
               delay(1000); 
               break;
                                 
             } 


          case '5':                                   //Si dato es 5, gira a la esquerra
            {
               foratsDreta = 0;
               foratsEsquerra = 0;
               while (foratsDreta<25)
                  {
                     analogWrite  (E1, velocitatD);   //Encen motor
                     digitalWrite (M1,1);
                     analogWrite  (E2, velocitatE);   //Encen motor
                     digitalWrite (M2,0);
                  } 
               analogWrite  (E2, 0);                  //Atura motor
               digitalWrite (M2,1);
               analogWrite  (E1, 0);                  //Atura motor
               digitalWrite (M1,1);
               Serial.print(j);
               Serial.println(dato);
               delay(1000); 
               break;                                  
             }                   
      }           
}
      

void loop()                                         // El loop es el que es repetira constantment
{

            
    if(BT.available())                              //Si el Bluetooth esta disponible fes això:
      {
          char dato=BT.read();                      //Guarda les dades caràcter a caràcter en la variable dato 
          cadena[i++]=dato;                         //Ho posa en cadena
          switch(dato)
          {
            case '1':                            
             {                                     
                digitalWrite(13,1);                 //Si es clica ON executa el programa en 2 segons i s'indica encenent el led 13(verd)                 
                j=0;                                //Reset índex llista
                while (j<sizeof(cadena))
                  {
                    executamoviment();              //executa moviement a dato
                    j++;                            //Pasem a la següent ordre 
                  }
                  clean();
                  break;
              }
          
           case '2':                         //Si dato es 2 renta la cadena per a tornar a començar i para el cotxe
            {
                analogWrite  (E1,0);
                digitalWrite (M1,1);
                analogWrite  (E2, 0);
                digitalWrite (M2,1);
                clean();
                break;

                                  
            }
          }
      }
}


