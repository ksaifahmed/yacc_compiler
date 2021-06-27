#ifndef SYMBOLTABLE_
#define SYMBOLTABLE_

#include <bits/stdc++.h>
#include <cstring>
using namespace std;

class var_type{
	string type; string name; string data_type;
	public:
		string code;
		var_type(string n, string k, string d){ name = n; type = k; data_type = d; }
		void setName(string n){ name = n; }
		void setType(string k){ type = k; }
		void setDataType(string s) { data_type = s; }
		
		string getName(){ return name; }
		string getType(){ return type; }
		string getDataType() { return data_type; }
		
		bool match(string n, string t)
		{
			return (!t.compare(data_type) && !n.compare(name));
		}
		
		bool match(string t)
		{
			return (!t.compare(data_type));
		}		
};

class FunctionInfo{
	bool isDefined;
	vector<var_type*> params;
	public:
		FunctionInfo(){ isDefined = false; }
		void setDefinition(bool n){ isDefined = n; }
		void insertParams(string n, string t, string d){ params.push_back(new var_type(n,t,d));}
		int param_num(){ return params.size(); }
		bool getIsDefined(){ return isDefined; }
		
		bool match(string n, string t, int i)
		{
			return params.at(i) -> match(n,t);
		}
		
		bool match(string t, int i)
		{
			return params.at(i) -> match(t);
		}		
};




class SymbolInfo{
	string name, type, data_type;
	SymbolInfo * next;
	string var_type;
	FunctionInfo func;
	
	public:
		SymbolInfo(){ next = NULL; }

		SymbolInfo(string n, string k)
		{
			name = n;
			type = k;
    			next = NULL;
    			data_type ="";
    			var_type = "";
		}

		~SymbolInfo()
		{
			delete next;
		}

		void setName(string n){ name = n; }
		void setType(string k){ type = k; }
		void setNext(SymbolInfo *s) { next = s; }
		void setDataType(string s) { data_type = s; }
		void setVarType(string s){ var_type = s; }
		void setFunctionInfo(FunctionInfo f) { func = f; }
		bool isDefined() {return func.getIsDefined(); }
		void setDefinition(bool b) { func.setDefinition(b); }

		string getName(){ return name; }
		string getType(){ return type; }
		string getDataType() { return data_type; }
		string getVarType() { return var_type; }
		FunctionInfo getFunctionInfo() { return func; }
		SymbolInfo * getNext(){ return next; }
		bool matchDataType(string str){ return !data_type.compare(str); }

};







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
        ScopeTable(){}

        ScopeTable(int n)
        {
            bucket_size = n;
            scope_array = new SymbolInfo*[n];
            for(int i = 0; i < n; i++) scope_array[i] = NULL;
            parentScope = NULL;
        }


        ~ScopeTable()
        {
            for(int i = 0; i < bucket_size; i++)
                {
                    if(scope_array[i] != NULL) delete scope_array[i];
                    scope_array[i] = NULL;
                }
                if(scope_array != NULL) delete[] scope_array;

                ScopeTable * curr = parentScope;
                ScopeTable * par;
                while(true)
                {
                    if(curr == NULL) break;
                    par = curr -> getParent();
                    delete curr;
                    curr = par;
                }
        }

        string getID(){ return id; }
        ScopeTable * getParent() { return parentScope; }

        void setID(string i) { id = i; }
        void setParent(ScopeTable * parent){ parentScope = parent; }

        //the main functions
        SymbolInfo* Lookup(string key)
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

        SymbolInfo* LookupWithPrint(string key)
        {
            int index = hash_fuc(key);
            int chain_position = 0;
            SymbolInfo * curr = scope_array[index];
            while(true)
            {
                if(curr == NULL) break;
                if(!key.compare(curr -> getName()))
                {
                    cout << "Found in ScopeTable# " << id << " at position " << index << ", " << chain_position << endl << endl;
                    return curr;
                }
                chain_position++;
                curr = curr -> getNext();
            }
            return NULL;
        }

        bool Insert(string key, string type, string data_type)
        {
            SymbolInfo * curr = Lookup(key);
            if(curr != NULL)
            {
                //cout << curr -> getName() << " already exists in current ScopeTable" << endl << endl;
                return false;
            }
            int index = hash_fuc(key);
            int chain_position = 0;
            SymbolInfo * p = new SymbolInfo(key, type);
            p -> setDataType(data_type);

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
            //cout << "Inserted in ScopeTable# " << id << " at position " << index << ", " << chain_position << endl << endl;
            return true;
        }


        bool Insert(string key, string type, string data_type, string var)
        {
            SymbolInfo * curr = Lookup(key);
            if(curr != NULL)
            {
                //cout << curr -> getName() << " already exists in current ScopeTable" << endl << endl;
                return false;
            }
            int index = hash_fuc(key);
            int chain_position = 0;
            SymbolInfo * p = new SymbolInfo(key, type);
            p -> setDataType(data_type);
            p -> setVarType(var);	    		
		
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
            //cout << "Inserted in ScopeTable# " << id << " at position " << index << ", " << chain_position << endl << endl;
            return true;
        }
        
        bool Insert(string key, string type, string data_type, string var, FunctionInfo f)
        {
            SymbolInfo * curr = Lookup(key);
            if(curr != NULL)
            {
                //cout << curr -> getName() << " already exists in current ScopeTable" << endl << endl;
                return false;
            }
            int index = hash_fuc(key);
            int chain_position = 0;
            SymbolInfo * p = new SymbolInfo(key, type);
            p -> setDataType(data_type);
            p -> setVarType(var);
            p -> setFunctionInfo(f);	    		
		
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
            //cout << "Inserted in ScopeTable# " << id << " at position " << index << ", " << chain_position << endl << endl;
            return true;
        }
        

        bool Delete(string key)
        {
            if(Lookup(key) == NULL)
            {
                cout << "Not found" << endl << endl;
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
            cout << "Found in ScopeTable# " << id << " at position " << index << ", " << chain_position << endl << endl;
            cout << "Deleted Entry " << index << ", " << chain_position << " from current ScopeTable" << endl << endl;
            return true;
        }

        void Print()
        {
            cout << "\nScopeTable # " << id << endl;
            for(int i = 0; i < bucket_size; i++)
            {
                SymbolInfo * curr = scope_array[i];
                if(curr == NULL) continue;
                cout << " " << i << " --> ";
                while(true)
                {
                    if(curr == NULL) break;
                    cout << "< " << curr -> getName() << " : " << curr -> getType() << "> ";
                    curr = curr -> getNext();
                }
                cout << endl;
            }
            cout << endl;
        }
        
        void Fprint(FILE * lp)
        {
            fprintf(lp, "\nScopeTable # %s\n", id.c_str());
            for(int i = 0; i < bucket_size; i++)
            {
                SymbolInfo * curr = scope_array[i];
                if(curr == NULL) continue;
                fprintf(lp, " %d --> ", i);
                while(true)
                {
                    if(curr == NULL) break;
                    fprintf(lp, "< %s , %s > ", curr -> getName().c_str(), curr -> getType().c_str());
                    curr = curr -> getNext();
                }
                fprintf(lp, "\n");
            }
            fprintf(lp, "\n");
        }

};
















