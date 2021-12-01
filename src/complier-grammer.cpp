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

class WenFa {
    public:
        string left;
        vector<string> right;
};

class Grammer {
    public:
        vector<WenFa> wenfas;
        unordered_map<string, vector<WenFa>> vn;
        unordered_map<string, vector<string>> First_set;
        int TMP_CNT = 1;
        struct FOUR {
            public:
                vector<string> val;
                FOUR() {
                    for (int i = 0; i < 4; i++) val.push_back("-");
                }
                FOUR(vector<string> a) {
                    val = a;
                }
        };




        Grammer() {
            //把文法打进去，别问我为什么不用文件输入
            WenFa tmp = { {"P"}, {"program", "ID", ";", "EP", "MSEN"} };
            wenfas.push_back(tmp);
            tmp = { {"EP"}, {"var", "DF"} };
            wenfas.push_back(tmp);
            tmp = { {"EP"}, {"epsilon"} };
            wenfas.push_back(tmp);
            tmp = { {"DF"}, {"IDM", ":", "TYPE", ";", "DF"} };
            wenfas.push_back(tmp);
            tmp = { {"DF"}, {"IDM", ":", "TYPE", ";"} };
            wenfas.push_back(tmp);
            tmp = { {"IDM"}, {"ID", ",", "IDM"} };
            wenfas.push_back(tmp);
            tmp = { {"IDM"}, {"ID"} };
            wenfas.push_back(tmp);
            tmp = { {"MSEN"}, {"begin", "SENM", "end"} };
            wenfas.push_back(tmp);
            tmp = { {"SENM"}, {"SEN", ";", "SENM"} };
            wenfas.push_back(tmp);
            tmp = { {"SENM"}, {"SEN"} };
            wenfas.push_back(tmp);
            tmp = { {"SEN"}, {"ASS"} };
            wenfas.push_back(tmp);
            tmp = { {"SEN"}, {"IFS"} };
            wenfas.push_back(tmp);
            tmp = { {"SEN"}, {"WHI"} };
            wenfas.push_back(tmp);
            tmp = { {"SEN"}, {"REP"} };
            wenfas.push_back(tmp);
            tmp = { {"SEN"}, {"MSEN"} };
            wenfas.push_back(tmp);
            tmp = { {"ASS"}, {"ID", ":=", "NUMEX"} };
            wenfas.push_back(tmp);
            tmp = { {"IFS"}, {"if", "BOOLEX", "then", "SEN"} };
            wenfas.push_back(tmp);
            tmp = { {"IFS"}, {"if", "BOOLEX", "then", "SEN", "else", "SEN"} };
            wenfas.push_back(tmp);
            tmp = { {"WHI"}, {"while", "BOOLEX", "do", "SEN"} };
            wenfas.push_back(tmp);
            tmp = { {"REP"}, {"repeat", "SEN", "until", "BOOLEX"} };
            wenfas.push_back(tmp);
            tmp = { {"NUMEX"}, {"NUMEX", "+", "X"} };
            wenfas.push_back(tmp);
            tmp = { {"NUMEX"}, {"NUMEX", "-", "X"} };
            wenfas.push_back(tmp);
            tmp = { {"NUMEX"}, {"X"} };
            wenfas.push_back(tmp);
            tmp = { {"X"}, {"YZ", "X1"} };
            wenfas.push_back(tmp);
            tmp = { {"X1"}, { "*", "YZ", "X1"} };
            wenfas.push_back(tmp);
            tmp = { {"X1"}, { "/", "YZ", "X1"} };
            wenfas.push_back(tmp);
            tmp = { {"YZ"}, {"CAL"} };
            wenfas.push_back(tmp);
            tmp = { {"YZ"}, {"-", "YZ"} };
            wenfas.push_back(tmp);
            tmp = { {"CAL"}, {"NUM"} };
            wenfas.push_back(tmp);
            tmp = { {"CAL"}, {"ID"} };
            wenfas.push_back(tmp);
            tmp = { {"CAL"}, {"(", "NUMEX", ")"} };
            wenfas.push_back(tmp);
            tmp = { {"BOOLEX"}, {"BOOLEX", "or", "BOOLX"} };
            wenfas.push_back(tmp);
            tmp = { {"BOOLEX"}, {"BOOLX"} };
            wenfas.push_back(tmp);
            tmp = { {"BOOLX"}, {"BOOLX", "and", "BOOLYZ"} };
            wenfas.push_back(tmp);
            tmp = { {"BOOLX"}, {"BOOLYZ"} };
            wenfas.push_back(tmp);
            tmp = { {"BOOLYZ"}, {"BOOLL"} };
            wenfas.push_back(tmp);
            tmp = { {"BOOLYZ"}, {"not", "BOOLYZ"} };
            wenfas.push_back(tmp);
            tmp = { {"BOOLL"}, {"bool"} };
            wenfas.push_back(tmp);
            tmp = { {"BOOLL"}, {"ID"} };
            wenfas.push_back(tmp);
            tmp = { {"BOOLL"}, {"(", "BOOLEX", ")"} };
            wenfas.push_back(tmp);
            tmp = { {"BOOLL"}, {"ID", "RELATION", "ID"} };
            wenfas.push_back(tmp);
            tmp = { {"BOOLL"}, {"NUMEX", "RELATION", "NUMEX"} };
            wenfas.push_back(tmp);

            for (auto a : wenfas) vn[a.left].push_back(a);

        }

