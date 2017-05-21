#ifndef __SYSTRACE_PRIV_H__
#define __SYSTRACE_PRIV_H__

#include "cpu.h"
#include "systrace.h"

extern void systrace_on_syscall(CPUX86State *env);
extern void systrace_on_sysret(CPUX86State *env);
extern void cb_log_syscall_info( X86CPU *x86cpu, bool is_invoke, void *args, const syscall_context* syscall_info );

#endif
