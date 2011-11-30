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
	ins->flush=1;
	bsdconv(ins);

	*length = ins->output.len;
	return ins->output.data;
}