        //得到first集合
        //得到第一个终结符
        vector<string> get_vt(string a, unordered_map<string, bool>& mark) {
            vector<string> ret;
            if (mark[a]) return ret;
            mark[a] = true;
            if (is_reserved[a]) {
                ret.push_back(a);
                return ret;
            }

            for (auto wenfa_tmp : vn[a]) {
                vector<string> next = get_vt(wenfa_tmp.right[0], mark);
                for (auto b : next) {
                    if (find(ret.begin(), ret.end(), b) == ret.end()) ret.push_back(b);
                }
            }
            return ret;
        }
        void get_first() {
            for (auto a : vn) {
                string cur = a.first;
                for (auto wenfa_tmp : a.second) {
                    unordered_map<string, bool> mark_tmp;
                    vector<string> get_vt_tmp = get_vt(wenfa_tmp.right[0], mark_tmp);
                    for (auto b : get_vt_tmp) {
                        if(find(First_set[cur].begin(), First_set[cur].end() , b) == First_set[cur].end()) First_set[cur].push_back(b);
                    }
                }
            }
        }

        //记录当前位置
        int token_pos = 0;

        //记录id相关信息
        unordered_map<string, string> ID_TYPE;
        unordered_map<string, int> Used_id;
        void Print_id() {
            for (PSS a : ID_TYPE) {
                cout << a.first << "     " << a.second << endl;
            }
            cout << endl;
        }

        vector<FOUR> ret;
        void Print_Four() {
            for (int i = 0; i < ret.size(); i++) {
                if (ret[i].val[3] == "wait") ret[i].val[3] = to_string(ret.size() - 1);
                cout << "(" << i << ") " << "(" << ret[i].val[0] << ", " << ret[i].val[1] << ", " << ret[i].val[2] << ", " << ret[i].val[3] << ")" << endl;
            }
        }

        bool DOWN(){
            P();
            if (token_pos == token.size()) {
                cout << "AC" << endl;
                cout << endl;
                cout << "Curent ID is followed\n";
                Print_id();
                vector<string> STMP = { "sys", "-", "-", "-" };
                ret.push_back(STMP);
                Print_Four();
                cout << endl;
                return true;
            }
            else {
                cout << "WA" << endl;
                return false;
            }

        }

