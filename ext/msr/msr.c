#include "msr.h"
#include "msr206.h"

VALUE mMSR = Qnil;

void Init_msr()
{
	mMSR = rb_define_module("MSR");

	Init_msr_msr206();
}
