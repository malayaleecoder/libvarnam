/* varnam.c

Copyright (C) 2010 Navaneeth.K.N

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <string.h>

#include "varnam-types.h"
#include "varnam-util.h"
#include "varnam-result-codes.h"

int varnam_init(const char *symbols_file, size_t file_length, varnam **handle, char **msg)
{
    int rc;
    varnam *c;
    struct varnam_internal *vi;

    if(symbols_file == NULL || file_length <= 0)
        return VARNAM_MISUSE;

    c = (varnam *) xmalloc(sizeof (varnam));
    if(!c) 
        return VARNAM_MEMORY_ERROR;

    vi = (struct varnam_internal *) xmalloc(sizeof (struct varnam_internal));
    if(!vi)
        return VARNAM_MEMORY_ERROR;

    vi->message = (char *) xmalloc(sizeof (char) * VARNAM_LIB_TEMP_BUFFER_SIZE);
    if(!vi->message)
        return VARNAM_MEMORY_ERROR;

    c->internal = vi;

    rc = sqlite3_open(symbols_file, &vi->db);
    if( rc ) {
        asprintf(msg, "Can't open %s: %s\n", symbols_file, sqlite3_errmsg(vi->db));
        sqlite3_close(vi->db);
        return VARNAM_ERROR;
    }

    c->symbols_file = (char *) xmalloc(file_length + 1);
    if(!c->symbols_file)
        return VARNAM_MEMORY_ERROR;

    strncpy(c->symbols_file, symbols_file, file_length + 1);
    handle = &c;
    return VARNAM_SUCCESS;
}

