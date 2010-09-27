/*
** AsDevices - Helpers functions for other modules
**
**    Copyright (C) 2009-2010  The armadeus systems team
**    Fabien Marteau <fabien.marteau@armadeus.com>
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public
** License along with this library; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
//#include <fcntl.h>  /* for open()   */
#include <unistd.h> /* for write()  */

#ifdef DEBUG
#   define ERROR(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
#   define ERROR(fmt, ...) /*fmt, ##__VA_ARGS__*/
#endif

/** @brief Write an int value as string in a /sys/ or /proc interface
 *
 * @param fd: file handler
 * @param value: int value to write
 *
 * @return number of chars written, negative value on error
 */
int as_write_buffer(int fd, int value);

/** @brief read a string value in file
 *
 * @param fd: file handler
 * @param buf: char pointer to read buffer
 *
 * @return number of chars read, negative value on error
 */
int as_read_buffer(int fd, char *buf);

