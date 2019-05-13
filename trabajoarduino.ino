//pin del led que se enciende cunado se emite el sonido de raya
#define ledRaya 7
//pin del led que se enciende cunado se emite el sonido de punto
#define ledPunto 8
//pin del led que se enciende cunado hay un espacio entre palabras
#define ledEspacio 9
//pin del led que se enciende cunado se termina el mensaje
#define ledFin 10
#define pulRaya 2
#define pulPunto 3
#define pulEspacio 4
#define pulFin 5

//lista de caracteres
char caracteres[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'ñ', 'o', 'p', 'q','r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z','0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.', ',', '?', '"', '!'};

// caracteres convertidos a morse
String codigo[] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "--.--", "---", ".--.", "--.-", ".-.", "...", "-", "..--", "...-", ".--", "-..-", "-.--", "--..", "-----", ".----", "..--", "...--", "....-", ".....", "-....", "--...", "---..", "---..", "----.", ".-.-.-", "--..--", "..--..", ".-..-.", "-.-.--"};

//variable que contiene la frase error por si la palabra enviada esta mal
//variable que contendrá la cadena de caracteres traducida a morse
//variable que contendrá la palabra digitada por el usuario
//variable que contrendra la palabra traducida
String error = "../../..", traduccion = "", palabra = "",traduccion2="",comp="",traduccion3="";
//representa el espacio vacío entre palabras
char vacio = '/';
String inv[50];

//representa el tiempo de duración del punto
int estado=LOW;
int tiempoPunto = 70;
//representa el tiempo de duración de la raya
int tiempoRaya = tiempoPunto * 3;
//representa el tiempo que hay entre letra y letra
//representa el tiempo que hay entre palabra y palabra
int silencioLetra = tiempoPunto * 3, silencioPalabra = tiempoPunto * 5;
//para validar que ya se haya enviado el mensaje a través del puerto serial
boolean enviado = false;
int opc=0;
int raya;
int punt;
int fin;
int espac;

void setup(){
  //se configuran los pines
    
    pinMode(ledRaya, OUTPUT);
    pinMode(ledPunto, OUTPUT);
    pinMode(ledEspacio, OUTPUT);
    pinMode(ledFin, OUTPUT);
    pinMode(pulRaya,INPUT);
    pinMode(pulPunto,INPUT);
    pinMode(pulEspacio,INPUT);
    pinMode(pulFin,INPUT);
    Serial.begin(9600);
    Serial.println("Que quieres hacer");
    Serial.println("1.traducir una palabra");
    Serial.println("2. enviar una palabra");
    
}

void loop(){
  opc=Serial.parseInt();
    if(opc==1){
   Serial.println("Dime una palabra");
   delay(4000);
    traducirPalabra();
    if(!enviado){
      Serial.print(traduccion); 
      enviado = true;  
    }   
    emitirMorse();
    finMensaje();
}
else if(opc==2){
  Serial.println("Escribe cada letra con un espacio");
  do{
   for(int i=0;i<50;i++){
    delay(500);
  raya=digitalRead(pulRaya);
  punt=digitalRead(pulPunto);
  espac=digitalRead(pulEspacio);
  fin=digitalRead(pulFin);
   if(raya==HIGH){
    inv[i]='-';
   }
    if(punt==HIGH){
    inv[i]='.';
   }  
   if(espac==HIGH){
    inv[i]='/';
    for(int j = 0; j < sizeof(codigo) ; j++){
                      if(comp==codigo[j]){
                      traduccion2 += caracteres[j];
                      traduccion3 += traduccion2;
                      traduccion2="";
                      inv[i]="";
                      }
                    }
                    comp="";
   }
   if(fin==HIGH){
    inv[i]='_';
    
     for(int j = 0; j < sizeof(codigo) ; j++){
                      if(comp==codigo[j]){
                      traduccion2 += caracteres[j];
                      traduccion3 += traduccion2;
                      traduccion2="";
                      }
                    }
    break;
   }
  comp += inv[i];
  Serial.println(comp);
   }
  }while(fin==LOW);
  Serial.println("Has escrito:");
  Serial.println(comp);
  Serial.println("La traduccion es:");                 
  Serial.println(traduccion3);
   } 
 }
       
  

