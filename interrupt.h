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

#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <setjmp.h>
#include <signal.h>

extern sigjmp_buf jmpBuf; /* used when aborting a command due to an interrupt */
extern struct sigaction oldSigHandler; /* reinstalled before longjmp */
extern unsigned intrUsed;

/* signal handler used to handle an interrupt during commands */
extern struct sigaction intrSigHandler;
void interrupted(int);

/* signal handler used to handle an interrupt during scans */
extern struct sigaction intrScanSigHandler;
void interrupt_scan(int);

#define INTERRUPTABLE() ((sigaction(SIGINT, &intrSigHandler, &oldSigHandler)), intrUsed = 1, sigsetjmp(jmpBuf, 1))
#define INTERRUPTABLESCAN() ((sigaction(SIGINT, &intrScanSigHandler, &oldSigHandler)), intrUsed = 1)
#define ENDINTERRUPTABLE() (intrUsed ? ((void)sigaction(SIGINT, &oldSigHandler, NULL), intrUsed = 0) : (intrUsed = 0))

#endif /* INTERRUPT_H */