        //P -> program ID ; EP MSEN
        void P() {
            if (token[token_pos].first == word_value["program"]) {
                token_pos++;
            }
            else {
                cout << "P -> program ID ; EP MSEN WRONG" << endl;
                return;
            }
            string CUR_ID;
            if (token[token_pos].first == word_value["ID"]) {
                CUR_ID = token[token_pos].second;
                if (Used_id.find(CUR_ID) != Used_id.end()) {
                    cout << CUR_ID << " is already used" << endl;
                    return;
                }
                Used_id[CUR_ID] = ID_TYPE.size();
                PSS PSS_TMP = { token[token_pos].second, "program"};
                ID_TYPE[CUR_ID] = "program";
            }
            else {
                cout << "P -> program ID ; EP MSEN WRONG" << endl;
                return;
            }
            vector<string> ret_tmp({ "program",CUR_ID,"-","-" });
            FOUR ftmp(ret_tmp);
            ret.push_back(ftmp);
            token_pos++;
            if (token[token_pos].first == word_value[";"]) token_pos++;
            else {
                cout << "P -> program ID ; EP MSEN WRONG" << endl;
                return;
            };
            if (token[token_pos].first == word_value["var"]) {
                EP();
            }
            MSEN();
        }

        //EP -> var DF
        void EP() {
            if (token[token_pos].first == word_value["var"]) {
                token_pos++;
            }
            else {
                cout << "EP -> var DF WRONG" << endl;
                return;
            }
            DF();
        }

        //EP->epsilon,不用下降

        //DF->IDM : TYPE; DF
        //DF->IDM : TYPE;
        //IDM->ID, IDM
        //IDM->ID

        void DF() {
            if (token[token_pos].first == word_value["ID"]) {
                //token_pos++;
            }
            else {
                cout << "DF WRONG" << endl;
                return;
            }
            int mark_pos = token_pos;
            while (mark_pos < token.size() && token[mark_pos].first != word_value[":"]) mark_pos++;
            if (mark_pos == token.size()) {
                cout << "DF WRONG" << endl;
                return;
            }
            mark_pos++;
            if (mark_pos == token.size()) {
                cout << "DF WRONG" << endl;
                return;
            }
            string type = token[mark_pos].second;
            mark_pos += 2;
            if (mark_pos >= token.size()) {
                cout << "DF WRONG" << endl;
                return;
            }
            while (token_pos < mark_pos) {
                if (token[token_pos].first == word_value["ID"]) {
                    string CUR_ID = token[token_pos].second;
                    if (ID_TYPE.find(CUR_ID) != ID_TYPE.end()) {
                        cout << CUR_ID << " is already used" << endl;
                        return;
                    }
                    Used_id[CUR_ID] = ID_TYPE.size();
                    ID_TYPE[CUR_ID] = type;
                }
                token_pos++;
            }
            if (token[token_pos].first == word_value["var"]) {
                token_pos++;
                DF();
            }

        }


        //MSEN->begin SENM end.

        void MSEN() {
            if (token[token_pos].first == word_value["begin"]) {
                token_pos++;
            }
            else {
                cout << "MSEN->begin SENM end WROND" << endl;
                return;
            }

            SENM();

            if (token_pos < token.size()&&token[token_pos].first == word_value["end"]) {
                token_pos++;
                if (token_pos < token.size() && token[token_pos].second == ".") {
                    token_pos++;
                }
                else {
                    cout << "MSEN->begin SENM end WROND, 没." << endl;
                    return;
                }
            }
            else {
                cout << "MSEN->begin SENM end WROND" << endl;
                return;
            }
        }

        //SENM->SEN; SENM
        //SENM->SEN end

        void SENM() {
            SEN();
            if (token_pos < token.size()) {
                while (token[token_pos].first == word_value[";"]) {
                    token_pos++;
                    SENM();
                }
                if (token[token_pos].second == "end") {
                    return;
                }		
            }
            else {
                cout << "SENM WROND" << endl;
                return;
            }
        }

        //SEN->ASS
        //SEN->IFS
        //SEN->WHI
        //SEN->REP
        //SEN->MSEN

        void SEN() {
            if (token[token_pos].first == word_value["if"]) {
                IFS();
            }
            else if (token[token_pos].first == word_value["while"]) {
                WHI();
            }
            else if (token[token_pos].first == word_value["repeat"]) {
                REP();
            }
            else if (token[token_pos].first == word_value["ID"]) {
                ASS();
            }
            else if (token[token_pos].first == word_value["begin"]) {
                MSEN();
            }
            else cout << "SEN WRONG" << endl;
        }