class SymbolTable
{
    ScopeTable * currentScopeTable;
    int bucket_size;
    int relative_id;

    public:
        SymbolTable(){}

        SymbolTable(int n)
        {
            bucket_size = n;
            relative_id = 1;

            //globalScope
            currentScopeTable = new ScopeTable(bucket_size);
            currentScopeTable -> setID("1");
            currentScopeTable -> setParent(NULL);

        }

        ~SymbolTable()
        {
            if(currentScopeTable != NULL)
                delete currentScopeTable;
            currentScopeTable = NULL;
        }


        //main functions
        void EnterScope()
        {
            //new scope
            ScopeTable * temp = new ScopeTable(bucket_size);
            temp ->setParent(currentScopeTable);
            string id = currentScopeTable -> getID() + "." + to_string(relative_id);
            temp -> setID(id);

            currentScopeTable = temp;
            relative_id = 1;

            //cout << "New ScopeTable with id " << id <<" created" << endl << endl;

        }

        void ExitScope()
        {
            //if currentScope is not Global
            if(currentScopeTable -> getParent() != NULL)
            {
                string id = currentScopeTable -> getID();
                //cout << "ScopeTable with id " << id <<" removed" << endl << endl;

                id = id[id.length() - 1];
                relative_id = stoi(id) + 1;

            currentScopeTable = currentScopeTable -> getParent();
            }
        }

        bool Insert(string key, string type, string data_type)
        {
            return currentScopeTable -> Insert(key, type, data_type);
        }
        
        bool Insert(string key, string type, string data_type, string var)
        {
            return currentScopeTable -> Insert(key, type, data_type, var);
        }
        
        bool Insert(string key, string type, string data_type, string var, FunctionInfo f)
        {
            return currentScopeTable -> Insert(key, type, data_type, var, f);
        }


        bool Remove(string key)
        {
            return currentScopeTable -> Delete(key);
        }

        SymbolInfo* Lookup(string key)
        {
            ScopeTable * curr = currentScopeTable;
            SymbolInfo * temp = NULL;
            while(curr != NULL)
            {
                temp = curr -> Lookup(key);
                if(temp != NULL)
                {
                    break;
                }
                curr = curr -> getParent();
            }
            //if(temp == NULL) cout << "Not found" << endl << endl;
            return temp;
        }

        void PrintCurrentScope()
        {
            currentScopeTable -> Print();
        }

        void PrintAllScopes()
        {
            ScopeTable * curr = currentScopeTable;
            while(curr != NULL)
            {
                curr -> Print();
                curr = curr -> getParent();
            }
        }
        
        
        void PrintInFile(FILE * lp)
        {
            ScopeTable * curr = currentScopeTable;
            while(curr != NULL)
            {
                curr -> Fprint(lp);
                curr = curr -> getParent();
            }
        }
};






#endif
