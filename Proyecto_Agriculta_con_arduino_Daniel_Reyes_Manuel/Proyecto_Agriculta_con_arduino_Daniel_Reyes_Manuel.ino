/*
 * POST Data 1.1
 * 
 */

 /************TCP**********************************/
#include <WiFi.h>
const char *ssid = "test";//WIFI
const char * password = "12345678"; // contraseña WIFI
 const char * serverIP = "183.230.40.33"; // La dirección a la que se accede
 uint16_t serverPort = 80; // Número de puerto del servidor
 String url = "http://api.heclouds.com/devices/526038005/datapoints?type=3";// ID de dispositivo de URL
String api="NkjCsVzErzqKLv8pKlWMIG6e8DE=";//api-key
 Publicación de cadena; // solicitud http
int Content_Length;
 Cliente WiFiClient; // Declarar un objeto cliente para conectarse al servidor
/****************************************************/


 /************recopilación de datos*********************************/ 
 size_t data_get [5] [8] = {// id de instrucción
     {1,3,0,1,0,2,149,203}, // valor TDS 1
     {2,3,0,8,0,1,5,251}, // valor de ph del suelo 2
     {3,3,0,0,0,6,196,42}, // Temperatura y humedad 3
  {4,3,0,5,0,1,148,94},//CO2       4
     {5,3,0,7,0,2,116,78} // ligero 5
  };
 //Datos del sensor
long tsd_value=0,co2_value=0,lig_value=0;
float ph_value=0,tem_value=0,hum_value=0;
String string1,string2,string3,string4;
int value = 0;
 static unsigned long time_ = 0; // Defina el tiempo de envío inicial a 0
 static unsigned long time_out = 0; // Defina el tiempo de espera de envío como 0
/****************************************************/

