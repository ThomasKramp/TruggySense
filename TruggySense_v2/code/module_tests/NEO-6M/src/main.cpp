/*
 * Rui Santos 
 * Complete Project Details https://randomnerdtutorials.com
 */

#include <HardwareSerial.h>
#include <TinyGPS++.h>

TinyGPSPlus gps;
HardwareSerial &gpsSerial = Serial5;

struct localization {
  long date;
  long time;
  float latitude;
  float longitude;
  float velocity;
  float altitude;
  uint8_t connected_satellites;
};

// UBX CFG-RATE: 10Hz
byte UBX_10HZ[] = {
  0xB5, 0x62, 0x06, 0x08, 0x06, 0x00,
  0x64, 0x00,
  0x01, 0x00,
  0x01, 0x00,
  0x7A, 0x12
};

// Disable GLL, GSA, GSV, VTG — keep only GGA + RMC
byte DIS_GLL[] = {0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x2A};
byte DIS_GSA[] = {0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x31};
byte DIS_GSV[] = {0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x38};
byte DIS_VTG[] = {0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x46};

void sendUBX(byte *cmd, int len, byte cls, byte id, const char* label) {
  gpsSerial.write(cmd, len);
  gpsSerial.flush();
}

localization getLocalization() {
  localization loc = {0};

  // date: DDMMYY as long
  if (gps.date.isValid()) {
    loc.date = (long)gps.date.day() * 10000
             + (long)gps.date.month() * 100
             + (long)(gps.date.year() % 100);
  }

  // time: HHMMSSmm as long
  if (gps.time.isValid()) {
    loc.time = (long)gps.time.hour() * 1000000
             + (long)gps.time.minute() * 10000
             + (long)gps.time.second() * 100
             + (long)gps.time.centisecond();
  }

  if (gps.location.isValid()) {
    loc.latitude  = gps.location.lat();
    loc.longitude = gps.location.lng();
  }

  // velocity in km/h
  if (gps.speed.isValid()) {
    loc.velocity = gps.speed.kmph();
  }

  if (gps.altitude.isValid()) {
    loc.altitude = gps.altitude.meters();
  }

  if (gps.satellites.isValid()) {
    loc.connected_satellites = gps.satellites.value();
  }

  return loc;
}

void printLocalization(const localization &loc) {
  Serial.print("Date=");       Serial.print(loc.date);
  Serial.print(" Time=");      Serial.print(loc.time);
  Serial.print(" Lat=");       Serial.print(loc.latitude, 6);
  Serial.print(" Lng=");       Serial.print(loc.longitude, 6);
  Serial.print(" Vel=");       Serial.print(loc.velocity, 2);
  Serial.print("km/h Alt=");   Serial.print(loc.altitude, 1);
  Serial.print("m Sats=");     Serial.println(loc.connected_satellites);
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("=== GPS Init ===");

  gpsSerial.begin(9600);
  delay(500);

  // Serial.println("Disabling unused NMEA sentences...");
  // sendUBX(DIS_GLL, sizeof(DIS_GLL), 0x06, 0x01, "  GLL off");
  // sendUBX(DIS_GSA, sizeof(DIS_GSA), 0x06, 0x01, "  GSA off");
  // sendUBX(DIS_GSV, sizeof(DIS_GSV), 0x06, 0x01, "  GSV off");
  // sendUBX(DIS_VTG, sizeof(DIS_VTG), 0x06, 0x01, "  VTG off");

  // Serial.println("Setting 10Hz...");
  // sendUBX(UBX_10HZ, sizeof(UBX_10HZ), 0x06, 0x08, "  10Hz");

  // Serial.println("=== Ready ===");
}

void loop() {
    byte gpsData;
  while (gpsSerial.available() > 0) {
    gpsData = gpsSerial.read();
    gps.encode(gpsData);
    Serial.write(gpsData);
  }

  // Only print when a full new location fix arrives
  if (gps.location.isUpdated()) {
    localization loc = getLocalization();
    printLocalization(loc);
  }
}