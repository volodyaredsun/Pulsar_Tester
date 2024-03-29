/*******************************************************
This program was created by the
CodeWizardAVR V3.12 Advanced
Automatic Program Generator
© Copyright 1998-2014 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : PulsarTest
Version : 
Date    : 22.03.2016
Author  : Volodya
Company : 
Comments: 


Chip type               : ATmega8A
Program type            : Application
AVR Core Clock frequency: 8,000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 256
*******************************************************/

#include <mega8.h>
#include <alcd.h>
#include <pulsar_test.h>
#include <delay.h>
#include <stdio.h>

void main(void)
{
    InitMCU();
    //set_time_jam = 2;
    set_time_jam = e_time_jam;
    cycle = e_cycle;
    p_puls = 1;
    lcd_init(16);
    lcd_clear();
    lcd_gotoxy(0,0);
    lcd_puts(">>>PULSAR TESTER");
    lcd_gotoxy(0,1);
    lcd_puts("HNDL");
    lcd_gotoxy(5,1);
    sprintf(string,"C = %07u",cycle);
    lcd_puts(string);
    set_cycle = 0;
    c = 0;
    #asm("sei")

while (1)
      {
        ReadKey();
        ReadSensor();
        SAS();
        SM();
        PressedKey = key_none;
        ActiveSens = key_none;
        
        if(f.start){p_dwn = 1;f.start = 0;c = 1;};
        
        if(f.up)
        {
            p_up = 0;f.up = 0;
            if(set_cycle){p_dwn = 1;c = 1;}
        };
        
        if(c==0){f.jam = 0;};
        if(f.jam)
        {
            if(set_time_jam <= time_jam_min)
            {
                jam = 0;
                f.jam = 0;
                p_up = 1;
                c = 0;
                cycle++;
                lcd_gotoxy(5,1);
                sprintf(string,"C = %07u",cycle);
                lcd_puts(string);
                if(set_cycle){lcd_gotoxy(0,1);lcd_puts("AUTO");}
                else{lcd_gotoxy(0,1);lcd_puts("HNDL");};
            }
            else
            {
                p_jam = 1;
                timer_jam_on;
                if(jam >= set_time_jam)
                {
                    p_jam = 0;
                    timer_jam_off;
                    jam = 0;
                    f.jam = 0;
                    p_up = 1;
                    c = 0;
                    cycle++;
                    lcd_gotoxy(5,1);
                    sprintf(string,"C = %07u",cycle);
                    lcd_puts(string);
                    if(set_cycle){lcd_gotoxy(0,1);lcd_puts("AUTO");}
                    else{lcd_gotoxy(0,1);lcd_puts("HNDL");};
                };
            };        
        };
      }
};

void SM(void)
{
    switch(PressedKey)
    {
        case key_start:
        {
            f.start = 1;
            lcd_gotoxy(5,1);
            sprintf(string,"C = %07u",cycle);
            break;
        };
        case key_auto:
        {
            if(set_cycle){set_cycle = 0;lcd_gotoxy(0,1);lcd_puts("HNDL");}
            else
            {set_cycle = 1;lcd_gotoxy(0,1);lcd_puts("AUTO");};
            lcd_gotoxy(5,1);
            sprintf(string,"C = %07u",cycle);
            break;
        };
        case key_null:
        {
            cycle = 0;
            lcd_gotoxy(5,1);
            sprintf(string,"C = %07u",cycle);
            lcd_puts(string);
            break;
        };
        case key_stop:
        {
            p_up = p_jam = p_dwn = f.start = set_cycle = f.jam = f.up = jam = 0;
            p_puls = 1;
            timer_jam_off;
            #asm("cli");
            e_cycle = cycle;
            #asm("sei");
            lcd_gotoxy(0,1);
            lcd_puts("HNDL");
            lcd_gotoxy(5,1);
            sprintf(string,"C = %07u",cycle); 
            break;    
        };
        case key_up:
        {
            if(set_time_jam<time_jam_max){set_time_jam++;};
            lcd_gotoxy(5,1);
            sprintf(string,"J = %07u",set_time_jam);
            lcd_puts(string);
            break;
        };
        case key_dwn:
        {
            if(set_time_jam>time_jam_min){set_time_jam--;};
            lcd_gotoxy(5,1);
            sprintf(string,"J = %07u",set_time_jam);
            lcd_puts(string);
            break;
        };
        case key_jam_save:
        {
            #asm("cli");
            e_time_jam = set_time_jam;
            #asm("sei");
            lcd_gotoxy(0,1);
            lcd_puts("SAVE");
            break;
        };
    };
};

