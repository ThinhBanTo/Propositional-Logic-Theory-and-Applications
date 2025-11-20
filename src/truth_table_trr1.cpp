#include <bits/stdc++.h>
using namespace std;
#define pb push_back
#define MASK(i) (1LL << (i))
#define sz(v) ((int)(v).size())
#define BIT(x, i) (((x) >> (i)) & 1)
#define REP(i, n) for (int i = 0, _n = (n); i < _n; ++i)
#define FOR(i, a, b) for (int i = (a), _b = (b); i <= _b; ++i) 
#define FORD(i, a, b) for (int i = (a), _b = (b); i >= _b; --i) 

map<string, int> val, mp;
string s; int n;

int calc(string s) {
    stringstream ss(s);
    string word;
    int Not = 0, res = 0;
    vector<string> v;

    while (ss >> word) {
        if (val.count(word)) {
            int Val = val[word];
            if (!sz(v)) {
                res = Val;
                continue;
            }
            if (v[0] == "NOT")        res = Val ^ 1;
            if (v[0] == "AND")        res &= Val;
            if (v[0] == "OR")         res |= Val;
            if (v[0] == "IMPLIES")    res = !res || Val;
            if (v[0] == "EQUIVALENT") res = res == Val;
            if (v[0] == "XOR")        res ^= Val;
            v.clear();
        }
        else v.pb(word);
    }
    return res;	
}

string priorityCalc(string tmp, string Logic) {
    while (1) {
        int lop = 0, id = 0;
        while (id < sz(tmp)) {
            if (tmp[id] >= 'A' && tmp[id] <= 'Z') {
                string s1 = "";
                int nxt;
                FOR(j, id, sz(tmp) - 1) {
                    if (tmp[j] >= 'A' && tmp[id] <= 'Z') s1 += tmp[j], nxt = j;
                    else break;
                }
                if (s1 == Logic) {

                    string t = tmp.substr(id - 2, nxt + 2 - (id - 2) + 1);
                    int res = calc(t);
                    string sub = to_string(res);
                    tmp.erase(id - 2, nxt + 2 - (id - 2) + 1);
                    tmp.insert(id - 2, sub);

                    lop = 1;
                    break;
                }
                id = nxt + 1;
            }
            else ++id;
        }
        if (!lop) break;
    }
    return tmp;
}

int process(string s) {
    while (1) {
        int k = 0, mx = 0;
        REP(i, sz(s)) if (s[i] == '(' || s[i] == ')') {
            k += (s[i] == '(' ? 1 : -1);
            mx = max(mx, k);
        }

        REP(i, sz(s)) if (s[i] == '(' || s[i] == ')') {
            k += (s[i] == '(' ? 1 : -1);
            if (k == mx) {
                int l = i, r;
                FOR(j, l, sz(s) - 1) if (s[j] == ')') {
                    r = j;
                    break;
                }
                string tmp = s.substr(i + 1, r - l - 1);
                for (string logic : {"AND", "OR", "XOR", "IMPLIES", "EQUIVALENT"})
					tmp = priorityCalc(tmp, logic);

                int res = calc(tmp);
                string sub = to_string(res);
                s.erase(i, r - l + 1);
                s.insert(i, sub);
                break;
            }
        }
        if (!mx) break;
    }

    for (string logic : {"AND", "OR", "XOR", "IMPLIES", "EQUIVALENT"})
					s = priorityCalc(s, logic);
    return calc(s);
}

void Solve(int n_thTest) {
    
    cout << "- Nhap bieu thuc thu " << n_thTest << " : W = ";
    getline(cin, s);
    
    string S = s;
    stringstream ss(S);
    string word;
    vector<string> Var;
    while (ss >> word) {
        while (word[0] == '(') word.erase(0, 1);
        while (word.back() == ')') word.pop_back();
        if (sz(word) == 1) {
             ++mp[word];
             if (mp[word] == 1) Var.pb(word);
        }
    }
    
    n = sz(mp);
    
    cout << "- Bang chan tri: " << endl;
    cout << "  ";
    for (auto w : Var) cout << w << "     ";
    cout << "W" << endl;
    cout << ' '; REP(i, 6 * sz(Var) + 3) cout << '-'; cout << endl;

    int cnt1 = 0, cnt0 = 0;
    FORD(i, MASK(n) - 1, 0) {
        int p = n;
        cout << "  ";
        for (auto w : Var) {
            --p;
            val[w] = 0;
            if (BIT(i, p)) val[w] = 1;
            cout << (val[w] == 1 ? "T" : "F") << "     ";
        }

        int res = process(s);
        cnt1 += res == 1;
        cnt0 += res == 0;
        cout << (res == 1 ? "T" : "F") << endl;
    }

    if (cnt1 == MASK(n)) cout << "=> Tautology!" << endl;
    else if (cnt0 == MASK(n)) cout << "=> Contradiction!" << endl;
    else cout << "=> Contingency!" << endl;
    cout << endl;
}

signed main() {
    cout << "- Nhap so bieu thuc can thuc hien: ";
    int t; cin >> t; 
    cin.ignore();
    REP(i, t) {
        val.clear();
        val["1"] = 1;
        val["0"] = 0;
        mp.clear();
        Solve(i + 1);
    }
}
