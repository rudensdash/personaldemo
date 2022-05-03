#include "Main.h"

//���ڴ洢���ϣ���������ԭ��ַ�Ͻ��в�������Ҫ��ջreturn��

void delay1s() {
    uchar h = 0, i = 0, j = 0, k = 0;
    for (h = 5; h > 0; h--)
        for (i = 4; i > 0; i--)
            for (j = 116; j > 0; j--)
                for (k = 39; k > 0; k--)
                    ;
}

void Timer0Init() {
    TMOD = 0x21;  // 16bit
    TH0 = 0x3C;
    TL0 = 0xB0;
    EA = 1;
    ET0 = 1;
}

// Print to Screen
void PrintData(char* Text) {
    int i = 0;
    while (Text[i] != '\0') {
        LCD_Write_Data(Text[i]);
        i++;
    }
}

// INTERRUPTION PART

void Timer0() interrupt 1 {
    S++;
    if (S == 20) {
        S = 0;
        TimerFlag = 1;
    }
    TH0 = 0x3c;
    TL0 = 0xB0;
}

void usart() interrupt 4 {
    uchar temp;
    if (RI == 1) {
        RI = 0;
        if (SBUF != '\n') {
            if ((SBUF >= '0' && SBUF <= '9') || SBUF == ':') {
                temp = SBUF;
                RcvTime[COUNT++] = temp;
                SBUF = temp;
            }
        } else {
            if (COUNT == 8) COUNT = 0;
            RI = 0;
        }
    }
    RcvTime[8] = '\0';
    UpdataTimeFlag = 1;
}

// Time Request
void timeRequest() {
    int i = 0;
    while (order[i] != '\0') {
        SBUF = order[i];
        while (!TI)
            ;
        TI = 0;
        i++;
    }
}
/*  // reverse
void reverse(char* String) {
        int len = 0;
        int  i = 0;
        char c = 0;
        if (!String)
                return;
        len = sizeof(String) / sizeof(String[0]);
        while (i <= len / 2 - 1) {
                c = *(String + i);
                *(String + i) = *(String + len - 1 - i);
                *(String + len - 1 - i) = c;
                i++;
        }

}
*/
// Number to String
char* NtoS(int num, char* str) {
    int i = 0;
    int j = 0;
    if (num < 0) {
        num = -num;
        str[i++];
    }

    do {
        str[i++] = num % 10 + '0';
        num /= 10;
    } while (num);

    str[i] = '\0';

    if (str[0] == '-') {
        j = 1;
        ++i;
    }

    for (; j < i / 2; j++) {  // �Գ�ת��
        str[j] = str[j] + str[i - j - 1];
        str[i - 1 - j] = str[j] - str[i - j - 1];
        str[j] = str[j] - str[i - 1 - j];
    }
    return str;
}
//  String to Number
int StoN(char* str) {
    char flag = '+';
    long res = 0;

    if (*str == '-') {
        ++str;
        flag = '-';
    }

    while (*str >= 48 && *str < 57) {
        res = 10 * res + *str++ - 48;
    }

    if (flag == '-') {
        res = -res;
    }

    return (int)res;
}

// Timer Format to extract number in TimeString.
void TimeFormat(char* TimeString) {
    char i = 0, j = 0;
    char tempS2[3] = {0};
    char count = 0;
    if (!TimeString) return;
    while (i < 3) {
        while (TimeString[j] != ':' && TimeString[j] != 0) {
            tempS2[count++] = TimeString[j++];
        }
        tempS2[2] = '\0';
        if (i == 0) time.hour = StoN(tempS2);
        if (i == 1) time.min = StoN(tempS2);
        if (i == 2) time.sec = StoN(tempS2);
        i++;
        j++;
        count = 0;
    }
}

// print format

void printFormat() {
    char b[4] = {0};
    LCD_Write_CMD(0x80);
    LCD_Write_CMD(0x01);
    if (time.hour < 10) {
        PrintData("0");
        PrintData(NtoS(time.hour, b));
    } else {
        PrintData(NtoS(time.hour, b));
    }
    PrintData(":");
    if (time.min < 10) {
        PrintData("0");
        PrintData(NtoS(time.min, b));
    } else {
        PrintData(NtoS(time.min, b));
    }
    PrintData(":");
    if (time.sec < 10) {
        PrintData("0");
        PrintData(NtoS(time.sec, b));
    } else {
        PrintData(NtoS(time.sec, b));
    }
}

// Calculate
void Calculate() {
    time.sec += 1;
    if (time.sec == 60) {
        time.sec = 0;
        time.min++;
        if (time.min == 60) {
            time.min = 0;
            time.hour++;
            if (time.hour == 24) time.hour = 0;
        }
    }
}

void Usart_Init() {
    TH1 = 0xF4;
    TL1 = 0XF4;
    // PCON = 0X80; // �����ʷ���
    TR1 = 1;
    SCON = 0x50;
    ES = 1;
    if (!EA) EA = 1;
}

// system Init
void system_Init() {
    PrintData("System Init..");
    delay1s();
    Timer0Init();
    TimerFlag = 0;
    Usart_Init();
    time.hour = 0;
    time.min = 0;
    time.sec = 0;
    TR0 = 1;
}

void main() {
    int i;
    LCDInit();
    system_Init();
    while (1) {
        // ��ʱ�����ڲ���
        if (TimerFlag == 1) {
            Calculate();
            TimerFlag = 0;
            printFormat();
            timeRequest();  // ÿһ�뷢��У׼ָ��
            // ����ʱ����²���
            if (UpdataTimeFlag) {
                if (RcvTime[1] != 0 && RcvTime[2] != 0 && RcvTime[3] != 0) {
                    TimeFormat(RcvTime);
                    for (i = 0; i < 9; i++) {
                        RcvTime[i] = 0;
                    }
                    UpdataTimeFlag = 0;
                }
            }
        }
    }
}