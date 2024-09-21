/*
   Interrupt handling.

   This file is part of libscanmem.

   This library is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published
   by the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this library.  If not, see <http://www.gnu.org/licenses/>.
   */

#include "interrupt.h"
#include "scanmem.h"

sigjmp_buf jmpBuf; /* used when aborting a command due to an interrupt */
struct sigaction oldSigHandler; /* reinstalled before longjmp */
unsigned intrUsed;

/* signal handler used to handle an interrupt during commands */
struct sigaction intrSigHandler = { .sa_handler = interrupted };
void interrupted(int n)
{
	(void)n;
	siglongjmp(jmpBuf, 1);
}

/* signal handler used to handle an interrupt during scans */
struct sigaction intrScanSigHandler = { .sa_handler = interrupt_scan };
void interrupt_scan(int n)
{
	(void)n;
	sm_set_stop_flag(true);
}
