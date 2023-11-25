/*
   Common macros and helpers.

   Copyright (C) 2017 Andrea Stacchiotti  <andreastacchiotti(a)gmail.com>

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

#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>
#include <sys/types.h>
#include "scanmem.h"

#ifndef MIN
# define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif

/* From `include/linux/compiler.h`, in the linux kernel:
 * Offers a simple interface to the expect builtin */
#ifdef __GNUC__
# define LIKELY(x)     __builtin_expect(!!(x), 1)
# define UNLIKELY(x)   __builtin_expect(!!(x), 0)
#else
# define LIKELY(x)     (x)
# define UNLIKELY(x)   (x)
#endif

/* Use the best `getenv()` implementation we have */
#if HAVE_SECURE_GETENV
# define util_getenv secure_getenv
#else
# define util_getenv getenv
#endif

/* Function declarations */
bool sm_process_is_dead(pid_t pid);
bool sm_add_current_match_to_history();

#endif /* COMMON_H */