        //ASS->ID := NUMEX
        int ADD_CNT = 1;

        stack<string> YuanSu;
        vector<FOUR> FourTmp;

        void ASS() {
            string Cur_ID = token[token_pos].second;
            if (ID_TYPE.find(Cur_ID) == ID_TYPE.end()) {
                cout << Cur_ID << " is not defined" << endl;
                return;
            }
            string type1 = ID_TYPE[Cur_ID];

            vector<string> RET_TMP{ ":=", "-", "-", Cur_ID };
            token_pos++;
            if (token_pos < token.size() && token[token_pos].first == word_value[":="]) {
                token_pos++;
                if (token[token_pos].first == word_value["ID"]) {
                    if (ID_TYPE.find(token[token_pos].second) == ID_TYPE.end()) {
                        cout << token[token_pos].second << " is not defined" << endl;
                        return;
                    }
                    string type2 = ID_TYPE[token[token_pos].second];

                    if (type1 != type2) {
                        cout << Cur_ID << " and " << token[token_pos].second << " not match" << endl;
                        return;
                    }
                }
                else if (token[token_pos].first == word_value["NUM"]) {
                    string type2 = "integer";
                    if (type1 != type2) {
                        cout << Cur_ID << " and " << token[token_pos].second << " not match" << endl;
                        return;
                    }
                }
                else if (token[token_pos].second == "true" || token[token_pos].second == "false") {
                    string type2 = "bool";
                    if (type1 != type2) {
                        cout << Cur_ID << " and " << token[token_pos].second << " not match" << endl;
                        return;
                    }
                }
            }
            else {
                cout << "ASS WROND" << endl;
                return;
            }
            if (token_pos >= token.size()) cout << "ASS WRONG" << endl;
            else {
                NUMEX();
                int ii = 1;
                while (!YuanSu.empty()) {
                    int MM = 1;
                    while (!YuanSu.empty() && MM <= 2) {
                        RET_TMP[MM] = YuanSu.top();
                        YuanSu.pop();
                        MM++;
                    }
                    if (MM == 3) {
                        string ttmp = RET_TMP[1];
                        RET_TMP[1] = RET_TMP[2];
                        RET_TMP[2] = ttmp;
                    }
                    ret.push_back(RET_TMP);
                }
            }
        }

        //IFS -> if BOOLEX then SEN
        //IFS -> if BOOLEX then SEN else SEN

        void IFS() {
            token_pos++;
            if (token_pos >= token.size()) cout << "IFS WRONG" << endl;
            else	BOOLEX();
            if (token_pos < token.size() && token[token_pos].first == word_value["then"]) {

                token_pos++;
            }
            else {
                cout << "IFS WRONG" << endl;
                return;
            }

            if (token_pos >= token.size()) cout << "IFS WRONG" << endl;
            else	SEN();
            bool add = 1;
            if (!WHI_POS.empty()) {
                stack<int> ST = WHI_POS;
                while (!ST.empty()) {
                    vector<string> STMP = { "j", "-", "-", "wait" };
                    string k = to_string(ST.top());
                    ST.pop();
                    STMP[3] = k;
                    ret.push_back(STMP);
                }
                add = 0;
            }




            if (token_pos < token.size() && token[token_pos].first == word_value["else"]) {
                token_pos++;
                for (int i = ret.size() - 1; 1; i--) {
                    if (ret[i].val[3] == "wait") {
                        int mark = ret.size();
                        if (add) mark++;
                        ret[i].val[3] = to_string(mark);
                        break;
                    }
                }
                if (add) {
                    vector<string> STMP = { "j", "-", "-", "wait" };
                    ret.push_back(STMP);
                }
                if (token_pos >= token.size()) cout << "IFS WRONG" << endl;
                else	SEN();
                if (!WHI_POS.empty()) {
                    stack<int> ST = WHI_POS;
                    while (!ST.empty()) {
                        vector<string> STMP = { "j", "-", "-", "wait" };
                        string k = to_string(ST.top());
                        ST.pop();
                        STMP[3] = k;
                        ret.push_back(STMP);
                    }
                }
            }
        }

