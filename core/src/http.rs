mod request;

use std::net::TcpListener;

use request::handle_request;

pub struct Config {
    port: u16,
}

pub fn start(config: Config) -> ! {
    let listener = TcpListener::bind(format!("0.0.0.0:{}", config.port))
        .expect(format!("Failed to bind to 0.0.0.0:{}", config.port).as_str());

    for stream in listener.incoming() {
        let stream = stream.expect("Failed to accept connection");
        handle_request(stream);
    }

    unreachable!();
}
