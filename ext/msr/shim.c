#include "msr.h"

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

	return msr_track_initialize(obj, tk_data_ary);
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

	return msr_tracks_initialize(obj, tk1_obj, tk2_obj, tk3_obj);
}

/*
	A convenience function for unwrapping an MSR::Track instance.
*/
void msr_unwrap_track(VALUE tk_obj, msr_track_t *tk)
{
	VALUE tk_data_ary = rb_funcall(tk_obj, rb_intern("data"), 0);
	VALUE tk_data_len = rb_funcall(tk_obj, rb_intern("length"), 0);
	uint8_t len = (uint8_t) NUM2CHR(tk_data_len);

	tk->msr_tk_len = len;

	for (int i = 0; i < len; i++) {
		tk->msr_tk_data[i] = (uint8_t) NUM2CHR(rb_ary_entry(tk_data_ary, i));
	}
}

/*
	A convenience function for unwrapping an MSR::Tracks instance.
*/
void msr_unwrap_tracks(VALUE tks_obj, msr_tracks_t *tks)
{
	VALUE tk1_obj = rb_funcall(tks_obj, rb_intern("track1"), 0);
	VALUE tk2_obj = rb_funcall(tks_obj, rb_intern("track2"), 0);
	VALUE tk3_obj = rb_funcall(tks_obj, rb_intern("track3"), 0);

	msr_unwrap_track(tk1_obj, &(tks->msr_tracks[0]));
	msr_unwrap_track(tk2_obj, &(tks->msr_tracks[1]));
	msr_unwrap_track(tk3_obj, &(tks->msr_tracks[2]));
}
