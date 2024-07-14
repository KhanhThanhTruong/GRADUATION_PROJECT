#include <SD_mybuit.h>

String SD_JS::pushData(const char *filename, String js_str)
{
    // SD.begin(5);
    // if the file doesn't exist, create it
    if (!SD.exists(filename))
    {
        SD.open(filename, FILE_WRITE);
    }

    // open the file
    File myFile = SD.open(filename, FILE_APPEND);
    if (!myFile)
    {
        return "OPEN FILE FAILED";
    }

    // if the file opened okay, write to it
    // String js_str;
    // serializeJson(js, js_str);
    myFile.println(js_str);
    // Check write error
    if (myFile.getWriteError())
    {
        myFile.close();
        return "ERROR: PUSH JSON FAILED";
    }
    // close the file
    myFile.close();
    return "PUSH JSON SUCCESS";
}

String SD_JS::pushData(StaticJsonDocument<200> js)
{ // 2024-05-28T10:00:00Z
    // std::string time_str = js["time"];
    // std::string filename = "/" + time_str.substr(0, 4) +
    //                        "/" + time_str.substr(5, 2) + "/" +
    //                        time_str.substr(8, 2) + "/" + time_str.substr(11, 2) + ".json";
    // if (!SD.exists(filename.c_str()))
    // {
    //     SD_JS::creatfile(filename);
    //     return SD_JS::pushData(filename.c_str(), js);
    // }
    // else
    // {
    //     return SD_JS::pushData(filename.c_str(), js);
    // }
}

String SD_JS::getData(const char *filename, StaticJsonDocument<200> &js)
{
    // Mở file để đọc
    File myfile = SD.open(filename, FILE_READ);
    if (!myfile)
    {
        return "OPEN FILE FAILED";
    }

    // Đọc dòng đầu tiên
    String firstLine = myfile.readStringUntil('\n');
    myfile.close();

    return firstLine;
}

String SD_JS::readAndDeleteLine(const char *filename, StaticJsonDocument<200> &js)
{
    // Mở file để đọc
    File myfile = SD.open(filename, FILE_READ);
    if (!myfile)
    {
        return "OPEN FILE FAILED";
    }

    // Đọc dòng đầu tiên
    String firstLine = myfile.readStringUntil('\n');
    if (firstLine.length() == 0)
    {
        myfile.close();
        SD.remove(filename);
        return "END OF FILE";
    }

    // Đọc phần còn lại của file vào một String
    String remainingData;
    while (myfile.available())
    {
        remainingData += myfile.readStringUntil('\n') + "\n";
    }
    myfile.close();

    // Xóa file hiện tại
    if (!SD.remove(filename))
    {
        return "DELETE FILE FAILED";
    }

    // Ghi phần còn lại của file vào một file mới
    myfile = SD.open(filename, FILE_WRITE);
    if (!myfile)
    {
        return "REOPEN FILE FAILED";
    }
    myfile.print(remainingData);
    myfile.close();

    // Chuyển đổi dòng đầu tiên sang JSON
    DeserializationError error = deserializeJson(js, firstLine);
    if (error)
    {
        return "ERROR: " + String(error.c_str());
    }
    else
    {
        return "READ AND DELETE LINE SUCCESS";
    }
}
std::string SD_JS::creatfile(std::string dir)
{ // /2024/05/28/16.json
    if (SD.exists(dir.c_str()))
    {
        return "FILE " + dir + " EXISTS";
    }
    else
    {
        creatFolder(dir.substr(0, 5));
        creatFolder(dir.substr(0, 8));
        creatFolder(dir.substr(0, 11));
        // File myfile = SD.open(dir, FILE_WRITE);
        if (!SD.open(dir.c_str(), FILE_WRITE))
        {
            return "CREAT FILE " + dir + " FAILED";
        }
        else
        {
            return "CREATE FILE " + dir + " SUCCESS";
        }
        // myfile.close();
    }
}

std::string SD_JS::creatFolder(std::string dir)
{
    if (SD.exists(dir.c_str()))
    {
        return "FOLDER " + dir + " EXISTS";
    }
    else
    {
        if (SD.mkdir(dir.c_str()))
        {
            return "CREAT FOLDER" + dir + "SUCCESS";
        }
        else
        {
            return "CREAT FOLDER" + dir + "FAILED";
        }
    }
}

String SD_JS::deleteFile(const char *filename)
{
    if (!SD.exists(filename))
    {
        return "FILE " + String(filename) + " NOT EXISTS";
    }
    else
    {
        if (SD.remove(filename))
        {
            return "DELETE FILE " + String(filename) + " SUCCESS";
        }
        else
        {
            return "DELETE FILE " + String(filename) + " FAILED";
        }
    }
}

String SD_JS::init(const int cs_pin)
{
    if (!SD.begin(cs_pin))
    {
        return "INIT SD CARD FAILED";
    }
    else
    {
        return "INIT SD CARD SUCCESS";
    }
}

String SD_JS::listFile(File dir, int numTabs)
{
    // File dir = SD.open(dir_str);
    while (true)
    {
        File entry = dir.openNextFile();
        if (!entry)
        {
            // Không còn tệp để liệt kê
            break;
        }
        for (uint8_t i = 0; i < numTabs; i++)
        {
            Serial.print('\t'); // In tab để thụt đầu dòng
        }
        Serial.print(entry.name());
        if (entry.isDirectory())
        {
            Serial.println("/");
            SD_JS::listFile(entry, numTabs + 1);
        }
        else
        {
            // In kích thước tệp
            Serial.print("\t\t");
            Serial.println(entry.size(), DEC);
        }
        entry.close();
    }
}