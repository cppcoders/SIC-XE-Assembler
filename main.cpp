#include <bits/stdc++.h>
using namespace std;
map<string, pair<int, string>> OPTAB;
map<string, int> SYMTAB;
vector<pair<int, vector<string>>> prog;
map<string, string> REGS;
vector<pair<int, string>> obcode;
vector<int> modify;
string programName;
int LOCCTR = 0, base = 0, pc = 0, indx = 0;
bool checkpc = 0;
int wrdsize(int i)
{
    string s = prog[i].second[2] ;
    return s.length()/ 2 ;
}
int bytsize(int i)
{
    string adr = prog[i].second[2];
    if (adr[0] == 'C')
    {
        string s = adr.substr(2, adr.size() - 3);
        return s.size();
    }
    else if (adr[0] == 'X')
        return 1;
}

int hextoint(string hexstring)
{
    int number = (int)strtol(hexstring.c_str(), NULL, 16);
    return number;
}
string inttohex(int x, int b)
{
    string s;
    stringstream sstream;
    sstream << setfill('0') << setw(b) << hex << (int)x;
    s = sstream.str();
    sstream.clear();
    for (int i = 0; i < s.length(); i++)
        if (s[i] >= 97)
            s[i] -= 32;
    return s;
}
string bintohex(bool a, bool b, bool c, bool d)
{
    string s;
    int sum = 0;
    sum += (int)d * 1;
    sum += (int)c * 2;
    sum += (int)b * 4;
    sum += (int)a * 8;
    s = inttohex(sum, 1);
    return s;
}

string readdr(string res)
{
    int x = hextoint(res);
    if (x - pc > -256 && x - pc < 4096)
    {
        checkpc = 1;
        return inttohex(x - pc, 3);
    }
    else
    {
        checkpc = 0;
        return inttohex(x - base, 3);
    }
}

string format2(int i)
{
    string s, r1, r2 = "A", result;
    s = prog[i].second[2];
    int j;
    for (j = 0; j < s.size() && s[j] != ','; j++)
        ;
    r1 = s.substr(0, j);
    if (j < s.size())
        r2 = s.substr(j + 1, s.size() - j - 1);
    result = OPTAB[prog[i].second[1]].second;
    result += REGS[r1];
    result += REGS[r2];
    return result;
}

