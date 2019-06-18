#[macro_use]
extern crate slog;
extern crate slog_term;
extern crate slog_async;

use std::collections::HashMap;
use std::fs::OpenOptions;
use slog::Drain;

static mut logger_map : HashMap<&str, slog::Logger> = HashMap::new();

//pub fn init() {
//unsafe {
//logger_map = std::collections::HashMap<str, slog::Logger>::new();
//}
//}

pub fn getLogger(logger_name: &str) -> &slog::Logger {
    if !logger_map.contains_key(logger_name) {
        let file = OpenOptions::new()
            .create(true)
            .write(true)
            .truncate(true)
            .open("fastype.log")
            .unwrap();

        let decorator = slog_term::PlainDecorator::new(file);
        let drain = slog_term::FullFormat::new(decorator).build().fuse();
        let drain = slog_async::Async::new(drain).build().fuse();
        let logger = slog::Logger::root(drain, o!());
        logger_map.insert(logger_name, logger);
    }
    logger_map.get(logger_name).unwrap()
}
