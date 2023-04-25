#define LED_RED    16 // D0
#define LED_GREEN   2 // D4
#define LED_BLUE   15 // D8
#define BUTTON      0 // D3

void setup(void)
{
    pinMode(LED_RED,OUTPUT);
    pinMode(LED_GREEN,OUTPUT);
    pinMode(LED_BLUE,OUTPUT);
    pinMode(BUTTON,INPUT);
}

void loop(void)
{
    // blink red LED
    digitalWrite(LED_RED,HIGH);
    delay(500);
    digitalWrite(LED_RED,LOW);
    delay(500);

    // blink green LED
    digitalWrite(LED_GREEN,HIGH);
    delay(500);
    digitalWrite(LED_GREEN,LOW);
    delay(500);

    // blink blue LED
    digitalWrite(LED_BLUE,HIGH);
    delay(500);
    digitalWrite(LED_BLUE,LOW);
    delay(500);

    // wait for button pressed (blocking)
    while(digitalRead(BUTTON))
    {
        delay(1);
    }
}