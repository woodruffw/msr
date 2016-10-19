#include "msr.h"

static VALUE allocate(VALUE klass);
static void deallocate_track(msr_track_t *tk);
static VALUE initialize(VALUE self, VALUE data);
static VALUE msr_track_data(VALUE self);

VALUE c_MSR_Track = Qnil;

void Init_msr_track()
{
	c_MSR_Track = rb_define_class_under(mMSR, "Track", rb_cObject);

	rb_define_const(c_MSR_Track, "MAX_TRACK_LEN", INT2NUM(MSR_MAX_TRACK_LEN));

	rb_define_alloc_func(c_MSR_Track, allocate);
	rb_define_method(c_MSR_Track, "initialize", initialize, 1);
	rb_define_method(c_MSR_Track, "data", msr_track_data, 0);
}

/*
	A convenience function for instantiating a new MSR::Track in C.
*/
VALUE msr_track_new(msr_track_t tk)
{
	VALUE obj = allocate(c_MSR_Track);

	VALUE tk_data_ary = rb_ary_new();

	for (int i = 0; i < tk.msr_tk_len; i++) {
		rb_ary_push(tk_data_ary, INT2NUM(tk.msr_tk_data[i]));
	}

	return initialize(obj, tk_data_ary);
}

static VALUE allocate(VALUE klass)
{
	msr_track_t *tk = malloc(sizeof(msr_track_t));

	return Data_Wrap_Struct(klass, NULL, deallocate_track, tk);
}

static void deallocate_track(msr_track_t *tk)
{
	free(tk);
}

static VALUE initialize(VALUE self, VALUE tk_data_ary)
{
	msr_track_t *tk;
	int tk_len;

	Check_Type(tk_data_ary, T_ARRAY);
	Data_Get_Struct(self, msr_track_t, tk);

	tk_len = RARRAY_LEN(tk_data_ary);

	if (tk_len > MSR_MAX_TRACK_LEN) {
		rb_raise(rb_eArgError, "Track >%d bytes", MSR_MAX_TRACK_LEN);
	}

	tk->msr_tk_len = tk_len;

	for (int i = 0; i < tk_len; i++) {
		tk->msr_tk_data[i] = NUM2CHR(rb_ary_entry(tk_data_ary, i));
	}

	rb_iv_set(self, "@data", tk_data_ary);

	return self;
}

static VALUE msr_track_data(VALUE self)
{
	return rb_iv_get(self, "@data");
}
