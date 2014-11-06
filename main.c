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

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "rs232/rs232.h"
#include "tracer.h"

int debug=0;
int verbose=0;
int demoMode=0;

void showHelp(void) {
printf(
"-----------------------------------------------------------------------------\n"
"|          EP Solar Tracer communicator v1.0                                |\n"
"-----------------------------------------------------------------------------\n"
"--port <port>\n"
"\t0 = /dev/ttyS0   \t1 = /dev/ttyS1   \t2 = /dev/ttyS2   \n"
"\t3 = /dev/ttyS3   \t4 = /dev/ttyS4   \t5 = /dev/ttyS5   \n"
"\t6 = /dev/ttyS6   \t7 = /dev/ttyS7   \t8 = /dev/ttyS8   \n"
"\t9 = /dev/ttyS9   \t10 = /dev/ttyS10 \t11 = /dev/ttyS11 \n"
"\t12 = /dev/ttyS12 \t13 = /dev/ttyS13 \t14 = /dev/ttyS14 \n"
"\t15 = /dev/ttyS15 \t16 = /dev/ttyUSB0\t17 = /dev/ttyUSB1\n"
"\t18 = /dev/ttyUSB2\t19 = /dev/ttyUSB3\t20 = /dev/ttyUSB4\n"
"\t21 = /dev/ttyUSB5\t22 = /dev/ttyAMA0\t23 = /dev/ttyAMA1\n"
"\t24 = /dev/ttyACM0\t25 = /dev/ttyACM1\t26 = /dev/rfcomm0\n"
"\t27 = /dev/rfcomm1\t28 = /dev/ircomm0\t29 = /dev/ircomm1\n"
"--debug Do some debug stuff\n"
"--demo Fake a response from the Tracer\n"
"--verbose Talk more\n"
"--help Show this help\n"
"--baud <baudrate>\n"
"\n"
);
}

int main ( int argc, char *argv[] )
{
  int n,j,
      cport_nr=22,        /* /dev/ttyAMA0 */
      bdrate=9600;       /* 9600 baud */
  unsigned char buf[128];

  // Parse the command line options
  for (j = 1; j < argc; j++) {
	int more = j+1 < argc; // There are more arguments
	if (!strcmp(argv[j],"--port") && more) {
		cport_nr=atoi(argv[++j]);
        } else if (!strcmp(argv[j],"--baud") && more) {
                bdrate=atoi(argv[++j]);
	} else if (!strcmp(argv[j],"--debug")) {
		debug=1;
	} else if (!strcmp(argv[j],"--verbose")) {
		verbose=1;
        } else if (!strcmp(argv[j],"--demo")) {
                demoMode=1;
	} else if (!strcmp(argv[j],"--help")) {
		showHelp();
		exit(0);
	} else {
	fprintf(stderr,	"Unknown or not enough arguments for option '%s'.\n\n",	argv[j]);
	showHelp();
	exit(1);
	}
    }

  if (debug==1)
  {
    printf("Using port %i\n", cport_nr);
    printf("Baud Rate %i\n", bdrate);
  } 

  unsigned char requestString[]  = {0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xEB, 0x90, 0xEB, 0x90, 0xEB, 0x90, 0x16, 0xA0, 0x00, 0xB1, 0xA7, 0x7F};
  int sentChars;

  if (demoMode == 0)
  {
    if(RS232_OpenComport(cport_nr, bdrate))
    {
      if (debug == 1)
        printf("Can not open comport\n");
      return(0);
    }
    
    sentChars = RS232_SendBuf(cport_nr,requestString,18);
    if(sentChars<0) {
      if (debug == 1)
        printf("Can not send request\n");
      return(0);
    }
  }
  else
  {
    sentChars = 18;
  }

  if (debug == 1)
    {
      printf("Sent %i bytes: ", sentChars);
      print_hex_memory((char *)requestString, 18);
    }

  if (demoMode == 0)
  {
    usleep(100000);
    n = RS232_PollComport(cport_nr, buf, 127);
  }
  else
  {
    //Manually fake a response
    unsigned char buf2[]  = {0xEB, 0x90, 0xEB, 0x90, 0xEB, 0x90, 0x00, 0xA0, 0x18, 0xC9, 0x05, 0x36, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x4E, 0x04, 0xAE, 0x05, 0x00, 0x00, 0x00, 0x64, 0x00, 0x00, 0x01, 0x01, 0x2E, 0x5D, 0x01, 0x00, 0x21, 0xF2, 0x7F, 0x00};
    memcpy(buf, buf2, 36);
    n=36;
  }

    if(n > 0)
    {
      buf[n] = 0;   /* always put a "null" at the end of a string! */
      if (debug == 1)
      {
        printf("received %i bytes\n", n);
        print_hex_memory((char *)buf, n);
      }
      if (verbose == 1)
        process_string_verbose((char *)buf);
      else
        process_string((char *)buf);
    }


  return(0);
}
