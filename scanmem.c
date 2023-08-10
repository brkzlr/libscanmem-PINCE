/*
   Provide interfaces for front-ends.

   Copyright (C) 2006,2007,2009 Tavis Ormandy <taviso@sdf.lonestar.org>
   Copyright (C) 2009           Eli Dupree <elidupree@charter.net>
   Copyright (C) 2009-2013      WANG Lu <coolwanglu@gmail.com>
   Copyright (C) 2016           Sebastian Parschauer <s.parschauer@gmx.de>

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

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include "config.h"

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>

#include "scanmem.h"
#include "commands.h"
#include "handlers.h"
#include "show_message.h"

/* global settings */
globals_t sm_globals = {
	0,                          /* exit flag */
	0,                          /* pid target */
	NULL,                       /* matches */
	NULL,                       /* current */
	TAILQ_HEAD_INITIALIZER(sm_globals.match_history), /* history */
	0,                          /* history_length */
	0,                          /* match count */
	0,                          /* scan progress */
	false,                      /* stop flag */
	NULL,                       /* regions */
	NULL,                       /* commands */
	NULL,                       /* current_cmdline */
	/* options */
	{
		1,                      /* alignment */
		0,                      /* debug */
		ANYINTEGER,             /* scan_data_type */
		REGION_HEAP_STACK_EXECUTABLE_BSS, /* region_detail_level */
		1,                      /* dump_with_ascii */
		0,                      /* reverse_endianness */
	}
};

/* signal handler - use async-signal safe functions ONLY! */
static void sighandler(int n){
	const char err_msg[] = "error: \nKilled by signal ";
	const char msg_end[] = ".\n";
	char num_str[4] = {0};
	ssize_t num_size;
	ssize_t wbytes;

	wbytes = write(STDERR_FILENO, err_msg, sizeof(err_msg) - 1);
	if (wbytes != sizeof(err_msg) - 1)
		goto out;
	/* manual int to str conversion */
	if (n < 10) {
		num_str[0] = (char) (0x30 + n);
		num_size = 1;
	}
	else if (n >= 100) {
		goto out;
	}
	else {
		num_str[0] = (char) (0x30 + n / 10);
		num_str[1] = (char) (0x30 + n % 10);
		num_size = 2;
	}
	wbytes = write(STDERR_FILENO, num_str, num_size);
	if (wbytes != num_size)
		goto out;
	wbytes = write(STDERR_FILENO, msg_end, sizeof(msg_end) - 1);
	if (wbytes != sizeof(msg_end) - 1)
		goto out;
out:
	_exit(EXIT_FAILURE);   /* also detaches from tracee */
}

