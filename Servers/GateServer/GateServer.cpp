﻿#include "CServer.h"
#include "ConfigMgr.h"

int main() {
	ConfigMgr gConfigMgr;
	std::string gate_port_str = gConfigMgr["GateServer"]["Port"];
	unsigned short gate_port = atoi(gate_port_str.c_str());
	try {
		net::io_context ioc{1};
		net::signal_set signals(ioc, SIGINT, SIGTERM);
		signals.async_wait([&ioc](const boost::system::error_code& error, int signal_number) {
			if (error) {
				return;
			}

			ioc.stop();
		});

		std::make_shared<CServer>(ioc, gate_port)->Start();
		std::cout << "GateServer listen on port: " << gate_port << std::endl;
		ioc.run();
	} catch (std::exception const& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
}