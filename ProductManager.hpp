#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <typeinfo>
#include <regex>

using namespace std;

vector<string> split(string str,string del);
class Category;
class Product;
class DbSetProduct;
class TextDbContext;
class RandomIntegerGenerator;
class IntegerToThousandSeparatedUIConverter;
class IntegerToCurrencyUIConverter;
bool nameFilter(Product* product, string searchname);
bool rangeFilter(Product* product, string range);

class RandomIntegerGenerator {
public:
    RandomIntegerGenerator() {
        srand(time(NULL));
    }

    long long next() {
        return rand();
    }

    long long next(int ceiling) {
        return rand() % ceiling;
    }

    long long next(int left, int right) {
        return rand() % (right - left + 1) + left;
    }
};


class IntegerToThousandSeparatedUIConverter {
private:
    string _thousandSeparator = ".";
public:
    bool isValidFormat(string);
    string convert(const long long&);
    tuple<bool, int, string, long long> convertBack(string);
};

class IntegerToCurrencyUIConverter {
private:
    string _thousandSeparator = ".";
    string _currency = "d";
public:
    bool isValidFormat(string);
    string convert(const long long&);
    tuple<bool, int, string, long long> convertBack(string);
};

class Category {
private:
    int _categoryId;
    string _categoryName;
    vector<shared_ptr<Product>> _products;
public:
    Category() {
        
    }
    
    Category(int categoryId, string categoryName) {
        this->_categoryId = categoryId;
        this->_categoryName = categoryName;
    }
public:
    int getId() { return _categoryId; }
    string getName() { return _categoryName; }
    vector<shared_ptr<Product>> getProducts() { return _products; }
public:
    void setCategoryID(int categoryId) { _categoryId = categoryId; }
    void setCategoryName(int categoryName) { _categoryName = categoryName; }
    void setProducts(vector<shared_ptr<Product>> products) { _products = products; }
};

class Product {
private:
    int _productId;
    string _productName;
    string _price;
    int _category;
public:
    Product() {
        
    }
    Product(int productId, string productName, string price, int category) {
        this->_productId = productId;
        this->_productName = productName;
        this->_price = price;
        this->_category = category;
    }
public:
    int getId() { return this->_productId; }
    string getName() { return this->_productName; }
    string getPrice() { return this->_price; }
    int getCategory() { return this->_category; }
 };


class DbSetProduct {
private:
    string _filename;
    vector<Product*> _products;
    string _searchname;
    string _searchrange;
    IntegerToThousandSeparatedUIConverter _priceFormat;
public:
    bool (*predicate)(Product*, string);
    DbSetProduct() {
        this->_filename = "";
    }
    
    DbSetProduct(string filename) {
        this->_filename = filename;
        this->_products = all();
    }
    
    string getSearchName() {
        return this->_searchname;
    }

    void setSearchName(string searchname) {
        this->_searchname = searchname;
    }
    
    void setPriceRange(string searchrange) {
        this->_searchrange = searchrange;
    }
    
    string getPriceRange() {
        return this->_searchrange;
    }
    
    void setFileName(string filename) {
        this->_filename = filename;
        this->_products = all();
    }
    
    void setProducts() {
        this->_products = all();
    }
    
    string getFileName() {
        return this->_filename;
    }
public:
    bool isValidFormat(string pattern);
    vector<Product*> all();
    tuple<bool, int, string, vector<Product*>> find(bool (*predicate)(Product*, string), string);
};

class DbSetCategory {
private:
    string _filename;
    typedef bool (*predicate)(Category);
public:
    DbSetCategory() {
        this->_filename = "";
    }
    
    DbSetCategory(string filename) {
        this->_filename = filename;
    }

    void setFileName(string filename) {
        this->_filename = filename;
    }
    
    string getFileName() {
        return this->_filename;
    }
public:
    bool isValidFormat(string pattern);
    vector<Category*> all();
    vector<Category*> find(predicate);
};

class TextDbContext {
private:
    map<string, string> _filenames;
public:
    string getFileName( string fileName) { return _filenames[fileName]; }
public:
    DbSetCategory Categories;
    DbSetProduct Products;
    
    
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

