#ifndef RUBY_MSR_SHIM
#define RUBY_MSR_SHIM

VALUE msr_track_new(msr_track_t tk);
VALUE msr_tracks_new(msr_tracks_t tks);
void msr_unwrap_track(VALUE tk_obj, msr_track_t *tk);
void msr_unwrap_tracks(VALUE tks_obj, msr_tracks_t *tks);

#endif
