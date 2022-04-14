#include <SPI.h>
#include <WiFiNINA.h>
int    HTTP_PORT   = 80;
String HTTP_METHOD = "GET";
char   HOST_NAME[] = "172.31.90.230:9000"; // change to your PC's IP address
String PATH_NAME   = "";
String queryString = "test?temp=10&humidity=20&imu=30gps=abc";

//Enter your hotspot 
const char SSID[]     =  "Johann's iPhone";    // Network SSID (name)
const char PASS[]     =  "lol12345";    // Network password (use for WPA, or use as key for WEP)
//HOST ip address
const char server[]   =  "172.31.90.230:9000";      // Server IP Address

int status = WL_IDLE_STATUS;

// Initialize the Wifi client library
WiFiClient client;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600); 
  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(SSID, PASS);
    // wait 10 seconds for connection:
    delay(10000);
  }
  httpRequest();
}

void loop() {

}

// this method makes a HTTP connection to the server:
void httpRequest() {
  // close any connection before send a new request.
  // This will free the socket on the Nina modul
  client.stop();
  // if there's a successful connection:
  if (client.connect(server, 80)) {
    Serial.println("Connected to server");
    // make a HTTP request:
    // send HTTP header
    client.println(HTTP_METHOD + " " + PATH_NAME + queryString + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println(); // end HTTP header
  } else {
    // if you couldn't make a connection:
    //Serial.println("connection failed");
  }
}
