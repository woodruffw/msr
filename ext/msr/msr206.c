#include "msr.h"

VALUE cMSR_MSR206 = Qnil;

static VALUE allocate(VALUE klass);
static void deallocate_206_ctx(msr206_ctx_t *ctx);

/*
	Create a new {MSR::MSR206} instance for a serial device.
	@param device [String] the serial device to connect to
	@raise [RuntimeError] if the device can't be opened and/or initialized
	@example
		msr = MSR::MSR206.new("/dev/ttyUSB0")
*/
static VALUE initialize(VALUE self, VALUE rb_string);

/*
	Run a communications test between the system and the device.
	@return [Symbol] `:pass` if the test succeeded, or `:fail` if it failed
*/
static VALUE comm_test(VALUE self);

/*
	Run a test of the device's magnetic sensor.
	@return [Symbol] `:pass` if the test succeeded, or `:fail` if it failed
	@note This method is interactive.
*/
static VALUE sensor_test(VALUE self);

/*
	Run a test of the device's internal RAM.
	@return [Symbol] `:pass` if the test succeeded, or `:fail` if it failed
*/
static VALUE ram_test(VALUE self);

/*
	Reset the device to a ready state.
	@note This method pauses for 100ms.
*/
static VALUE reset(VALUE self);

/*
	Get the device's firmware version.
	@return [String] the firmware version, in "REV?X.XX" format
	@raise [RuntimeError] if the device returns a bad response
*/
static VALUE firmware(VALUE self);

/*
	Get the device's model.
	@return [String] the model, in "MSR-206-?" format
	@raise [RuntimeError] if the device returns a bad response
*/
static VALUE model(VALUE self);

/*
	Get the device's current coercivity level.
	@return [Symbol] `:hi` if the device is in Hi-Co mode, `:lo` if Lo-Co
	@raise [RuntimeError] if the device returns a bad response
*/
static VALUE get_coercivity(VALUE self);

/*
	Set the device's coercivity level.
	@param coercivity [Symbol] `:hi` for Hi-Co, and `:lo` for Lo-Co
	@raise [ArgumentError] if the coercivity level is unrecognized
	@raise [RuntimeError] if the device returns a bad response
*/
static VALUE set_coercivity(VALUE self, VALUE co_sym);

/*
	Set the bits-per-inch on the second track.
	@param bpi [Fixnum] the BPI setting, either 75 or 210
	@raise [RuntimeError] if the device returns a bad response
	@note This only applies to the second track.
*/
static VALUE set_bpi(VALUE self, VALUE bpi);

/*
	Set the bits-per-character on each track.
	@param bpc_ary [Array<Fixnum>] the array of BPC values
	@raise [RuntimeError] if the device returns a bad response
*/
static VALUE set_bpc(VALUE self, VALUE bpc_ary);

/*
	Control the LEDs on the device.
	@param led [Symbol] the LED command (`:green`, `:yellow`, `:red`, `:all`,
		`:none`)
	@raise [ArgumentError] if the LED command is unrecognized
*/
static VALUE set_led(VALUE self, VALUE led_sym);

/*
	Read raw data from a card.
	@return [MSR::Tracks] a new tracks object containing the raw card data
	@note This method is interactive.
*/
static VALUE raw_read(VALUE self);

/*
	Read ISO-formatted data from a card.
	@return [MSR::Tracks] a new tracks object containing the ISO card data
	@note This method is interactive.
*/
static VALUE iso_read(VALUE self);

/*
	Write raw data to a card.
	@param tracks [MSR::Tracks] the raw tracks to write to the card
	@note This method is interactive.
*/
static VALUE raw_write(VALUE self, VALUE tks_obj);

/*
	Write ISO-formatted data to a card.
	@param tracks [MSR::Tracks] the ISO tracks to write to the card
	@note This method is interactive.
*/
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
	rb_define_method(cMSR_MSR206, "firmware", firmware, 0);
	rb_define_method(cMSR_MSR206, "model", model, 0);
	rb_define_method(cMSR_MSR206, "coercivity", get_coercivity, 0);
	rb_define_method(cMSR_MSR206, "coercivity=", set_coercivity, 1);
	rb_define_method(cMSR_MSR206, "bpi=", set_bpi, 1);
	rb_define_method(cMSR_MSR206, "bpc=", set_bpc, 1);
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

	xfree(ctx);
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

static VALUE firmware(VALUE self)
{
	msr206_ctx_t *ctx;
	uint8_t buf[9];
	int ret;

	Data_Get_Struct(self, msr206_ctx_t, ctx);

	ret = msr_fwrev(ctx->fd, buf);

	if (ret != LIBMSR_ERR_OK) {
		rb_raise(rb_eRuntimeError, "Couldn't get device firmware (%d)", ret);
	}

	return rb_str_new_cstr(buf);
}

static VALUE model(VALUE self)
{
	msr206_ctx_t *ctx;
	uint8_t buf[10];
	int ret;

	Data_Get_Struct(self, msr206_ctx_t, ctx);

	ret = msr_model(ctx->fd, buf);

	if (ret != LIBMSR_ERR_OK) {
		rb_raise(rb_eRuntimeError, "Couldn't get device model (%d)", ret);
	}

	return rb_str_new_cstr(buf);
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

static VALUE set_bpc(VALUE self, VALUE bpc_ary)
{
	uint8_t bpcs[3] = {0};
	msr206_ctx_t *ctx;
	int ret;

	Check_Type(bpc_ary, T_ARRAY);
	Data_Get_Struct(self, msr206_ctx_t, ctx);

	if (RARRAY_LEN(bpc_ary) != MSR_MAX_TRACKS) {
		rb_raise(rb_eArgError, "Expected %d BPC values, got %ld",
			MSR_MAX_TRACKS, RARRAY_LEN(bpc_ary));
	}

	for (int i = 0; i < MSR_MAX_TRACKS; i++) {
		Check_Type(rb_ary_entry(bpc_ary, i), T_FIXNUM);
		bpcs[i] = (uint8_t) NUM2CHR(rb_ary_entry(bpc_ary, i));
	}

	ret = msr_set_bpc(ctx->fd, bpcs[0], bpcs[1], bpcs[2]);

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
