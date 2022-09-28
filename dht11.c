#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#define MAXTIMINGS 85
#define DHTPIN 7
int dht11_dat[5] = { 0, 0, 0, 0, 0 };

void read_dht11_dat(){

    uint8_t laststate = HIGH;
    uint8_t counter = 0;
    uint8_t j = 0, i;
    float f;
    dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;
    pinMode( DHTPIN, OUTPUT );
    digitalWrite( DHTPIN, LOW );
    delay( 18 );
    digitalWrite( DHTPIN, HIGH );
    delayMicroseconds( 40 );
    pinMode( DHTPIN, INPUT );
    
    for ( i = 0; i < MAXTIMINGS; i++ ){
        counter = 0;
        while ( digitalRead( DHTPIN ) == laststate ){
            counter++;
            delayMicroseconds( 1 );
            if ( counter == 255 ){
                break;
            }
        }	
        laststate = digitalRead( DHTPIN );
        if(counter==255){
            break;
        }
        if ( (i >= 4) && (i % 2 == 0) ){
            dht11_dat[j / 8] <<= 1;
            if ( counter > 50 )
            dht11_dat[j / 8] |= 1;
            j++;
        }
    }
    
    if ( (j >= 40) && (dht11_dat[4] == ( (dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF) ) ){

        char num1[2];
        num1[0] = (dht11_dat[2] / 10)+'0';
        num1[1] = (dht11_dat[2] % 10)+'0';


        char num[2];
        num[0] = (dht11_dat[0] / 10)+'0';
        num[1] = (dht11_dat[0] % 10)+'0';
        printf("%s",num);

        char command[500];
        strcpy(command,"echo -e \"Subject: Test Mail\r\n\r\nDie Feuchtigkeit 
        ist: ");
        strncat(command,num,2);
        strcat(command,"% .\nDie Temparatur: ");
        strncat(command,num1,2);
        strcat(command," C.\nDie Feuchtigkeit ist mehr als 50%.
        Bitte lassen Sie Ihren Garten die Sonne sehen! \" |msmtp
        --debug --from=default -t bahceaklli@gmail.com");

        char command1[500];
        strcpy(command1,"echo -e \"Subject: Test Mail\r\n\r\nDie
        Feuchtigkeit ist: ");
        strncat(command1,num,2);
        strcat(command1,"% .\nDie Temparatur: ");
        strncat(command1,num1,2);
        strcat(command1," C.\nDie Feuchtigkeit ist weniger als
        50%. Bitte bewaessern Sie Ihren Garten! \" |msmtp --debug
        --from=default -tbahceaklli@gmail.com");

        char command2[500];
        strcpy(command2,"echo -e \"Subject: Test Mail\r\n\r\nDie
        Feuchtigkeit ist: ");
        strncat(command2,num,2);
        strcat(command2,"% .\nDie Temparatur: ");
        strncat(command2,num1,2);
        strcat(command2," C.\nTemparatur ist ueber 28 Grad. Bitte
        kuehlen Sie Ihren Garten!  \" |msmtp --debug --from=default
        -t bahceaklli@gmail.com");

        char command3[500];
        strcpy(command3,"echo -e \"Subject: Test Mail\r\n\r\nDie
        Feuchtigkeit ist: ");
        strncat(command3,num,2);
        strcat(command3,"% .\nDie Temparatur: ");
        strncat(command3,num1,2);
        strcat(command3," C.\nTemparatur ist unter 21 Grad. Bitte
        halten Sie Ihren Garten warm!\" |msmtp --debug
        --from=default -t bahceaklli@gmail.com");

        char command4[500];
        strcpy(command4,"echo -e \"Subject: Test Mail\r\n\r\nDie
        Feuchtigkeit ist: ");
        strncat(command4,num,2);
        strcat(command4,"% .\nDie Temparatur: ");
        strncat(command4,num1,2);
        strcat(command4," C.\nDie Werte sind  normal. \" |msmtp
        --debug --from=default -t bahceaklli@gmail.com");

        f = dht11_dat[2] * 9. / 5. + 32;

        printf( "Humidity = %d.%d %% Temperature = %d.%d C (%.1f
        F)\n", dht11_dat[0], dht11_dat[1], dht11_dat[2],
        dht11_dat[3], f );

        if(dht11_dat[0]>75){ system(command);}
        if(dht11_dat[0]<50){ system(command1);}
        if(dht11_dat[2]>28) {system(command2);}
        if(dht11_dat[2]<21) { system(command3);}
        else{system(command4);}

    }else  {printf( "Data not good, skip\n" );}

}
int main( void ){

    printf( "Raspberry Pi wiringPi DHT11 Temperature test program\n" );
    int counter=0;
    if ( wiringPiSetup() == -1 ) exit( 1 );
    while ( 1 ){
        read_dht11_dat();
        delay( 2000 );
        counter++;
        if(counter==5) return 0;
    }
    
    return(0);
}
