/*
***************************************************************************
*
* Author:     John Burns
* Copyright:  (C) 2014
* Contact:    github@john.geek.nz
* License:    GNU GPLv2
*
***************************************************************************
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*
***************************************************************************
* 
* The following document was a useful reference during development:
* Protocoll-Trcaer-MT-5（111213）(1).pdf
*     SHA1:      7235ba2a7392c1597f6b727f27e9488223baa360
*     Source:    http://dl.dropbox.com/u/34579956/Protocoll-Trcaer-MT-5（111213）(1).pdf
*     Author:    alexruhmann@body-soft.de
*     Copyright: Unknown
*
***************************************************************************
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

char preamble[]  = { 0xEB, 0x90, 0xEB, 0x90, 0xEB, 0x90 };

void print_hex_memory(void *mem,int length) {
  int i;
  unsigned char *p = (unsigned char *)mem;
  for (i=0;i<length;i++) {
    printf("%02X ", p[i]);
  }
  printf("\n");
}

uint16_t tracerCRC(uint8_t *CRC_Buff,uint8_t crc_len)
{
     uint8_t crc_i,crc_j,r1,r2,r3,r4;
     uint16_t crc_result;

     r1=*CRC_Buff;
     CRC_Buff++;
     r2=*CRC_Buff;
     CRC_Buff++;
     for (crc_i=0;crc_i<crc_len-2;crc_i++)
     {
         r3=*CRC_Buff;
         CRC_Buff++;
         for (crc_j=0;crc_j<8;crc_j++)
         {
             r4=r1;
             r1=(r1<<1);
             if ((r2&0x80)!=0)r1++;
             r2=r2<<1;
             if((r3&0x80)!=0)r2++;
             r3=(r3<<1);
             if ((r4&0x80)!=0)
             {
                r1=r1^0x10;
                r2=r2^0x41;
             }
         }
     }
     crc_result=r1;
     crc_result=crc_result<<8 | r2;
     return(crc_result);
}

void process_string(char* mem)
{
  char *pch = strstr(mem, preamble);
  if (pch) {
    pch += 6;  //Jump to the end of the preamble packet

    int tracerDataLength = (int)*(pch+2);
    uint16_t theCRC = tracerCRC((uint8_t *)pch,(uint8_t)tracerDataLength + 5);
    if (theCRC == 0)
    {
      //Byte 0 is not required in non-verbose mode
      //Byte 1 is not required in non-verbose mode
      //Byte 2 is collected above in the CRC
      double tracerBatteryVoltage = (((double)*(pch+3)) + ((double)*(pch+4) * 256.0)) / 100.0;
      double tracerSolarVoltage = (((double)*(pch+5)) + ((double)*(pch+6) * 256.0)) / 100.0;
      //Byte 7 & 8 are reserved
      double tracerLoadCurrent = (((double)*(pch+9)) + ((double)*(pch+10) * 256.0)) / 100.0;
      //Byte 11 & 12 are not required in non verbose mode
      //Byte 13 & 14 are not required in non verbose mode
      int tracerLoadDetected = ((int)*(pch+15));
      int tracerOverLoaded = ((int)*(pch+16));
      int tracerShortCircuit = ((int)*(pch+17));
      //Byte 18 is reserved
      int tracerOvercharged = ((int)*(pch+19));
      int tracerBatteryVoltageTooLow = ((int)*(pch+20));
      int tracerBatteryVoltageFull = ((int)*(pch+21));
      int tracerCharging = ((int)*(pch+22));
      int tracerTemperature = ((int)*(pch+23)) - 30;
      double tracerChargeCurrent = (((double)*(pch+24)) + ((double)*(pch+25) * 256.0)) / 100.0;

      printf("%.2f,",tracerBatteryVoltage);
      printf("%.2f,",tracerSolarVoltage);
      printf("%.2f,",tracerChargeCurrent);
      printf("%.2f,",tracerLoadCurrent);
      printf("%d,",tracerTemperature);
      printf("%i,", tracerLoadDetected);
      printf("%i,", tracerOverLoaded);
      printf("%i,", tracerShortCircuit);
      printf("%i,", tracerOvercharged);
      printf("%i,", tracerBatteryVoltageTooLow);
      printf("%i,", tracerBatteryVoltageFull);
      printf("%i\n", tracerCharging);
    }
  }
}

void process_string_verbose(char* mem)
{
  char *pch = strstr(mem, preamble);
  if (pch) {
    pch += 6;  //Jump to the end of the preamble packet

    int tracerDataLength = (int)*(pch+2);
    uint16_t theCRC = tracerCRC((uint8_t *)pch,(uint8_t)tracerDataLength + 5);

    int tracerID = (int)*(pch+0);
    int tracerCommand = (int)*(pch+1);
    //Byte 2 is collected above in the CRC
    double tracerBatteryVoltage = (((double)*(pch+3)) + ((double)*(pch+4) * 256.0)) / 100.0;
    double tracerSolarVoltage = (((double)*(pch+5)) + ((double)*(pch+6) * 256.0)) / 100.0;
    //Byte 7 & 8 are reserved
    double tracerLoadCurrent = (((double)*(pch+9)) + ((double)*(pch+10) * 256.0)) / 100.0;
    double tracerODVoltage = (((double)*(pch+11)) + ((double)*(pch+12) * 256.0)) / 100.0;
    double tracerFullVoltage = (((double)*(pch+13)) + ((double)*(pch+14) * 256.0)) / 100.0;
    int tracerLoadDetected = ((int)*(pch+15));
    int tracerOverLoaded = ((int)*(pch+16));
    int tracerShortCircuit = ((int)*(pch+17));
    //Byte 18 is reserved
    int tracerOvercharged = ((int)*(pch+19));
    int tracerBatteryVoltageTooLow = ((int)*(pch+20));
    int tracerBatteryVoltageFull = ((int)*(pch+21));
    int tracerCharging = ((int)*(pch+22));
    int tracerTemperature = ((int)*(pch+23)) - 30;
    double tracerChargeCurrent = (((double)*(pch+24)) + ((double)*(pch+25) * 256.0)) / 100.0;

    printf("CRC Result*     \t%i (%s)\n",theCRC, (theCRC ==0) ? "Pass" : "FAIL!");
    printf("Tracer ID*      \t%i\n",tracerID);
    printf("Command*        \t%i\n",tracerCommand);
    printf("Length Received*\t%i\n",tracerDataLength);
    printf("Battery Voltage \t%.2f\tVolts\n",tracerBatteryVoltage);
    printf("Solar Voltage   \t%.2f\tVolts\n",tracerSolarVoltage);
    printf("Charge Current  \t%.2f\tAmps\n",tracerChargeCurrent);
    printf("Load Current    \t%.2f\tAmps\n",tracerLoadCurrent);
    printf("Over Disch V*   \t%.2f\tVolts\n",tracerODVoltage);
    printf("Full Voltage*   \t%.2f\tVolts\n",tracerFullVoltage);
    printf("Temperature     \t%d\tCentigrade\n",tracerTemperature);
    printf("Load Detected   \t%s\n", (tracerLoadDetected == 1) ? "True" : "False");
    printf("Over Loaded     \t%s\n", (tracerOverLoaded == 1) ? "True" : "False");
    printf("Short Circuit   \t%s\n", (tracerShortCircuit == 1) ? "True" : "False");
    printf("Overcharged     \t%s\n", (tracerOvercharged == 1) ? "True" : "False");
    printf("Batt V Too Low  \t%s\n", (tracerBatteryVoltageTooLow == 1) ? "True" : "False");
    printf("Battery Full    \t%s\n", (tracerBatteryVoltageFull == 1) ? "True" : "False");
    printf("Charging        \t%s\n", (tracerCharging == 1) ? "True" : "False");
    printf("\n* = Values are not provided in non-verbose mode\n");
  }
}

