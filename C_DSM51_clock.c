#include <8051.h>
#define SEG P1_6
#define DIODA P1_7
#define KLAW P3_5
#define true 1
#define false 0

//#define TH0_ (65535-921)/256  //252

__xdata unsigned char * wyswietlacz = (__xdata unsigned char *) 0xFF30;
__xdata unsigned char * cyfra_wyswietlacza = (__xdata unsigned char *) 0xFF38;

__code unsigned char cyfry[10]={0b00111111, 0b00000110, 0b01011011,
0b01001111, 0b01100110, 0b01101101,0b01111101, 0b00000111, 0b01111111,
0b01101111};
__bit T0_flaga;
__bit minela_sekunda;
__code unsigned char klawisze[6]={0b000001, 0b000010, 0b000100, 0b001000, 0b010000, 0b100000};
unsigned char wcisniete_klawisze[4]={0, 0, 0, 0};       //anty odbijanie d-.-b
unsigned int timer_bufor;
unsigned char ss, mm, hh;
unsigned char tryb_edycji=0;
unsigned char ustawiany_wyswietlacz=0;
unsigned char aktywny_wyswietlacz=1;
unsigned char help=0;
unsigned char klaw_stan=0;
unsigned char klaw_poprz=0;
unsigned char i=0;
unsigned char j=0;
unsigned char czas[6]={cyfry[0], cyfry[0], cyfry[0], cyfry[0], cyfry[0], cyfry[0]};
void t0_interrupt() __interrupt(1);
void przelicz(unsigned char czas[], unsigned char ss, unsigned char mm, unsigned char hh);
void odswiezanie(void);
void obsluga_klawiszy(void);
void increment(void);
void decrement(void);
void setup_bitow(void);
void zlicz(void);

void t0_interrupt() __interrupt(1)      //funkcja totalna totaliti
{

        T0_flaga=1;
        TH0=252;

        if(aktywny_wyswietlacz==32)
	{
                aktywny_wyswietlacz=1;
        }
        else
        {
                aktywny_wyswietlacz+=aktywny_wyswietlacz;
        }

        timer_bufor++;
	if(timer_bufor>=900)
        {
		minela_sekunda=1;
	}

}


void przelicz(unsigned char czas[], unsigned char ss, unsigned char mm, unsigned char hh)
{
 	czas[5] = cyfry[hh/10];
 	czas[4] = cyfry[hh%10];
 	czas[3] = cyfry[mm/10];
 	czas[2] = cyfry[mm%10];
 	czas[1] = cyfry[ss/10];
 	czas[0] = cyfry[ss%10];
        //czas[]={cyfry[ss%10],cyfry[ss/10],cyfry[mm%10],cyfry[mm/10],cyfry[hh%10],cyfry[hh/10]};
}

void main(void)
{
        setup_bitow();
        hh=23;
        mm=59;
        ss=50;
        while(true)
        {
                if(T0_flaga==1)
                {
                        T0_flaga=0;
                        odswiezanie();
                        if(klaw_stan==klaw_poprz && !KLAW)
                        {
				klaw_stan=0;

			}
			else if(klaw_stan==0 && KLAW)
			{
                                klaw_stan=klaw_poprz;
                                obsluga_klawiszy();
                        }
                        zlicz();
                }
        }
}

