#define CLK 2
#define DT 4
#define SW 3
#include "GyverEncoder.h"
Encoder enc1(CLK, DT, SW);  // для работы c кнопкой
#include <iarduino_OLED.h>  // Подключаем библиотеку iarduino_OLED.
iarduino_OLED myOLED(0x3C); // Объявляем объект myOLED, указывая адрес дисплея на шине I2C: 0x3C или 0x3D.
#include <iarduino_RTC.h>
iarduino_RTC watch(RTC_DS1307); // Объявляем объект watch для работы с RTC модулем на базе чипа DS1307, используется шина I2C.
extern uint8_t MediumFont[];    // Подключаем шрифт MediumFont.
uint32_t i;                     // Объявляем переменную для хранения времени прошедшего с момента старта скетча.
int h = 0;                      // Объявляем переменную для хранения времени в часах, минутах и секундах.
int m = 0;
int s = 0;
int LedNumber = 0;
bool FlagTime = true;
bool FlagTimeSettings = true;
bool FlagAlarmSettings = true;
unsigned long period_time = 800;
unsigned long sec_timer;
//Флаги настроек будильника и времени
bool FlagMinutTime = true;
bool FlagHourTime = true;
void setup()
{
    sec_timer = millis(); // "сбросить" таймер
    watch.begin();
    Serial.begin(9600);
    myOLED.begin();             // Инициируем работу с дисплеем.
    myOLED.setFont(MediumFont); // Указываем шрифт который требуется использовать для вывода цифр и текста.
    enc1.setType(TYPE1);
} //
//Вывод часов
void WriteH(int hour)
{
    myOLED.setCursor(16, 39); // Устанавливаем курсор в координату 16:31, это будет нижняя левая точка первой цифры выведенной функцией print().
    if (hour < 10)
    {
        myOLED.print(0);
    }
    myOLED.print(hour);
}
void WriteM(int minut)
{
    myOLED.setCursor(51, 39); // Устанавливаем курсор в координату 16:31, это будет нижняя левая точка первой цифры выведенной функцией print().
    if (minut < 10)
    {
        myOLED.print(0);
    }
    myOLED.print(minut);
}
//Вывод минут
//п.п секунды
void SecTimer()
{
    if (millis() - sec_timer >= period_time)
    {
        sec_timer = millis(); // "сбросить" таймер
        s += 1;               // дейтвия, которые хотим выполнить один раз за период
    }
}
//п.п расчета времени
void TimeOnline()
{
    //Формируем часы, минуты, секунды
    if (s >= 59)
    {
        if (m >= 59)
        {
            if (h >= 23)
            {
                h = 0;
                m = 0;
                s = 0;
            }
            else
            {
                h += 1;
                m = 0;
            }
        }
        else
        {
            m += 1;
            s = 0;
        }
    }
    //Выводим на экран

    myOLED.setCursor(16, 39); // Устанавливаем курсор в координату 16:31, это будет нижняя левая точка первой цифры выведенной функцией print().
    if (h < 10)
    {
        myOLED.print(0);
    }
    myOLED.print(h);   // Выводим часы прошедшие с момента старта скетча, в формате ЧЧ.
    myOLED.print(":"); // Выводим текст состоящий из одного символа «:»
    if (m < 10)
    {
        myOLED.print(0);
    }
    myOLED.print(m);   // Выводим минуты прошедшие с момента старта скетча, в формате ММ.
    myOLED.print(":"); // Выводим текст состоящий из одного символа «:»
    if (s < 10)
    {
        myOLED.print(0);
    }
    myOLED.print(s); // Выводим секунды прошедшие с момента старта скетча, в формате СС.
}
void loop()
{
    enc1.tick();
    SecTimer();
    if (enc1.isClick()) //Если зажали энкодер
    {

        if (LedNumber < 2)
        {
            LedNumber += 1;
        }
        else
        {
            LedNumber = 0;
        }
        Serial.print("LedNumber=");
        Serial.println(LedNumber);
    }
    //Переходы по меню
    switch (LedNumber)
    {
    case 0:
        if (FlagTime)
        {
            myOLED.clrScr();
            FlagTime = false;
            FlagTimeSettings = true;
            FlagAlarmSettings = true;
        }
        TimeOnline(); //Расчет времени
        break;
    case 1:
        //Настраиваем время
        //Выводим надпись Settings Time
        if (FlagTimeSettings)
        {
            enc1.tick();
            myOLED.clrScr();
            myOLED.setCursor(0, 20);
            myOLED.print("Time");
            FlagTimeSettings = false;
            FlagTime = true;
            TimeOnline();
        }
        //Настройка времени
        while (!FlagTimeSettings)
        {
            enc1.tick();
            //Настройка часов
            while (FlagHourTime)
            {
                enc1.tick(); //Энкодер, опрос
                //Поворот направо увеличение часов
                if (enc1.isRight())
                {
                    // Serial.println("right");
                    if (h >= 23)
                    {
                        h = 0;
                    }
                    else
                    {
                        h += 1;
                    }
                    WriteH(h);
                }
                //Поворот налево уменьшение часов
                if (enc1.isLeft())
                {
                    // Serial.println("left");
                    if (h <= 0)
                    {
                        h = 23;
                    }
                    else
                    {
                        h -= 1;
                    }
                    WriteH(h);
                }
                //Вывод часов

                enc1.tick();
                // TimeOnline();
                //Если нажали выходим
                if (enc1.isClick())
                {
                    // Serial.println("Exit Hour");
                    FlagHourTime = false;
                }
            }

            //Настройка минут
            while (FlagMinutTime)
            {
                enc1.tick(); //Энкодер, опрос
                //Поворот направо увеличение часов
                if (enc1.isRight())
                {
                    // Serial.println("right");
                    if (m >= 59)
                    {
                        m = 0;
                    }
                    else
                    {
                        m += 1;
                    }
                    WriteM(m);
                }
                //Поворот налево уменьшение часов
                if (enc1.isLeft())
                {
                    // Serial.println("left");
                    if (m <= 0)
                    {
                        m = 59;
                    }
                    else
                    {
                        m -= 1;
                    }
                    WriteM(m);
                }
                //Вывод часов
                // TimeOnline();
                //Если нажали выходим
                enc1.tick();
                if (enc1.isClick())
                {
                    // Serial.println("Exit hour");
                    FlagMinutTime = false;
                    FlagTimeSettings = true;
                    FlagHourTime = true;
                }
            }

            LedNumber += 1;
            FlagTimeSettings = true;
        }

        break;
    case 2:
        //Настраиваем будильник
        if (FlagAlarmSettings)
        {
            myOLED.clrScr();
            myOLED.setCursor(0, 20);
            myOLED.print("Alarm");
            FlagAlarmSettings = false;
            FlagTime = true;
        }

        break;
    }

    //Энкодер
    // if (enc1.isLeft())
    //     Serial.println("left"); // поворот налево
    // if (enc1.isRight())
    //     Serial.println("right"); // поворот направо
    // if (enc1.isClick())
    //     Serial.println("press");

    // if (millis() % 1000 == 0)
    // {                                                     // Если прошла 1 секунда.
    //     Serial.println(watch.gettime("d-m-Y, H:i:s, D")); // Выводим время.
    //     delay(1);                                         // Приостанавливаем скетч на 1 мс, чтоб не выводить время несколько раз за 1мс.
    // }
}
