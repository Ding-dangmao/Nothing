# 奇怪的想法

## 搜索

对于查询数据进行数据的字符拆分,通过拆分后的结果依次查询, 并对每个结果进行查询次数的权值叠加。然后展示,当然权值之间有差异。完整字符串权值为最高,其余依次递减。

模糊搜索的进阶版(复杂度堪忧啊!)

~~~C++
#include<mysql/jdbc.h>
#include<iostream>
#include<cmath>
#include"mysqlControl.h"
using namespace std;
#define BASE_NUM 2
#define F(x) pow(BASE_NUM,x)
MysqlControl con;

void init() {
	con.prestmt = con.con->prepareStatement("update value set val=0");
	con.prestmt->execute();
}
//结果叠加
void modify(const string& name,const int& val) {
	con.prestmt = con.con->prepareStatement("update value set val=val+? where name=?");
	con.prestmt->setInt(1, val);
	con.prestmt->setString(2, name);
	con.prestmt->execute();
}
//初步查询
void search(const string& st,const int& val) {

	string s = " select * from search where name like '%";
	s = s + st + "%'";
	con.prestmt = con.con->prepareStatement(s);
	con.res = con.prestmt->executeQuery();
	while (con.res->next()) {
		//cout << con.res->getString("name") << endl;
		modify(con.res->getString("name"), val);
	}
}
//最终查询结果
void getAns() {
	con.prestmt = con.con->prepareStatement("select * from value order by val DESC");
	con.res = con.prestmt->executeQuery();
	while (con.res->next()) {
		cout << con.res->getInt(1) << ' ' << con.res->getString(2) <<' '<<con.res->getInt(3)<< endl;
	}
}
//字符串拆解
void splitString(string s)
{
	init();
	for (int i = 1; i <=s.size(); i++)
		for (int j = 0; j < s.size(); j++)
		{
			if (i + j  <= s.size()) {
				search(s.substr(j, i),F(i));
			}
		}
	getAns();
}

int main() {
	splitString("lucky_boy");
	return 0;
}
~~~

## 线程池

​	为避免重复创建销毁线程造成的资源时间消耗

- 线程组
- 总任务队列
- 线程任务队列
- 任务分配线程
- 任务处理线程
- 任务接收线程