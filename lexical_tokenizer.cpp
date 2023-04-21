#include <bits/stdc++.h>
using namespace std;
set<string> symboltable;
bool isDelimiter(char ch) {
    if (ch == ' ' || ch == '+' || ch == '-' || ch == '*' ||ch == '#'||
        ch == '/' || ch == ',' || ch == ';' || ch == '>' ||
        ch == '<' || ch == '=' || ch == '(' || ch == ')' ||
        ch == '[' || ch == ']' || ch == '{' || ch == '}')
        return true;

    return false;
}
 
bool isOperator(char ch) {
    if (ch == '+' || ch == '-' || ch == '*' ||
        ch == '/' || ch == '>' || ch == '<' ||
        ch == '=')
        return true;
    
    return false;
}

bool validIdentifier(string str) {
    if (str[0] == '0' || str[0] == '1' || str[0] == '2' ||
        str[0] == '3' || str[0] == '4' || str[0] == '5' ||
        str[0] == '6' || str[0] == '7' || str[0] == '8' ||
        str[0] == '9' || isDelimiter(str[0]) == true)
        return false;

    return true;
}

bool isKeyword(string str) {
    if (str == "if" || str == "else" ||
        str == "while" || str == "do" ||
        str == "break" ||
        str == "continue" || str == "int"
        || str == "double" || str == "float"
        || str == "return" || str == "char"
        || str == "case" || str == "char"
        || str == "sizeof" || str == "long"
        || str == "short" || str == "typedef"
        || str == "switch" || str == "unsigned"
        || str == "void" || str == "static"
        || str == "struct" || str == "goto")
        return true;

    return false;
}

bool isInteger(string str) {
    int i, len = str.length();
 
    if (len == 0)
        return false;

    for (i = 0; i < len; i++) {
        if (str[i] != '0' && str[i] != '1' && str[i] != '2'
            && str[i] != '3' && str[i] != '4' && str[i] != '5'
            && str[i] != '6' && str[i] != '7' && str[i] != '8'
            && str[i] != '9' || (str[i] == '-' && i > 0))
            return false;
    }

    return true;
}

string subString(string str, int left, int right) {
	int i;
	string subStr = "";

	for (i = left; i <= right; i++)
		subStr += str[i];
	return (subStr);
}

bool isRealNumber(string str) {
    int i, len = str.length();
    int decimal_count = 0;
 
    if (len == 0)
        return (false);
    for (i = 0; i < len; i++) {
        if (str[i] != '0' && str[i] != '1' && str[i] != '2'
            && str[i] != '3' && str[i] != '4' && str[i] != '5'
            && str[i] != '6' && str[i] != '7' && str[i] != '8'
            && str[i] != '9' && str[i] != '.' ||
            (str[i] == '-' && i > 0))
            return (false);
        if (str[i] == '.')
            decimal_count++;
    }
    return (decimal_count==1);
}

void parse(string str, int row) {
    ofstream outfile;
    outfile.open("tokens.txt", std::ios_base::app);

    int left = 0, right = 0;
    int len = str.length();
    string type;
    if(row==0)
    {
        outfile<<"----------------------------------------------------------------------\n";
        outfile<<"\tToken\t\tType\t\tRow \tColumn \n";
        outfile<<"----------------------------------------------------------------------\n";
    }
    //outfile<<"\t"<<token<<"\t\t"<<type<<" \t"<<row<<"\t"<<"\t"<<col<<"\n";
    while (right <= len && left <= right) {
        string tok(8,' ');
        if (isDelimiter(str[right]) == false)
            right++;
 
        if (isDelimiter(str[right]) == true && left == right) {
            if (isOperator(str[right]) == true) {
                tok[0]=str[right];
                outfile<<"\t"<<tok<<"\t"<<"operator   "<<"\t "<<row<<"\t"<<"\t"<<right<<"\n";
            }
            else if (str[right] != ' ') {
                tok[0]=str[right];
                outfile<<"\t"<<tok<<"\t"<<"symbol     "<<"\t "<<row<<"\t"<<"\t"<<right<<"\n";
            }
            
            right++;
            left = right;
        } else if (isDelimiter(str[right]) == true && left != right
                   || (right == len && left != right)) {
            string subStr = subString(str, left, right - 1);
 
            if (subStr == "\n") {
                left = right;
                continue;
            }

            if (isKeyword(subStr) == true)
                type="keyword    ";
 
            else if (isInteger(subStr) == true)
                type="integer    ";
 
            else if (isRealNumber(subStr) == true)
                type="real number";
 
            else if (validIdentifier(subStr) == true
                     && isDelimiter(str[right - 1]) == false)
                type="identifier ";
 
            else if (validIdentifier(subStr) == false
                     && isDelimiter(str[right - 1]) == false)
                type="invalid    ";
            int i=0;
            for(char ch:subStr)
            tok[i++]=ch;
            symboltable.insert(tok);
            outfile<<"\t"<<tok<<"\t"<<type<<"\t "<<row<<"\t"<<"\t"<<left<<"\n";
            
            left = right;
        }
    }
    outfile.close();
}
void generateSymbolTableFile() 
{
	ofstream fo("symboltable.txt",ios::out | ios::trunc);
	if(fo.is_open()) 
    {
        for(auto str:symboltable) 
		fo<<str<<"\n";
    }		
	fo.close();
}
int main() {
    
    string file_name;
    cout<<"Enter file name: ";
    cin>>file_name;
    ifstream MyReadFile(file_name);
    string myline;
    int row = 0;
    
    while (getline (MyReadFile, myline)) {
        parse(myline, row);
        row++;
    }
    generateSymbolTableFile();
    MyReadFile.close();
 
    return (0);
}