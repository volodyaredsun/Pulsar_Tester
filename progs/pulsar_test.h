
    void InitMCU(void);                             // ������������� �����������
    void ReadKey(void);                             // ����� ������
    void ReadSensor(void);                          // ����� ��������
    void SM(void);                                  // ������������� ��������
    void SAS(void);                                 // ���������� ��������
    interrupt [TIM1_OVF] void timer1_ovf_isr(void); // ������ ������� ����� [0.1sec]
//�������� ������ ������
    #define one_press       700
    #define old_press       65000
    #define ONE_SENS        200 
//������� �������
    #define istart       PINB.6
    #define istop        PINB.7
    #define iup          PIND.5
    #define idwn         PIND.6
    #define ibl1         PIND.0
    #define ibl2         PIND.1
    #define ibl3         PIND.2
    #define ibl4         PIND.3
//�������� �������
    #define p_up        PORTC.4
    #define p_dwn       PORTC.5
    #define p_jam       PORTC.2
    #define p_puls      PORTC.3
//�������� ������� ��������
    #define P_UP_IN     PINC.4
    #define P_DWN_IN    PINC.5
    unsigned char set_time_jam;     //������������� ����� ������
    #define time_jam_max    50      //������������ ����� �����
    #define time_jam_min    0       //����������� ����� �����
    unsigned char jam;              //������� ������� �����
    long int cycle;                 //���������� ������
//��������� �������� �����
    #define timer_jam_on    TIMSK|=(1<<TOIE1)
    #define timer_jam_off   TIMSK&=~(1<<TOIE1)  
    
// ��������� ������� ��������
    enum{key_none=0,key_start,key_stop,key_up,key_dwn,key_bl1,key_bl2,key_bl3,key_bl4,key_auto,key_null, key_jam_save}; 
    unsigned char PressedKey;       // ������� �������
    unsigned char ActiveSens;       // �������� ������
    unsigned char string[11];       // ������ ��� �������������� ��������� �����
   
    eeprom unsigned char e_time_jam = 2; //����� ����� � ����������������� ������
    eeprom unsigned char e_cycle;        //���������� ������ � ����������������� ������
    struct
    {
        unsigned start  : 1;         //���� ������
        unsigned jam    : 1;         //���� �����
        unsigned up     : 1;         //���� ����    
    } f;
    unsigned char c;                 //���� ������� ����������� �����
    unsigned char set_cycle;         //���� �����