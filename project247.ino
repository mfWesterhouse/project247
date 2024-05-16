#include <Wifi.h>
#include <WifiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

const char* ssid = "WR3005N3-757E";
const char* password = "70029949";

String button = "<html><body id='bdy_1' style='height: 100px; width: 100px;'> <div id='bdy_2' style='height: 400px; width: 600px; margin-top: 50px; margin-left: 500px;'> <div style='margin-top: 100px; margin-left: 100px;'> <h2 style='color:white;'>ESP32 Webserver</h2> <p style='color:white;'>Click on the respected button to switch the Appliances:</p><div class='row'><table> <tr> <th><h3 style='color:white;'>Room Light</h3><input id= 'b1' type=button onClick=\"parent.location='/roomLight/on'\" value='On'> <input id= 'b2' type=button onClick=\"parent.location='/roomLight/off'\" value='Off'></th><th><h3 style='color:white;'>Room Bell</h3> <input id= 'b1' type=button onClick=\"parent.location='/roomBell/on'\" value='On'> <input id= 'b2' type=button onClick=\"parent.location='/roomBell/off'\" value='Off'></th></table></div></body></div></div><style>#bdy_1{background-image: url('https://i.postimg.cc/bvw7DS9S/Technological-background.jpg');background-repeat: no-repeat;background-size: 1920px 900px;font-family: 'helvetica neue';font-weight: 200;font-size: 20px;}#bdy_2{font-family: 'helvetica neue';font-weight: 200;font-size: 20px;}#b1{border: none;color: white;padding: 15px 32px;text-align: center;text-decoration: none;display: inline-block;font-size: 16px;margin: 4px 2px;cursor: pointer;}#b2{border: none;color: white;padding: 15px 32px;text-align: center;text-decoration: none;display: inline-block;font-size: 16px;margin: 4px 2px;cursor: pointer;}#b1{background-color: #4CAF50;}#b2{background-color: red;}</style></html>"

WebServer server(800);

void handleNotFound(){
  
  String message = "File Not Found\n\n";
  message += "URI: "
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_Get) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server, args(); i++){
    message += " " + server.argName(i) + "." + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

const int LED_1 = 2;
const int Buzzer = 15;

void setup(void){
  pinMode(LED_1, OUTPUT);
  pinMode(Buzzer, OUTPUT);

  Serial.begin(115200);
  Wifi.mode(WIFI_STA);
  Wifi.begin(ssid, password);
  Serial.println("");

  while (Wifi.status()!=WI_CONNECTED){
    delay(500);
    Serial.print("");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(Wifi.localIP());
  
  if(MDNS.begin("esp32")){
    Serial.println("MDNS responder started");
  }
  
  server.on("/", handleRoot);
  
  server.on("/roomLight/on", [](){
    server.send(200, "text/html", button);
  });

  server.on("/roomLight/off", [](){
    server.send(200, "text.html", button);
    digitalWrite(LED_1, LOW);
  });

  server.on("/roomBell/on", [](){
    server.send(200, "text/html", button)
    digitalWrite(Buzzer, HIGH);
  });

  server.on("/roomBell/of", [](){
    server.send(200, "text/html", button)
    digitalWrite(Buzzer, LOW);
  });

  server.onNotFound(handleNotFound);

  server.begin()
  Serial.println("HTTP server started");

  digitalWrite(LED_1, LOW);
  digitalWrite(Buzzer, LOW);
}

void loop(void){
  server.handleClient();
}