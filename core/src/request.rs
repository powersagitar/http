use std::{
    collections::HashMap,
    io::{BufRead, BufReader},
    net::TcpStream,
};

enum Method {
    Get,
}

impl Method {
    fn from(str: &str) -> Method {
        match str {
            "GET" => Method::Get,
            _ => unreachable!(),
        }
    }
}

enum Version {
    OnePointOne,
}

impl Version {
    fn from(str: &str) -> Version {
        match str {
            "HTTP/1.1" => Version::OnePointOne,
            _ => unreachable!(),
        }
    }
}

pub struct RequestHeader {
    method: Method,
    uri: String,
    version: Version,
    header_fields: HashMap<String, String>,
}

impl RequestHeader {
    fn parse_first_line(first_line: &str) -> (Method, String, Version) {
        let mut first_line_iter = first_line.split_whitespace();
        let method = first_line_iter.next().unwrap();
        let uri = first_line_iter.next().unwrap();
        let version = first_line_iter.next().unwrap();

        if first_line_iter.next().is_some() {
            panic!("Invlid request first line");
        }

        (Method::from(method), uri.to_owned(), Version::from(version))
    }

    fn from(request: &[String]) -> Self {
        let (method, uri, version) = Self::parse_first_line(request.first().unwrap());

        // TODO: header fields parser

        Self {
            method,
            uri,
            version,
            header_fields: HashMap::new(),
        }
    }
}

pub fn handle_request(stream: TcpStream) {
    let reader = BufReader::new(stream);
    let lines: Vec<String> = reader.lines().map(|line| line.unwrap()).collect();
}
