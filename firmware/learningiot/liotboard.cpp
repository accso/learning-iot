/**
 *******************************************************************************
 ** Created by Manuel Schreiner
 **
 ** Copyright © 2023 Accso - Accelerated Solutions GmbH. All rights reserved.
 **
 ** 1. Redistributions of source code must retain the above copyright notice,
 **    this condition and the following disclaimer.
 **
 ** This software is provided by the copyright holder and contributors "AS IS"
 ** and any warranties related to this software are DISCLAIMED.
 ** The copyright owner or contributors be NOT LIABLE for any damages caused
 ** by use of this software.

 *******************************************************************************
 */

/**
 *******************************************************************************
 **\file liotboard.cpp
 **
 ** Learning IoT Board Support
 ** A detailed description is available at
 ** @link LIoTBoardGroup file description @endlink
 **
 ** History:
 ** - 2023-4-20  1.00  Manuel Schreiner
 *******************************************************************************
 */

#define __LIOTBOARD_CPP__

/**
 *******************************************************************************
 ** Include files
 *******************************************************************************
 */


#include <string.h> //required also for memset, memcpy, etc.
#include <stdint.h>
#include <stdbool.h>
#include "liotboard.h"
#include <Arduino.h>
#include <ArduinoJson.h>

/**
 *******************************************************************************
 ** Local pre-processor symbols/macros ('#define') 
 *******************************************************************************
 */

#define LED_RED    16 // D0
#define LED_GREEN   2 // D4
#define LED_BLUE   15 // D8
#define BUTTON      0 // D3

const char* strColors[8] = {
  "black",   //0
  "red",     //1
  "green",   //2
  "yellow",  //3
  "blue",    //4
  "purple",  //5
  "cyan",    //6
  "white"    //7
};


/**
 *******************************************************************************
 ** Global variable definitions (declared in header file with 'extern') 
 *******************************************************************************
 */

bool bLedRed = true;
bool bLedGreen = false;
bool bLedBlue = false;

bool bButtonPressed = false;
bool bWebButtonPressed = false;
bool bHwButtonPressed = false;

#if defined(ARDUINO_ARCH_ESP8266)
ESP8266WebServer* pServer;
#else
WebServer* pServer;
#endif

char acTmp[256];
StaticJsonDocument<256> doc;

/**
 *******************************************************************************
 ** Local type definitions ('typedef') 
 *******************************************************************************
 */

/**
 *******************************************************************************
 ** Local variable definitions ('static') 
 *******************************************************************************
 */

/**
 *******************************************************************************
 ** Local function prototypes ('static') 
 *******************************************************************************
 */

static void handleAPI(void);
static void InitializeBoardHW(void);
#if defined(ARDUINO_ARCH_ESP8266)
  static void InitializeRestApi(ESP8266WebServer* pWebServer);
#else
  static void InitializeRestApi(WebServer* pWebServer);
#endif

/**
 *******************************************************************************
 ** Function implementation - global ('extern') and local ('static') 
 *******************************************************************************
 */



/**
 * \brief Handle requests at /api/status
 *
 */
static void handleAPI(void) {
    static bool bWebButtonPressedOld = false;
    if (pServer->method() == HTTP_GET) {
        LIoTBoard_GetJsonStatusString(acTmp,true);
        pServer->send(200, "text/plain", acTmp);
    } else if (pServer->method() == HTTP_POST)
    {
      Serial.printf("args: %d\r\n", pServer->args());
      if (pServer->args() == 1)
      {
          if (pServer->arg(0) != NULL)
          {
              Serial.printf("arg(0): %s\r\n", pServer->arg(0));
              deserializeJson(doc, pServer->arg(0));
              if (doc.containsKey("red"))
              {
                  bLedRed = doc["red"];
              }
              if (doc.containsKey("green"))
              {
                  bLedGreen = doc["green"];
              }
              if (doc.containsKey("blue"))
              {
                  bLedBlue = doc["blue"];
              }
              if (doc.containsKey("button"))
              {
                  bWebButtonPressed = doc["button"];
                  if (bWebButtonPressedOld != bWebButtonPressed)
                  {
                     bWebButtonPressedOld = bWebButtonPressed;
                     bButtonPressed = bWebButtonPressed;
                  }
              }
              pServer->send(200, "text/plain", "ok");
          } else
          {
              pServer->send(405, "text/plain", "error");
          }
      }
    }
    else
    {
        pServer->send(405, "text/plain", "error");
    }
}

