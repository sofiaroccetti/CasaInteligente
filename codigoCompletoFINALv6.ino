#include <Servo.h>
#include <SPI.h>
#include <Ethernet.h>

/*####################################################################################################################################################
 * ###################################################################################################################################################
 * ################################################# DECLARACIÃ“N DE VARIABLES ########################################################################
 * ###################################################################################################################################################
 * ###################################################################################################################################################*/
 
/*#################################################Variables para manejar la puerta#################################################*/
int botonPuerta = 2;
int estadoBotonPuerta = 0;
int contadorPulsaciones = 0;
int banderaAlarma = 0;
int estadoPuerta = 0;
Servo motorPuerta;

/*#################################################Variables para manejar la ventana#################################################*/
int botonVentana = 4;
int contadorPulsacionesVentana = 0;
int estadoBotonVentana = 0;
int estadoVentana = 0;
Servo motorVentana;

/*#################################################Variables para manejar la alarma#################################################*/
int sensorMovimientoAlarma = 6;
int pinAlarma = 7;
int lecturaMovimientoAlarma = 0;
int cant = 0;

/*#################################################Variables para manejar el sensor del jardin#################################################*/
int sensorMovimientoJardin = 8;
int pinLucesJardin = 9;
int lecturaMovimientoJardin = 0;
int banderaJardin = 1;

/*#################################################Variables para manejar las luces#################################################*/
int luzUno = 13; // Cocina
int estadoLuzUno = 0; //0 apagada, 1 prendida
int luzDos = 12; // Living
int estadoLuzDos = 0;
int luzTres = 11; // Dormitorio
int estadoLuzTres = 0;
int luzCuatro = 1; // Descanso
int estadoLuzCuatro = 0;


/*#################################################Variables para manejar conexión ethernet#################################################*/
byte mac[] = { 0xAF, 0xBE, 0xCD, 0xDC, 0xEB, 0xFA };   //Direccion Fisica MAC
IPAddress ip(192,168,0,100); 
EthernetServer server(80);                             //Se usa el puerto 80 del servidor     
String readString;

/*####################################################################################################################################################
 * ###################################################################################################################################################
 * ################################################# FIN DECLARACIÃ“N DE VARIABLES ####################################################################
 * ###################################################################################################################################################
 * ###################################################################################################################################################*/

void setup () {
  Serial.begin(9600); 
  pinMode(botonPuerta, INPUT);
  pinMode(botonVentana, INPUT);
  pinMode(sensorMovimientoAlarma, INPUT);
  pinMode(sensorMovimientoJardin, INPUT);
  pinMode(pinAlarma, OUTPUT);
  pinMode(pinLucesJardin, OUTPUT);
  pinMode(luzUno, OUTPUT);
  pinMode(luzDos, OUTPUT);
  pinMode(luzTres, OUTPUT); 
  pinMode(luzCuatro, OUTPUT);
  digitalWrite(pinLucesJardin, LOW);
  digitalWrite(luzUno, LOW); 
  digitalWrite(luzDos, LOW);
  digitalWrite(luzTres, LOW);
  digitalWrite(luzCuatro, LOW);
  motorPuerta.attach(3);
  motorPuerta.write(90);
  motorVentana.attach(5);
  motorVentana.write(50);
  Ethernet.begin(mac,ip); // Inicializa la conexion Ethernet y el servidor
  server.begin();
  Serial.print("El Servidor es: ");
  Serial.println(Ethernet.localIP());    // Imprime la direccion IP Local
}

/* ################################################# PROGRAMA PRINCIPAL #################################################################### */