        //WHI -> while BOOLEX do SEN
        int backup = -1;
        bool is_relation(string aaa) {
            if(aaa == ">" || aaa == "<" || aaa == ">=" || aaa == "<=" || aaa == "=") return true;
            return false;
        }
        stack<int> WHI_POS;

        void WHI() {
            int MARKP = ret.size();

            token_pos++;
            if (token_pos >= token.size()) cout << "WHI WRONG" << endl;
            else	BOOLEX();
            if (ret.back().val[3] == "wait" && !WHI_POS.empty()) ret.back().val[3] = to_string(WHI_POS.top());
            WHI_POS.push(MARKP);
            if (token_pos < token.size() && token[token_pos].first == word_value["do"]) {

                token_pos++;
            }
            else {
                cout << "WHI WRONG" << endl;
            }
            if (token_pos >= token.size()) cout << "WHI WRONG" << endl;
            else	SEN();
            if (!WHI_POS.empty()) {
                stack<int> ST = WHI_POS;
                while (ST.size() > 1) {
                    vector<string> STMP = { "j", "-", "-", "wait" };
                    string k = to_string(ST.top());
                    ST.pop();
                    STMP[3] = k;
                    ret.push_back(STMP);
                }
            }
            WHI_POS.pop();
        }
        stack<int> REP_POS;
        //REP->repeat SEN until BOOLEX
        void REP() {
            REP_POS.push(ret.size());
            if (token_pos < token.size() && token[token_pos].first == word_value["repeat"]) {
                token_pos++;
            }
            else {
                cout << "REP WRONG" << endl;
            }
            if (token_pos >= token.size()) cout << "REP WRONG" << endl;
            else	SEN();

            if (token_pos < token.size() && token[token_pos].first == word_value["until"]) {
                token_pos++;
            }
            else {
                cout << "REP WRONG" << endl;
            }
            if (token_pos >= token.size()) cout << "REP WRONG" << endl;
            else	BOOLEX();
            if (!REP_POS.empty()) {
                for (int i = ret.size() - 1; i >= 0; i--) {
                    if (ret[i].val[3] == "wait") {
                        ret[i].val[3] = to_string(REP_POS.top());
                        break;
                    }
                }
            }
            REP_POS.pop();
        }

        //NUMEX->X NUMEX1
        //NUMEX1 -> + X NUMEX1
        //NUMEX1 -> - X NUMEX1
        //NUMEX1->epsilon
        void NUMEX() {
            X();
            NUMEX1();
        }

        void NUMEX1() {
            if (token[token_pos].second == "+" || token[token_pos].second == "-") {
                string fuhao = token[token_pos].second;
                string type1 = "bool";
                if (token[token_pos - 2].first == word_value["ID"]) {
                    string Cur_ID = token[token_pos - 2].second;
                    if (ID_TYPE.find(token[token_pos - 2].second) == ID_TYPE.end()) {
                        cout << Cur_ID << " is not defined" << endl;
                        return;
                    }
                    type1 = ID_TYPE[Cur_ID];
                }
                else if (token[token_pos - 2].first == word_value["NUM"]) type1 = "integer";

                if (token[token_pos].first == word_value["ID"]) {
                    if (ID_TYPE.find(token[token_pos].second) == ID_TYPE.end()) {
                        cout << token[token_pos].second << " is not defined" << endl;
                        return;
                    }
                    string type2 = ID_TYPE[token[token_pos].second];
                    if (type1 != type2) {
                        cout << token[token_pos].second << " and " << token[token_pos].second << " not match" << endl;
                        return;
                    }
                }
                else if (token[token_pos].first == word_value["NUM"]) {
                    string type2 = "integer";
                    if (type1 != type2) {
                        cout << token[token_pos].second << " and " << token[token_pos].second << " not match" << endl;
                        return;
                    }
                }
                else if (token[token_pos].second == "true" || token[token_pos].second == "false") {
                    string type2 = "bool";
                    if (type1 != type2) {
                        cout << token[token_pos].second << " and " << token[token_pos].second << " not match" << endl;
                        return;
                    }
                }
                token_pos++;
                X();

                string stmp = "T";
                stmp += to_string(TMP_CNT);
                TMP_CNT++;
                string stmp1, stmp2;
                if (!YuanSu.empty()) {
                    stmp2 = YuanSu.top();
                    YuanSu.pop();
                }
                if (!YuanSu.empty()) {
                    stmp1 = YuanSu.top();
                    YuanSu.pop();
                }
                vector<string> ret_tmp{ fuhao,stmp1 ,stmp2, stmp };
                ret.push_back(FOUR(ret_tmp));
                YuanSu.push(stmp);

                NUMEX1();
            }
        }
        //X->YZ X1
        //X1 ->* YZ X1
        //X1 -> / YZ X1
        //X1->epsilon
        void X() {
            int Spos = token_pos;
            YZ();
            /*	string Cur_ID;
                string Cur_type = "integer";
                for (int i = 0; Spos < token_pos; Spos++) {
                Cur_ID += token[Spos].second;
                if (token[Spos].first == word_value["ID"]) {

                }
                }*/
            X1();
        }