void setup()
{
         Serial2.begin (9600); // Inicializar el puerto serie del transceptor de datos de 9600 baudios
         Serial.begin (9600); // Debug print serial port
    Serial.println();
    
   ;
    Serial.print("Connecting to ");
    Serial.println(ssid);

         WiFi.begin (ssid, contraseña); // Conectarse a la red

         while (WiFi.status ()! = WL_CONNECTED) {// Espere una conexión de red exitosa
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
         Serial.println (WiFi.localIP ()); // IP del módulo de impresión

}

void loop()
{
       // Obtener la longitud de la matriz
   int len=sizeof(data_get)/sizeof(size_t);
   int len2=sizeof(data_get[0])/sizeof(size_t);
   int len_data=len/len2;

         if (millis () - time_> 200) {// Cuando el tiempo del sistema es superior a 200ms 0.2s para recopilar datos de un sensor
     for(int j=0;j<8;j++){
             Serial2.write (data_get [value] [j]); // Enviar instrucciones al sensor 
     }
      time_=millis();
    }
    
     
  
  
  if(Serial2.available()){
               value ++; // se envía acumulación para la siguiente instrucción
       
               size_t counti = Serial2.available (); // El búfer serial devuelve el número exacto de bytes legibles en el búfer
       uint8_t sbuf[counti];
               Serial2.readBytes (sbuf, counti); // Leer caracteres de la longitud especificada counti desde el puerto serie a la matriz de búfer sbuf. Formación
//       Serial.write(sbuf,counti);
       delay(50);
               switch (sbuf [0]) {// Determine el ID del sensor
       case 1:string1=tohex(int(sbuf[3]));
              string2=tohex(int(sbuf[4]));
              tsd_value=todec(string1,string2);
              Serial.print("tds: ");
              Serial.println(tsd_value);
              break;
       case 2:string1=tohex(int(sbuf[3]));
              string2=tohex(int(sbuf[4]));
              ph_value=((float)todec(string1,string2))/10;
              Serial.print("ph: ");
              Serial.println(ph_value);
              break;
       case 3:string1=tohex(int(sbuf[3]));
              string2=tohex(int(sbuf[4]));
              hum_value=((float)todec(string1,string2))/10;
              string1=tohex(int(sbuf[5]));
              string2=tohex(int(sbuf[6]));
              tem_value=((float)todec(string1,string2))/10;
              Serial.print("tem: ");
              Serial.print(tem_value);
              Serial.print("  hum: ");
              Serial.println(hum_value);
              break;
       case 4:string1=tohex(int(sbuf[3]));
              string2=tohex(int(sbuf[4]));
              co2_value=((float)todec(string1,string2));
              Serial.print("CO2: ");
              Serial.println(co2_value);
              break;
       case 5:string1=tohex(int(sbuf[3]));
              string2=tohex(int(sbuf[4]));
              string3=tohex(int(sbuf[5]));
              string4=tohex(int(sbuf[6]));
              lig_value=((float)todec_lig(string1,string2,string3,string4));
              Serial.print("light: ");
              Serial.println(lig_value);
              break;
       }
       
  }

     if (millis () - time_out> 4000) {// Procesamiento del tiempo de espera de la colección
    value=5;
    time_out=millis();
  }
     if (value == len_data) {// Juzgar cuándo se han enviado las instrucciones
         valor = 0; // Establecer en 0  
         Serial.print ("conectando a"); // IP del servidor de conexión de impresión
    Serial.println(serverIP);

    // Use WiFiClient class to create TCP connections
         if (! client.connect (serverIP, serverPort)) {// Determine si conectarse al servidor
        Serial.println("connection failed");
        return;
         } else {// Conectado correctamente
       Serial.println("connection succ");
       Serial.println();
       String data_value="{\"tem\":"+String(tem_value)+",\"hum\":"+String(hum_value)
          +",\"lig\":"+String(lig_value)+",\"tds\":"+String(tsd_value)
          +",\"CO2\":"+String(co2_value)+",\"soil_ph\":"+String(ph_value)
          +",\"ec\":"+String(tsd_value/0.47)+"}\r\n\r\n";
       Content_Length=data_value.length();
       post=String("POST ")+url+" HTTP/1.1\r\n"+"api-key: "+api+"\r\n"
          +"Host:api.heclouds.com\r\n"+"Connection:close\r\n"+"Content-Length:"
          +String(Content_Length)+"\r\n\r\n"+data_value;
//          +"{\"tem\":"+String(tem_value)+",\"hum\":"+String(hum_value)
//          +",\"lig\":"+String(lig_value)+",\"tds\":"+String(tsd_value)
//          +",\"CO2\":"+String(co2_value)+",\"soil_ph\":"+String(ph_value)
//          +"}\r\n\r\n";
      
          
          Serial.println(Content_Length);
                       Serial.print (post); // Solicitud HTTP de impresión de puerto serie
                       client.print (post); // Enviar solicitud HTTP al servidor
   
      }
  }
  
     while (client.available ()) {// Juzgar si el WIFI tiene datos
                 String line = client.readStringUntil ('\ r'); // Leer datos WIFI y asignarlos a la línea
                 Serial.print (línea); // línea de impresión
          
    }
   
}

 void init_data () {// Inicializa todos los datos, cuando el sensor no recopila datos, el valor es 0;
  tsd_value=0;
  ph_value=0;
  hum_value=0;
  tem_value=0;
  co2_value=0;
  lig_value=0;
}
 
 // Combinar cadenas y convertirlas en datos largos  
long todec(String string1,String string2){ 
               String str = string1 + string2; // Combina dos cadenas
               int str_len = str.length () + 1; // str longitud de la cadena
               char char_array [str_len]; // Crear matriz de caracteres
               str.toCharArray (char_array, str_len); // Convertir str a char array
       long iOut;
       iOut = strtol (char_array, NULL, 16); // Convierta la matriz char_array en datos largos con 16 como peso largo
       return iOut;
}

 // String merge (iluminación, convertida a datos largos
long todec_lig(String string1,String string2,String string3,String string4){   
       String str=string1+string2+string3+string4;
       int str_len = str.length() + 1;
       char char_array[str_len];
       str.toCharArray(char_array, str_len);
       long iOut;
       iOut = strtol(char_array, NULL, 16);
       return iOut;
}


 // Decimal a hexadecimal
String tohex(int n) {
  if (n == 0) {
         return "00"; // n es 0
  }
  String result = "";
  char _16[] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
  };
  const int radix = 16;
  while (n) {
         int i = n% radix; // resto
         result = _16 [i] + result; // Suma el número hexadecimal correspondiente al resto del resultado
         n / = radix; // Divide entre 16 para obtener el cociente, que es el dividendo en la siguiente ronda
  }
  if (result.length() < 2) {
         resultado = '0' + resultado; // Cero relleno para menos de dos dígitos
  }
  return result;
}
