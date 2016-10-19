#include "msr.h"

VALUE mMSR = Qnil;

void Init_msr()
{
	mMSR = rb_define_module("MSR");

	Init_msr_msr206();
	Init_msr_track();
}
