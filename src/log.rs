use std::collections::HashMap;
use std::fs::OpenOptions;
use slog::Drain;

pub fn init() {
    let file = OpenOptions::new()
        .create(true)
        .write(true)
        .truncate(true)
        .open("fastype.log")
        .unwrap();

    let decorator = slog_term::PlainDecorator::new(file);
    let drain = slog_term::FullFormat::new(decorator).build().fuse();
    let drain = slog_async::Async::new(drain).build().fuse();
    let log = slog::Logger::root(drain, slog_o!());
    let _guard = slog_scope::set_global_logger(log);
    slog_stdlog::init().unwrap();
}
