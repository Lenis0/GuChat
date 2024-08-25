#include "MysqlDao.h"

SqlConnection::SqlConnection(sql::Connection* con, int64_t lasttime):
	_con(con), _last_oper_time(lasttime) {

}

MySqlPool::MySqlPool(const std::string& url, const std::string& user, const std::string& pass,
	const std::string& schema, int poolSize)
	:url_(url), user_(user), pass_(pass), schema_(schema), poolSize_(poolSize), b_stop_(false) {
	try {
		for (int i = 0; i < poolSize_; ++ i) {
			sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
			sql::Connection* con = driver->connect(url_, user_, pass_);
			std::cout << "���ݿ����ӳɹ�: " << url << std::endl;
			con->setSchema(schema_);
			// ��ȡ��ǰʱ���
			auto currentTime = std::chrono::system_clock::now().time_since_epoch();
			// ��ʱ���ת��Ϊ��
			long long timestamp = std::chrono::duration_cast<std::chrono::seconds>(currentTime).count();
			pool_.push(std::make_unique<SqlConnection>(con, timestamp));
		}

		// ÿ��60����һ��
		_check_thread = std::thread([this]() {
			while (!b_stop_) {
				checkConnection();
				std::this_thread::sleep_for(std::chrono::seconds(60));
			}
		});

		_check_thread.detach(); // ��̨���뷽ʽ�����߳� ��������ϵͳ����
	} catch (sql::SQLException& e) {
		// �����쳣
		std::cout << "mysql pool init failed, error is " << e.what() << std::endl;
	}
}

void MySqlPool::checkConnection() {
	std::lock_guard<std::mutex> guard(mutex_);
	int poolsize = pool_.size();
	// ��ȡ��ǰʱ���
	auto currentTime = std::chrono::system_clock::now().time_since_epoch();
	// ��ʱ���ת��Ϊ��
	long long timestamp = std::chrono::duration_cast<std::chrono::seconds>(currentTime).count();
	for (int i = 0; i < poolsize; i ++) {
		std::unique_ptr<SqlConnection> con = std::move(pool_.front());
		pool_.pop();
		Defer defer([this, &con]() {
			pool_.push(std::move(con));
		});

		// ���ϴβ����Ƿ���5���� ����5�������ִ��һ�μ򵥵Ĳ�����������
		if (timestamp - con->_last_oper_time < 300) {
			continue;
		}

		try {
			std::unique_ptr<sql::Statement> stmt(con->_con->createStatement());
			stmt->executeQuery("SELECT 1");
			con->_last_oper_time = timestamp;
			std::cout << "execute timer alive query , cur is " << timestamp << std::endl;
		} catch (sql::SQLException& e) {
			std::cout << "Error keeping connection alive: " << e.what() << std::endl;
			// ���´������Ӳ��滻�ɵ�����
			sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
			sql::Connection* newcon = driver->connect(url_, user_, pass_);
			newcon->setSchema(schema_);
			con->_con.reset(newcon);
			con->_last_oper_time = timestamp;
		}
	}
}

std::unique_ptr<SqlConnection> MySqlPool::getConnection() {
	std::unique_lock<std::mutex> lock(mutex_);
	cond_.wait(lock, [this] {
		if (b_stop_) {
			return true;
		}
		return !pool_.empty();
	});
	if (b_stop_) {
		return nullptr;
	}
	std::unique_ptr<SqlConnection> con(std::move(pool_.front()));
	pool_.pop();
	return con;
}

void MySqlPool::reclaimConnection(std::unique_ptr<SqlConnection> con) {
	std::unique_lock<std::mutex> lock(mutex_);
	if (b_stop_) {
		return;
	}
	pool_.push(std::move(con));
	cond_.notify_one();
}

void MySqlPool::Close() {
	b_stop_ = true;
	cond_.notify_all();
}

MySqlPool::~MySqlPool() {
	std::unique_lock<std::mutex> lock(mutex_);
	while (!pool_.empty()) {
		pool_.pop();
	}
}

MysqlDao::MysqlDao() {
	auto& gConfigMgr = ConfigMgr::Inst();
	const auto& host = gConfigMgr["Mysql"]["Host"];
	const auto& port = gConfigMgr["Mysql"]["Port"];
	const auto& user = gConfigMgr["Mysql"]["User"];
	const auto& pwd = gConfigMgr["Mysql"]["Passwd"];
	const auto& schema = gConfigMgr["Mysql"]["Schema"];
	pool_.reset(new MySqlPool(host + ":" + port, user, pwd, schema, 5));
}

MysqlDao::~MysqlDao() {
	pool_->Close();
}

int MysqlDao::RegUser(const std::string& name, const std::string& email, const std::string& pwd) {
	auto con = pool_->getConnection();
	try {
		if (con == nullptr) {
			return false;
		}
		// ׼�����ô洢����
		std::unique_ptr<sql::PreparedStatement> stmt(con->_con->prepareStatement("CALL reg_user(?,?,?,@result)"));
		// �����������
		stmt->setString(1, name);
		stmt->setString(2, email);
		stmt->setString(3, pwd);

		// ����PreparedStatement��ֱ��֧��ע�����������������Ҫʹ�ûỰ������������������ȡ���������ֵ

		// ִ�д洢����
		stmt->execute();
		// ����洢���������˻Ự��������������ʽ��ȡ���������ֵ�������������ִ��SELECT��ѯ����ȡ����
		// ���磬����洢����������һ���Ự����@result���洢������������������ȡ��
		std::unique_ptr<sql::Statement> stmtResult(con->_con->createStatement());
		std::unique_ptr<sql::ResultSet> res(stmtResult->executeQuery("SELECT @result AS result"));
		if (res->next()) {
			int result = res->getInt("result");
			std::cout << "Result: " << result << std::endl;
			pool_->reclaimConnection(std::move(con));
			return result;
		}
		pool_->reclaimConnection(std::move(con));
		return -1;
	} catch (sql::SQLException& e) {
		pool_->reclaimConnection(std::move(con));
		std::cerr << "SQLException: " << e.what();
		std::cerr << " (MySQL error code: " << e.getErrorCode();
		std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		return -1;
	}
}