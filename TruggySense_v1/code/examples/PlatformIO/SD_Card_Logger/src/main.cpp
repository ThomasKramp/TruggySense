// Test Teensy SDIO with write busy in a data logger demo.
//
// The driver writes to the uSDHC controller's FIFO then returns
// while the controller writes the data to the SD.  The first sector
// puts the controller in write mode and takes about 11 usec on a
// Teensy 4.1. About 5 usec is required to write a sector when the
// controller is in write mode.
// #include "SdFat.h"
// #include "RingBuf.h"

#include "sd_logger.h"

// Use Teensy SDIO
// #define SD_CONFIG SdioConfig(FIFO_SDIO)

// #ifndef SDCARD_SS_PIN
// const uint8_t SD_CS_PIN = SS;
// #else  // SDCARD_SS_PIN
// const uint8_t SD_CS_PIN = SDCARD_SS_PIN;
// #endif // SDCARD_SS_PIN

// // Try to select the best SD card configuration.
// #if defined(HAS_TEENSY_SDIO)
// #define SD_CONFIG SdioConfig(FIFO_SDIO)
// #elif defined(RP_CLK_GPIO) && defined(RP_CMD_GPIO) && defined(RP_DAT0_GPIO)
// // See the Rp2040SdioSetup example for RP2040/RP2350 boards.
// #define SD_CONFIG SdioConfig(RP_CLK_GPIO, RP_CMD_GPIO, RP_DAT0_GPIO)
// #elif ENABLE_DEDICATED_SPI
// #define SD_CONFIG SdSpiConfig(SD_CS_PIN, DEDICATED_SPI, SD_SCK_MHZ(16))
// #else // HAS_TEENSY_SDIO
// #define SD_CONFIG SdSpiConfig(SD_CS_PIN, SHARED_SPI, SD_SCK_MHZ(16))
// #endif // HAS_TEENSY_SDIO

// SdFs sd;
// FsFile file;

// Base filename for the CSV file

// const char *baseFilename = "data.csv";

// String filename = "";

// Function to generate a unique filename

SDLogger sd_logger;

// String generateUniqueFilename()
// {
//   String filename = baseFilename;
//   int counter = 1;

//   // Check if the file exists
//   while (sd.exists(filename.c_str()))
//   {
//     // If it exists, create a new filename with a counter
//     filename = "data_" + String(counter) + ".csv";
//     counter++;
//   }
//   return filename;
// }

// void writeCSVDataHeader(SdFile &file)
// {
//   String header = "Timestamp,IBUS_Channel_1,IBUS_Channel_2,IBUS_Channel_3,IBUS_Channel_4,FS,Encoder_RPM_1,Encoder_RPM_2,Encoder_RPM_3,Encoder_RPM_4,Temp_HLC,Temp_LLC,Temp_ESC_1,Temp_BM_1,Temp_SS,Temp_BP,Current_HLC,Current_LLC,Current_ESC_1,Current_SS,Voltage_HLC,Voltage_LLC,Voltage_3V3PER,Voltage_C1,Voltage_C2,Voltage_C3,Voltage_C4,Voltage_BATT,IMU_Roll,IMU_Pitch,IMU_Yaw,IMU_W,IMU_I,IMU_J,IMU_K,Safety_Dead_Switch,FS_Status";
//   if (!file.isOpen())
//   {
//     if (!file.open(filename.c_str(), O_WRITE | O_CREAT | O_APPEND))
//     {
//       Serial.println("Failed to open file for writing!");
//       return;
//     }
//   }

//   file.println(header);
//   file.close();
// }

// // Function to write CSV data to the file
// void writeCSVData(SdFile &file)
// {
//   // Mock data to write
//   long time = millis();
//   String mockCSVData = String(time) + ",1500,1500,1500,1500,0,100,110,105,95,30,25,35,40,20,22,5,6,3,4,12.5,12.0,3.3,4.1,4.0,4.2,4.3,0.5,0.1,0.0,0.0,0.0,0.0,0.0,1,0";

//   // Write the data to the file
//   file.println(mockCSVData);

//   // TODO: do it with the actual data
// }

