#include <iarduino_OLED.h>                                 // Подключаем библиотеку iarduino_OLED.
iarduino_OLED myOLED(0x3C);                                // Объявляем объект myOLED, указывая адрес дисплея на шине I2C: 0x3C или 0x3D.
                                                           //
extern uint8_t MediumFont[];                               // Подключаем шрифт MediumFont.
uint32_t       i;                                          // Объявляем переменную для хранения времени прошедшего с момента старта скетча.
int            h, m, s;                                    // Объявляем переменную для хранения времени в часах, минутах и секундах.
                                                           //
void setup(){                                              //
    myOLED.begin();                                        // Инициируем работу с дисплеем.
    myOLED.setFont(MediumFont);                            // Указываем шрифт который требуется использовать для вывода цифр и текста.
}                                                          //
void loop(){                                               //
    i=millis();                                            // Получаем количество миллисекунд прошедшее с момента старта скетча.
    if(i%1000==0){ delay(1);                               // Выполняем скетч 1 раз в секунду. Так как условие истинно в течении 1 миллисекунды, то устанавливаем задержку в 1 миллисекунду.
        i/=1000; h=i/60/60%24; m=i/60%60; s=i%60;          // Рассчитываем часы, минуты и секунды.
                                   myOLED.setCursor(16,39);// Устанавливаем курсор в координату 16:31, это будет нижняя левая точка первой цифры выведенной функцией print().
        if(h<10){myOLED.print(0);} myOLED.print(h);        // Выводим часы прошедшие с момента старта скетча, в формате ЧЧ.
                                   myOLED.print(":");      // Выводим текст состоящий из одного символа «:»
        if(m<10){myOLED.print(0);} myOLED.print(m);        // Выводим минуты прошедшие с момента старта скетча, в формате ММ.
                                   myOLED.print(":");      // Выводим текст состоящий из одного символа «:»
        if(s<10){myOLED.print(0);} myOLED.print(s);        // Выводим секунды прошедшие с момента старта скетча, в формате СС.
    }                                                      //
}                                                          //
                                      