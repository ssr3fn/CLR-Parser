#include<bits/stdc++.h>
using namespace std;

//add lookahead
//add rules
//parse tokens

vector<string> grammar;
map<string, set<vector<string> > > productions;
map<int, string > getprodleft;
string startsymbol;
class Item
{
	public:
	string left;
	vector<string> prod;
	int dot;
	set<string> lookahead;
	bool operator <(const Item& b){
		if(left!=b.left)
        return left<b.left;
		if(prod!=b.prod)
		return prod<b.prod;
		if(dot!=b.dot)
		return dot<b.dot;
		if(lookahead!=b.lookahead)
		return lookahead<b.lookahead;
		return 0;
	}
	bool operator ==(const Item& a){
		return (left == a.left&&prod==a.prod&&dot==a.dot&&lookahead==a.lookahead);
	}
	bool operator !=(const Item& a){
		return (left != a.left||prod!=a.prod||dot!=a.dot||lookahead!=a.lookahead);
	}
};
vector<pair<string,vector<string>>> rules;
struct cmp1 {
    bool operator() (Item a, Item b) const {
		if(a.left!=b.left)
        return a.left<b.left;
		if(a.prod!=b.prod)
		return a.prod<b.prod;
		if(a.dot!=b.dot)
		return a.dot<b.dot;
		if(a.lookahead!=b.lookahead)
		return a.lookahead<b.lookahead;
		return 0;
    }
};
struct cmp2 {
    bool operator() (set<Item,cmp1> a, set<Item,cmp1> b) const {
		if(a.size()!=b.size())
        return a.size()<b.size();
		auto it1=a.begin(),it2=b.begin();
		while(it1!=a.end())
		{
			auto ia=*it1,ib=*it2;
			if(ia!=ib)
			return ia<ib;
			it1++,it2++;
		}
		return 0;
    }
};
bool comp(set<Item,cmp1> a, set<Item,cmp1> b)
{
	if(a.size()!=b.size())
	return 0;
	auto it1=a.begin(),it2=b.begin();
	while(it1!=a.end())
	{
		auto ia=*it1,ib=*it2;
		if(ia!=ib)
		return 0;
		it1++,it2++;
	}
	return 1;
}
int setno=0;
vector<set<Item,cmp1>> itemset; 
vector<set<Item,cmp1>> setkeys;
set<set<Item,cmp1>,cmp2> setkey;
void buildprod(string grammarfile)
{
    ifstream fi;
	fi.open(grammarfile);
	if(!fi.is_open())
    {
		cout << "File not found!\n";
		return;
	}
    string str;
    int flag=0;
	while(getline(fi, str))
    {
        if(!flag)
        {
            flag=1;
            string aug,start;
            for(char ch: str)
            {
                if(ch==' ')
                break;
                start+=ch;
            }
            aug=start+"' -> " + start;
            grammar.push_back(aug);
            startsymbol=start+"'";  
        }
        grammar.push_back(str);
    }
    fi.close();
	int idx = 1;
    for(auto prod : grammar) {
		//cout<<prod<<"\n";
		string tmp = "";
		prod += '~';
		bool flag = 0;
		string left;
		vector<string> right;
		for(auto ch : prod) {
			if(ch == ' ' or ch == '~') {
				if(flag==0) left = tmp;
				else if(tmp != "->") right.push_back(tmp);
				tmp = "";
				flag = 1;
				
			}
			else tmp += ch;
		}
		productions[left].insert(right);
		flag = 0;
		for(auto it : getprodleft) {
			if(it.second == left) {
				flag = 1;
				break;
			}
		}
		if(flag == 0) {
			getprodleft[idx]=left;
			idx++;
		}
	} 
}
void printprod() {
	cout<<"The production rules are: \n";
	for(auto idx : getprodleft) {
            string str=idx.second ;
			cout<<str<<" -> ";
			bool flag = 0;
			for(auto it : productions[str]) {
				rules.push_back({str,it});
				if(flag) cout<<" | ";
				flag = 1;
				for(auto it1 : it) cout<<it1<<" ";
			}
			cout<<"\n";
	}
	cout<<"\n";
}
set<string> nonterminals, terminals;
map<string, set<string> > first;
bool containsEpsilon(set<string> prod) {
	for(auto it : prod)
		if(it == "epsilon") return 1;
	return 0;
}
void computeFirst() {
	for(auto it : productions)
		nonterminals.insert(it.first);
	for(auto it : productions) {
		for(auto it1 : it.second) {
			for(auto str : it1) {
				if(nonterminals.find(str) == nonterminals.end() and str!="epsilon"){
					terminals.insert(str);
					first[str].insert(str);
				}
				else if(str == "epsilon") {
					first[it.first].insert(str);
				}
			}
		}
	}
	cout<<"Non Terminals are: ";for(auto it : nonterminals) cout<<it<<" "; cout<<"\n\n";
	cout<<"Terminals are: "; for(auto it : terminals) cout<<it<<" "; cout<<"\n\n";
	
	int cnt = 0;
	map<string, set<string> > recprod;
	for(auto prod : productions) {
		//cout<<prod.first<<" ";
		for(auto vec : prod.second) {
			for(auto str : vec) {
				if(terminals.find(str) != terminals.end()) {
					first[prod.first].insert(str);
					break;
				}
				else if(nonterminals.find(str) != nonterminals.end()) {
					recprod[prod.first].insert(str);
				}
				if(str!="epsilon" and containsEpsilon(first[str])==0) 
					break;
			}
		}
		
	}
	bool flag = 0;
	while(1) {
		flag = 0;
		for(auto it : recprod) {
			for(auto it1 : it.second) {
				for(auto f : first[it1]) {
					if(f!="epsilon" and first[it.first].find(f) == first[it.first].end()) {
						first[it.first].insert(f);
						flag = 1;
					}
				}
			}
		}
		if(!flag) break;
	}
	for(auto prod : productions) {
		for(auto vec : prod.second) {
			cnt = 0;
			for(auto str : vec) {
				if(str!="epsilon" and containsEpsilon(first[str])==0)
					break;
				cnt++;
			}
			if(cnt == vec.size()) 
				if(!(vec.size() == 1 and vec[0] == "epsilon")){
					first[prod.first].insert("epsilon");
				}
		}
		
	}
}
void printFirst() {
	cout<<"---------------------------------\n";
	cout<<"Symbol \t\tFIRST Set\n";
	cout<<"---------------------------------\n";
	for(auto it : first){
		cout<<it.first<<" \t\t{ ";
		bool flag = 0;
		for(auto it1 : it.second) {
			if(flag) cout<<" , ";
			flag = 1;
			cout<<it1;
		}
		cout<<" }\n";
	}
	cout<<"---------------------------------\n\n";
}
set<string> computeLookahead (vector<string> v)
{
	set<string> la;
	int flag=0;
	for(auto it: v)
	{

		if(flag)
		break;
	}
	return la;
}
map<string, set<string>> follow;
void computeFollow() {
	follow[startsymbol].insert("$");
	map<string, set<string> > folunion;
	string cur, nex;
	for(auto prod : productions) {
		for(auto vec : prod.second) {
			//cout<<prod.first<<" ";
			for(int i=0;i<vec.size();i++) {
				cur = vec[i];
				if(nonterminals.find(cur) != nonterminals.end()){
					if(i == vec.size()-1) {
						if(cur != prod.first) folunion[cur].insert(prod.first);
					} else {
						set<string> fr;
						bool ep = 1;
						for(int j = i+1;j<vec.size();j++) {
							nex = vec[j];
							bool tempep = 0;
							for(auto it : first[nex]) {
								if(it == "epsilon") tempep = 1;
								else follow[cur].insert(it);
							}
							ep = ep and tempep;
							if(ep == 0) break;
						}
						if(ep) folunion[cur].insert(prod.first);
					}
				}
				
			}
		}
	}
	bool flag = 0;
	while(1) {
		flag = 0;
		for(auto it : folunion) {
			for(auto it1 : it.second) {
				for(auto f : follow[it1]) {
					if(f!="epsilon" and follow[it.first].find(f)==follow[it.first].end()) {
						flag = 1;
						follow[it.first].insert(f);
					}
				}
			}
		}
		if(!flag) break;
	}
}
void printFollow() {
	cout<<"---------------------------------\n";
	cout<<"Symbol \t\tFOLLOW Set\n";
	cout<<"---------------------------------\n";
	for(auto it : follow){
		cout<<it.first<<" \t\t{ ";
		bool flag = 0;
		for(auto it1 : it.second) {
			if(flag) cout<<" , ";
			flag = 1;
			cout<<it1;
		}
		cout<<" }\n";
	}
	cout<<"---------------------------------\n\n";
}
map<pair<int,string>,string> parsetable;
void createset(set<Item,cmp1> items)
{
	if(setkey.find(items)!=setkey.end())
	return;
	set<Item,cmp1> is;
	int curr_set=itemset.size();
	setkey.insert(items);
	setkeys.push_back(items);
	vector<Item> ilist;
	for(auto it:items)
	{
		ilist.push_back(it);
		is.insert(it);
	}
	int idx=0;
	map<string,set<Item,cmp1>> nextsets;
	while(idx!=ilist.size())
	{
		if(ilist[idx].dot==ilist[idx].prod.size())
		{
			//check
			idx++;
			continue;
		}
		string next=ilist[idx].prod[ilist[idx].dot];
		Item next_set=ilist[idx];
		next_set.dot++;
		nextsets[next].insert(next_set);
		for(auto p:productions[next])
		{
			Item new_item;
			new_item.left=next;
			new_item.prod=p;
			new_item.dot=0;
			//check
			new_item.lookahead={};
			if(is.find(new_item)!=is.end())
			continue;
			is.insert(new_item);
			ilist.push_back(new_item);
		}
		idx++;
	}
	itemset.push_back(is);
	for(auto it: nextsets)
	{
		int val=itemset.size();
		if(setkey.find(it.second)!=setkey.end())
		{
			for(int i=0;i<setkeys.size();i++)
			{
				if(comp(setkeys[i],it.second))
				{
					val=i;
					break;
				}
			}
		}
		if(terminals.find(it.first)!=terminals.end())
		parsetable[{curr_set,it.first}]="S";
		parsetable[{curr_set,it.first}]+=to_string(val);
		createset(it.second);
	}
}
void printsets()
{
	for(int i=0;i<itemset.size();i++)
	{
		cout<<"I"<<i<<endl;
		for(auto it:itemset[i])
		{
			cout<<it.left<<" -> ";
			for(int ii=0;ii<it.prod.size();ii++)
			{
				if(ii==it.dot)
				cout<<".";
				cout<<it.prod[ii]<<" ";
			}
			if(it.dot==it.prod.size())
			cout<<".";
			cout<<endl;
		}
	}
}
void buildparsetable()
{

}
void printparsetable()
{
	cout<<"\nParse Table: \n\n";
	cout<<"\t";
	for(auto it: terminals)
	cout<<it<<"\t";
	for(auto it: nonterminals)
	cout<<it<<"\t";
	cout<<endl;
	for(int i=0;i<itemset.size();i++)
	{
		cout<<i<<"\t";
		for(auto it:terminals)
		{
			string s=parsetable[{i,it}];
			if(s!="")
			cout<<s;
			cout<<"\t";
		}
		for(auto it:nonterminals)
		{
			string s=parsetable[{i,it}];
			if(s!="")
			cout<<s;
			cout<<"\t";
		}
		cout<<endl;
	}
}
void parsetokens()
{

}
int main()
{
    string grammarfile,filename;
    cout<<"Enter filename containing grammar: ";
    cin>>grammarfile;
    buildprod(grammarfile);
	printprod();
	computeFirst();
	printFirst();
	computeFollow();
	//printFollow();
	Item first;
	set<Item,cmp1> firstset;
	first.left=startsymbol;
	first.prod=*productions[startsymbol].begin();
	first.dot=0;
	firstset.insert(first);
	createset(firstset);
	printsets();
	printparsetable();
	// cout<<"Input the filename which contains tokens to be parsed : ";
	// cin>>filename;
    return 0;
}
