#[macro_use(slog_o, slog_info, slog_log, slog_record, slog_record_static, slog_b, slog_kv)]
extern crate slog;
#[macro_use]
extern crate slog_term;
extern crate slog_async;
extern crate slog_scope;

mod log;

pub fn init() {
    log::init();
}