void SAS(void)
{
    switch(ActiveSens)
    {
        case key_bl1:{f.up = 1;break;};
        case key_bl2:
        {
            if(P_UP_IN){p_puls = 1;};
            if(P_DWN_IN){p_puls = 0;};
            break;
        };
        case key_bl3:
        {
            p_dwn = 0;
            f.jam = 1;
            break;
        };
        case key_bl4:
        {
            p_up = p_jam = p_dwn = f.start = set_cycle = f.jam = f.up = jam = 0;
            p_puls = 1;
            timer_jam_off;
            lcd_gotoxy(0,1);
            lcd_puts("CRSH");
            break;
        };    
    };
};

void ReadSensor(void)
{
    static unsigned char sens;                // код активного сенсора
    static unsigned int CountSens;            // счетчик активного состояния
    if(ibl1==0){sens = key_bl1;}else
    if(ibl2==0){sens = key_bl2;}else
    if(ibl3==0){sens = key_bl3;}else
    if(ibl4==0){sens = key_bl4;}
    else{sens = key_none;};
    
    if(sens)
    {
        CountSens++;
        if(CountSens == ONE_SENS){ActiveSens = sens;};
    }
    else
    {
        CountSens = 0;    
    };
};

void ReadKey(void)
{
    static unsigned char key;                  //код нажатой клавишы
    static unsigned int CountKey;              //счетчик нажатий клавиши
    static unsigned char buf_key;              //буфер еденичного нажатия
    static unsigned char f_old_key;            //флаг долгого нажатия (исключает считывание одиночного нажатия после удержания)
    if((iup==0)&&(idwn==0)){key = key_jam_save;}else
    if(istart==0){key = key_start;}else 
    if(istop==0){key = key_stop;}else 
    if(iup==0){key = key_up;}else
    if(idwn==0){key = key_dwn;}
    else{key = key_none;};
    
    if(key)
    {
        if (CountKey>=old_press)
        {
            switch (key)
            {
                case key_start:
                {
                    PressedKey = key_auto;
                    break; 
                };
                case key_stop:
                {
                    PressedKey = key_null;
                    break;
                };
                case key_jam_save:
                {
                    PressedKey = key_jam_save;
                    break;
                };
            };
            CountKey = 0;
            f_old_key = 1;      
        };
        buf_key = key;
        CountKey++;
    }
    else 
    {
        if ((CountKey>=one_press)&&(!PressedKey)&&(!f_old_key))
        {
            PressedKey = buf_key;
        };
        f_old_key = 0;
        CountKey = 0;
    };      
};

interrupt [TIM1_OVF] void timer1_ovf_isr(void)  // 0.1sec
{
    TCNT1H=0xCF2C >> 8;                         // 10Hz
    TCNT1L=0xCF2C & 0xff;
    jam++;
};

