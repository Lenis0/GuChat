﻿#include "CServer.h"
#include "ConfigMgr.h"
#include "RedisMgr.h"

//void TestRedisMgr() {
//    //assert(RedisMgr::GetInstance()->Connect("127.0.0.1", 6379));
//    //assert(RedisMgr::GetInstance()->Auth("Lenis0", "375229"));
//    assert(RedisMgr::GetInstance()->Set("blogwebsite", "llfc.club"));
//    std::string value = "";
//    assert(RedisMgr::GetInstance()->Get("blogwebsite", value));
//    assert(RedisMgr::GetInstance()->Get("nonekey", value) == false);
//    assert(RedisMgr::GetInstance()->HSet("bloginfo", "blogwebsite", "llfc.club"));
//    assert(RedisMgr::GetInstance()->HGet("bloginfo", "blogwebsite") != "");
//    assert(RedisMgr::GetInstance()->ExistsKey("bloginfo"));
//    assert(RedisMgr::GetInstance()->Del("bloginfo"));
//    assert(RedisMgr::GetInstance()->Del("bloginfo"));
//    assert(RedisMgr::GetInstance()->ExistsKey("bloginfo") == false);
//    assert(RedisMgr::GetInstance()->LPush("lpushkey1", "lpushvalue1"));
//    assert(RedisMgr::GetInstance()->LPush("lpushkey1", "lpushvalue2"));
//    assert(RedisMgr::GetInstance()->LPush("lpushkey1", "lpushvalue3"));
//    assert(RedisMgr::GetInstance()->RPop("lpushkey1", value));
//    assert(RedisMgr::GetInstance()->RPop("lpushkey1", value));
//    assert(RedisMgr::GetInstance()->LPop("lpushkey1", value));
//    assert(RedisMgr::GetInstance()->LPop("lpushkey2", value) == false);
//    //RedisMgr::GetInstance()->Close();
//}

int main() {
    //TestRedisMgr();
	auto& gConfigMgr = ConfigMgr::Inst();
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

