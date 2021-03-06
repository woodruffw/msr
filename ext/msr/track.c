#include "msr.h"

/*
	The data associated with the track.
	@return [Array<Fixnum>] the track data
*/
static VALUE msr_track_get_data(VALUE self);

/*
	The length of the track's data.
	@return [Fixnum] the track length
*/
static VALUE msr_track_get_length(VALUE self);

/*
	Reverse the direction of the track, returning a new object.
	@return [MSR::Track] the reversed track
*/
static VALUE msr_track_reverse(VALUE self);

VALUE c_MSR_Track = Qnil;

void Init_msr_track()
{
	c_MSR_Track = rb_define_class_under(mMSR, "Track", rb_cObject);

	rb_define_const(c_MSR_Track, "MAX_TRACK_LEN", INT2NUM(MSR_MAX_TRACK_LEN));

	rb_define_method(c_MSR_Track, "initialize", msr_track_initialize, 1);
	rb_define_method(c_MSR_Track, "data", msr_track_get_data, 0);
	rb_define_method(c_MSR_Track, "length", msr_track_get_length, 0);
	rb_define_method(c_MSR_Track, "reverse", msr_track_reverse, 0);
}

VALUE msr_track_initialize(VALUE self, VALUE tk_data_ary)
{
	int tk_len;

	Check_Type(tk_data_ary, T_ARRAY);

	tk_len = RARRAY_LEN(tk_data_ary);

	if (tk_len > MSR_MAX_TRACK_LEN) {
		rb_raise(rb_eArgError, "Track >%d bytes", MSR_MAX_TRACK_LEN);
	}

	for (int i = 0; i < tk_len; ++i) {
		Check_Type(rb_ary_entry(tk_data_ary, i), T_FIXNUM);
	}

	rb_iv_set(self, "@data", tk_data_ary);

	return self;
}

static VALUE msr_track_get_data(VALUE self)
{
	return rb_iv_get(self, "@data");
}

static VALUE msr_track_get_length(VALUE self)
{
	VALUE tk_data_ary = rb_iv_get(self, "@data");

	return INT2NUM(RARRAY_LEN(tk_data_ary));
}

static VALUE msr_track_reverse(VALUE self)
{
	msr_track_t tk;

	msr_unwrap_track(self, &tk);

	msr_reverse_track(&tk);

	return msr_track_new(tk);
}