        void X1() {
            if (token[token_pos].second == "*" || token[token_pos].second == "/") {
                string fuhao = token[token_pos].second;
                token_pos++;

                string type1 = "bool";
                if (token[token_pos - 2].first == word_value["ID"]) {
                    string Cur_ID = token[token_pos - 2].second;
                    if (ID_TYPE.find(token[token_pos - 2].second) == ID_TYPE.end()) {
                        cout << Cur_ID << " is not defined" << endl;
                        return;
                    }
                    type1 = ID_TYPE[Cur_ID];
                }
                else if (token[token_pos - 2].first == word_value["NUM"]) type1 = "integer";

                if (token[token_pos].first == word_value["ID"]) {
                    if (ID_TYPE.find(token[token_pos].second) == ID_TYPE.end()) {
                        cout << token[token_pos].second << " is not defined" << endl;
                        return;
                    }
                    string type2 = ID_TYPE[token[token_pos].second];
                    if (type1 != type2) {
                        cout << token[token_pos].second << " and " << token[token_pos].second << " not match" << endl;
                        return;
                    }
                }
                else if (token[token_pos].first == word_value["NUM"]) {
                    string type2 = "integer";
                    if (type1 != type2) {
                        cout << token[token_pos].second << " and " << token[token_pos].second << " not match" << endl;
                        return;
                    }
                }
                else if (token[token_pos].second == "true" || token[token_pos].second == "false") {
                    string type2 = "bool";
                    if (type1 != type2) {
                        cout << token[token_pos].second << " and " << token[token_pos].second << " not match" << endl;
                        return;
                    }
                }


                YZ();

                string stmp = "T";
                stmp += to_string(TMP_CNT);
                TMP_CNT++;
                string stmp1, stmp2;
                if (!YuanSu.empty()) {
                    stmp2 = YuanSu.top();
                    YuanSu.pop();
                }
                if (!YuanSu.empty()) {
                    stmp1 = YuanSu.top();
                    YuanSu.pop();
                }
                vector<string> ret_tmp{ fuhao,stmp1 ,stmp2, stmp };
                ret.push_back(FOUR(ret_tmp));
                YuanSu.push(stmp);

                X1();
            }
        }
        //YZ->CAL
        //YZ -> - YZ
        void YZ() {
            if (token[token_pos].second == "-") {
                token_pos++;

                YZ();
            }
            else CAL();
        }
        //CAL->NUM
        //CAL->ID
        //CAL -> (NUMEX)
        void CAL() {
            if (token[token_pos].first == word_value["NUM"]) {
                YuanSu.push(token[token_pos].second);
                token_pos++;
            }
            else if (token[token_pos].first == word_value["ID"]) {
                YuanSu.push(token[token_pos].second);
                token_pos++;
            }
            else if (token[token_pos].first == word_value["("]) {
                token_pos++;
                NUMEX();
                if (token[token_pos].second != ")") cout << "CAL WRONG" << endl;
                else token_pos++;
            }
            else cout << "CAL WRONG" << endl;
        }
        //BOOLEX->BOOLX BOOLEX1
        //BOOLEX1 -> or BOOLX BOOLEX1
        //BOOLEX1->epsilon
        void BOOLEX() {
            BOOLX();
            BOOLEX1();
        }
        void BOOLEX1() {
            if (token[token_pos].second == "or") {
                token_pos++;
                BOOLX();
                BOOLEX1();
            }
        }
        //BOOLX->BOOLYZ BOOLX1
        //BOOLX1 ->and BOOLYZ BOOLX1
        //BOOLX1->epsilon
        void BOOLX() {
            BOOLYZ();
            BOOLX1();
        }
        void BOOLX1() {
            if (token[token_pos].second == "and") {
                token_pos++;
                BOOLYZ();
                BOOLX1();
            }
        }

