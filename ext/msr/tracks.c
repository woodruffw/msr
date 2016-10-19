#include "msr.h"

VALUE c_MSR_Tracks = Qnil;

static VALUE allocate(VALUE klass);
static void deallocate_tracks(msr_tracks_t *tks);
static VALUE initialize(VALUE self, VALUE data);
static VALUE msr_tracks_data(VALUE self);

void Init_msr_tracks()
{
	c_MSR_Tracks = rb_define_class_under(mMSR, "Tracks", rb_cObject);

	rb_define_alloc_func(c_MSR_Tracks, allocate);
	rb_define_method(c_MSR_Tracks, "initialize", initialize, 1);
	rb_define_method(c_MSR_Track, "data", msr_tracks_data, 0);
}

static VALUE allocate(VALUE klass)
{
	return Qnil;
}

static void deallocate_tracks(msr_tracks_t *tks)
{

}

static VALUE initialize(VALUE self, VALUE data)
{
	return Qnil;
}

static VALUE msr_tracks_data(VALUE self)
{
	return rb_iv_get(self, "@data");
}

