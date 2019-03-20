#ifndef HELPERS_H
#define HELPERS_H

#include <Arduino.h>
     
static const char PROGMEM SETTINGS_HTML[] = R"rawliteral(  
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
                  <button type="submit">View Save Data</button>
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

String getContentType(String filename, boolean isDownload);
String formatBytes(size_t bytes);

#endif
