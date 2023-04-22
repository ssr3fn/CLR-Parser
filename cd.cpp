#include <bits/stdc++.h>
#include <string>
#include <map>
#include <vector>
#define ll int
//let stack size=101


using namespace std;
set<int> st;
//I maintain scope using a parent vector
vector<int> parent(102,-1);
stack<int> scopes;

// Symbol table entry structure
struct SymTableEntry {
    string name;
    string type;
    int size;
    int scope;
};

// Symbol table structure
class SymbolTable {
    public:
        void insert(string name, string type, int size, int scope) {
            SymTableEntry entry;
            entry.name = name;
            entry.type = type;
            entry.size = size;
            entry.scope = scope;
            symTable.push_back(entry);
        }
        void display() {
        	cout << "---------------------------" << endl;
            cout << "Symbol Table" << endl;
            cout << "---------------------------" << endl;
            cout << "Name\tType\tSize\tScope" << endl;
            for (auto entry : symTable) {
                cout << entry.name << "\t" << entry.type << "\t" << entry.size << "\t" << entry.scope << endl;
            }
            cout << "---------------------------" << endl;
        }
            void display_func() {
            cout << "Functions" << endl;
            cout << "---------------------------" << endl;
            cout << "Name\tType\t\tScope" << endl;
            for (auto entry : symTable) {
                cout << entry.name << "\t" << entry.type  << "\t" << entry.scope << endl;
            }
            cout << "---------------------------" << endl;
        }
    private:
        vector<SymTableEntry> symTable;
};
SymbolTable func_table;
 SymbolTable symTable;

// Function to parse a function definition and generate the symbol table
void parseFunction(string code, string datatype, int scope) {
    int i = 0;
    int n=code.length();
   
    func_table.insert(code,datatype,0,scope);


}

// Function to generate the symbol table from the input code
void generateSymbolTable(string code) {
   
    int scope = 0;
    int i = 0;
    int n=code.length();

    if(code.length()==0)
    	return;

    while (i < code.length()) {
        if (code[i] == '{') {
            	int el=*(st.begin());
        	st.erase(el);
        	//make a directed edge to upper scopes
        	if(!scopes.empty())
        	{
        		int upper=scopes.top();
        		parent[el]=upper;
        	}
        	scopes.push(el);
        }
        else if (code[i] == '}') {
             scopes.pop();
        }

        else if (i==0 and i+2<n and code[i] == 'i' && code[i+1] == 'n' && code[i+2] == 't') {
        	
        	int scope=scopes.top();
            string name = "";
            int j = i+4;
            if(j>=n)
            	break;
            bool fl=false;
            while (j<n and code[j] != '(') {
            	if(code[j]==';' or code[j]=='=')
            	{
            		fl=true;
            		break;
            	}
                if (code[j] != ' ') {
                    name += code[j];
                }
                j++;
            }
            //if fl is true..then variable...else function
            cerr<<name<<endl;
            if(fl)
            symTable.insert(name, "int", 4, scope);
        else
            parseFunction(name, "int", (*st.begin()));
        }
        else if(i==0 and i+4<n and code.substr(i,5)=="float")
        {
        	int scope=scopes.top();
        	 string name = "";
            int j = i+6;
        	 bool fl=false;
            while (j<n and code[j] != '(') {
            	if(code[j]==';' or code[j]=='=')
            	{
            		fl=true;
            		break;
            	}
                if (code[j] != ' ') {
                    name += code[j];
                }
                j++;
            }
        if(fl)
            symTable.insert(name, "float", 4, scope);
        else
            parseFunction(name,"float", (*st.begin()));

        }
        else if(i==0 and i+3<n and code.substr(i,4)=="void")
        {
        	
        	int scope=scopes.top();
        	 string name = "";
            int j = i+5;
        	 bool fl=false;
            while (j<n and code[j] != '(') {
            	if(code[j]==';' or code[j]=='=')
            	{
            		fl=true;
            		break;
            	}
                if (code[j] != ' ') {
                    name += code[j];
                }
                j++;
            }
            if(!fl)
            	parseFunction(name,"void", (*st.begin()));


        }
        i++;
    }

 
}
//scopes that can be accessed by a given scope
void visiting_scopes(int scope)
{
	vector<int> visiting_order;
	//the visiting order of scopes are
	int orig_scope=scope;
	visiting_order.push_back(scope);
	while(parent[scope]!=-1)
	{
		scope=parent[scope];
		visiting_order.push_back(scope);
	}
	reverse(visiting_order.begin(),visiting_order.end());
	cout<<"Scopes that can be accessed by given scope-"<<orig_scope<<" is:-"<<endl;
	cout << "---------------------------" << endl;
	for(auto c:visiting_order)
		cout<<c<<" ";
	cout<<endl;

}
void init_code(){
    // fastio();
    #ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    #endif 
}
int main() {
	init_code();
	#ifndef ONLINE_JUDGE
   freopen("Error.txt", "w", stderr);
#endif
    string code = "int a;\nint main() {\nint b;\n{\nint c;\n}\n}\nint foo()\n{\nint d;\n}\n";
    scopes.push(0);
    for(ll i=1;i<=100;i++)
    	st.insert(i);
    string temp;
    while(getline(cin,temp))
    {
    	reverse(temp.begin(),temp.end());
    	while(!temp.empty() and temp.back()=='\t')
    		temp.pop_back();
    	reverse(temp.begin(),temp.end());

    	generateSymbolTable(temp);
    }
   symTable.display();
   func_table.display_func();
    // visiting_scopes(3);
    return 0;
}
