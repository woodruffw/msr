#ifndef RUBY_MSR_206
#define RUBY_MSR_206

extern VALUE cMSR_MSR206;

typedef struct
{
	int fd;
} msr206_ctx_t;

void Init_msr_msr206();

#endif
