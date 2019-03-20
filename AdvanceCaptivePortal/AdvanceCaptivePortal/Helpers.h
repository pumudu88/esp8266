#ifndef HELPERS_H
#define HELPERS_H

#include <Arduino.h>
     
static const char PROGMEM SETTINGS_HTML[] = R"rawliteral(  
<!DOCTYPE html>
<html>
   <head>
      <meta name="viewport" content="width=device-width, initial-scale=1">
      <style>
          table, th, td {
            border: 1px solid black;
          }
          body { text-align: center; padding: 10px; }
          body, table { font: 20px Helvetica, sans-serif; color: #333; }
      </style>
   </head>
   <body>
      <h2>Settings Page</h2>
      <table align="center" style="width:80%">
         <tr>
            <td>
              View save data :
            </td>
            <td align="center">
               <form method="get" action="/savedata.txt">
                  <button style="height:40px;width:100px" type="submit"><b>View File</b></button>
               </form>
            </td>
         </tr>
         <tr>
            <td>
              Clear save data :
            </td>
            <td align="center">
               <form method="get" action="/remove">
                  <button style="height:40px;width:100px" type="submit"><b>Remove File</b></button>
               </form>
            </td>
         </tr>
      </table>
   </body>
</html>
)rawliteral";

String getContentType(String filename, boolean isDownload);
String formatBytes(size_t bytes);

#endif
