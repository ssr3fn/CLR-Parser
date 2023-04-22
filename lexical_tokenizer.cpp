// Write a program to detect tokens in C++
#include <bits/stdc++.h>
using namespace std;

bool isDelimiter(char ch) {
  if (ch == ' ' || ch == '+' || ch == '-' || ch == '*' || ch == '/' ||
      ch == ',' || ch == ';' || ch == '>' || ch == '<' || ch == '=' ||
      ch == '(' || ch == ')' || ch == '[' || ch == ']' || ch == '{' ||
      ch == '}' || ch == '\t' || ch == '\r')
    return true;

  return false;
}

bool isOperator(char ch) {
  if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '>' ||
      ch == '<' || ch == '=')
    return true;

  return false;
}

bool validIdentifier(string str) {
  if (str[0] == '0' || str[0] == '1' || str[0] == '2' || str[0] == '3' ||
      str[0] == '4' || str[0] == '5' || str[0] == '6' || str[0] == '7' ||
      str[0] == '8' || str[0] == '9' || isDelimiter(str[0]) == true)
    return false;

  return true;
}

bool isKeyword(string str) {
  if (str == "if" || str == "else" || str == "while" || str == "do" ||
      str == "break" || str == "continue" || str == "int" || str == "double" ||
      str == "float" || str == "return" || str == "char" || str == "case" ||
      str == "char" || str == "sizeof" || str == "long" || str == "short" ||
      str == "typedef" || str == "switch" || str == "unsigned" ||
      str == "void" || str == "static" || str == "struct" || str == "goto" || str == "main" ||
      str == "#include" || str == "print" || str =="read")
    return true;

  return false;
}

bool isInteger(string str) {
  int i, len = str.length();

  if (len == 0)
    return false;

  for (i = 0; i < len; i++) {
    if (str[i] != '0' && str[i] != '1' && str[i] != '2' && str[i] != '3' &&
            str[i] != '4' && str[i] != '5' && str[i] != '6' && str[i] != '7' &&
            str[i] != '8' && str[i] != '9' ||
        (str[i] == '-' && i > 0))
      return false;
  }

  return true;
}

string subString(string str, int left, int right) {
  int i;
  string subStr = "";

  for (i = left; i <= right; i++)
    subStr += str[i];
  // 	subStr += '\0';
  return (subStr);
}

bool isRealNumber(string str) {
  int i, len = str.length();
  bool hasDecimal = false;

  if (len == 0)
    return (false);
  int dec = 0;
  for (i = 0; i < len; i++) {
    if (str[i] == '.') {
      dec++;
    }
    if (str[i] != '0' && str[i] != '1' && str[i] != '2' && str[i] != '3' &&
            str[i] != '4' && str[i] != '5' && str[i] != '6' && str[i] != '7' &&
            str[i] != '8' && str[i] != '9' && str[i] != '.' ||
        (str[i] == '-' && i > 0))
      return (false);
    if (str[i] == '.')
      hasDecimal = true;
  }
  if (dec > 1) {
    hasDecimal = false;
  }
  return (hasDecimal);
}

void parse(string str, int row) {
  int left = 0, right = 0;
  int len = str.length();
  while (right < len && left <= right) {
    if (isDelimiter(str[right]) == false){
      right++;
    }
    if (isDelimiter(str[right]) == true && left == right) {
      if (isOperator(str[right]) == true) {
        cout << str[right] << " ";

      } 
      else if (str[right] != ' ' && str[right] !='\t' && str[right]!='\r') {
        cout << str[right] << " ";
      }
        
      right++;
      left = right;
    } else if (isDelimiter(str[right]) == true && left != right ||
               (right == len && left != right)) {
      string subStr = subString(str, left, right - 1);
      if (subStr == "\n") {
        left = right;
        continue;
      }

      if (isKeyword(subStr) == true){
        cout << subStr << " ";
      }
      else if (isInteger(subStr) == true){
        cout << "NUMCONST ";
      }
      else if (isRealNumber(subStr) == true){
        cout << "NUMCONST ";
      }
      else if (validIdentifier(subStr) == true &&
               isDelimiter(str[right - 1]) == false){
        cout << "ID ";
        }
      else if (validIdentifier(subStr) == false &&
               isDelimiter(str[right - 1]) == false){
        cout << subStr << "  ->   INVALID IDENTIFIER\n";
       }
      
      left = right;
    }
  }
}

int main() {

  ifstream MyReadFile("sample.txt");
  string myline;
  int row = 0;
 (void*)freopen("tokens.txt","w",stdout);
  while (getline(MyReadFile, myline)) {
    parse(myline, row);
    row++;
  }

  MyReadFile.close();

  return (0);
}
