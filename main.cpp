#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
using namespace std;

class Category;
class Product;
template <class Entity> class DbSet;
class TextDbContext;


class Category {
private:
	int _categoryId;
	string _categoryName;
	vector<shared_ptr<Product>> _produtes;
public:
    int getCategoryId() { return _categoryId;}
    string getCategoryName() { return _categoryName;}
    vector<shared_ptr<Product>> getProducts() { return _produtes; }
    void setCategoryID(int categoryId) { _categoryId = categoryId; }
    void setCategoryName(int categoryName) { _categoryName = categoryName; }
    void setProducts(vector<shared_ptr<Product>> products) { _produtes = products; }
};

class Product {
private:
	int _productId;
	string _productName;
	int _price;
	shared_ptr<Category> _category;
};

template <class Entity>
class DbSet {
private:
	string _filename;
	typedef bool (*predicate)(Entity);
public:
    DbSet() {
        this->_filename = "";
    }
    
    DbSet(string filename) {
        this->_filename = filename;
    }
    void setFileName(string filename) {
        this->_filename = filename;
    }
    bool isValidFormat(string pattern);
    
    vector<Entity> all() {
        vector<Entity> result;
        ifstream fin(this->_filename);
        string buffer;
        while (!fin.eof()) {
            getline(fin, buffer);
            cout << buffer;
        }
        fin.close();
        return result;
    }
	vector<Entity> find(predicate);
};

vector<string> split(string str,string del) {
	vector<string> result;
	unsigned long long start = 0;
	unsigned long long end = str.find(del);
	while (end != -1) {
		result.push_back(str.substr(start, end - start));
		start = end + del.size();
		end = str.find(del, start);
	}
	result.push_back(str.substr(start, end - start));
	return result;
}

class TextDbContext {
private:
	map<string, string> _filenames;
public:
    string getFileName( string fileName) { return _filenames[fileName]; }
public:
	DbSet<Category> Categories;
	DbSet<Product> Products;
	void parse(string filenames) {
		vector<string> semicolonSlash;
		semicolonSlash = split(filenames, "; ");
		for (auto u : semicolonSlash) {
			_filenames[split(u, "=")[0]] = split(u, "=")[1];
		}
	}
	TextDbContext(string filenames) {
		parse(filenames);
	}
};




int main() {
    TextDbContext t("Category=categories.txt; Product=products.txt");
    cout << t.getFileName("Product") << endl;
    t.Products.setFileName(t.getFileName("Product"));
    t.Products.all();
	return 0;
}





