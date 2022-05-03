

// ------------------包含文件--------------------
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

// ---------------------------------------------

// ------------------WiFiInf--------------------
const char *ssid = "NuclearSchool";                              // WiFi Name
const char *password = "teacher123456";                          // WiFi Password
const char *TimeServer = "http://quan.suning.com/getSysTime.do"; // website Host
const char *Host = "quan.suning.com";
// ---------------------------------------------

// -----------------相关定义---------------------
WiFiClient client;
HTTPClient http;

char *User_Agent = " Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/83.0.4103.61 Safari/537.36";
// ---------------------------------------------
void setup()
{
    // 初始化部分
    Serial.begin(2400); // 以 2400 波特率 打开串口。
    while (!Serial);  //等待串口打开

    //Serial.println("Connecting to ");
    //Serial.println(ssid);

    WiFi.begin(ssid, password); // WiFi链接

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
    } // 等待WiFi连接成功
    //Serial.println("");
    //Serial.print("WiFi connected,IP:");
    //Serial.println(WiFi.localIP());
}

bool TimeStringGet(const char *timeinfo, char *opbuffer)
{
    const char *ptr1 = timeinfo;
    const char *ptr2 = NULL;
    char count = 0;

    while (*ptr1 != '\0')
    {
        ptr1++;
        if (*ptr1 == ' ')
        {
            ptr2 = ptr1 + 1;
            while (*ptr2 != '\0')
            {
                opbuffer[count++] = *ptr2;
                ptr2++;
            }
            if (opbuffer[9] != '\0')
                opbuffer[9] = '\0';
            //cout << "successful :" << opbuffer << endl;
            return true;
        }
    }
    return true;
}

/*
bool ParseJsonFromString(const String info, char* opbuffer)
{


  Json::CharReaderBuilder builder;
  JSONCPP_STRING errs;
  std::unique_ptr<Json::CharReader> reader (builder.newCharReader());
  Json::Value root;

  if (!reader->parse(info.c_str(),info.c_str() + info.length(), &root,&errs))
  {
    Serial.printf("ParseJson Failed..");
    return false;
  }

  //cout << "SysTime2: " << root["sysTime2"].asString() << endl;

  if (TimeStringGet(root["sysTime2"].asCString(), opbuffer))
  {
    Serial.printf( "successful Get Time");
  }

  return true;
}
*/
bool ParseJson(const String info, char *buffer)
{
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, info);
    if (error)
    {
        Serial.print(F("deserializeJson() Failed: "));
        Serial.println(error.c_str());
        return false;
    }

    const char *systime = doc["sysTime2"];
    TimeStringGet(systime, buffer);
    return true;
}
void loop()
{
  //  循环部分
  //Serial.print("[HTTP] GET ...\n");
    if (Serial.available() > 0)
    {
        String order = "";
        order = Serial.readString();
        if (order == "!time")
        {
            http.begin(TimeServer);
            int httpCode = http.GET();
            char Time[9] = {0};
            if (httpCode > 0)
            {
            //Serial.printf("YES\n");

                if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
                {
                    String payload = http.getString();
                    if (ParseJson(payload, Time))
                    {
                        Serial.println(Time);
                    }
                }
            }
            else
            {
                Serial.printf("[HTTP] GET.. failed, error: %s\n", http.errorToString(httpCode).c_str());
            }
            http.end();
        }
    }
}