void setup()
{
  pinMode(13, OUTPUT);

  Serial.begin(9600);
  // while (!Serial)
  // {
  // }
  // Initialize SD card
  // if (!sd.begin(SD_CONFIG))
  // {
  //   Serial.println("SD card initialization failed!");
  //   return;
  // }

  // Generate a unique filename
  // filename = generateUniqueFilename();
  // Serial.print("Creating file: ");
  // Serial.println(filename);
  sd_logger.init();

  Serial.print("Creating file: ");
  Serial.println(sd_logger.getFileName());

  // SdFile file; // Use SdFile instead of File

  long timer = micros();

  // writeCSVDataHeader(file);

  // file.println("This is a test!");
  // file.close();

  // uint8_t data[4] = {0,0,0,0};

  // Explicit open file
  long write_timer = millis();
  for (uint8_t i = 0; i < 2; i++) // run for 10 minutes -> (10*60)/ 0.01s or 10ms
  {
    // if (!file.isOpen())
    // {
    //   if (!file.open(filename.c_str(), O_WRITE | O_CREAT | O_APPEND))
    //   {
    //     Serial.println("Failed to open file for writing!");
    //     return;
    //   }
    // }

    bool report = false;
    // cli();
    // writeCSVData(file);
    // sei();

    logging_data data = {
        1, // timestamp
        2,   // date
        12345,      // time (in seconds from start of day)
        35.5f,      // t_llc
        45.3f,      // t_hlc
        30.2f,      // t_bp
        50.1f,      // t_ss
        60.4f,      // t_bm_1
        55.3f,      // t_bm_2
        58.2f,      // t_bm_3
        57.8f,      // t_bm_4
        42.0f,      // t_esc_1
        41.5f,      // t_esc_2
        43.1f,      // t_esc_3
        40.9f,      // t_esc_4
        2.0f,       // i_ss
        1.8f,       // i_llc
        2.2f,       // i_hlc
        10.0f,      // roll
        15.5f,      // pitch
        20.5f,      // yaw
        1.0f,       // w
        0.5f,       // i
        0.5f,       // j
        0.5f,       // k
        300.0f,     // rpm_wheel_1
        310.0f,     // rpm_wheel_2
        320.0f,     // rpm_wheel_3
        330.0f,     // rpm_wheel_4
        1500.0f,    // rpm_bm_1
        1400.0f,    // rpm_bm_2
        1450.0f,    // rpm_bm_3
        1600.0f,    // rpm_bm_4
        45.12345f,  // latitude
        -93.12345f, // longitude
        75.0f,      // velocity
        300.0f,     // altitude
        3.7f,       // v_cell_1
        3.7f,       // v_cell_2
        3.7f,       // v_cell_3
        3.7f,       // v_cell_4
        14.8f,      // v_batt
        5.0f,       // v_llc
        5.0f,       // v_hlc
        3.3f,       // v_phrp_3v3
        1500,       // ch_1
        1500,       // ch_2
        1500,       // ch_3
        1500,       // ch_4
        1500,       // ch_5
        1500,       // ch_6
        1500,       // ch_7
        1500,       // ch_8
        1500,       // ch_9
        1500,       // ch_10
        1500,       // ch_11
        1500,       // ch_12
        1500,       // ch_13
        1500,       // ch_14
        1500,       // pwm_bm_1
        1500,       // pwm_bm_2
        1500,       // pwm_bm_3
        1500,       // pwm_bm_4
        1500,       // pwm_ss
        10,         // c_s (connected satellites)
        1,          // n_failsafe (failsafe inactive)
        0,          // dead_switch (inactive)
        5,          // i_esc_1
        5,          // i_esc_2
        5,          // i_esc_3
        5           // i_esc_4
    };

    Serial.print("timestamp: ");
    // Serial.println(data[0]);
    Serial.println(data.timestamp);
    Serial.println(data.t_hlc);

    // data.timestamp = millis();

    if (!sd_logger.logData(data))
    {
      break;
    }

    sd_logger.closeFile();

    // Serial.println(sd_logger.getFileSize());

    while (millis() - write_timer < 10)
    {
      if (!report)
      {
        Serial.println(millis() - write_timer);
        report = true;
      }
      yield();
    }

    write_timer = millis();
  }

  Serial.print("closeFile");
  Serial.println(sd_logger.closeFile());

  Serial.println(micros() - timer);
}

void loop()
{
  Serial.println("Type any character to start");
  while (!Serial.available())
  {
    delay(100);
    digitalWrite(13, !digitalRead(13));
  }
}