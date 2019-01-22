MODULES = pg_truncate_log_message

PG_CPPFLAGS = -I$(libpq_srcdir) -L$(libdir)

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)


