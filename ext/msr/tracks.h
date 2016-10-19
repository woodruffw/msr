#ifndef RUBY_MSR_TRACKS
#define RUBY_MSR_TRACKS

extern VALUE c_MSR_Tracks;

void Init_msr_tracks();
VALUE msr_tracks_new(msr_tracks_t tk);

#endif
