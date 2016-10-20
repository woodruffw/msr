#include "msr.h"

VALUE cMSR_MSR206 = Qnil;

static VALUE allocate(VALUE klass);
static void deallocate_206_ctx(msr206_ctx_t *ctx);
static VALUE initialize(VALUE self, VALUE rb_string);
static VALUE comm_test(VALUE self);
static VALUE sensor_test(VALUE self);
static VALUE ram_test(VALUE self);
static VALUE reset(VALUE self);
static VALUE get_coercivity(VALUE self);
static VALUE set_coercivity(VALUE self, VALUE co_sym);
static VALUE set_bpi(VALUE self, VALUE bpi);
static VALUE set_bpc(VALUE self, VALUE bpc1, VALUE bpc2, VALUE bpc3);
static VALUE set_led(VALUE self, VALUE led_sym);
static VALUE raw_read(VALUE self);
static VALUE iso_read(VALUE self);
static VALUE raw_write(VALUE self, VALUE tks_obj);
static VALUE iso_write(VALUE self, VALUE tks_obj);

void Init_msr_msr206()
{
	cMSR_MSR206 = rb_define_class_under(mMSR, "MSR206", rb_cObject);
	rb_define_alloc_func(cMSR_MSR206, allocate);
	rb_define_method(cMSR_MSR206, "initialize", initialize, 1);
	rb_define_method(cMSR_MSR206, "comm_test!", comm_test, 0);
	rb_define_method(cMSR_MSR206, "sensor_test!", sensor_test, 0);
	rb_define_method(cMSR_MSR206, "ram_test!", ram_test, 0);
	rb_define_method(cMSR_MSR206, "reset!", reset, 0);
	rb_define_method(cMSR_MSR206, "coercivity", get_coercivity, 0);
	rb_define_method(cMSR_MSR206, "coercivity=", set_coercivity, 1);
	rb_define_method(cMSR_MSR206, "bpi=", set_bpi, 1);
	rb_define_method(cMSR_MSR206, "bpc=", set_bpc, 3);
	rb_define_method(cMSR_MSR206, "led=", set_led, 1);
	rb_define_method(cMSR_MSR206, "raw_read", raw_read, 0);
	rb_define_method(cMSR_MSR206, "iso_read", iso_read, 0);
	rb_define_method(cMSR_MSR206, "raw_write", raw_write, 1);
	rb_define_method(cMSR_MSR206, "iso_write", iso_write, 1);
}