string format3(int i)
{
    string adr = prog[i].second[2], res1, res2, res3;
    bool flags[6] = {}, dr = 0;
    int no = 0;
    if (adr[adr.size() - 1] == 'X' && adr[adr.size() - 2] == ',')
    {
        flags[2] = 1;
        adr = adr.substr(0, adr.size() - 2);
    }
    if (adr[0] == '#')
    {
        flags[1] = 1;
        adr = adr.substr(1, adr.size() - 1);
        if (SYMTAB.find(adr) != SYMTAB.end())
        {
            res2 = inttohex(SYMTAB[adr], 3);
        }
        else
        {
            res2 = adr;
            dr = 1;
        }
        no = 1;
    }
    else if (adr[0] == '@')
    {
        flags[0] = 1;
        adr = adr.substr(1, adr.size() - 1);
        no = 2;
        int z = SYMTAB[adr], j;
        for (j = 0; j < prog.size(); j++)
            if (prog[j].first == z)
                break;
        res2 = adr;
        adr = inttohex(prog[j].first, 3);
        if (prog[j].second[1] != "WORD" && prog[j].second[1] != "BYTE" && prog[j].second[1] != "RESW" && prog[j].second[1] != "RESB")
        {
            adr = prog[j].second[2];
            z = SYMTAB[adr];
            for (j = 0; j < prog.size(); j++)
                if (prog[j].second[0] == res2)
                    break;
            adr = prog[j].second[2];
            res2 = inttohex(SYMTAB[adr], 3);
        }
        else
        {
            res2 = adr;
        }
    }
    else if (adr[0] == '=')
    {
        adr = adr.substr(3, adr.size() - 4);
        dr = 1;
    }
    else
    {
        res2 = inttohex(SYMTAB[adr], 3);
        flags[0] = 1;
        flags[1] = 1;
        no = 3;
    }
    if (dr != 1 && adr != "*")
    {

        res2 = readdr(res2);

        res2 = res2.substr(res2.size() - 3, 3);
        flags[4] = checkpc;
        flags[3] = !checkpc;
    }
    if (flags[2] == 1)
    {
        res2 = inttohex(hextoint(res2) - indx, 3);
    }
    while (res2.size() < 3)
        res2 = "0" + res2;
    res3 = OPTAB[prog[i].second[1]].second;
    res3 = inttohex(hextoint(res3) + no, 2) + bintohex(flags[2], flags[3], flags[4], flags[5]) + res2;
    return res3;
}
string format4(int bb)
{
    string z = prog[bb].second[2], te = prog[bb].second[1], TA = "", obcode;
    int no = 0;
    bool nixbpe[6] = {0, 0, 0, 0, 0, 0};
    nixbpe[0] = (z[0] == '@');
    nixbpe[1] = (z[0] == '#');
    if (nixbpe[0] == nixbpe[1])
    {
        nixbpe[0] = !nixbpe[0];
        nixbpe[1] = !nixbpe[1];
    }
    nixbpe[2] = (z[z.length() - 1] == 'X' && z[z.length() - 2] == ',') ? 1 : 0;
    nixbpe[3] = 0;
    nixbpe[4] = 0;
    nixbpe[5] = 1;
    if (z[0] == '@' || z[0] == '#')
        z = z.substr(1, z.length() - 1);
    if (z[z.length() - 1] == 'X' && z[z.length() - 2] == ',')
        z = z.substr(0, z.length() - 2);
    if (nixbpe[0] == 1 && nixbpe[1] == 1)
    {
        string s = inttohex(SYMTAB[z], 5);
        for (int i = 0; i < prog.size(); i++)
        {
            if (inttohex(prog[i].first, 5) == s)
            {
                if (nixbpe[2] == 0)
                    TA = s;
                else
                    TA = inttohex(hextoint(s) + indx, 5);
            }
        }
        no = 3;
    }
    else if (nixbpe[0] == 1 && nixbpe[1] == 0 && nixbpe[2] == 0)
    {
        string s = to_string(SYMTAB[z]);
        for (int i = 0; i < prog.size(); i++)
            if (to_string(prog[i].first) == s)
            {
                s = prog[i].second[2];
                for (int j = 0; i < prog.size(); j++)
                    if (to_string(prog[j].first) == s)
                        TA = prog[j].second[2];
            }
        no = 2;
    }
    else if (nixbpe[0] == 0 && nixbpe[1] == 1)
    {

        if (z[0] < 65)
            TA = inttohex(stoi(z), 5);
        else
            TA = inttohex(SYMTAB[z], 5);
        no = 1;
    }
    string res3 = OPTAB[prog[bb].second[1].substr(1, prog[bb].second[1].size() - 1)].second;

    res3 = inttohex(hextoint(res3) + no, 2) + bintohex(nixbpe[2], nixbpe[3], nixbpe[4], nixbpe[5]) + TA;
    return res3;
}


void ObjectFile()
{
    freopen("objectfile22.txt", "w", stdout);
    int sz = obcode.size();
    cout << "H^" << programName;
    for (int j4 = 0; j4 < 6 - programName.size(); j4++)
        cout << " ";
    cout << "^" << inttohex(obcode[0].first, 6) << "^" << inttohex(LOCCTR, 6) << endl;

    for (int i = 0; i < obcode.size(); i += 5)
    {
        long long sum = 0;
        for (int j = i; j < i + min(sz - i, 5); j++)
        {
            sum += obcode[j].second.size() / 2;
        }
        cout << "T^" << inttohex(obcode[i].first, 6) << "^" << inttohex(sum, 2);

        for (int j = i; j < i + min(sz - i, 5); j++)
        {
            cout << "^" << obcode[j].second;
        }
        cout << endl;
    }
    for (int i = 0; i < modify.size(); i++)
        cout << "M^" << inttohex(modify[i] + 1, 6) << "^05" << endl;
    cout << "E^" << inttohex(obcode[0].first, 6);
}
void getInput(string l, string *a, string *b, string *c)
{
    string x, y, z;

    if (l[0] == ' ')
        x = "*";
    else
    {
        int j = 0;
        for (j; j < l.size() && l[j] != ' '; j++)
            ;
        x = l.substr(0, j);
    }
    l = l.substr(x.size() + 1, l.size() - x.size() - 1);
    int j = 0;
    for (j; j < l.size() && l[j] != ' '; j++)
        ;
    y = l.substr(0, j);
    l = l.substr(y.size() + 1, l.size() - y.size() - 1);
    if (l[0] == ' ')
        z = "*";
    else
    {
        z = l;
    }

    (*a) = x;
    (*b) = y;
    (*c) = z;
}

