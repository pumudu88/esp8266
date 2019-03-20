void handleRoot(){
  server.sendHeader("Location", "/index.html",true);
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
    
  String contentType = getContentType(path, server.hasArg("download"));
  
  if (SPIFFS.exists(path)) {
    Serial.println("index file exist!!");
    File file = SPIFFS.open(path, "r");
    server.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}

void handleSave() {
  String str = "Done ...\r\n";

  Serial.print("number of arguments "); 
  Serial.println(server.args());


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