        //BOOLYZ->BOOLL
        //BOOLYZ -> not BOOLYZ
        void BOOLYZ() {
            if (token[token_pos].second == "not") {
                token_pos++;
                BOOLYZ();
            }
            else BOOLL();
        }

        //BOOLL -> bool
        //BOOLL->ID
        //BOOLL -> (BOOLEX)
        //BOOLL->ID RELATION ID
        //BOOLL->NUMEX RELATION NUMEX

        void BOOLL() {
            if (token[token_pos].second == "true" || token[token_pos].second == "false") {
                token_pos++;
            }
            else if (token[token_pos].first ==  word_value["ID"]) {
                token_pos++;
                if (token[token_pos].second == "=" || token[token_pos].second == ">" || token[token_pos].second == ">=" || token[token_pos].second == "<" || token[token_pos].second == "<=") {

                    string aaa = token[token_pos - 1].second, bbb = token[token_pos + 1].second;
                    int CUR_POS = ret.size();
                    string mark1 = "j";
                    mark1 += token[token_pos].second;
                    vector<string> STMP = { mark1, aaa, bbb, to_string(CUR_POS + 2) };
                    ret.push_back(STMP);
                    STMP = { "j", "-", "-", "wait" };
                    ret.push_back(STMP);

                    token_pos++;
                }
                token_pos++;
            }
            else if (token[token_pos].second == "(") {
                token_pos++;
                BOOLEX();
                if (token[token_pos].second ==")") {
                    token_pos++;
                }
                else {
                    cout << "BOOLL WRONG" << endl;
                    return;
                }
            }
            else {
                NUMEX();
                if (token[token_pos].second == "=" || token[token_pos].second == ">" || token[token_pos].second == ">=" || token[token_pos].second == "<" || token[token_pos].second == "<=") {

                    string aaa = token[token_pos - 1].second, bbb = token[token_pos + 1].second;
                    int CUR_POS = ret.size();
                    string mark1 = "j";
                    mark1 += token[token_pos].second;
                    vector<string> STMP = { mark1, aaa, bbb, to_string(CUR_POS + 2) };
                    ret.push_back(STMP);
                    STMP = { "j", "-", "-", "wait" };
                    ret.push_back(STMP);

                    token_pos++;
                }
                NUMEX();
            }

        }





        void test_3() {
            ifstream ifs;
            ifs.open("example2.txt");
            string str, tmp;
            while (getline(ifs, tmp)) str += tmp, str += ' ';
            ifs.close();
            if(Word.read(str)) DOWN();
        }

        void test_4() {
            ifstream ifs;
            ifs.open("example3.txt");
            string str, tmp;
            while (getline(ifs, tmp)) str += tmp, str += ' ';
            ifs.close();
            if(Word.read(str)) DOWN();
        }
}grammer;


int main() {
    init();
    int test_num;
    cout << "张博    20级计科2班    " << endl;
    cout << "请输入测试程序编号 1 / 2 / 3 / 4 /, 1,2为词法分析， 3,4 为语法分析 :   ";
    cin >> test_num;
    if (test_num == 1) Word.test_1(), print();
    if (test_num == 2) Word.test_2(), print();
    if (test_num == 3) grammer.test_3();
    if (test_num == 4) grammer.test_4();
    token.clear();
    cout << "已完成";
    return 0;
}