static void InitializeBoardHW(void)
{
    pinMode(LED_RED,OUTPUT);
    pinMode(LED_GREEN,OUTPUT);
    pinMode(LED_BLUE,OUTPUT);
    pinMode(BUTTON,INPUT);
}

#if defined(ARDUINO_ARCH_ESP8266)
  static void InitializeRestApi(ESP8266WebServer* pWebServer)
#else
  static void InitializeRestApi(WebServer* pWebServer)
#endif
{
    pServer = pWebServer;
    if (pServer != NULL)
    {
        pServer->on("/api/status", handleAPI);
    }
}

/**
 * \brief Initialize the Learning IoT board support module
 *
 * \param pServer Pointer to webserver object
 */
#if defined(ARDUINO_ARCH_ESP8266)
  void LIoTBoard_Init(ESP8266WebServer* pWebServer)
#else
  void LIoTBoard_Init(WebServer* pWebServer)
#endif
{
    InitializeBoardHW();
    InitializeRestApi(pWebServer);
}


void LIoTBoard_GetJsonStatusString(char* pstrOut,bool bUpdateAll)
{
    static bool bLedRedOld = false;
    static bool bLedGreenOld = false;
    static bool bLedBlueOld = false;
    static bool bButtonPressedOld = false;
    char* pStr = pstrOut;
    *pStr = '\0';
    pStr = strcat(pStr,"{\"mode\":\"status\"");
    if ((bLedRedOld != bLedRed) || bUpdateAll)
    {
        if (bLedRed)
        {
            pStr = strcat(++pStr,",\"red\":\"true\"");
        } else
        {
            pStr = strcat(++pStr,",\"red\":\"false\"");
        }
    }
    if ((bLedGreenOld != bLedGreen) || bUpdateAll)
    {
        if (bLedGreen)
        {
            pStr = strcat(++pStr,",\"green\":\"true\"");
        } else
        {
            pStr = strcat(++pStr,",\"green\":\"false\"");
        }
    }
    if ((bLedBlueOld != bLedBlue) || bUpdateAll)
    {
        if (bLedBlue)
        {
            pStr = strcat(++pStr,",\"blue\":\"true\"");
        } else
        {
            pStr = strcat(++pStr,",\"blue\":\"false\"");
        }
    }
    if ((bButtonPressedOld != bButtonPressed) || bUpdateAll)
    {
        bButtonPressedOld = bButtonPressed;
        if (bButtonPressed)
        {
            pStr = strcat(++pStr,",\"button\":\"true\"");
        } else
        {
            pStr = strcat(++pStr,",\"button\":\"false\"");
        }
    }
    pStr = strcat(++pStr,"}");
}

bool LIoTBoard_GetButtonPressed(void)
{
    /*if (bHwButtonPressed || bWebButtonPressed)
    {
        return true;
    } else
    {
        return false;
    }*/
    return bButtonPressed;
}

en_liot_board_led_color_t LIoTBoard_ColorToEnum(const char* strColor)
{
    int i;
    for(i = 0;i < 8;i++)
    {
        if (strcmp(strColor,strColors[i]) == 0)
        {
            return (en_liot_board_led_color_t)i;
        }
    }
    return LIoTBoardLedColorBlack;
}

const char* LIoTBoard_ColorEnumToString(en_liot_board_led_color_t enColor)
{
    int i = (int)enColor;
    if ((i >= 0) && (i < 8))
    {
      return strColors[i];
    }
    return "Unknown";
}


void LIoTBoard_SetLeds(bool bLedRedOnOff, bool bLedRedGreenOnOff, bool bLedBlueOnOff)
{
    bLedRed = bLedRedOnOff;
    bLedGreen = bLedRedGreenOnOff;
    bLedBlue = bLedBlueOnOff;
}

void LIoTBoard_GetLeds(bool* pbLedRedOnOff, bool* pbLedRedGreenOnOff, bool* pbLedBlueOnOff)
{
    *pbLedRedOnOff = bLedRed;
    *pbLedRedGreenOnOff = bLedGreen;
    *pbLedBlueOnOff = bLedBlue;
}

