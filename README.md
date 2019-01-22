# pg_truncate_log_message

PostgreSQL module that truncates long log message with emit_log_hook.

## Install

Do the following commands where PATH for PostgreSQL commands of the target
server is set.

    $ cd pg_truncate_log_message
    $ make
    $ su -c 'make install'

## Usage

Add pg_truncate_log_message into shared_preload_libraries parameter.
Set pg_truncate_log_message.max_length parameter to maximum length
of log messages. This module will truncate the message in this length.
Its default unit is byte. -1 means no limit.

postgresql.conf sample:

    shared_preload_libraries = 'pg_truncate_log_message'
    
    pg_truncate_log_message.max_length = '100'

