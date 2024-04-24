#include<mysql/jdbc.h>
#include<iostream>
#include<vector>
#include<cmath>
#include"mysqlControl.h"
using namespace std;
vector<string>w;
#define BASE_NUM 2
#define F(x) pow(BASE_NUM,x)
MysqlControl con;

void init() {
	con.prestmt = con.con->prepareStatement("update value set val=0");
	con.prestmt->execute();
}
//�������
void modify(const string& name,const int& val) {
	con.prestmt = con.con->prepareStatement("update value set val=val+? where name=?");
	con.prestmt->setInt(1, val);
	con.prestmt->setString(2, name);
	con.prestmt->execute();
}
//������ѯ
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
//���ղ�ѯ���
void getAns() {
	con.prestmt = con.con->prepareStatement("select * from value order by val DESC");
	con.res = con.prestmt->executeQuery();
	while (con.res->next()) {
		cout << con.res->getInt(1) << ' ' << con.res->getString(2) <<' '<<con.res->getInt(3)<< endl;
	}
}
//�ַ������
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