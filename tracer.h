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

#include <stdint.h>

void print_hex_memory(void *mem,int length);
void process_string(char* mem);
void process_string_verbose(char* mem);
uint16_t tracerCRC(uint8_t *CRC_Buff,uint8_t crc_len);