void loop () {
  /*Actualizamos conexión ethernet*/
  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {   
      if (client.available()) {
        char c = client.read(); //Lee caracter por caracter HTTP
        if (readString.length() < 100) {  
          readString += c; //Almacena los caracteres a un String
        }
         // si el requerimiento HTTP fue finalizado
        if (c == '\n') {          
          Serial.println(readString); //Imprime en el monitor serial
          client.println("HTTP/1.1 200 OK");           //envia una nueva pagina en codigo HTML
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();
          client.println("<!DOCTYPE html>");      
          client.println("<HTML>");
          client.println("<HEAD>");
          client.println("<TITLE>Casa inteligente V1.0</TITLE>");
          client.println("</HEAD>");
          client.println("<BODY>");
          client.println("<hr />");
          client.println("<H1><b><u>Conexion ethernet</u></b></H1>");
          client.println("<br />");
          
          client.println("<hr />");
          client.println("<hr />");
          client.println("<br />");
          client.println("<br />");

          client.println("<b>Manejador luces</b>");
          client.println("<br />");
          client.println("<br />");
          client.println("<a href=\"/?buttonDescansooff\"\"> Encender Descanso</a><br /> "); // construye en la pagina cada uno de los botones
          client.println(" | | | ");
          client.println("<a href=\"/?buttonDescansoon\"\"> Apagar Descanso</a> ");   
          client.println("<br />");
         
          client.println("<br />");
          client.println("<a href=\"/?buttonDormitoriooff\"\"> Encender Dormitorio</a><br /> ");  
          client.println(" | | | ");
          client.println("<a href=\"/?buttonDormitorioon\"\"> Apagar Dormitorio</a> ");  
          client.println("<br />");
         
          client.println("<br />"); 
          client.println("<a href=\"/?buttonLivingoff\"\"> Encender Living</a><br /> ");
          client.println(" | | | ");   
          client.println("<a href=\"/?buttonLivingon\"\"> Apagar Living</a> ");   
          client.println("<br />");
         
          client.println("<br />");
          client.println("<a href=\"/?buttonCocinaoff\"\"> Encender Cocina</a><br />");     
          client.println(" | | | ");
          client.println("<a href=\"/?buttonCocinaon\"\"> Apagar Cocina</a>"); 
          client.println("<br />");
          client.println("<br />");

          client.println("<hr />");

          client.println("<b>Manejador ventana</b>");
          client.println("<br />");
          client.println("<br />"); 
          client.println("<a href=\"/?abrirVentana\"\">Abrir ventana</a>");
          client.println(" | | | ");
          client.println("<a href=\"/?cerrarVentana\"\">Cerrar ventana</a><br />"); 
          client.println("<br />");
          client.println("<hr />");
          client.println("<br />"); 
          client.println("<p>Por: Agustina Galasso - Sofia Roccetti - Rodrigo Mansilla</p>");  
          client.println("<br />"); 
          client.println("</BODY>");
          client.println("</HTML>");
   
          delay(100);
          client.stop();//detiene el cliente servidor
         
          //control del arduino si un boton es presionado
          if (readString.indexOf("?buttonCocinaon") >0){
            digitalWrite(luzUno, HIGH);
            estadoLuzUno = 1;
          }
          if (readString.indexOf("?buttonCocinaoff") >0){
            digitalWrite(luzUno, LOW);
            estadoLuzUno = 0;
          }

          if (readString.indexOf("?buttonLivingon") >0){
            digitalWrite(luzDos, HIGH);
            estadoLuzDos = 1;
          }
          if (readString.indexOf("?buttonLivingoff") >0){
            digitalWrite(luzDos, LOW);
            estadoLuzDos = 0;
          }

          if (readString.indexOf("?buttonDormitorioon") >0){
            digitalWrite(luzTres, HIGH);
            estadoLuzTres = 1;
          }
          if (readString.indexOf("?buttonDormitoriooff") >0){
            digitalWrite(luzTres, LOW);
            estadoLuzTres = 0;
          }
                 
          if (readString.indexOf("?buttonDescansoon") >0){
            digitalWrite(luzCuatro, HIGH);
            estadoLuzCuatro = 1;
          }
          if (readString.indexOf("?buttonDescansooff") >0){
            digitalWrite(luzCuatro, LOW);
            estadoLuzCuatro = 0;
          }

          if (readString.indexOf("?abrirVentana") >0){
            motorVentana.write(-100);
            estadoVentana = 1;
           }
           if (readString.indexOf("?cerrarVentana") >0){
            motorVentana.write(85);
            estadoVentana = 0; 
           }
                  
          delay(100);
          client.stop(); //detiene el cliente servidor     
          readString=""; // Limpia el String(Cadena de Caracteres para una nueva lectura  
        }
      }
    }
  }
  
  /*Escuchar boton puerta*/
  estadoBotonPuerta = digitalRead(botonPuerta); 
  if (estadoBotonPuerta == 1){
    manejadorServoPuerta();
  }

  /*Escuchar boton ventana*/
  estadoBotonVentana = digitalRead(botonVentana); 
  if (estadoBotonVentana == 1){
    manejadorServoVentana();
  }
  
  /*Escuchar sensor movimiento alarma*/
  lecturaMovimientoAlarma = digitalRead(sensorMovimientoAlarma); 
  if (lecturaMovimientoAlarma == HIGH){
    manejadorAlarma();  
  } 
  
 /*Escuchar sensor movimiento jardin*/
  lecturaMovimientoJardin = digitalRead(sensorMovimientoJardin); 
  if (lecturaMovimientoJardin == HIGH) {
      digitalWrite(pinLucesJardin, HIGH);
      Serial.println("Hay presencia en el jardin");
    } else {
      digitalWrite(pinLucesJardin, LOW);
    }
}

