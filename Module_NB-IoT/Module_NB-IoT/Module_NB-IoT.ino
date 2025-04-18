//// WORK WORK WORK 
#include <MKRNB.h>
#include "arduino_secrets.h"

const char PINNUMBER[] = "0000";

String message = "";

String timestamp = "1726605647";
String UUID = "89e82d42-fbc1-4b58-9a80-9d5142c27d8d";
float lati = 48.84146123956756;
float longi = 2.253043716973881;
String flags = "10010";

unsigned int localPort = 2390;      // Port local sur l'objet IoT
unsigned int serverPort = 9999;      // Port d'écoute du serveur AWS

unsigned int numero = 1;     // Numero du message



IPAddress AWS_IP(51,44,183,1); // IP du serveur AWS

NB nbAccess; // Objet Accès
NBUDP Udp; // Objet Socket UDP

void setup()
{
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Démarrage de l'objet NB-IoT");
  boolean connected = false; // status de connexion

  while (!connected) {
    if ((nbAccess.begin(PINNUMBER) == NB_READY) ) {

      connected = true;
      Serial.println("--Module connecté au réseau--");

    } else {
      Serial.println("Not connected");
      delay(1000);
    }
  }

  Udp.begin(localPort);
  Serial.println("--Socket UDP UP--");
}

void loop()
{
  
  message = concat(timestamp , UUID , lati , longi , flags);
  sendUDPpacket(AWS_IP, serverPort, message); // Envoi d'un datagram UDP
  
  
  // wait to see if a reply is available
  delay(1000);
  if ( Udp.parsePacket() ) {
    Serial.println("Datagram reçu");
    // We've received a packet, read the data from it
    //Udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

  }
  delay(10000);
}



// Fonction d'envoi de datagrames UDP
unsigned long sendUDPpacket(IPAddress& address, unsigned int port, String messageToSend)
{

  Udp.beginPacket(address, port); // Initialisation du message
  Udp.println(messageToSend); // message envoyé
  Udp.endPacket(); // Envoi du message
  Serial.println("\n--message envoyé--");

}


String concat (String timestamp , String UUID, float lati, float longi , String flags)
{
  String data = "##";
  data += timestamp;
  data += "#";
  data += UUID;
  data += "#";
  data += String (lati,14);
  data += "#";
  data += String (longi,14);
  data += "#";
  data += flags;
  data += "##";

  Serial.println("Data à envoyer : " + data);

return data;
}