//método que se encarga de dar a saber que ya se termino de transmitir el mensaje
//esperando medio segundo para continuar
void finMensaje(){
  digitalWrite(ledFin, HIGH);
  delay(1000);
  digitalWrite(ledFin, LOW);
  delay(1000);

}

//método que se encarga de encender un led (raya o punto) y emitir un sonido
//a través del buzzer dejando un respectivo tiempo de diferencia
//función que se usa para emitir un punto
void punto(){
  //se deja un espacio entre la anterior letra y esta
    delay(silencioLetra);
    digitalWrite( ledPunto,HIGH);
    delay(500);
    digitalWrite(ledPunto,LOW);
    delay(500);
}

//función que se usa para emitir un raya
void linea(){
    //se deja un espacio entre la anterior letra y esta
    delay(silencioLetra);
    digitalWrite( ledRaya,HIGH);
    delay(1000);
    digitalWrite(ledRaya,LOW);
    delay(500);
}

//función que se usa para emitir el espacio entre palabras
void espacio(){
    digitalWrite(ledEspacio, HIGH);
    delay(1000);
    digitalWrite(ledEspacio, LOW);
    delay(500);
}

//metodo que se encarga de convertir la palabra digitada a código morse
void traducirPalabra(){
    palabra = "";
    if(Serial.available()){
      //se lee el contenido de Serial
        palabra = Serial.readString();
        //se borra la anterior palabra traducida
        traduccion = "";
        enviado = false;
        //se usa para validar el hecho de encontrar un carácter
        boolean encontre = false;
        //si se digita la palabra error
        if(palabra == "error")
            traduccion = error;
        else{
          //se lee la palabra digitada y se convierte a morse
            for(int i = 0; i < palabra.length(); i++){
                for(int j = 0; j < sizeof(caracteres) && !encontre; j++){
                    if(palabra.charAt(i) == caracteres[j]){
                      encontre = true;
                      traduccion += codigo[j] + " ";
                      traduccion += vacio;
                    }
                    else if(palabra.charAt(i) == ' '){
                      encontre = true;
                      traduccion += vacio;
                    }
                     
                }
               
                encontre = false;
            }
             Serial.print("La traduccion es:"); 
        }
    }
    
}
//método que se encarga de emitir la palabra traducida
void emitirMorse(){
    for(int i = 0; i < traduccion.length(); i++){
      //se lee caracter por caracter de la palabra traducida a morse
      if(traduccion.charAt(i) == '.')
        punto();
      else if(traduccion.charAt(i) == '-')
        linea();
      else if(traduccion.charAt(i) == '/')
        espacio();
    }
}
/*
void enviarpalabra(){
palabra = "";
    if(Serial.available()){
      //se lee el contenido de Serial
        palabra = Serial.readString();
        //se borra la anterior palabra traducida
        traduccion = "";
        enviado = false;
        //se usa para validar el hecho de encontrar un carácter
        boolean encontre = false;
        //si se digita la palabra error
        if(palabra == "error")
            traduccion = error;
        else{
          //se lee la palabra digitada y se convierte a morse
            for(int i = 0; i < palabra.length(); i++){
                for(int j = 0; j < sizeof(caracteres) && !encontre; j++){
                    if(palabra.charAt(i) == caracteres[j]){
                      encontre = true;
                      traduccion += codigo[j] + " ";
                      traduccion += vacio;
                    }
                    else if(palabra.charAt(i) == ' '){
                      encontre = true;
                      traduccion += vacio;
                    }
                     
                }
               
                encontre = false;
            }
             Serial.print("La traduccion es:"); 
        }
    }
} */