/* scanmem general */
bool sm_init(void){
	globals_t *vars = &sm_globals;
	TAILQ_INIT(&vars->match_history);
	/* before attaching to target, install signal handler to detach on error */
	if (vars->options.debug == 0){ /* in debug mode, let it crash and see the core dump */
		(void) signal(SIGHUP, sighandler);
		(void) signal(SIGINT, sighandler);
		(void) signal(SIGSEGV, sighandler);
		(void) signal(SIGABRT, sighandler);
		(void) signal(SIGILL, sighandler);
		(void) signal(SIGFPE, sighandler);
		(void) signal(SIGTERM, sighandler);
	}

	/* linked list of commands and function pointers to their handlers */
	if ((vars->commands = l_init()) == NULL) {
		show_error("sorry, there was a memory allocation error.\n");
		return false;
	}

	/* NULL shortdoc means don't display this command in `help` listing */
	sm_registercommand("set", handler__set, vars->commands, SET_SHRTDOC,
			SET_LONGDOC, NULL);
	sm_registercommand("list", handler__list, vars->commands, LIST_SHRTDOC,
			LIST_LONGDOC, NULL);
	sm_registercommand("delete", handler__delete, vars->commands, DELETE_SHRTDOC,
			DELETE_LONGDOC, NULL);
	sm_registercommand("snapshot", handler__snapshot, vars->commands,
			SNAPSHOT_SHRTDOC, SNAPSHOT_LONGDOC, NULL);
	sm_registercommand("dregion", handler__dregion, vars->commands,
			DREGION_SHRTDOC, DREGION_LONGDOC, NULL);
	sm_registercommand("dregions", handler__dregion, vars->commands,
			NULL, DREGION_LONGDOC, NULL);
	sm_registercommand("lregions", handler__lregions, vars->commands,
			LREGIONS_SHRTDOC, LREGIONS_LONGDOC, NULL);
	sm_registercommand("=", handler__operators, vars->commands, NOTCHANGED_SHRTDOC,
			NOTCHANGED_LONGDOC, NULL);
	sm_registercommand("!=", handler__operators, vars->commands, CHANGED_SHRTDOC,
			CHANGED_LONGDOC, NULL);
	sm_registercommand("<", handler__operators, vars->commands, LESSTHAN_SHRTDOC,
			LESSTHAN_LONGDOC, NULL);
	sm_registercommand(">", handler__operators, vars->commands, GREATERTHAN_SHRTDOC,
			GREATERTHAN_LONGDOC, NULL);
	sm_registercommand("+", handler__operators, vars->commands, INCREASED_SHRTDOC,
			INCREASED_LONGDOC, NULL);
	sm_registercommand("-", handler__operators, vars->commands, DECREASED_SHRTDOC,
			DECREASED_LONGDOC, NULL);
	sm_registercommand("\"", handler__string, vars->commands, STRING_SHRTDOC,
			STRING_LONGDOC, NULL);
	sm_registercommand("update", handler__update, vars->commands, UPDATE_SHRTDOC,
			UPDATE_LONGDOC, NULL);
	sm_registercommand("exit", handler__exit, vars->commands, EXIT_SHRTDOC,
			EXIT_LONGDOC, NULL);
	sm_registercommand("quit", handler__exit, vars->commands, NULL,
			EXIT_LONGDOC, NULL);
	sm_registercommand("q", handler__exit, vars->commands, NULL,
			EXIT_LONGDOC, NULL);
	sm_registercommand("shell", handler__shell, vars->commands, SHELL_SHRTDOC,
			SHELL_LONGDOC, NULL);
	sm_registercommand("!", handler__shell, vars->commands, NULL, SHELL_LONGDOC,
			NULL);
	sm_registercommand("watch", handler__watch, vars->commands, WATCH_SHRTDOC,
			WATCH_LONGDOC, NULL);
	sm_registercommand("dump", handler__dump, vars->commands, DUMP_SHRTDOC,
			DUMP_LONGDOC, NULL);
	sm_registercommand("write", handler__write, vars->commands, WRITE_SHRTDOC,
			WRITE_LONGDOC, WRITE_COMPLETE);
	sm_registercommand("option", handler__option, vars->commands, OPTION_SHRTDOC,
			OPTION_LONGDOC, OPTION_COMPLETE);
	/* commands beginning with __ have special meaning */
	sm_registercommand("__eof", handler__eof, vars->commands, NULL, NULL, NULL);

	/* special value NULL means no other matches */
	sm_registercommand(NULL, handler__default, vars->commands, DEFAULT_SHRTDOC,
			DEFAULT_LONGDOC, NULL);

	return true;
}

void sm_cleanup(void){
	/* free any allocated memory used */
	l_destroy(sm_globals.regions);
	if (sm_globals.commands)
		sm_free_all_completions(sm_globals.commands);
	l_destroy(sm_globals.commands);

	/* free matches array */
	if (sm_globals.matches)
		free(sm_globals.matches);

	if(!TAILQ_EMPTY(&sm_globals.match_history)) {
		struct history_entry_t *next = NULL, *iterator = TAILQ_FIRST(&sm_globals.match_history);
		while(iterator != NULL) {
			next = TAILQ_NEXT(iterator, list);
			free(iterator->matches);
			free(iterator);
			iterator = next;    
		}

	}
	/* attempt to detach just in case */
	sm_detach(sm_globals.target);
}

