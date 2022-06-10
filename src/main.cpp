#include <iarduino_OLED.h>  // Подключаем библиотеку iarduino_OLED.
iarduino_OLED myOLED(0x3C); // Объявляем объект myOLED, указывая адрес дисплея на шине I2C: 0x3C или 0x3D.
#include <EncButton.h>
#include <iarduino_RTC.h>
EncButton<EB_TICK, 2, 4, 3> enc; // энкодер с кнопкой <A, B, KEY>
iarduino_RTC watch(RTC_DS1307);  // Объявляем объект watch для работы с RTC модулем на базе чипа DS1307, используется шина I2C.

extern uint8_t MediumFont[]; // Подключаем шрифт MediumFont.
uint32_t i;                  // Объявляем переменную для хранения времени прошедшего с момента старта скетча.
int h = 0;                   // Объявляем переменную для хранения времени в часах, минутах и секундах.
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
    enc.setEncType(EB_FULLSTEP);
} //
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
    enc.tick(); //Энкодер, опрос
    SecTimer();
    if (enc.click()) //Если зажали энкодер
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
            myOLED.clrScr();
            myOLED.setCursor(0, 20);
            myOLED.print("Time");
            FlagTimeSettings = false;
            FlagTime = true;
        }
        //Настройка времени
        while (!FlagTimeSettings)
        {
            //Настройка часов
            while (FlagHourTime)
            {
                enc.tick(); //Энкодер, опрос
                //Поворот направо увеличение часов
                if (enc.right())
                {
                    Serial.println("right");
                    if (h >= 23)
                    {
                        h = 0;
                    }
                    else
                    {
                        h += 1;
                    }
                }
                //Поворот налево уменьшение часов
                if (enc.left())
                {
                    Serial.println("left");
                    if (h <= 0)
                    {
                        h = 23;
                    }
                    else
                    {
                        h -= 1;
                    }
                }
                //Вывод часов
                TimeOnline();
                //Если нажали выходим
                if (enc.press())
                {
                    Serial.println("Exit Minute");
                    FlagHourTime = false;
                }
            }

            //Настройка минут
            while (FlagMinutTime)
            {
                enc.tick(); //Энкодер, опрос
                //Поворот направо увеличение часов
                if (enc.right())
                {
                    Serial.println("right");
                    if (m >= 59)
                    {
                        h = 0;
                    }
                    else
                    {
                        h += 1;
                    }
                }
                //Поворот налево уменьшение часов
                if (enc.left())
                {
                    Serial.println("left");
                    if (h <= 0)
                    {
                        h = 59;
                    }
                    else
                    {
                        h -= 1;
                    }
                }
                //Вывод часов
                TimeOnline();
                //Если нажали выходим
                if (enc.press())
                {
                    Serial.println("Exit hour");
                    FlagMinutTime = false;
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
    if (enc.left())
        Serial.println("left"); // поворот налево
    if (enc.right())
        Serial.println("right"); // поворот направо
    if (enc.press())
        Serial.println("press");

    // if (millis() % 1000 == 0)
    // {                                                     // Если прошла 1 секунда.
    //     Serial.println(watch.gettime("d-m-Y, H:i:s, D")); // Выводим время.
    //     delay(1);                                         // Приостанавливаем скетч на 1 мс, чтоб не выводить время несколько раз за 1мс.
    // }
}
