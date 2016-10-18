#ifndef RUBY_MSR_206
#define RUBY_MSR_206

#include <ruby.h>
#include <libmsr.h>

#include "msr.h"

typedef struct msr206_ctx_t
{
	int fd;
} msr206_ctx_t;

void Init_msr_msr206();

#endif
