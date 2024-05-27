#include <SD_mybuit.h>

String pushData(const char *filename, StaticJsonDocument<200> js)
{   
    SD.begin(5);
    File myFile = SD.open(filename, FILE_WRITE);
    if (!myFile)
    {
        return "Open file failed";
    }

    // if the file opened okay, write to it
    String js_str;
    serializeJson(js, js_str);
    myFile.println(js_str);
    myFile.close();
    return "Done";
}