void InitMCU(void)
{
    // Function: Bit7=In Bit6=In Bit5=Out Bit4=Out Bit3=Out Bit2=Out Bit1=Out Bit0=Out 
    DDRB=(0<<DDB7) | (0<<DDB6) | (1<<DDB5) | (1<<DDB4) | (1<<DDB3) | (1<<DDB2) | (1<<DDB1) | (1<<DDB0);
    // State: Bit7=T Bit6=T Bit5=0 Bit4=0 Bit3=0 Bit2=0 Bit1=0 Bit0=0 
    PORTB=(0<<PORTB7) | (0<<PORTB6) | (0<<PORTB5) | (0<<PORTB4) | (0<<PORTB3) | (0<<PORTB2) | (0<<PORTB1) | (0<<PORTB0);

    // Function: Bit6=In Bit5=Out Bit4=Out Bit3=Out Bit2=Out Bit1=Out Bit0=Out 
    DDRC=(0<<DDC6) | (1<<DDC5) | (1<<DDC4) | (1<<DDC3) | (1<<DDC2) | (1<<DDC1) | (1<<DDC0);
    // State: Bit6=T Bit5=0 Bit4=0 Bit3=0 Bit2=0 Bit1=0 Bit0=0 
    PORTC=(0<<PORTC6) | (0<<PORTC5) | (0<<PORTC4) | (0<<PORTC3) | (0<<PORTC2) | (0<<PORTC1) | (0<<PORTC0);

    // Function: Bit7=Out Bit6=In Bit5=In Bit4=Out Bit3=Out Bit2=In Bit1=In Bit0=In 
    DDRD=(1<<DDD7) | (0<<DDD6) | (0<<DDD5) | (1<<DDD4) | (0<<DDD3) | (0<<DDD2) | (0<<DDD1) | (0<<DDD0);
    // State: Bit7=0 Bit6=T Bit5=T Bit4=0 Bit3=0 Bit2=T Bit1=T Bit0=T 
    PORTD=(0<<PORTD7) | (0<<PORTD6) | (0<<PORTD5) | (0<<PORTD4) | (0<<PORTD3) | (0<<PORTD2) | (0<<PORTD1) | (0<<PORTD0);

    // Timer/Counter 0 initialization
    // Clock source: System Clock
    // Clock value: Timer 0 Stopped
    TCCR0=(0<<CS02) | (0<<CS01) | (0<<CS00);
    TCNT0=0x00;

    // Timer/Counter 1 initialization
    // Clock source: System Clock
    // Clock value: 125,000 kHz
    // Mode: Normal top=0xFFFF
    // OC1A output: Disconnected
    // OC1B output: Disconnected
    // Noise Canceler: Off
    // Input Capture on Falling Edge
    // Timer Period: 0,1 s
    // Timer1 Overflow Interrupt: On
    // Input Capture Interrupt: Off
    // Compare A Match Interrupt: Off
    // Compare B Match Interrupt: Off
    TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
    TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (1<<CS11) | (1<<CS10);
    TCNT1H=0xCF;
    TCNT1L=0x2C;
    ICR1H=0x00;
    ICR1L=0x00;
    OCR1AH=0x00;
    OCR1AL=0x00;
    OCR1BH=0x00;
    OCR1BL=0x00;

    // Timer/Counter 2 initialization
    // Clock source: System Clock
    // Clock value: Timer2 Stopped
    // Mode: Normal top=0xFF
    // OC2 output: Disconnected
    ASSR=0<<AS2;
    TCCR2=(0<<PWM2) | (0<<COM21) | (0<<COM20) | (0<<CTC2) | (0<<CS22) | (0<<CS21) | (0<<CS20);
    TCNT2=0x00;
    OCR2=0x00;

    //отключены все таймеры
    TIMSK=(0<<OCIE2) | (0<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (0<<TOIE1) | (0<<TOIE0);

    // External Interrupt(s) initialization
    // INT0: Off
    // INT1: Off
    MCUCR=(0<<ISC11) | (0<<ISC10) | (0<<ISC01) | (0<<ISC00);

    // USART initialization
    // USART disabled
    UCSRB=(0<<RXCIE) | (0<<TXCIE) | (0<<UDRIE) | (0<<RXEN) | (0<<TXEN) | (0<<UCSZ2) | (0<<RXB8) | (0<<TXB8);

    // Analog Comparator initialization
    // Analog Comparator: Off
    // The Analog Comparator's positive input is
    // connected to the AIN0 pin
    // The Analog Comparator's negative input is
    // connected to the AIN1 pin
    ACSR=(1<<ACD) | (0<<ACBG) | (0<<ACO) | (0<<ACI) | (0<<ACIE) | (0<<ACIC) | (0<<ACIS1) | (0<<ACIS0);
    SFIOR=(0<<ACME);

    // ADC initialization
    // ADC disabled
    ADCSRA=(0<<ADEN) | (0<<ADSC) | (0<<ADFR) | (0<<ADIF) | (0<<ADIE) | (0<<ADPS2) | (0<<ADPS1) | (0<<ADPS0);

    // SPI initialization
    // SPI disabled
    SPCR=(0<<SPIE) | (0<<SPE) | (0<<DORD) | (0<<MSTR) | (0<<CPOL) | (0<<CPHA) | (0<<SPR1) | (0<<SPR0);

    // TWI initialization
    // TWI disabled
    TWCR=(0<<TWEA) | (0<<TWSTA) | (0<<TWSTO) | (0<<TWEN) | (0<<TWIE);

    // Alphanumeric LCD initialization
    // Connections are specified in the
    // Project|Configure|C Compiler|Libraries|Alphanumeric LCD menu:
    // RS - PORTD Bit 7
    // RD - PORTB Bit 0
    // EN - PORTB Bit 1
    // D4 - PORTB Bit 2
    // D5 - PORTB Bit 3
    // D6 - PORTB Bit 4
    // D7 - PORTB Bit 5
    // Characters/line: 8
};