/* ################################################# FIN PROGRAMA PRINCIPAL #################################################################### */

/*####################################################################################################################################################
 * ###################################################################################################################################################
 * ################################################# DECLARACIÃ“N DE FUNCIONES ########################################################################
 * ###################################################################################################################################################
 * ###################################################################################################################################################*/

/*#################################################Funcion para manejar la puerta#################################################*/
void manejadorServoPuerta(){
  long startTime = millis();
  while(millis() - startTime < 1000)
  {
    estadoBotonPuerta = digitalRead(botonPuerta); 
    if (estadoBotonPuerta == 1)
    {
      contadorPulsaciones = contadorPulsaciones + 1;
      delay(200);
      Serial.println(contadorPulsaciones, DEC); 
    } 
  }
  if (contadorPulsaciones >= 2){
    if(estadoPuerta == 0){
      if (banderaAlarma == 0){
        Serial.println("Se activo alarma");
        banderaAlarma = 1;
      }
      else {
        Serial.println("Se desactivo alarma");
        banderaAlarma = 0;
        digitalWrite(pinAlarma, LOW);
      }
    }
    else {
      Serial.println("Puerta abierta, no se puede activar la alarma");
      banderaAlarma = 0;
    }
  }
  else {
    if (banderaAlarma == 0){
      if (estadoPuerta == 0){
        Serial.println("Abrir puerta");
        motorPuerta.write(-80);
        estadoPuerta = 1;
      }
      else {
        Serial.println("Cerrar puerta");
        motorPuerta.write(80);
        estadoPuerta = 0;
      }
    }
    else {
      Serial.println("No se abre, alarma activa");
    }
  }
  contadorPulsaciones = 0;
}

/*#################################################Funcion para manejar la ventana#################################################*/
void manejadorServoVentana() {
   long startTime = millis();
  while(millis() - startTime < 1000){
    estadoBotonVentana = digitalRead(botonVentana); 
    if (estadoBotonVentana == 1){
      contadorPulsacionesVentana = contadorPulsacionesVentana + 1;
      delay(200);
      Serial.println(contadorPulsacionesVentana, DEC); 
    } 
  }

  if (contadorPulsacionesVentana >= 1){
    if(estadoVentana == 0){
      Serial.println("Abrir ventana");
      motorVentana.write(-100);
      estadoVentana = 1;
    } else {
      Serial.println("Cerrar ventana");
      motorVentana.write(85);
      estadoVentana = 0;
    }
  }
}

/*#################################################Funcion para manejar la alarma#################################################*/
void manejadorAlarma(){
    if (banderaAlarma == 1) {
      digitalWrite(pinAlarma, HIGH);
      Serial.println("INTRUSO");
      
      while(banderaAlarma == 1 && cant <=5){
          digitalWrite(luzUno, HIGH);
          delay(100);
          digitalWrite(luzUno, LOW);
          delay(100);
          digitalWrite(luzDos, HIGH);
          delay(100);
          digitalWrite(luzDos, LOW);
          delay(100);
          digitalWrite(luzTres, HIGH);
          delay(100);
          digitalWrite(luzTres, LOW);
          delay(100);
          digitalWrite(luzCuatro, HIGH);
          delay(100);
          digitalWrite(luzCuatro, LOW);
          delay(100);
          cant++;
      }
     } else {
          digitalWrite(pinAlarma, LOW);
          Serial.println("Hay presencia autorizada");
          cant = 0;  
     }
}

/*####################################################################################################################################################
 * ###################################################################################################################################################
 * ################################################# FIN DECLARACIÃ“N DE FUNCIONES ####################################################################
 * ###################################################################################################################################################
 * ###################################################################################################################################################*/


