#include "assist_func2.h"


//s为待分割串，按照delim中的各种字符分割，结果保存在ans中
void stringSplit(string s,string delim,vector<string> &ans)
{
	string::size_type pos_1,pos_2=0;
	while(pos_2!=s.npos){
	pos_1=s.find_first_not_of(delim,pos_2);
	if(pos_1==s.npos) break;
	pos_2=s.find_first_of(delim,pos_1);
	ans.push_back(s.substr(pos_1,pos_2-pos_1));
	}
}