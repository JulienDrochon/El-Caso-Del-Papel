/** Handle root or redirect to captive portal */
void handleRoot() {
  if (captivePortal()) {  // If caprive portal redirect instead of displaying the page.
    return;
  }
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");

  String Page;
  Page += F("<!DOCTYPE html><html lang='en'><head>"
            "<meta name='viewport' content='width=device-width'>"
            "<style>body{font-family: sans-serif;color:#ddd;background-color:rgb(50, 50, 50);}a, a:visited{color:#ddd; font-variant:small-caps;}a:active{border-bottom:2px solid #ddd;}</style>"
            "<title>BI/O Wifi Module Settings</title></head><body>"
            "<h1>Welcome to the B/IO wifi module settings</h1>");
  if (server.client().localIP() == apIP) {
    Page += String(F("<p>Connected through the soft AP:<br>")) + softAP_ssid + F("</p>");
  } else {
    Page += String(F("<p>Connected through the wifi network:<br>")) + ssid + F("</p>");
  }
  Page += F("<p><a href='/wifi'>config the wifi connection</a></p>"
            "<p><a href='/osc'>config osc connection</a></p>"
            "</body></html>");

  server.send(200, "text/html", Page);
}

/** Redirect to captive portal if we got a request for another domain. Return true in that case so the page handler do not try to handle the request again. */
boolean captivePortal() {
  if (!isIp(server.hostHeader()) && server.hostHeader() != (String(myHostname) + ".local")) {
    // Serial.println("Request redirected to captive portal");
    server.sendHeader("Location", String("http://") + toStringIp(server.client().localIP()), true);
    server.send(302, "text/plain", "");  // Empty content inhibits Content-length header so we have to close the socket ourselves.
    server.client().stop();              // Stop is needed because we sent no content length
    return true;
  }
  return false;
}

void handleOsc() {
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");

  String Page;
  Page += F("<!DOCTYPE html><html lang='en'><head>"
            "<meta name='viewport' content='width=device-width'>"
            "<style>body{font-family: sans-serif;color:#ddd;background-color:rgb(50, 50, 50);}a, a:visited{color:#ddd; font-variant:small-caps;}a:active{border-bottom:2px solid #ddd;}</style>"
            "<title>BI/O Wifi Module | OSC</title></head><body>");

  Page += String(F(    "<div><a href='/'>home</a> | <a href='/wifi'>wifi</a> | <a href='/osc'>osc</a></div>"
                       "<p>Last IP destination used : ")) + oscIP + F("</p>"
                           "\r\n<br /><form method='POST' action='oscsave'><h4>Enter computer IP's destination :</h4>"
                           "<input type='text' placeholder='ipaddress' name='o'/>"
                           "<br /><input type='submit' value='Save'/></form>"
                           "</body></html>");
 
  server.send(200, "text/html", Page);
  server.client().stop();
}

void handleSaveOsc() {
  server.arg("o").toCharArray(oscIP, sizeof(oscIP) - 1);
  //server.arg("p").toCharArray(password, sizeof(password) - 1);
  server.sendHeader("Location", "osc", true);
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send(302, "text/plain", "");  // Empty content inhibits Content-length header so we have to close the socket ourselves.
  server.client().stop();              // Stop is needed because we sent no content length
  saveCredentials();
  connect = strlen(ssid) > 0;
}

/** Wifi config page handler */
void handleWifi() {
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");

  String Page;
  Page += F("<!DOCTYPE html><html lang='en'><head>"
            "<meta name='viewport' content='width=device-width'>"
            "<style>body{font-family: sans-serif;color:#ddd;background-color:rgb(50, 50, 50);}a, a:visited{color:#ddd; font-variant:small-caps;}a:active{border-bottom:2px solid #ddd;}</style>"
            "<title>BI/O Wifi Module | Wifi</title></head><body>"
            "<div><a href='/'>home</a> | <a href='/wifi'>wifi</a> | <a href='/osc'>osc</a></div>"
            "<h1>Wifi config</h1>");
  if (server.client().localIP() == apIP) {
    Page += String(F("<p>You are connected through the soft AP: ")) + softAP_ssid + F("</p>");
  } else {
    Page += String(F("<p>You are connected through the wifi network: ")) + ssid + F("</p>");
  }
  Page += String(F("\r\n<br />"
                   "<table><tr><th align='left'>SoftAP config</th></tr>"
                   "<tr><td>SSID "))
          + String(softAP_ssid) + F("</td></tr>"
                                    "<tr><td>IP ")
          + toStringIp(WiFi.softAPIP()) + F("</td></tr>"
              "</table>"
              "\r\n<br />"
              "<table><tr><th align='left'>WLAN config</th></tr>"
              "<tr><td>SSID ")
          + String(ssid) + F("</td></tr>"
                             "<tr><td>IP ")
          + toStringIp(WiFi.localIP()) + F("</td></tr>"
              "</table>"
              "\r\n<br />"
              "<table><tr><th align='left'>WLAN list (refresh if any missing)</th></tr>");
  // Serial.println("scan start");
  int n = WiFi.scanNetworks();
  // Serial.println("scan done");
  if (n > 0) {
    for (int i = 0; i < n; i++) {
      Page += String(F("\r\n<tr><td>SSID ")) + WiFi.SSID(i) + ((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? F(" ") : F(" *")) + F(" (") + WiFi.RSSI(i) + F(")</td></tr>");
    }
  } else {
    Page += F("<tr><td>No WLAN found</td></tr>");
  }
  Page += F("</table>"
            "\r\n<br /><form method='POST' action='wifisave'><h4>Connect to network:</h4>"
            "<input type='text' placeholder='network' name='n'/>"
            "<br /><input type='password' placeholder='password' name='p'/>"
            "<br /><input type='submit' value='Connect/Disconnect'/></form>"
            "</body></html>");
  server.send(200, "text/html", Page);
  server.client().stop();  // Stop is needed because we sent no content length
}

/** Handle the WLAN save form and redirect to WLAN config page again */
void handleWifiSave() {
  // Serial.println("wifi save");
  server.arg("n").toCharArray(ssid, sizeof(ssid) - 1);
  server.arg("p").toCharArray(password, sizeof(password) - 1);
  server.sendHeader("Location", "wifi", true);
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send(302, "text/plain", "");  // Empty content inhibits Content-length header so we have to close the socket ourselves.
  server.client().stop();              // Stop is needed because we sent no content length
  saveCredentials();
  connect = strlen(ssid) > 0;  // Request WLAN connect with new credentials if there is a SSID
}

void handleNotFound() {
  if (captivePortal()) {  // If caprive portal redirect instead of displaying the error page.
    return;
  }
  String message = F("File Not Found\n\n");
  message += F("URI: ");
  message += server.uri();
  message += F("\nMethod: ");
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += F("\nArguments: ");
  message += server.args();
  message += F("\n");

  for (uint8_t i = 0; i < server.args(); i++) {
    message += String(F(" ")) + server.argName(i) + F(": ") + server.arg(i) + F("\n");
  }
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send(404, "text/plain", message);
}