static VALUE allocate(VALUE klass)
{
	msr206_ctx_t *ctx = ALLOC(msr206_ctx_t);

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

static VALUE comm_test(VALUE self)
{
	VALUE pass = ID2SYM(rb_intern("pass"));
	VALUE fail = ID2SYM(rb_intern("fail"));
	msr206_ctx_t *ctx;
	int ret;

	Data_Get_Struct(self, msr206_ctx_t, ctx);

	ret = msr_commtest(ctx->fd);

	if (ret != LIBMSR_ERR_OK) {
		return fail;
	}

	return pass;
}

static VALUE sensor_test(VALUE self)
{
	VALUE pass = ID2SYM(rb_intern("pass"));
	VALUE fail = ID2SYM(rb_intern("fail"));
	msr206_ctx_t *ctx;
	int ret;

	Data_Get_Struct(self, msr206_ctx_t, ctx);

	ret = msr_sensor_test(ctx->fd);

	if (ret != LIBMSR_ERR_OK) {
		return fail;
	}

	return pass;
}

static VALUE ram_test(VALUE self)
{
	VALUE pass = ID2SYM(rb_intern("pass"));
	VALUE fail = ID2SYM(rb_intern("fail"));
	msr206_ctx_t *ctx;
	int ret;

	Data_Get_Struct(self, msr206_ctx_t, ctx);

	ret = msr_ram_test(ctx->fd);

	if (ret != LIBMSR_ERR_OK) {
		return fail;
	}

	return pass;
}

static VALUE reset(VALUE self)
{
	msr206_ctx_t *ctx;

	Data_Get_Struct(self, msr206_ctx_t, ctx);

	msr_reset(ctx->fd);

	return self;
}

static VALUE get_coercivity(VALUE self)
{
	VALUE hi_co = ID2SYM(rb_intern("hi"));
	VALUE lo_co = ID2SYM(rb_intern("lo"));
	msr206_ctx_t *ctx;
	int ret;

	Data_Get_Struct(self, msr206_ctx_t, ctx);

	ret = msr_get_co(ctx->fd);

	if (ret == MSR_CO_HI) {
		return hi_co;
	}
	else if (ret == MSR_CO_LO) {
		return lo_co;
	}
	else {
		rb_raise(rb_eRuntimeError, "Couldn't get coercivity (%d)", ret);
	}
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

static VALUE set_bpi(VALUE self, VALUE bpi)
{
	uint8_t bpi_tk2;
	msr206_ctx_t *ctx;
	int ret;

	Check_Type(bpi, T_FIXNUM);
	Data_Get_Struct(self, msr206_ctx_t, ctx);

	bpi_tk2 = NUM2CHR(bpi);

	ret = msr_set_bpi(ctx->fd, bpi_tk2);

	if (ret != LIBMSR_ERR_OK) {
		rb_raise(rb_eRuntimeError, "Couldn't change bpi (%d)", ret);
	}

	return self;
}

static VALUE set_bpc(VALUE self, VALUE bpc1, VALUE bpc2, VALUE bpc3)
{
	uint8_t bpc_tk1, bpc_tk2, bpc_tk3;
	msr206_ctx_t *ctx;
	int ret;

	Check_Type(bpc1, T_FIXNUM);
	Check_Type(bpc2, T_FIXNUM);
	Check_Type(bpc3, T_FIXNUM);
	Data_Get_Struct(self, msr206_ctx_t, ctx);

	bpc_tk1 = NUM2CHR(bpc1);
	bpc_tk2 = NUM2CHR(bpc2);
	bpc_tk3 = NUM2CHR(bpc3);

	ret = msr_set_bpc(ctx->fd, bpc_tk1, bpc_tk2, bpc_tk3);

	if (ret != LIBMSR_ERR_OK) {
		rb_raise(rb_eRuntimeError, "Couldn't change bpc (%d)", ret);
	}

	return self;
}

static VALUE set_led(VALUE self, VALUE led_sym)
{
	VALUE all = ID2SYM(rb_intern("all"));
	VALUE grn = ID2SYM(rb_intern("green"));
	VALUE ylw = ID2SYM(rb_intern("yellow"));
	VALUE red = ID2SYM(rb_intern("red"));
	VALUE off = ID2SYM(rb_intern("none"));
	msr206_ctx_t *ctx;
	uint8_t led;

	Check_Type(led_sym, T_SYMBOL);
	Data_Get_Struct(self, msr206_ctx_t, ctx);

	if (led_sym == all) {
		led = MSR_CMD_LED_ON;
	}
	else if (led_sym == grn) {
		led = MSR_CMD_LED_GRN_ON;
	}
	else if (led_sym == ylw) {
		led = MSR_CMD_LED_YLW_ON;
	}
	else if (led_sym == red) {
		led = MSR_CMD_LED_RED_ON;
	}
	else if (led_sym == off) {
		led = MSR_CMD_LED_OFF;
	}
	else {
		rb_raise(rb_eArgError, "Unknown LED color command");
	}

	msr_flash_led(ctx->fd, led);

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
	msr206_ctx_t *ctx;
	msr_tracks_t tks = {0};
	int ret;
	VALUE tks_obj;

	Data_Get_Struct(self, msr206_ctx_t, ctx);

	for (int i = 0; i < MSR_MAX_TRACKS; i++) {
		tks.msr_tracks[i].msr_tk_len = MSR_MAX_TRACK_LEN;
	}

	ret = msr_iso_read(ctx->fd, &tks);

	if (ret != LIBMSR_ERR_OK) {
		rb_raise(rb_eRuntimeError, "Device read failed (%d)", ret);
	}

	tks_obj = msr_tracks_new(tks);

	return tks_obj;
}

static VALUE raw_write(VALUE self, VALUE tks_obj)
{
	msr206_ctx_t *ctx;
	msr_tracks_t tks = {0};
	int ret;

	if (CLASS_OF(tks_obj) != c_MSR_Tracks) {
		rb_raise(rb_eArgError, "expected tracks object");
	}

	Data_Get_Struct(self, msr206_ctx_t, ctx);

	msr_unwrap_tracks(tks_obj, &tks);

	ret = msr_raw_write(ctx->fd, &tks);

	if (ret != LIBMSR_ERR_OK) {
		rb_raise(rb_eRuntimeError, "Device write failed (%d)", ret);
	}

	return self;
}

static VALUE iso_write(VALUE self, VALUE tks_obj)
{
	msr206_ctx_t *ctx;
	msr_tracks_t tks = {0};
	int ret;

	if (CLASS_OF(tks_obj) != c_MSR_Tracks) {
		rb_raise(rb_eArgError, "expected tracks object");
	}

	Data_Get_Struct(self, msr206_ctx_t, ctx);

	msr_unwrap_tracks(tks_obj, &tks);

	ret = msr_iso_write(ctx->fd, &tks);

	if (ret != LIBMSR_ERR_OK) {
		rb_raise(rb_eRuntimeError, "Device write failed (%d)", ret);
	}

	return self;
}
