#include<iostream>
#include<fstream>
#include<unordered_map>
#include<string>
#include<stack>
#include<algorithm>
#include<cmath>
#include<queue>
#include<string>
#include<algorithm>
#include<vector>
using namespace std;

typedef pair<int, string> PIS;
typedef pair<pair<int, int>, char > PPC;
typedef pair<string, string> PSS;
vector<PIS> token;

//判断字符是否为字母
bool isalpha(char ch) {
    if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) return true;
    return false;
}


//判断字符是否为数字
bool isnum(char ch) {
    if (ch >= '0' && ch <= '9') return true;
    return false;
}

//打印token列表
void print() {
    for (auto a : token) cout << '<' << a.first << ',' << a.second << '>' << endl;
}

//哈希表存保留字
unordered_map<string, bool> is_reserved;
unordered_map<string, int> word_value;
unordered_map<int, string> value_word;
vector<string> reversed_word;

//初始化，保留关键字
void init() {
    ifstream ifs;
    ifs.open("test.txt");
    string tmp;
    int cnt = 1;
    while (ifs >> tmp) {
        reversed_word.push_back(tmp);
        is_reserved[tmp] = 1;
        word_value[tmp] = cnt++;
    }
    ifs.close();
    //for (int i = 0; i < reversed_word.size(); i++)	cout << word_value[reversed_word[i]] << " " << reversed_word[i] << endl;
    is_reserved["TYPE"] = 1;
    is_reserved["epsilon"] = 1;
}


//词法分析
class word_calculate {
    public:

        //36是ID，37是NUM，38是LETTER
        PIS word(string tmp) {
            PIS ret{ -1, "" };
            if (tmp.empty()) return ret;
            if (isalpha(tmp[0])) ret.first = 36;
            else ret.first = 37;
            int k = ret.first;
            for (int i = 1; i < tmp.size(); i++) {
                char ch = tmp[i];
                if ((!isalpha(ch) && !isnum(ch)) || (k == 8 && isalpha(ch))) {
                    ret.first = -1;
                    return ret;
                }
            }
            ret.second = tmp;
            return ret;
        }

        bool read(string s) {
            int cur = 0;
            string tmp;
            while (cur < s.size()) {
                while (cur < s.size() && s[cur] == ' ') cur++;
                if (cur < s.size()) {

                    if (!isalpha(s[cur]) && !isnum(s[cur])) {
                        tmp += s[cur++];
                        //字符常量
                        if (tmp[0] == '\'') {
                            while (cur < s.size() && s[cur] != '\'') tmp += s[cur++];
                            if (cur < s.size()) {
                                tmp += s[cur++];
                            }
                            else {
                                cout << "字符常量错误";
                                return false;
                            }
                            PIS p{ 38, tmp };
                            token.push_back(p);
                        }
                        else {
                            //注释
                            if (tmp[0] == '/' && cur < s.size() && s[cur] == '*') {
                                while (cur < s.size() && !(s[cur] == '/' && s[cur - 1] == '*')) cur++;
                                cur++;
                                if (cur >= s.size()) {
                                    cout << "注释错误" << endl;
                                    return false;
                                }
                            }
                            else {
                                //其他分隔符或者运算符
                                if ((tmp[0] == '.' && cur < s.size() && s[cur] == '.') || (tmp[0] == ':' && cur < s.size() && s[cur] == '=') || (tmp[0] == '<' && cur < s.size() && s[cur] == '=') || (tmp[0] == '>' && cur < s.size() && s[cur] == '=') || (tmp[0] == '<' && cur < s.size() && s[cur] == '>')) {
                                    tmp += s[cur++];
                                    PIS p{ word_value[tmp], tmp };
                                    token.push_back(p);
                                }
                                else {
                                    PIS p{ word_value[tmp], tmp };
                                    token.push_back(p);
                                }
                            }
                        }
                    }


                    else {
                        while (cur < s.size() && (isalpha(s[cur]) || isnum(s[cur]))) tmp += s[cur++];
                        if (is_reserved[tmp]) token.push_back({ word_value[tmp], tmp });
                        else {
                            PIS p = word(tmp);
                            if (p.first == -1) {
                                cout << "整数格式错误" << endl;
                                return false;
                            }
                            else token.push_back(p);
                        }
                    }
                    tmp.clear();
                }
            }
            return true;
        }
        void test_1() {
            ifstream ifs;
            ifs.open("example.txt");
            string str, tmp;
            while (getline(ifs, tmp)) str += tmp, str += ' ';
            ifs.close();
            read(str);
        }

        void test_2() {
            ifstream ifs;
            ifs.open("example1.txt");
            string str, tmp;
            while (getline(ifs, tmp)) str += tmp, str += ' ';
            ifs.close();
            read(str);
        }

}Word;
int main() {
    init();
    int test_num;
    cout << "张博    20级计科2班   " << endl;
    cout << "请输入测试程序编号 1 / 2 / 3 / 4 /, 1,2 为词法分析";
    cin >> test_num;
    if (test_num == 1) Word.test_1(), print();
    if (test_num == 2) Word.test_2(), print();
    token.clear();
    cout << "已完成";
    return 0;
}

