#include<bits/stdc++.h>
using namespace std;
typedef struct node
{
	string data;
	node *parent,*left,*right;
	
}node;
typedef struct sentence
{
	vector<string> arguments;
	bool neg;
	string name;
}sentence;
map<char,int> variables;
node* new_node(string val)
{
	node *root=new node();
	root->data=val;
	root->left=NULL;
	root->right=NULL;
	root->parent=NULL;
	return root;
}
int priority(string op)
{
	 if (op == "&")
		return 3;
	else if (op == "=>")
		return 2;
	return 1;
}
map<string,vector<pair<int,int> > > positive_literal;
map<string,vector<pair<int,int> > > positive_sentence;
map<string,vector<pair<int,int> > > negative_literal;
map<string,vector<pair<int,int> > > negative_sentence;
vector<vector<sentence> > kb;
void add_to_stack(stack<node*> &store,string s)
{
	node* root=new_node(s);
	if (s=="&"||s=="=>") {
				
					node* op2 = store.top();
					op2->parent = root;
					store.pop();
					root->right = op2;
					node* op1 =store.top();
					op1->parent = root;
					store.pop();
					root->left = op1;
					
			}
			store.push(root);

}
int isConstant(vector<string> &args) {
	//vector<string> args=p.arguments;
	int s=args.size();
		for (int i = 0; i <s; i++) {
			
			if (args[i][0]>=97&&args[i][0]<=122)
				return 0;
		}
		
		return 1;
}
void store_in_kb(vector<sentence> sent)
{			
	kb.push_back(sent);
	int l=kb.size()-1;
	for (int i = 0; i < sent.size(); i++) {
				if (isConstant(sent[i].arguments)) {
					if (!sent[i].neg) {
						positive_literal[sent[i].name].push_back(make_pair(l, i));
					}
					else {
						
						negative_literal[sent[i].name].push_back(make_pair(l, i));
					}
				}
				else {
					if (!sent[i].neg) {
						positive_sentence[sent[i].name].push_back(make_pair(l, i));
					}
					else {
						
						negative_sentence[sent[i].name].push_back(make_pair(l, i));
					}
				}
			}
}
	map<string,vector<pair<int,int> > > positive_literal_copy;
	map<string,vector<pair<int,int> > > positive_sentence_copy;
	map<string,vector<pair<int,int> > > negative_literal_copy;
	map<string,vector<pair<int,int> > > negative_sentence_copy;
	vector<vector<sentence> > kb_copy;
	map< pair<string,vector<string> >,int> mp_store;
void store_in_kb_copy(vector<sentence> sent)
{
	kb_copy.push_back(sent);
	
	
	int l=kb_copy.size()-1;
	for (int i = 0; i < sent.size(); i++) {
				if (isConstant(sent[i].arguments)) {
					if (!sent[i].neg) {
						positive_literal_copy[sent[i].name].push_back(make_pair(l, i));
					}
					else {
						
						negative_literal_copy[sent[i].name].push_back(make_pair(l, i));
					}
				}
				else {
					if (!sent[i].neg) {
						positive_sentence_copy[sent[i].name].push_back(make_pair(l, i));
					}
					else {
						
						negative_sentence_copy[sent[i].name].push_back(make_pair(l, i));
					}
				}
			}
}

