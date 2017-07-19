/*
harvest-rogue is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

harvest-rogue is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with harvest-rogue.  If not, see <http://www.gnu.org/licenses/>.     */

#ifdef WIN32

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <cstdarg>

int vasprintf(char **strp, const char *fmt, va_list ap) {
   auto r = -1, size = _vscprintf(fmt, ap);

   if ((size >= 0) && (size < INT_MAX)) {
      *strp = static_cast<char *>(malloc(size + 1)); //+1 for null
      if (*strp) {
         r = vsnprintf(*strp, size + 1, fmt, ap);  //+1 for null
         if ((r < 0) || (r > size)) {
            free(*strp);
            *strp = nullptr;
            r = -1;
         }
      }
   } else { *strp = nullptr; }

   return(r);
}

int asprintf(char **strp, const char *fmt, ...) {
   va_list ap;
   va_start(ap, fmt);
   auto r = vasprintf(strp, fmt, ap);
   va_end(ap);
   return(r);
}


#endif