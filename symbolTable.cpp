#include <bits/stdc++.h>
#include <string>
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
    while(true)
    {
        if(curr == NULL)
        {
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
        void setParent(ScopeTable * parent){ parentScope = parent; }

        //the main functions
        SymbolInfo * Lookup(string);
        SymbolInfo * LookupWithPrint(string);
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
    while(true)
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

SymbolInfo* ScopeTable::LookupWithPrint(string key)
{
    int index = hash_fuc(key);
    int chain_position = 0;
    SymbolInfo * curr = scope_array[index];
    while(true)
    {
        if(curr == NULL) break;
        if(!key.compare(curr -> getName()))
        {
            cout << "Found in ScopeTable# " << id << " at position " << index << ", " << chain_position << endl;
            return curr;
        }
        chain_position++;
        curr = curr -> getNext();
    }
    return NULL;
}

bool ScopeTable::Insert(string key, string type)
{
    if(Lookup(key) != NULL)
    {
        cout << "<" << key << "," << type << "> already exists in current ScopeTable" << endl;
        return false;
    }
    int index = hash_fuc(key);
    int chain_position = 0;
    SymbolInfo * p = new SymbolInfo(key, type);

    if(scope_array[index] == NULL)
    {
        scope_array[index] = p;
    }
    else
    {
        SymbolInfo * curr = scope_array[index];
        while(true)
        {
            chain_position++;
            if(curr -> getNext() == NULL)
            {
                curr -> setNext(p);
                break;
            }
            curr = curr -> getNext();
        }
    }
    cout << "Inserted in ScopeTable# " << id << " at position " << index << ", " << chain_position << endl;
    return true;
}


bool ScopeTable::Delete(string key)
{
    if(Lookup(key) == NULL)
    {
        cout << "Not found" << endl;
        cout << key << " not found" << endl << endl;
        return false;
    }
    int index = hash_fuc(key);
    int chain_position = 0;

    SymbolInfo * curr = scope_array[index];
    SymbolInfo * prev = NULL;

    while(true)
    {
        //if(curr == NULL) return false;
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
        chain_position++;
        prev = curr;
        curr = curr -> getNext();
    }
    cout << "Found in ScopeTable# " << id << " at position " << index << ", " << chain_position << endl;
    return true;
}

void ScopeTable::Print()
{
    cout << "ScopeTable # " << id << endl;
    for(int i = 0; i < bucket_size; i++)
    {
        cout << i << " -->  ";
        SymbolInfo * curr = scope_array[i];
        while(true)
        {
            if(curr == NULL) break;
            cout << "< " << curr -> getName() << " : " << curr -> getType() << " >  ";
            curr = curr -> getNext();
        }
        cout << endl;
    }
    cout << endl << endl;
}









class SymbolTable
{
    ScopeTable * currentScopeTable;
    int bucket_size;
    int relative_id;

    public:
        SymbolTable();
        SymbolTable(int n);
        ~SymbolTable();

        //main functions
        void EnterScope();
        void ExitScope();
        bool Insert(string, string);
        bool Remove(string);
        SymbolInfo * Lookup(string);
        void PrintCurrentScope();
        void PrintAllScopes();
};



SymbolTable::SymbolTable(){}

SymbolTable::SymbolTable(int n)
{
    bucket_size = n;
    relative_id = 1;

    //globalScope
    currentScopeTable = new ScopeTable(bucket_size);
    currentScopeTable -> setID("1");
    currentScopeTable -> setParent(NULL);

}

SymbolTable::~SymbolTable()
{
    ScopeTable * parent = NULL;
    while(true)
    {
        if(currentScopeTable == NULL)
        {
            break;
        }
        parent = currentScopeTable -> getParent();
        delete currentScopeTable;
        currentScopeTable = parent;
    }
}


void SymbolTable::EnterScope()
{
    //new scope
    ScopeTable * temp = new ScopeTable(bucket_size);
    temp ->setParent(currentScopeTable);
    string id = currentScopeTable -> getID() + "." + to_string(relative_id);
    temp -> setID(id);

    currentScopeTable = temp;
    relative_id = 1;

    cout << "New ScopeTable with id " << id <<" created" << endl;

}

void SymbolTable::ExitScope()
{
    //if currentScope is not Global
    if(currentScopeTable -> getParent() != NULL)
    {
        string id = currentScopeTable -> getID();
        cout << "ScopeTable with id " << id <<" removed" << endl;

        id = id[id.length() - 1];
        relative_id = stoi(id) + 1;

        currentScopeTable = currentScopeTable -> getParent();
    }
}

bool SymbolTable::Insert(string key, string type)
{
    return currentScopeTable -> Insert(key, type);
}

bool SymbolTable::Remove(string key)
{
    return currentScopeTable -> Delete(key);
}

SymbolInfo * SymbolTable::Lookup(string key)
{
    ScopeTable * curr = currentScopeTable;
    SymbolInfo * temp = NULL;
    while(curr != NULL)
    {
        temp = curr -> LookupWithPrint(key);
        if(temp != NULL)
        {
            break;
        }
        curr = curr -> getParent();
    }
    if(temp == NULL) cout << "Not found" << endl;
    return temp;
}

void SymbolTable::PrintCurrentScope()
{
    currentScopeTable -> Print();
}

void SymbolTable::PrintAllScopes()
{
    ScopeTable * curr = currentScopeTable;
    while(curr != NULL)
    {
        curr -> Print();
        curr = curr -> getParent();
    }
}










int main()
{
    //buc_size
    int n;
    SymbolTable * symbolTable;

    cin >> n;
    symbolTable = new SymbolTable(n);

    string inp;
    string name, type;
    string printComm;


    while(true)
    {
        cin >> inp;
        if(!inp.compare("I"))
        {
            cin >> name >> type;
            cout << inp << " " << name << " " << " " << endl;
            symbolTable -> Insert(name, type);
        }
        else if(!inp.compare("L"))
        {
            cin >> name;
            cout << inp << " " << name << endl;
            symbolTable -> Lookup(name);
        }
        else if(!inp.compare("D"))
        {
            cin >> name;
            cout << inp << " " << name << endl;
            symbolTable -> Remove(name);
        }
        else if(!inp.compare("P"))
        {
            cin >> printComm;
            cout << inp << " " << name << " " << " " << endl;
            if(!printComm.compare("A")) symbolTable -> PrintAllScopes();
            else if(!printComm.compare("C")) symbolTable -> PrintCurrentScope();
        }
        else if(!inp.compare("S"))
        {
            cout << inp << endl;
            symbolTable -> EnterScope();
        }
        else if(!inp.compare("E"))
        {
            cout << inp << endl;
            symbolTable -> ExitScope();
        }
        else
        {
            cout << "Invalid Input; Program Ended!" << endl;
            break;
        }
    }


    return 0;
}
