#include "msr.h"

/*
	The first track on the card.
	@return [MSR::Track] the track object
*/
static VALUE msr_tracks_track1(VALUE self);

/*
	The second track on the card.
	@return [MSR::Track] the track object
*/
static VALUE msr_tracks_track2(VALUE self);

/*
	The third track on the card.
	@return [MSR::Track] the track object
*/
static VALUE msr_tracks_track3(VALUE self);

/*
	Reverse the direction of the tracks, returning a new object.
	@return [MSR::Tracks] the reversed tracks
*/
static VALUE msr_tracks_reverse(VALUE self);

VALUE c_MSR_Tracks = Qnil;

void Init_msr_tracks()
{
	c_MSR_Tracks = rb_define_class_under(mMSR, "Tracks", rb_cObject);

	rb_define_const(c_MSR_Tracks, "MAX_TRACKS", INT2NUM(MSR_MAX_TRACKS));

	rb_define_method(c_MSR_Tracks, "initialize", msr_tracks_initialize, 3);
	rb_define_method(c_MSR_Tracks, "track1", msr_tracks_track1, 0);
	rb_define_method(c_MSR_Tracks, "track2", msr_tracks_track2, 0);
	rb_define_method(c_MSR_Tracks, "track3", msr_tracks_track3, 0);
	rb_define_method(c_MSR_Tracks, "reverse", msr_tracks_reverse, 0);
}

VALUE msr_tracks_initialize(VALUE self, VALUE tk1, VALUE tk2, VALUE tk3)
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

static VALUE msr_tracks_reverse(VALUE self)
{
	msr_tracks_t tks;

	msr_unwrap_tracks(self, &tks);

	msr_reverse_tracks(&tks);

	return msr_tracks_new(tks);
}
