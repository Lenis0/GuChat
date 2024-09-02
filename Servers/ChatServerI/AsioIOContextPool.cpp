#include "AsioIOContextPool.h"
#include <iostream>

using IOContext = boost::asio::io_context;

AsioIOContextPool::AsioIOContextPool(std::size_t size):_ioContexts(size),
_works(size), _nextIOContext(0) {
	// ��iocontext��work ��ֹ���˳�
	for (std::size_t i = 0; i < size; ++ i) {
		_works[i] = std::unique_ptr<Work>(new Work(_ioContexts[i]));
	}

	//�������iocontext����������̣߳�ÿ���߳��ڲ�����iocontext
	for (std::size_t i = 0; i < _ioContexts.size(); ++ i) {
		_threads.emplace_back([this, i]() {
			_ioContexts[i].run();
		});
	}
}

AsioIOContextPool::~AsioIOContextPool() {
	Stop(); // RAII˼��
	std::cout << "AsioIOContextPool destruct" << std::endl;
}

IOContext& AsioIOContextPool::GetIOContext() {
	auto& context = _ioContexts[_nextIOContext ++];
	if (_nextIOContext == _ioContexts.size()) {
		_nextIOContext = 0;
	}
	return context;
}

void AsioIOContextPool::Stop() {
	//��Ϊ����ִ��work.reset��������iocontext��run��״̬���˳�
	//��iocontext�Ѿ����˶���д�ļ����¼��󣬻���Ҫ�ֶ�stop�÷���
	for (auto& work : _works) {
		//�ѷ�����ֹͣ
		work->get_io_context().stop();
		work.reset();
	}

	for (auto& t : _threads) {
		t.join();
	}
}