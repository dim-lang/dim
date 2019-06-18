mod log;

pub fn hello() {
    //log::init();
    info!(log::getLogger("core"), "hello");
}