void sm_backend_exec_cmd(const char *commandline){
	sm_execcommand(&sm_globals, commandline);
	fflush(stdout);
	fflush(stderr);
}

unsigned long sm_get_num_matches(void){
	return sm_globals.num_matches;
}

double sm_get_scan_progress(void){
	return sm_globals.scan_progress;
}

void sm_set_stop_flag(bool stop_flag){
	sm_globals.stop_flag = stop_flag;
}

/* scanmem commands */
bool sm_cmd_pid(unsigned long int pid){
	if (pid != 0) {
		sm_globals.target = (pid_t)pid;
		if (sm_globals.target == 0) {
			show_error("`%s` does not look like a valid pid.\n", pid);
			return false;
		}
	}
	else if (sm_globals.target) {
		/* print the pid of the target program */
		show_info("target pid is %u.\n", sm_globals.target);
		return true;
	}
	else {
		show_info("no target is currently set.\n");
		return false;
	}

	return sm_cmd_reset();
}

bool sm_cmd_reset(void){
	/* reset scan progress */
	sm_globals.scan_progress = 0;
	if (sm_globals.matches){
		free(sm_globals.matches);
		sm_globals.matches = NULL;
		sm_globals.num_matches = 0;
	}

	/* refresh list of regions */
	l_destroy(sm_globals.regions);

	/* create a new linked list of regions */
	if ((sm_globals.regions = l_init()) == NULL) {
		show_error("sorry, there was a problem allocating memory.\n");
		return false;
	}

	/* read in maps if a pid is known */
	if (sm_globals.target && sm_readmaps(sm_globals.target, sm_globals.regions, sm_globals.options.region_scan_level) != true) {
		show_error("sorry, there was a problem getting a list of regions to search.\n");
		show_warn("the pid may be invalid, or you don't have permission.\n");
		sm_globals.target = 0;
		return false;
	}

	/* reset history */
	struct history_entry_t *next = NULL, *cur = TAILQ_FIRST(&sm_globals.match_history);
	while(cur != NULL) {
		next = TAILQ_NEXT(cur, list);
		free(cur->matches);
		free(cur);
		cur = next;
	}
	TAILQ_INIT(&sm_globals.match_history);
	return true;
}

bool sm_cmd_undo(void){
	if(TAILQ_EMPTY(&sm_globals.match_history)) {
		show_info("nothing to undo\n");
		return true;
	}

	if (sm_globals.current != NULL) {
		struct history_entry_t *previous = TAILQ_PREV(sm_globals.current, history_list_t, list);

		if(previous != NULL) {
			matches_and_old_values_array *tmp = realloc(sm_globals.matches,
					previous->matches->bytes_allocated);
			if(tmp != NULL) {
				sm_globals.matches = tmp;
			}
			else {
				show_error("could not allocate memory\n");
				return false;
			}
			sm_globals.current = previous;
			/* copy since the array would be resized by null_terminate which would
			 * cause use after free at some point */ 
			memcpy(sm_globals.matches, previous->matches, previous->matches->bytes_allocated);
			sm_globals.num_matches = previous->num_matches;
		}
		else {
			show_info("already at first entry in history\n");
		}
	}

	return true;
}

bool sm_cmd_redo(void){
	if(sm_globals.current == NULL) {
		show_info("nothing to redo\n");
		return true;
	}

	struct history_entry_t *next = TAILQ_NEXT(sm_globals.current, list);
	if (next == NULL) {
		show_info("already at last entry in history\n");
		return true;
	}

	matches_and_old_values_array *tmp = realloc(sm_globals.matches, next->matches->bytes_allocated);
	if(tmp == NULL) {
		show_error("could not allocate memory\n");
		return false;
	}

	sm_globals.current = next;
	sm_globals.matches = tmp;
	/* copy since the array would be resized by null_terminate which would
	 * cause use after free at some point */ 
	memcpy(sm_globals.matches, next->matches, next->matches->bytes_allocated);
	sm_globals.num_matches = next->num_matches;
	return true;
}