node* negation(node* root) 
{
		if (root == NULL)
			return NULL;
		root->left = negation(root->left);
		if (root->left == NULL && root->right == NULL) {
				if (root->data[0] == '~') {
					root->data = root->data.substr(1);
				}
				else {
					
					root->data = "~" + root->data;
				}
			}
			else {
				if (root->data == "&") {
					root->data = "|";
				}
			}
			root->right = negation(root->right);
			
			return root;
}
void call_empty(node* root,vector<sentence> &vec)
{
	if(root==NULL)
	return;
	if(root->left==NULL&&root->right==NULL)
	{
		string t=root->data;
		sentence store;
		if(t[0]=='~')
		store.neg=true;
		else
		store.neg=false;
		int index = 0;
			while (t[index] != '(') 
			{
				index++;
			}
			index++;
			if (store.neg)
			{
				store.name=t.substr(1,index-2);
			}
			else
				store.name = t.substr(0, index - 1);

			int len = t.length();

			for (int i = index; i < len; i++) {
				if (t[i] == ',' || t[i] == ')') {
					string arg=t.substr(index,i-index);
					index = i + 1;
					store.arguments.push_back(arg);
				}
			}
			vec.push_back(store);
	}
}
void call_or(node* root,vector<sentence> &vec)
{
	if(root==NULL)
	return;
	if(root->left==NULL&&root->right==NULL)
	{
		string t=root->data;
		sentence store;
		if(t[0]=='~')
		store.neg=true;
		else
		store.neg=false;
		int index = 0;
			while (t[index] != '(') 
			{
				index++;
			}
			index++;
			if (store.neg)
			{
				store.name=t.substr(1,index-2);
			}
			else
				store.name = t.substr(0, index - 1);

			int len = t.length();

			for (int i = index; i < len; i++) {
				if (t[i] == ',' || t[i] == ')') {
					string arg=t.substr(index,i-index);
					index = i + 1;
					store.arguments.push_back(arg);
				}
			}
			vec.push_back(store);
	}
	call_or(root->left,vec);
	call_or(root->right,vec);
}
void standardize(vector<sentence> &pre)
{
			set<char> current_variables;
			for (int i = 0; i < pre.size(); i++) {
				vector<string> args = pre[i].arguments;
				for (int j = 0; j < args.size(); j++) {
					if (args[j][0]>=97&&args[j][0]<=122) {
						char var = args[j][0];
						if (current_variables.find(var) == current_variables.end()) {
							if (variables.find(var) == variables.end()) {
								variables[var] = 1;

							}
							else {
								variables[var]++;
							}
							current_variables.insert(var);
						}
						stringstream X;
						X<<variables[var];
						string s;
						X>>s;
						args[j] = var + s;
					}
				}
				pre[i].arguments=args;
			}
		
}
void convert_to_CNF(vector<string> str,node* &tree,vector<vector<sentence> > &kb1)
{
	stack<string> store;
	stack<node*> final_exp;
	int len=str.size();
	/*for(int i=0;i<len;i++)
	{
		cout<<str[i]<<endl;
	}*/
	for(int i=0;i<len;i++)
	{
		string s=str[i];
		if(s=="&"||s=="=>")
		{
			if(store.empty()==true)
			store.push(s);
			else if(priority(s)<=priority(store.top()))
			{
				while(store.empty()==false&&priority(s)<=priority(store.top()))
				{
					string x=store.top();
					store.pop();
					add_to_stack(final_exp,x);
					
				}
				store.push(s);
			}
			else
			store.push(s);
		}
		else
		{
			add_to_stack(final_exp,s);
		}
	}
	while(store.empty()==false)
	{
		add_to_stack(final_exp,store.top());
		store.pop();
	}
	//cout<<"checking"<<endl;
	tree=final_exp.top();
	if (tree->data == "=>") {
		
		negation(tree->left);
		tree->data = "|";
	}
	vector<sentence> ans;
	vector<vector<sentence> > ret;
	vector<node*> array;
	if(tree->data=="|")
	{
		//cout<<str[0]<<endl;
		call_or(tree,ans);
		ret.push_back(ans);
	}
	else if(tree->data=="&")
	{
		call_or(tree,ans);
		int l=ans.size();
		for(int i=0;i<l;i++)
		{
			vector<sentence> push;
			push.push_back(ans[i]);
			ret.push_back(push);
		}
		
	}
	else
	{
		call_empty(tree,ans);
		int l=ans.size();
		for(int i=0;i<l;i++)
		{
			vector<sentence> push;
			push.push_back(ans[i]);
			ret.push_back(push);
		}
	}
	//map<string,map<vector<string>,map<bool,int> > > mp;
	map< pair<string,pair<vector<string>,bool > > ,int> mp;
	for(int i=0;i<ret.size();i++)
	{
		vector<sentence> push=ret[i];
		vector<sentence> push_final;
		int len=ret[i].size();
		//vector<int> store_rem;
		for(int j=0;j<len;j++)
		{
			//cout<<" "<<push[j].name<<" ";
			if(mp[{push[j].name,{push[j].arguments,push[j].neg}}]==1)
			{
				//store_rem.push_back(j);
			}
			else
			mp[{push[j].name,{push[j].arguments,push[j].neg}}]=1,push_final.push_back(push[j]);
		}
		
		standardize(push_final);
		ret[i]=push_final;
		kb1.push_back(push_final);
	}
}
void find_match(sentence &p,vector<pair<int,int> > &lit,
	vector<pair<int,int> > &sent)
{
	//vector<pair<int,int> > lit;
	//vector<pair<int,int> > sent;
	if(p.neg==false)
	{
		lit=positive_literal_copy[p.name];
		sent=positive_sentence_copy[p.name];
	}
	else
	{	
		lit=negative_literal_copy[p.name];
		sent=negative_sentence_copy[p.name];
	}
}
void substitute(vector<string>  &x,map<string, string> &theta) {
		int l=x.size();
		int i=0;
		while(i<l) {
			while (theta.find(x[i])!=theta.end())
				x[i] = theta[x[i]];
				i++;
		}
		//return x;
}
bool ask(string sent) {
	kb_copy=kb;	
	mp_store.clear();
	node *tree=new_node(sent);
	vector<sentence> ans;
	vector<sentence> ans2;
	
	vector<vector<sentence> > ret;
	call_empty(tree,ans);
	ans2=ans;
	ans[0].neg=!ans[0].neg;
	
	positive_literal_copy=positive_literal;
	positive_sentence_copy=positive_sentence;
	negative_literal_copy=negative_literal;
	negative_sentence_copy=negative_sentence;
	store_in_kb_copy(ans);
	int n=kb_copy.size();
	for(int i=0;i<n;i++)
	{
		if(kb_copy[i].size()==1)
		{
			sentence s=kb_copy[i][0];
			vector<string> str=s.arguments;
		//	sort(str.begin(),str.end());
			//s.arguments=str;
			mp_store[{s.name,str}]=s.neg;
		}
	}
	list<vector<sentence> > que;
	que.push_back(ans);
	//queue<sentence> Frontier;
	map< vector< pair<string,pair<int,vector<string> > > > ,int> LoopDetector;
	
	double timeStart = clock();
	vector< pair<string,pair<int,vector<string> > > > p;
	for(int l=0;l<ans.size();l++)
	{
		sentence z=ans[l];
		pair<int,vector<string> > n=make_pair(z.neg,z.arguments);
		p.push_back({z.name,n});
	}
	sort(p.begin(),p.end());
	LoopDetector[p]=1;
		while (!que.empty()) {
			//Choose shallowest node from frontier
		//	sentence currentSentence = Frontier.front();
	/*	list<vector<sentence> > :: iterator it;
		cout<<"predicates "<<endl;
		for(it=que.begin();it!=que.end();it++)
		{
			vector<sentence> obj=*it;
			cout<<endl;
			for(int d=0;d<obj.size();d++)
			{
				cout<<obj[d].name<<"(";
				for(int b=0;b<obj[d].arguments.size();b++)
				cout<<obj[d].arguments[b]<<",";
				cout<<")";
				//cout<<endl;	
			}
			
			//cout<<endl;
		}
		cout<<"predicates end "<<endl;*/
		vector<sentence> ans1=que.front();
		que.pop_front();
		int len=ans1.size();
		for(int i=0;i<ans1.size();i++)
		{
			sentence store=ans1[i];
			sentence store_copy=ans1[i];
			store.neg=!store.neg;
			vector<pair<int,int> > lit;
			vector<pair<int,int> > sent;
			find_match(store,lit,sent);
			vector<pair<int,int> > res;
			//cout<<"match ";
			for(int j=0;j<lit.size();j++)
			{
				
			//	cout<<"lit"<<lit[j].first<<" ";
				res.push_back(lit[j]);
			}
			for(int j=0;j<sent.size();j++)
			{
				//cout<<sent[j].second<<" ";
				res.push_back(sent[j]);
			}
			for(int j=0;j<res.size();j++)
			{
				map<string,string> uni;
				if(kb_copy[res[j].first][res[j].second].name==store_copy.name&&kb_copy[res[j].first][res[j].second].neg!=store_copy.neg)
				{
					//cout<<"unify "<<store.name<<endl;
					vector<string> arg_1=store_copy.arguments;
					vector<string> arg_2=kb_copy[res[j].first][res[j].second].arguments;
					int flag=1;
					int len=arg_1.size();
					if(arg_1.size()==arg_2.size()){
					for(int k=0;k<len;k++)
						{
							if (arg_1[k] != arg_2[k]) {
								if (arg_1[k][0]>=97&&arg_1[k][0]<=122) {
							//If x[i] is a variable
								uni[arg_1[k]] = arg_2[k];
								substitute(arg_1, uni);
								substitute(arg_2, uni);
								//flag=1;
							}
							else if (arg_2[k][0]>=97&&arg_2[k][0]<=122) {
							//If y[i] is a variable
								uni[arg_2[k]] = arg_1[k];
								substitute(arg_1, uni);
								substitute(arg_2, uni);
								//flag=1;
							}
							else {
							flag=0;
							 break;
							}
							
							}
							//flag=1;
						}
					}
					else
					flag=0;
					
						if(flag==1) {
						vector<sentence>  sent1 = ans1;
						vector<sentence>  sent2 = kb_copy[res[j].first];
							for (int k = 0; k < sent1.size(); k++)
								substitute(sent1[k].arguments, uni);
							sent1.erase(sent1.begin() + i);
							for (int k = 0; k < sent2.size(); k++)
								substitute(sent2[k].arguments, uni);
							sent2.erase(sent2.begin() + res[j].second);
							vector<sentence> resolvent;
							
							map< pair<string,pair<vector<string>,bool> >,int > mp1;
						//	map< pair <pair <string,int>,vector<string> >  ,int> mp1;
							for(int p=0;p<sent1.size();p++)
							{
							//	cout<<" "<<push[j].name<<" ";
								if(mp1[{sent1[p].name,{sent1[p].arguments,sent1[p].neg}}]==1)
								{
									//store_rem.push_back(j);
									//cout<<"repeat ";
								}
								else
								{
									mp1[{sent1[p].name,{sent1[p].arguments,sent1[p].neg}}]=1;
									resolvent.push_back(sent1[p]);
								}
							}
							for(int p=0;p<sent2.size();p++)
							{
							//	cout<<" "<<push[j].name<<" ";
								if(mp1[{sent2[p].name,{sent2[p].arguments,sent2[p].neg}}]==1)
								{
									//store_rem.push_back(j);
									//cout<<"reper ";
								}
								else
								{
									mp1[{sent2[p].name,{sent2[p].arguments,sent2[p].neg}}]=1;
									resolvent.push_back(sent2[p]);
								}
							}	
							//If resolvent is empty then alpha is true
							if (resolvent.empty()) {
								//cout<<"hello"<<endl;								
								store_in_kb(ans2);
								return true;
							}
							else if(resolvent.size()==1)
							{
								sentence res=resolvent[0];
								vector<string> str=res.arguments;
								//sort(str.begin(),str.end());
								//res.arguments=str;
								if(mp_store.find({res.name,str})!=mp_store.end())
								{
									if(res.neg!=mp_store[{res.name,str}])
									{
										store_in_kb(ans2);
										return true;
									}	
								}
								else
								{
									mp_store[{res.name,str}]=res.neg;
								}
							}
							vector< pair<string,pair<int,vector<string> > > > p;
							for(int l=0;l<resolvent.size();l++)
							{
								sentence z=resolvent[l];
								pair<int,vector<string> > n=make_pair(z.neg,z.arguments);
								p.push_back({z.name,n});
							}
							sort(p.begin(),p.end());
							if (LoopDetector.find(p) == LoopDetector.end())
							{
								store_in_kb_copy(resolvent);
								que.push_back(resolvent);
								//cout<<"unify loop"<<que.size()<<endl;
								LoopDetector[p]=1;
							}
					}
				}
			}
			if ((clock() - timeStart) / CLOCKS_PER_SEC >= 56) // time in seconds
	        	return false;
		
			
		}
	}
		return false;
}


