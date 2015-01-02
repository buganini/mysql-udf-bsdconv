/*
 * Copyright (c) 2011-2014 Kuan-Chung Chiu <buganini@gmail.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <stdio.h>
#include <string.h>
#include <mysql.h>
#include <bsdconv.h>

my_bool Bsdconv_init(UDF_INIT* initid, UDF_ARGS* args, char* message){
	enum Item_result* type = args->arg_type;
	char *t;
	struct bsdconv_instance *ins;

	if(args->arg_count != 2 || type[0] != STRING_RESULT || type[1] != STRING_RESULT){
		strcpy(message,"Wrong arguments to bsdconv(conversion, text)");
		return 1;
	}

	ins = bsdconv_create((const char *)args->args[0]);
	if(ins == NULL){
		t=bsdconv_error();
		strcpy(message, "bsdconv_create() failed: ");
		strcat(message, t);
		free(t);
		return 1;
	}

	initid->ptr = (void *) ins;
	return 0;
}

void Bsdconv_deinit(UDF_INIT* initid){
	if(initid->ptr)
		bsdconv_destroy((struct bsdconv_instance *) initid->ptr);
}

char * Bsdconv(UDF_INIT *initid,
		UDF_ARGS *args, char *result, unsigned long *length,
		char *is_null, char *error __attribute__((unused))){
	struct bsdconv_instance *ins = (struct bsdconv_instance *)initid->ptr;

	bsdconv_init(ins);
	ins->output_mode=BSDCONV_AUTOMALLOC;
	ins->input.data=args->args[1];
	ins->input.len=args->lengths[1];
	ins->input.flags=0;
	ins->input.next=NULL;
	ins->flush=1;
	bsdconv(ins);

	*length = ins->output.len;
	return ins->output.data;
}
