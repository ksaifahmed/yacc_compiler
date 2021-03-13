#include <bits/stdc++.h>
using namespace std;

class SymbolInfo{
	string name, type;
	SymbolInfo * next;

	public:
		SymbolInfo();
		SymbolInfo(string, string);
		~SymbolInfo();

		void setName(string n){ name = n; }
		void setType(string k){ type = k; }
		void setNext(SymbolInfo *s) { next = s; }

		string getName(){ return name; }
		string getType(){ return type; }
		SymbolInfo * getNext(){ return next; }

};

SymbolInfo::SymbolInfo(){ next = NULL; }

SymbolInfo::SymbolInfo(string n, string k)
{
	name = n;
	type = k;
    next = NULL;
}

SymbolInfo::~SymbolInfo()
{
    SymbolInfo * curr = next;
    while(1)
    {
        if(curr == NULL)
        {
            delete curr;
            break;
        }
        SymbolInfo * next = curr -> getNext();
        delete curr;
        curr = next;
    }
}





///scopeTable
class ScopeTable
{
    SymbolInfo ** scope_array;
    ScopeTable * parentScope;
	int bucket_size;
	string id;

	int hash_fuc(string key)
	{
		int sum = 0;
		for(int i = 0; i < key.length(); i++)
			sum += key[i];
		return sum % bucket_size;
	}

	public:
		ScopeTable();
		ScopeTable(int);
		~ScopeTable();

		string getID(){ return id; }
        ScopeTable * getParent() { return parentScope; }

        void setID(string i) { id = i; }
        void setParent(ScopeTable * parent){ parentScope = parentScope; }

        //the main functions
        SymbolInfo * Lookup(string);
        bool Insert(string, string);
        void Print();
        bool Delete(string);

};


ScopeTable::ScopeTable(){}

ScopeTable::ScopeTable(int n)
{
	bucket_size = n;
	scope_array = new SymbolInfo*[n];
	for(int i = 0; i < n; i++) scope_array[i] = NULL;
	parentScope = NULL;
}


ScopeTable::~ScopeTable()
{
	for(int i = 0; i < bucket_size; i++)
    {
        if(scope_array[i] != NULL) delete scope_array[i];
    }
    delete[] scope_array;
    delete parentScope;
}



SymbolInfo* ScopeTable::Lookup(string key)
{
    int index = hash_fuc(key);
    SymbolInfo * curr = scope_array[index];
    while(1)
    {
        if(curr == NULL) break;
        if(!key.compare(curr -> getName()))
        {
            return curr;
        }
        curr = curr -> getNext();
    }

    return NULL;
}

bool ScopeTable::Insert(string key, string type)
{
    if(Lookup(key) != NULL) return false;
    int index = hash_fuc(key);
    SymbolInfo * p = new SymbolInfo(key, type);

    if(scope_array[index] == NULL)
    {
        scope_array[index] = p;
    }
    else
    {
        SymbolInfo * curr = scope_array[index];
        while(1)
        {
            if(curr -> getNext() == NULL)
            {
                curr -> setNext(p);
                break;
            }
            curr = curr -> getNext();
        }
    }
    return true;
}


bool ScopeTable::Delete(string key)
{
    if(Lookup(key) == NULL) return false;
    int index = hash_fuc(key);

    SymbolInfo * curr = scope_array[index];
    SymbolInfo * prev = NULL;

    while(1)
    {
        if(curr == NULL) return false;
        if(!key.compare(curr -> getName()))
        {
            if(prev == NULL)
            {
                scope_array[index] = curr -> getNext();
            }
            else
            {
                prev -> setNext(curr -> getNext());
            }
            break;
        }
        prev = curr;
        curr = curr -> getNext();
    }
    return true;
}

void ScopeTable::Print()
{
    for(int i = 0; i < bucket_size; i++)
    {
        cout << i << " -->  ";
        SymbolInfo * curr = scope_array[i];
        while(1)
        {
            if(curr == NULL) break;
            cout << "< " << curr -> getName() << " : " << curr -> getType() << " >  ";
            curr = curr -> getNext();
        }
        cout << endl;
    }
}









class SymbolTable
{
    vector<ScopeTable> scopeTables;
    ScopeTable * currentScopeTable;

    public:
        SymbolTable();
        ~SymbolTable();

};


SymbolTable::SymbolTable()
{
    currentScopeTable = NULL;
}

SymbolTable::~SymbolTable()
{
    delete currentScopeTable;
}












int main()
{
	SymbolInfo s;
	s.setName("kire");
	s.setType("string");
	cout << s.getName() << endl;
    ScopeTable st(10);
    cout << st.Insert("haha", "goesbrrr") << endl;
    st.Insert("gaia", "stons");
    st.Insert("aagi", "huhu");
    st.Print();
    st.Delete("aagi");
    st.Delete("gaia");
    st.Delete("haha");
    st.Delete("haha");
    st.Print();
	return 0;
}
