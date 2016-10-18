#include "msr206.h"

static VALUE allocate(VALUE klass);
static VALUE deallocate(void *ctx);
static VALUE initialize(VALUE self, VALUE rb_string);


void Init_msr_msr206()
{
	VALUE cMSR_MSR206 = rb_define_class_under(mMSR, "MSR206", rb_cObject);
	rb_define_alloc_func(cMSR_MSR206, allocate);
	rb_define_method(cMSR_MSR206, "initialize", initialize, 1);
}

static VALUE allocate(VALUE klass)
{
	msr206_ctx_t *ctx = malloc(sizeof(msr206_ctx_t));

	ctx->fd = -1;

	return Data_Wrap_Struct(klass, NULL, deallocate, ctx);
}

static VALUE deallocate(void *blob)
{
	msr206_ctx_t *ctx = (msr206_ctx_t *) blob;

	if (ctx->fd > 0) {
		msr_serial_close(ctx->fd);
	}

	free(ctx);
}

static VALUE initialize(VALUE self, VALUE rb_path)
{
	msr206_ctx_t *ctx;
	char *path_str;
	int ret;

	StringValue(rb_path);
	Data_Get_Struct(self, msr206_ctx_t, ctx);

	path_str = StringValueCStr(rb_path);

	ret = msr_serial_open(path_str, &(ctx->fd), MSR_BLOCKING, MSR_BAUD);

	if (ret != LIBMSR_ERR_OK) {
		rb_raise(rb_eRuntimeError, "Couldn't open the device (%d)", ret);
	}

	ret = msr_init(ctx->fd);

	if (ret != LIBMSR_ERR_OK) {
		rb_raise(rb_eRuntimeError, "Couldn't initialize the device (%d)", ret);
	}

	return self;
}
