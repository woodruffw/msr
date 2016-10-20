#include "msr.h"

static VALUE initialize(VALUE self, VALUE data);
static VALUE msr_track_data(VALUE self);
static VALUE msr_track_length(VALUE self);

VALUE c_MSR_Track = Qnil;

void Init_msr_track()
{
	c_MSR_Track = rb_define_class_under(mMSR, "Track", rb_cObject);

	rb_define_const(c_MSR_Track, "MAX_TRACK_LEN", INT2NUM(MSR_MAX_TRACK_LEN));

	rb_define_method(c_MSR_Track, "initialize", initialize, 1);
	rb_define_method(c_MSR_Track, "data", msr_track_data, 0);
	rb_define_method(c_MSR_Track, "length", msr_track_length, 0);
}

/*
	A convenience function for instantiating a new MSR::Track in C.
*/
VALUE msr_track_new(msr_track_t tk)
{
	VALUE obj = rb_obj_alloc(c_MSR_Track);

	VALUE tk_data_ary = rb_ary_new();

	for (int i = 0; i < tk.msr_tk_len; i++) {
		rb_ary_push(tk_data_ary, INT2NUM(tk.msr_tk_data[i]));
	}

	return initialize(obj, tk_data_ary);
}

static VALUE initialize(VALUE self, VALUE tk_data_ary)
{
	int tk_len;

	Check_Type(tk_data_ary, T_ARRAY);

	tk_len = RARRAY_LEN(tk_data_ary);

	if (tk_len > MSR_MAX_TRACK_LEN) {
		rb_raise(rb_eArgError, "Track >%d bytes", MSR_MAX_TRACK_LEN);
	}

	rb_iv_set(self, "@data", tk_data_ary);

	return self;
}

static VALUE msr_track_data(VALUE self)
{
	return rb_iv_get(self, "@data");
}

static VALUE msr_track_length(VALUE self)
{
	VALUE tk_data_ary = rb_iv_get(self, "@data");

	return INT2NUM(RARRAY_LEN(tk_data_ary));
}