en_liot_board_led_color_t LIoTBoard_GetLedColor(void)
{
    int i = 0;
    if (bLedRed)
    {
        i += 1;
    }
    if (bLedGreen)
    {
        i += 2;
    }
    if (bLedBlue)
    {
        i += 4;
    }
    return (en_liot_board_led_color_t)i;
}

const char* LIoTBoard_GetLedColorString(void)
{
    en_liot_board_led_color_t enColor = LIoTBoard_GetLedColor();
    return LIoTBoard_ColorEnumToString(enColor);
}

void LIoTBoard_SetLedColor(en_liot_board_led_color_t enColor)
{
    int i = (int)enColor;
    if (i & 0x01)
    {
        bLedRed = true;
    } else
    {
        bLedRed = false;
    }
    if (i & 0x02)
    {
        bLedGreen = true;
    } else
    {
        bLedGreen = false;
    }
    if (i & 0x04)
    {
        bLedBlue = true;
    } else
    {
        bLedBlue = false;
    }
}

void LIoTBoard_SetLedColorString(const char* strColor)
{
    en_liot_board_led_color_t enColor = LIoTBoard_ColorToEnum(strColor);
    LIoTBoard_SetLedColor(enColor);
}

void LIoTBoard_Deinit(void)
{
}

void LIoTBoard_Update(void)
{
    static bool bLedRedOld = false;
    static bool bLedGreenOld = false;
    static bool bLedBlueOld = false;
    static bool bHwButtonPressedOld = false;
    bool tmp = digitalRead(BUTTON);
    tmp = !tmp;
    if (tmp != bHwButtonPressedOld)
    {
        bHwButtonPressed = tmp;
        bHwButtonPressedOld = bHwButtonPressed;
        bButtonPressed = bHwButtonPressed;
    }
    if (bLedRed != bLedRedOld)
    {
        bLedRedOld = bLedRed;
        digitalWrite(LED_RED,(uint8_t)bLedRed);
    }
    if (bLedGreen != bLedGreenOld)
    {
        bLedGreenOld = bLedGreen;
        digitalWrite(LED_GREEN,(uint8_t)bLedGreen);
    }
    if (bLedBlue != bLedBlueOld)
    {
        bLedBlueOld = bLedBlue;
        digitalWrite(LED_BLUE,(uint8_t)bLedBlue);
    }
}

#ifdef __cplusplus
namespace liotboard {

  bool LearningIotBoardClass::begin(void)
  {
    LIoTBoard_Init(NULL);
    return true;
  }

  #if defined(ARDUINO_ARCH_ESP8266)
    bool LearningIotBoardClass::begin(ESP8266WebServer* pWebServer)
  #else
    bool LearningIotBoardClass::begin(WebServer* pWebServer)
  #endif
  {
    LIoTBoard_Init(pWebServer);
    return true;
  }

  bool LearningIotBoardClass::end()
  {
    LIoTBoard_Deinit();
    return true;
  }

  void LearningIotBoardClass::update()
  {
    LIoTBoard_Update();
  }

  void LearningIotBoardClass::setLed(en_liot_board_led_color_t enColor)
  {
      LIoTBoard_SetLedColor( enColor);
  }

  void LearningIotBoardClass::setLed(const char* strColor)
  {
      LIoTBoard_SetLedColorString(strColor);
  }

  void LearningIotBoardClass::setLed(String strColor)
  {
      LIoTBoard_SetLedColorString(strColor.c_str());
  }

  void LearningIotBoardClass::setLed(bool bRed, bool bGreen, bool bBlue)
  {
      LIoTBoard_SetLeds(bRed,bGreen,bBlue);
  }

  void LearningIotBoardClass::setLed(bool bWhiteBlack)
  {
      LIoTBoard_SetLeds(bWhiteBlack,bWhiteBlack,bWhiteBlack);
  }

  bool LearningIotBoardClass::getButtonPressed(void)
  {
      return LIoTBoard_GetButtonPressed();
  }

  String LearningIotBoardClass::getLedColorString(void)
  {
      const char* strColor = LIoTBoard_GetLedColorString();
      return String(strColor);
  }

  en_liot_board_led_color_t LearningIotBoardClass::getLedColor(void)
  {
      return LIoTBoard_GetLedColor();
  }

}

liotboard::LearningIotBoardClass LIoTBoard;
#endif

/**
 *******************************************************************************
 ** EOF (not truncated)
 *******************************************************************************
 */

