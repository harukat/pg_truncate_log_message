#include <limits.h>
#include "postgres.h"
#include "fmgr.h"
#include "utils/builtins.h"
#include "utils/guc.h"
#include "miscadmin.h"
#include "mb/pg_wchar.h"

PG_MODULE_MAGIC;

void _PG_init(void);
void _PG_fini(void);
void emit_log_hook_impl(ErrorData *edata);

static emit_log_hook_type prev_emit_log_hook = NULL;
static int32 pg_truncate_log_message_max_length = -1;

void
emit_log_hook_impl(ErrorData *edata)
{
	int len, cliplen;
	char* c;
	if (pg_truncate_log_message_max_length >= 0 && edata->message)
	{
		len = strlen(edata->message);
		if (len > pg_truncate_log_message_max_length)
		{
			cliplen = pg_mbcliplen(edata->message,
			            pg_truncate_log_message_max_length, len);
			*(edata->message + cliplen) = '\0';
			edata->hide_stmt = true;
		}
	}

	if (prev_emit_log_hook)
		prev_emit_log_hook(edata);
}


void
_PG_init(void)
{
	if (! process_shared_preload_libraries_in_progress)
		return;

	prev_emit_log_hook = emit_log_hook;
	emit_log_hook = emit_log_hook_impl;

	DefineCustomIntVariable("pg_truncate_log_message.max_length",
		"Sets log message truncation threshold (byte)", NULL,
        &pg_truncate_log_message_max_length, -1, -1, INT_MAX,
        PGC_SUSET, 
#if PG_VERSION_NUM < 110000
	    NULL,
#else
	    GUC_UNIT_BYTE,
#endif
	    NULL, NULL, NULL);

	elog(DEBUG1, "pg_truncate_log_message loaded");
}

void
_PG_fini(void)
{
	emit_log_hook = prev_emit_log_hook;
}


