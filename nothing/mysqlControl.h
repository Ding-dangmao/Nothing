#ifndef MYSQL_CONTROL
#define MYSQL_CONTROL
#include<mysql/jdbc.h>
class MysqlControl
{
public:
	MysqlControl() {
		driver = sql::mysql::get_mysql_driver_instance();
		con=driver->connect("tcp://127.0.0.1:3306", "root", "123456");
		con->setSchema("guess");
	}
	~MysqlControl() {
		delete con;
		delete stmt;
		delete prestmt;
		delete res;
	}
private:
	sql::mysql::MySQL_Driver* driver;
public:
	sql::Connection* con;
	sql::Statement* stmt;
	sql::PreparedStatement* prestmt;
	sql::ResultSet* res;
};

#endif // !1
