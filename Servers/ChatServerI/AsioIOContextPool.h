#include <vector>
#include <boost/asio.hpp>
#include "Singleton.h"
class AsioIOContextPool:public Singleton<AsioIOContextPool> {
	friend Singleton<AsioIOContextPool>;
public:
	using IOContext = boost::asio::io_context;
	using Work = boost::asio::io_context::work;
	using WorkPtr = std::unique_ptr<Work>;
	~AsioIOContextPool();
	AsioIOContextPool(const AsioIOContextPool&) = delete;
	AsioIOContextPool& operator= (const AsioIOContextPool&) = delete;
	// 使用 round-robin 的方式返回一个 io_context
	IOContext& GetIOContext();
	void Stop();
private:
	AsioIOContextPool(std::size_t size = 2/*std::thread::hardware_concurrency()*/);
	std::vector<IOContext> _ioContexts;
	std::vector<WorkPtr> _works; // 防止IOContext.run()没有任务时退出
	std::vector<std::thread> _threads;
	std::size_t _nextIOContext;
};