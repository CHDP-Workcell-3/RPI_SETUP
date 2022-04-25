#include <SPI.h>
#include <WiFiNINA.h>

// MAC address of MKR: 7C:9E:BD:3A:6F:AC
char   MKR_IP[] = "10.0.1.3"; // change to your PC's IP address (on cdp netowrk)

const char HOST_IP[]   =  "10.0.1.4";      // Server IP Address (on cdp network)
int    HTTP_PORT   = 9000;

//CDP Network (no internet)
const char SSID[] = "CDP_WC1_NoInternet";
const char PASS[] = "cdp@@1234";

int status = WL_IDLE_STATUS;

// Initialize the Wifi client library
WiFiClient client;

#define ENABLE_WIFI 1 // a bool to enable/disable wifi connections (useful for testing)

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);
  delay(1000);
  Serial.println("Starting WiFi module...");

  if (ENABLE_WIFI) {
    // get MAC address
    Serial.println("MAC address: ");
    // print your MAC address:  
    byte mac[6];   
    WiFi.macAddress(mac);   
    Serial.print("MAC: ");    
    Serial.print(mac[5], HEX);
    Serial.print(":");
    Serial.print(mac[4], HEX);
    Serial.print(":");
    Serial.print(mac[3], HEX);
    Serial.print(":");
    Serial.print(mac[2], HEX);
    Serial.print(":");
    Serial.print(mac[1], HEX);
    Serial.print(":");
    Serial.println(mac[0], HEX);
  
    // print your board's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);
  
    int numSsid = WiFi.scanNetworks();
    Serial.print("number of available networks:");  
    Serial.println(numSsid);
  
   // print the network number and name for each network found:   
   for (int thisNet = 0; thisNet < numSsid; thisNet++) {       
     Serial.print(thisNet);  
     Serial.print(") "); 
     Serial.print(WiFi.SSID(thisNet)); 
     Serial.print("\tSignal: ");      
     Serial.print(WiFi.RSSI(thisNet));  
     Serial.print(" dBm");    
     Serial.print("\tEncryption: ");    
     switch (WiFi.encryptionType(thisNet)) {      
       case ENC_TYPE_WEP:     
       Serial.println("WEP");      
       break;     
       case ENC_TYPE_TKIP:       
         Serial.println("WPA");         
         break;      
       case ENC_TYPE_CCMP:       
         Serial.println("WPA2");       
         break;       
       case ENC_TYPE_NONE:     
         Serial.println("None");     
         break;      
       case ENC_TYPE_AUTO:  
         Serial.println("Auto");     
         break;     
       case ENC_TYPE_UNKNOWN:      
       default:     
         Serial.println("Unknown");       
         break;   
     }  
   } 
   
    // attempt to connect to Wifi network:
    while (status != WL_CONNECTED) {
      Serial.println("Trying to connect...");
      // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
      status = WiFi.begin(SSID, PASS);
  //    status = WiFi.begin(SSID);
      delay(1000); // wait before reconnecting
    }
    Serial.println("Connected!");
  
    // print your board's IP address:
    ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);
  }
}

void loop() {
  delay(1000);
  Serial.println("Waiting for a message...\n");
  String packet = read_input();
  
  Serial.print("Got a packet: ");
  Serial.println(packet);
  Serial.println("Creating query string...");
  String query = create_query(packet);
  
  Serial.print("Query: ");
  Serial.println(query);
  Serial.println("Sending query...");
  send_request(query);
  
}

String read_input() {
  String message = "";
  char letter = ' ';
  Serial.print("Reading Message: ");
  while (1) {
    letter = ' ';
    if (Serial1.available()) {
      letter = (char)Serial1.read();
      message += letter;
    }
    else {
      Serial.println("Serial Line not available...");
      letter = '!';
      delay(2000);
    }
    Serial.print(letter);
    if (letter == '#') {
      Serial.print("#");
      return message;
    }
  }
}

String create_query(String str) {
  String temp = "NULL";
  String humidity = "NULL";
  String imu = "NULL";
  String gps = "NULL";
  int temp_index = str.indexOf("Device1");
  if (temp_index > 0) {
    temp_index += 12;
    int temp_end = str.indexOf(" ", temp_index);
    temp = str.substring(temp_index, temp_end);
  }
  int imu_index = str.indexOf("Device2");
  if (imu_index > 0) {
    imu_index += 12;
    int imu_end = str.indexOf(" ", imu_index);
    imu = str.substring(imu_index, imu_end);
  }
  int gps_index = str.indexOf("$");
  if (gps_index > 0) {
    gps_index += 12;
    int gps_end = str.indexOf("#", gps_index);
    gps = str.substring(gps_index, gps_end);
  }

  String query = "test?temp=" + temp + "&humidity=" + humidity + "&imu=" + imu + "&gps=" + gps; 
  return query;
}

// this method makes a HTTP connection to the server:
void send_request(String queryString) {
  String HTTP_METHOD = "GET";
  String PATH_NAME   = "";
  
  // close any connection before send a new request.
  // This will free the socket on the Nina module
  client.stop();
  // if there's a successful connection:
  if (client.connect(HOST_IP, HTTP_PORT)) {
    Serial.println("Connected to server");
    // make a HTTP request:
    // send HTTP header
    client.println(HTTP_METHOD + " " + PATH_NAME + queryString + " HTTP/1.1");
    client.println("Host: " + String(MKR_IP));
    client.println("Connection: close");
    client.println(); // end HTTP header
  } else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
  }
}
