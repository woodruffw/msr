#ifndef RUBY_MSR_TRACKS
#define RUBY_MSR_TRACKS

extern VALUE c_MSR_Tracks;

void Init_msr_tracks();
VALUE msr_tracks_initialize(VALUE self, VALUE tk1, VALUE tk2, VALUE tk3);

#endif