int main()
{
	
	FILE *fp1=freopen("input.txt", "r", stdin);
	int queries_count;
	cin>>queries_count;
	vector<string> query_sentences;
	vector<string> store;
	string str;
	cin.ignore();
	for(int i=0;i<queries_count;i++)
	{
		getline(cin,str);
		query_sentences.push_back(str);
	}
	//fflush(stdin);
	
	int kb_count;
	cin>>kb_count;
	cin.ignore();
	vector<string> temp;
	for(int i=0;i<kb_count;i++)
	{
		getline(cin,str);
		store.push_back(str);
	}
//	FILE* fp=freopen("output.txt", "w", stdout);
	vector<vector<string> > final_kb;
	for(int i=0;i<store.size();i++)
	{
		string str=store[i];
		stringstream X(str);
		
		vector<string> kb_sentences;
		while(X>>str)
		{
			kb_sentences.push_back(str);
		}
		final_kb.push_back(kb_sentences);
	}
	int a=final_kb.size();
	//cout<<a<<endl;
	node* root;
	vector<vector<sentence> > kb1;
	for(int i=0;i<a;i++)
	{
		int l=final_kb[i].size();
		//cout<<final_kb[i][0]<<endl;
		convert_to_CNF(final_kb[i],root,kb1);	
		int s=kb1.size();
		//cout<<endl<<s<<endl;
	}
	int s=kb1.size();
	for(int i=0;i<s;i++)
	{
			store_in_kb(kb1[i]);
	}
	//cout<<endl<<"kb that we have got"<<endl;
//	cout<<kb.size();
		/*for(int i=0;i<kb.size();i++)
		{
			for(int j=0;j<kb[i].size();j++)
			{
				cout<<kb[i][j].neg<<" "<<kb[i][j].name;
				int l=kb[i][j].arguments.size();
				cout<<"(";
				for(int k=0;k<l;k++)
				{
					cout<<kb[i][j].arguments[k]<<" ";
				}
				cout<<")";
			}
			cout<<endl;
		}*/
		vector<bool> prit;
	for(int i=0;i<queries_count;i++)
	{
		bool res=ask(query_sentences[i]);
		prit.push_back(res);
	}
//	cout<<endl;

	FILE* fp=freopen("output.txt", "w", stdout);
	for(int i=0;i<prit.size();i++)
	{
		if(prit[i]==1)
		cout<<"TRUE"<<endl;
		else
		cout<<"FALSE"<<endl;
	}
	return 0;
}