void setup_bitow(void)
{

	help=0;
	aktywny_wyswietlacz=1;
        TMOD = 0b00100001;
        //TL0=TL0_;
	TH0=252;
	T0_flaga=0;
        minela_sekunda=0;
	timer_bufor=0;
	ET0=1;
	EA=1;
	TR0=1;
	DIODA=1;
}
void odswiezanie(void)
{
        if (aktywny_wyswietlacz == 0b00000001)
        {
            SEG=1;
            *wyswietlacz = 1;
            *cyfra_wyswietlacza=czas[0];
            SEG=0;
        }
        if (aktywny_wyswietlacz == 0b00000010)
        {
            SEG=1;
            *wyswietlacz = 1<<1;
            *cyfra_wyswietlacza=czas[1];
            SEG=0;
        }
        if (aktywny_wyswietlacz == 0b00000100)
        {
            SEG=1;
            *wyswietlacz = 1<<2;
            *cyfra_wyswietlacza=czas[2];
            SEG=0; 
        }
        if (aktywny_wyswietlacz == 0b00001000)
        {
            SEG=1;
            *wyswietlacz = 1<<3;
            *cyfra_wyswietlacza=czas[3];
            SEG=0;
        }
        if (aktywny_wyswietlacz == 0b00010000)
        {
            SEG=1;
            *wyswietlacz = 1<<4;
            *cyfra_wyswietlacza=czas[4];
            SEG=0;
        }
        if (aktywny_wyswietlacz == 0b00100000)
        {
            SEG=1;
            *wyswietlacz = 1<<5;
            *cyfra_wyswietlacza=czas[5];
            SEG=0;
        }
        
        klaw_poprz=aktywny_wyswietlacz;
}
void obsluga_klawiszy(void)     // lewo/prawo - zmiana pomiedzy ss/mm/hh. gura/dul - inkrement/dekrement. esc - tryb edycji. enter - zatwierdz.
{

        if(klaw_stan==1) //enter
	{
		if(!DIODA)
		{
                        DIODA=!DIODA;
                }
		tryb_edycji=0;
        }
	if(klaw_stan==2) //kropa/esc
	{
		DIODA=!DIODA;
		tryb_edycji=1;
		ustawiany_wyswietlacz=1;
        }
        if(tryb_edycji==1)
        {
		if(klaw_stan==4) //prawo
		{
			DIODA=!DIODA;
			if(ustawiany_wyswietlacz>2)
			{
                                ustawiany_wyswietlacz/=4;
                        }
                        else
                        {
                                ustawiany_wyswietlacz=1;
                        }
                }
                if(klaw_stan==32) //lewo
		{
			if(ustawiany_wyswietlacz<16)
			{
				ustawiany_wyswietlacz*=4;
                        }
			DIODA=!DIODA;
                }
		if(klaw_stan==8)  //gura
		{
                        increment();
                }
		if(klaw_stan==16) //dul
		{
                        decrement();
                }
        }

}
void zlicz(void)
{
        if(minela_sekunda==1)
        {
		timer_bufor=timer_bufor-900;
                minela_sekunda=0;

                if(tryb_edycji!=1)
    		{
                        ss++;
                        if (ss == 60) 
                        {
                                ss = 0;
                                mm++;
                        }
                        if (mm == 60)
                        {
                                mm = 0;
                                hh++;
                        }
                        if (hh == 24)
                        {
                                hh = 0;        `
                        }
                }

        }
        przelicz(czas,ss,mm,hh);
}

void increment(void)
{
        if(ustawiany_wyswietlacz==1)
		{
			ss++;
                        if(ss==60)
			{
				ss=0;
                        }
                }
                else if(ustawiany_wyswietlacz==4)
                {
		        mm++;
			if(mm==60)
			{
				mm=0;
                        }
                }
                else if(ustawiany_wyswietlacz==16)
                {
			hh++;
			if(hh==24)
			{
				hh=0;
                        }
                }
                DIODA=!DIODA;
                przelicz(czas,ss,mm,hh);
}
void decrement(void)
{
        if(ustawiany_wyswietlacz==1 || ustawiany_wyswietlacz==2)
	{
                if(ss==0)
		{
                        ss=59;
                }
                else
                {
			ss--;
                }
        }
        else if(ustawiany_wyswietlacz==4 || ustawiany_wyswietlacz==8)
        {
		if(mm==0)
		{
			mm=59;
                }
                else
		{
			mm--;
                }
        }
        else if(ustawiany_wyswietlacz==16 || ustawiany_wyswietlacz==32)
        {
		if(hh==0)
		{
			hh=23;
                }
                else
                {
			hh--;
                }
        }
        DIODA=!DIODA;
        przelicz(czas,ss,mm,hh);
}

