#include "msr.h"

static VALUE initialize(VALUE self, VALUE tk1, VALUE tk2, VALUE tk3);
static VALUE msr_tracks_track1(VALUE self);
static VALUE msr_tracks_track2(VALUE self);
static VALUE msr_tracks_track3(VALUE self);

VALUE c_MSR_Tracks = Qnil;

void Init_msr_tracks()
{
	c_MSR_Tracks = rb_define_class_under(mMSR, "Tracks", rb_cObject);

	rb_define_method(c_MSR_Tracks, "initialize", initialize, 3);
	rb_define_method(c_MSR_Tracks, "track1", msr_tracks_track1, 0);
	rb_define_method(c_MSR_Tracks, "track2", msr_tracks_track2, 0);
	rb_define_method(c_MSR_Tracks, "track3", msr_tracks_track3, 0);
}

/*
	A convenience function for instantiating a new MSR::Tracks in C.
*/
VALUE msr_tracks_new(msr_tracks_t tks)
{
	VALUE obj = rb_obj_alloc(c_MSR_Tracks);
	VALUE tk1_obj = msr_track_new(tks.msr_tracks[0]);
	VALUE tk2_obj = msr_track_new(tks.msr_tracks[1]);
	VALUE tk3_obj = msr_track_new(tks.msr_tracks[2]);

	return initialize(obj, tk1_obj, tk2_obj, tk3_obj);
}

static VALUE initialize(VALUE self, VALUE tk1, VALUE tk2, VALUE tk3)
{
	if (CLASS_OF(tk1) != c_MSR_Track) {
		rb_raise(rb_eArgError, "expected track object for track 1");
	}

	if (CLASS_OF(tk2) != c_MSR_Track) {
		rb_raise(rb_eArgError, "expected track object for track 2");
	}

	if (CLASS_OF(tk3) != c_MSR_Track) {
		rb_raise(rb_eArgError, "expected track object for track 3");
	}

	rb_iv_set(self, "@track1", tk1);
	rb_iv_set(self, "@track2", tk2);
	rb_iv_set(self, "@track3", tk3);

	return self;
}

static VALUE msr_tracks_track1(VALUE self)
{
	return rb_iv_get(self, "@track1");
}

static VALUE msr_tracks_track2(VALUE self)
{
	return rb_iv_get(self, "@track2");
}

static VALUE msr_tracks_track3(VALUE self)
{
	return rb_iv_get(self, "@track3");
}
