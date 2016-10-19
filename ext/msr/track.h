#ifndef RUBY_MSR_TRACK
#define RUBY_MSR_TRACK

extern VALUE c_MSR_Track;

void Init_msr_track();
VALUE msr_track_new(msr_track_t tk);

#endif
