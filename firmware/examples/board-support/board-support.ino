# requires to add liotboard.h and liotboard.cpp to the project
#include "liotboard.h"

void setup(void)
{
    LIoTBoard.begin();

    LIoTBoard.setLed("red");
}

void loop(void)
{
    LIoTBoard.update();
    if (LIoTBoard.getButtonPressed())
    {
        LIoTBoard.setLed("green");
    } else
    {
        LIoTBoard.setLed("red");
    }
}