int main()
{
    string filename ;
     ifstream input1;
    do {
    cout<<"Enter the program file's name you want to assemble : " ;
    cin>>filename;
    input1.open((filename + ".txt")) ; // =========================== input2.txt is without "*" ================
        if(input1.fail())
            cout<<"Not Found"<<endl;
    } while(input1.fail());
    freopen("instructions.txt", "r", stdin);  // opens the instructions file
    string x, y, z;
    int a;
    for (int i = 0; i < 59; i++)
    {
        cin >> x >> a >> z;
        OPTAB[x] = {a, z};
    }
    freopen("registers.txt", "r", stdin);  // opens the registers file
    for (int i = 0; i < 9; i++)
    {
        cin >> x >> y;
        REGS[x] = y; // maps every register name with it's number
    }



    //-------------------------------------- Pass 1;
    string l;
    ifstream input(filename + ".txt") ;
    while (getline(input, l))
    {
        getInput(l, &x, &y, &z);
        if (x[0] == '.')
            continue;
        prog.push_back({LOCCTR, vector<string>()});
        prog.back().second.push_back(x);
        prog.back().second.push_back(y);
        prog.back().second.push_back(z);
        if (x != "*")
        {
            SYMTAB[x] = LOCCTR;
        }
        if (y == "RESW")
        {
            LOCCTR += stoi(z) * 3;
        }
        else if (y == "RESB")
        {
            LOCCTR += stoi(z);
        }
        else if (y == "WORD")
        {
            LOCCTR += wrdsize(prog.size() - 1);
        }
        else if (y == "BYTE")
        {
            LOCCTR += bytsize(prog.size() - 1);
        }
        else if (y[0] == '+')
        {
            LOCCTR += 4;
        }
        else if (OPTAB.find(y) != OPTAB.end())
        {
            LOCCTR += OPTAB[y].first;
        }
    }

    //------------------------------------------- Pass 2
    for (int i = 0; i < prog.size(); i++)
    {
        string s;
        int j;
        for (j = i; j < prog.size(); j++)
        {
            if (prog[j].first != prog[i].first)
                break;
        }
        pc = prog[j].first;
        string s2 = prog[i].second[1];
        int format = OPTAB[prog[i].second[1]].first;
        if (format == 1)
        {
            s = OPTAB[prog[i].second[1]].second;
        }
        else if (format == 2)
        {
            s = format2(i);
        }

        else if (format == 3)
        {
            s = format3(i);
        }
        else if (s2[0] == '+')
        {
            s = format4(i);
            string tt = prog[i].second[2];
            if (tt[0] != '#')
                modify.push_back(prog[i].first);

        }
        if (prog[i].second[1] == "BASE")
        {
            base = SYMTAB[prog[i].second[2]];
        }
        if (prog[i].second[2] == "LDX")
        {
            x = SYMTAB[prog[i].second[2]];
        }
        if (prog[i].second[1] == "NOBASE")
        {
            base = 0;
        }
        else if (prog[i].second[1] == "BYTE")
        {
            string adr = prog[i].second[2];
            s = adr.substr(2, adr.size() - 3);
            string s3 = "";
            for (int j = 0; j < s.size(); j++)
            {
                s3 += inttohex(s[j], 2);
            }
            if (adr[0] == 'C')
                s = s3;
        }

        prog[i].second.push_back(s);
    }

    for (int i = 0; i < prog.size(); i++)
    {
        if(prog[i].second[0] == "*")
        {
            if(prog[i].second[2].length()==8)
                cout << inttohex(prog[i].first, 4)<<"\t\t" << prog[i].second[1] << "\t" << prog[i].second[2] << "  " << prog[i].second[3] << endl;
                else if(prog[i].second[2] == "*")
                    cout << inttohex(prog[i].first, 4)<<"\t\t" << prog[i].second[1] << "\t" << " " << "\t  " << prog[i].second[3] << endl;
            else
                cout << inttohex(prog[i].first, 4)<<"\t\t" << prog[i].second[1] << "\t" << prog[i].second[2] << "\t  " << prog[i].second[3] << endl;

        }
        else
        cout << inttohex(prog[i].first, 4) << "\t" << prog[i].second[0] << "\t" << prog[i].second[1] << "\t" << prog[i].second[2] << "\t  " << prog[i].second[3] << endl;

        if (i == 0)
            programName = prog[i].second[0];
        else if (prog[i].second[3].length() != 0)
            obcode.push_back({prog[i].first, prog[i].second[3]});

    }
    ObjectFile();
}
