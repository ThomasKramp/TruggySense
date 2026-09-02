#include "sd_logger.h"

String SDLogger::generateUniqueFilenameAndUse()
{
    filename = BASE_FILE_NAME;
    int counter = 1;
    // Check if the file exists
    while (sd.exists(filename.c_str()))
    {
        // If it exists, create a new filename with a counter
        filename = "llc_data_" + String(counter) + ".bin";
        counter++;
    }
    // Serial.println(filename);
    return filename;
}

uint8_t SDLogger::writeBinData(logging_data_t bin_data)
{
    if (!file.isOpen())
    {
        if (!file.open(filename.c_str(), O_WRITE | O_CREAT | O_APPEND))
        {
            return 0;
        }
    }

    // Serial.println(sizeof(bin_data));

    uint8_t buffer[sizeof(bin_data)];
    memcpy(buffer, &bin_data, sizeof(bin_data));
    
    file.write(buffer, sizeof(buffer));

    return 1;
}

uint8_t SDLogger::writeBinData(uint8_t * bin_data)
{
    if (!file.isOpen())
    {
        if (!file.open(filename.c_str(), O_WRITE | O_CREAT | O_APPEND))
        {
            return 0;
        }
    }

    file.write(bin_data, sizeof(bin_data));
    return 1;
}

uint8_t SDLogger::init()
{
    if (!sd.begin(SD_CONFIG))
    {
        // Serial.println("SD card initialization failed!");
        return 0;
    }

    generateUniqueFilenameAndUse();
    return 1;
}

uint8_t SDLogger::logData(logging_data_t data)
{
    if (!writeBinData(data))
    {
        return 0;
    }

    return 1;
}

uint8_t SDLogger::closeFile()
{
    if(file.isOpen())
    {
        file.flush();
        file.close();
        return 1;
    }
    return 0;
}

String SDLogger::getFileName()
{
    return filename;
}

uint64_t SDLogger::getFileSize()
{
    return file.fileSize();
}