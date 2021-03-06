#include <iostream>
#include <string.h>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include "crc.hpp"


#define NUM_SIMULATIONS      2000
#define NUMBER_OF_CRC_CHECKS 5

#define  printDef(crc,g,howManyErrors,prob) \
printf("Detection probability for 0x%x (g=%d) with %d error:%.3f\n",(crc),(g),(howManyErrors),(prob))

//The message
 unsigned char msg[60]={0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xbc, 0x14,
                        0x85, 0xaf, 0xcf, 0x63, 0x08, 0x06, 0x00, 0x01,
 					   0x08, 0x00, 0x06, 0x04, 0x00, 0x01, 0xbc, 0x14,
 					   0x85, 0xaf, 0xcf, 0x63, 0x84, 0x44, 0x2e, 0x19,
 					   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x84, 0x44,
 					   0x2e, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 					   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 					   0x00, 0x00, 0x00, 0x00};



unsigned int CRC1=0x1; //g=1
unsigned int CRC64=0x0; //g=64
unsigned int CRC15=0x4001; //g=15
unsigned int CRC16=0xbb3d; //g=16
unsigned int CRC32=0x04c11db7; //g=32

int findDeg(unsigned char* buff,int maxLen){
    for(int i=maxLen*8-1;i>=0;i--){
        if((buff[i/8]&(1<<(i%8)))>>i%8){
            return i;
        }
    }
    return -1;
}
int main(){
    unsigned char buff[100];
    unsigned int crc[5] = {CRC1,CRC64,CRC15,CRC16,CRC32};
    int crcDegree[5] = {1,64,15,16,32};

    for(int i=0;i<NUMBER_OF_CRC_CHECKS;i++){
        double detectedOneError  = 0;
        double detectedTwoErrors = 0;
        memset(buff,0,100);
        encode(msg, 60, crc[i], crcDegree[i] ,buff);

        int deg = findDeg(buff,60+ceil(((double)crcDegree[i])/8));
        for(int j=0;j<NUM_SIMULATIONS;j++){
            int firstRand  = rand() % deg;
            int secondRand = rand() % deg;
            while (secondRand == firstRand) secondRand = rand() % deg;
            buff[firstRand/8] = buff[firstRand/8] ^ (1<<(firstRand%8)); //adding first error
            if(!validate(buff, 60+ceil(((double)crcDegree[i])/8), crc[i], crcDegree[i])){
                detectedOneError++;
            }
            buff[secondRand/8] = buff[secondRand/8] ^ (1<<(secondRand%8)); //adding second error
            if(!validate(buff, 60+ceil(((double)crcDegree[i])/8), crc[i], crcDegree[i])){
                detectedTwoErrors++;
            }
            buff[firstRand/8]  = buff[firstRand/8] ^ (1<<(firstRand%8)); //removing first error
            buff[secondRand/8] = buff[secondRand/8] ^ (1<<(secondRand%8)); //removing second error
        }
        printDef(crc[i],crcDegree[i],1,(detectedOneError/NUM_SIMULATIONS));
        printDef(crc[i],crcDegree[i],2,(detectedTwoErrors/NUM_SIMULATIONS));

     }
	return 0;
}
