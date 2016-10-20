#include "msr.h"

VALUE cMSR_MSR206 = Qnil;

static VALUE allocate(VALUE klass);
static void deallocate_206_ctx(msr206_ctx_t *ctx);
static VALUE initialize(VALUE self, VALUE rb_string);
static VALUE set_coercivity(VALUE self, VALUE co_sym);
static VALUE raw_read(VALUE self);
static VALUE iso_read(VALUE self);

void Init_msr_msr206()
{
	cMSR_MSR206 = rb_define_class_under(mMSR, "MSR206", rb_cObject);
	rb_define_alloc_func(cMSR_MSR206, allocate);
	rb_define_method(cMSR_MSR206, "initialize", initialize, 1);
	rb_define_method(cMSR_MSR206, "coercivity=", set_coercivity, 1);
	rb_define_method(cMSR_MSR206, "raw_read", raw_read, 0);
	rb_define_method(cMSR_MSR206, "iso_read", iso_read, 0);
}

static VALUE allocate(VALUE klass)
{
	msr206_ctx_t *ctx = malloc(sizeof(msr206_ctx_t));

	ctx->fd = -1;

	return Data_Wrap_Struct(klass, NULL, deallocate_206_ctx, ctx);
}

static void deallocate_206_ctx(msr206_ctx_t *ctx)
{
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

	Check_Type(rb_path, T_STRING);
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

static VALUE set_coercivity(VALUE self, VALUE co_sym)
{
	VALUE hi_co = ID2SYM(rb_intern("hi"));
	VALUE lo_co = ID2SYM(rb_intern("lo"));
	msr206_ctx_t *ctx;
	int ret;

	Check_Type(co_sym, T_SYMBOL);
	Data_Get_Struct(self, msr206_ctx_t, ctx);

	if (co_sym == hi_co) {
		ret = msr_set_hi_co(ctx->fd);
	}
	else if (co_sym == lo_co) {
		ret = msr_set_lo_co(ctx->fd);
	}
	else {
		rb_raise(rb_eArgError, "Unknown coercivity level");
	}

	if (ret != LIBMSR_ERR_OK) {
		rb_raise(rb_eRuntimeError, "Couldn't change coercivity (%d)", ret);
	}

	return self;
}

static VALUE raw_read(VALUE self)
{
	msr206_ctx_t *ctx;
	msr_tracks_t tks = {0};
	int ret;
	VALUE tks_obj;

	Data_Get_Struct(self, msr206_ctx_t, ctx);

	for (int i = 0; i < MSR_MAX_TRACKS; i++) {
		tks.msr_tracks[i].msr_tk_len = MSR_MAX_TRACK_LEN;
	}

	ret = msr_raw_read(ctx->fd, &tks);

	if (ret != LIBMSR_ERR_OK) {
		rb_raise(rb_eRuntimeError, "Device read failed (%d)", ret);
	}

	tks_obj = msr_tracks_new(tks);

	return tks_obj;
}

static VALUE iso_read(VALUE self)
{
	return Qnil;
}
