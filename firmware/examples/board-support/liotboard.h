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

#if !defined(__LIOTBOARD_H__)
#define __LIOTBOARD_H__


/**
 *******************************************************************************
 ** \defgroup LIoTBoardGroup Learning IoT Board Support
 **
 ** Provided functions of LIoTBoard:
 **
 **
 *******************************************************************************
 */

//@{

/**
 *******************************************************************************
** \page liotboard_module_includes Required includes in main application
** \brief Following includes are required
** @code
** #include "liotboard.h"
** @endcode
**
 *******************************************************************************
 */

/**
 *******************************************************************************
 ** (Global) Include files
 *******************************************************************************
 */


#if defined(ARDUINO_ARCH_ESP8266)
  #include <ESP8266WebServer.h>
#elif defined(ARDUINO_ARCH_ESP32)
  #include <WebServer.h>
#endif

/**
 *******************************************************************************
 ** Global pre-processor symbols/macros ('#define') 
 *******************************************************************************
 */

/**
 *******************************************************************************
 ** Global type definitions ('typedef') 
 *******************************************************************************
 */

typedef enum en_liot_board_led_color
{
    LIoTBoardLedColorBlack = 0,
    LIoTBoardLedColorRed = 1,
    LIoTBoardLedColorGreen = 2,
    LIoTBoardLedColorBlue = 4,
    LIoTBoardLedColorYellow = 3,
    LIoTBoardLedColorCyan = 6,
    LIoTBoardLedColorPurple = 5,
    LIoTBoardLedColorWhite = 7 
} en_liot_board_led_color_t;

/**
 *******************************************************************************
 ** Global variable declarations ('extern', definition in C source)
 *******************************************************************************
 */

extern bool bLedRed;
extern bool bLedGreen;
extern bool bLedBlue;
extern bool bButtonPressed;

/**
 *******************************************************************************
 ** Global function prototypes ('extern', definition in C source) 
 *******************************************************************************
 */


#if defined(ARDUINO_ARCH_ESP8266)
  void LIoTBoard_Init(ESP8266WebServer* pWebServer);
#else
  void LIoTBoard_Init(WebServer* pWebServer);
#endif
void LIoTBoard_Deinit(void);
void LIoTBoard_Update(void);
void LIoTBoard_GetJsonStatusString(char* pstrOut,bool bUpdateAll);
bool LIoTBoard_GetButtonPressed(void);
en_liot_board_led_color_t LIoTBoard_ColorToEnum(const char* strColor);
const char* LIoTBoard_ColorEnumToString(en_liot_board_led_color_t enColor);
void LIoTBoard_SetLeds(bool bLedRedOnOff, bool bLedRedGreenOnOff, bool bLedBlueOnOff);
void LIoTBoard_GetLeds(bool* pbLedRedOnOff, bool* pbLedRedGreenOnOff, bool* pbLedBlueOnOff);
en_liot_board_led_color_t LIoTBoard_GetLedColor(void);
const char* LIoTBoard_GetLedColorString(void);
void LIoTBoard_SetLedColor(en_liot_board_led_color_t enColor);
void LIoTBoard_SetLedColorString(const char* strColor);

#ifdef __cplusplus
namespace liotboard {
  class LearningIotBoardClass
  {
  public:
    /**
    * @brief Enable board
    * @return whether success.
    */
    bool begin(void);
    
    /**
    * @brief Enable board
    * @return whether success.
    */
    #if defined(ARDUINO_ARCH_ESP8266)
      bool begin(ESP8266WebServer* pWebServer);
    #else
      bool begin(WebServer* pWebServer);
    #endif

    /**
    * @brief Disable board.
    * @return whether success.
    */
    bool end();

    void update();

    void setLed(en_liot_board_led_color_t enColor);

    void setLed(const char* strColor);

    void setLed(String strColor);

    void setLed(bool bRed, bool bGreen, bool bBlue);

    void setLed(bool bWhiteBlack);

    bool getButtonPressed(void);

    String getLedColorString(void);

    en_liot_board_led_color_t getLedColor(void);

  };
}

extern liotboard::LearningIotBoardClass LIoTBoard;



#endif

//@} // LIoTBoardGroup

#endif /* __LIOTBOARD_H__ */

/**
 *******************************************************************************
 ** EOF (not truncated)
 *******************************************************************************
 */

