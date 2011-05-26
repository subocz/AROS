#ifndef _EXEC_UTIL_H
#define _EXEC_UTIL_H

/*
    Copyright � 1995-2011, The AROS Development Team. All rights reserved.
    $Id$

    Desc: Utility functions for exec.
    Lang: english
*/

#include <aros/asmcall.h>
#include <exec/types.h>
#include <utility/tagitem.h>
#include <proto/alib.h>
#include <proto/arossupport.h>

#include <stdarg.h>

#include "exec_intern.h"

/* PC and FP registers for various CPUs */
#ifdef __i386__
#define PC eip
#define FP ebp
#endif
#ifdef __x86_64__
#define PC rip
#define FP rbp
#endif
#ifdef __mc68000__
#define PC pc
#ifdef CONFIG_GCC_FP_A6
#define FP a[6]
#else
#define FP a[5]
#define CALLER_FRAME NULL
#endif
#endif
#ifdef __powerpc__
#define PC ip
#define FP gpr[1]
#endif
#ifdef __arm__
#define PC pc
#define FP lr
#define CALLER_FRAME NULL
#endif

#ifndef PC
#error unsupported CPU type
#endif

#ifndef CALLER_FRAME
#define CALLER_FRAME __builtin_frame_address(1)
#endif

#ifndef EXEC_TASKS_H
struct Task;
#endif
#ifndef EXEC_LISTS_H
struct List;
#endif
#ifndef ETASK_H
struct IntETask;
#endif

/*
    Prototypes
*/
APTR Exec_AllocTaskMem (struct Task * task, ULONG size, ULONG flags, struct ExecBase *SysBase);
void Exec_FreeTaskMem (struct Task * task, APTR mem, struct ExecBase *SysBase);

void Exec_InitETask(struct Task *task, struct ETask *etask, struct ExecBase *SysBase);
void Exec_CleanupETask(struct Task *task, struct ETask *et, struct ExecBase *SysBase);
struct Task *Exec_FindTaskByID(ULONG id, struct ExecBase *SysBase);
struct ETask *Exec_FindChild(ULONG id, struct ExecBase *SysBase);
struct IntETask *FindETask(struct List *, ULONG id, struct ExecBase *SysBase);

BOOL Exec_CheckTask(struct Task *task, struct ExecBase *SysBase);

STRPTR Alert_AddString(STRPTR dest, CONST_STRPTR src);
STRPTR Alert_GetTitle(ULONG alertNum);
STRPTR Alert_GetString(ULONG alertnum, STRPTR buf);
STRPTR FormatAlert(char *buffer, ULONG alertNum, struct Task *task, APTR location, UBYTE type, struct ExecBase *SysBase);
STRPTR FormatTask(STRPTR buffer, const char *text, struct Task *, struct ExecBase *SysBase);
STRPTR FormatLocation(STRPTR buf, const char *text, APTR location, struct ExecBase *SysBase);

void FormatAlertExtra(char *buffer, APTR stack, UBYTE type, APTR data, struct ExecBase *SysBase);
char *FormatCPUContext(char *buffer, struct ExceptionContext *ctx, struct ExecBase *SysBase);
APTR UnwindFrame(APTR fp, APTR *caller);

void Exec_ExtAlert(ULONG alertNum, APTR location, APTR stack, UBYTE type, APTR data, struct ExecBase *SysBase);
ULONG Exec_UserAlert(ULONG alertNum, struct ExecBase *SysBase);
void Exec_SystemAlert(ULONG alertNum, APTR location, APTR stack, UBYTE type, APTR data, struct ExecBase *SysBase);
void Exec_DoResetCallbacks(struct IntExecBase *SysBase);

APTR InternalRawDoFmt(CONST_STRPTR FormatString, APTR DataStream, VOID_FUNC PutChProc,
		      APTR PutChData, va_list VaListStream);

void FastPutMsg(struct MsgPort *port, struct Message *message, struct ExecBase *SysBase);
void InternalPutMsg(struct MsgPort *port, struct Message *message, struct ExecBase *SysBase);

/*
 *  Pseudo-functions, including SysBase for nicer calling...
 */
#define AllocTaskMem(t,s,f) Exec_AllocTaskMem(t,s,f,SysBase)
#define FreeTaskMem(t,m)    Exec_FreeTaskMem(t,m,SysBase)
#define FindTaskByID(i)	    Exec_FindTaskByID(i,SysBase)
#define FindChild(i)	    Exec_FindChild(i,SysBase)
#define FindETask(l,i)	    Exec_FindETask(l,i,SysBase)
#define InitETask(t,e)	    Exec_InitETask(t,e,SysBase)
#define CleanupETask(t,e)   Exec_CleanupETask(t,e,SysBase)

#endif /* _EXEC_UTIL_H */
