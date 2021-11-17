#include "ProductManager.hpp"

bool IntegerToThousandSeparatedUIConverter::isValidFormat(string value) {
    regex pricePattern("\\d{1,3}(\\.\\d{3})*\\.\\d{3}");
    bool result = regex_match(value, pricePattern);
    return result;
}

string IntegerToThousandSeparatedUIConverter::convert(const long long& value) {
    string result = "";
    result = to_string(value);
    unsigned long i = result.size() - 3;
    while (i > 0) {
        result.insert(i, ".");
        i -= 3;
    }
    return result;
}

tuple<bool, int, string, long long> IntegerToThousandSeparatedUIConverter::convertBack(string value) {
    bool succeeded = true;
    int error_code = 0;
    string message = "";
    long long data = 0;

    bool isValid = isValidFormat(value);
    if (isValid) {
        succeeded = true;
        value.erase(remove(value.begin(), value.end(), '.'), value.end());
        data = stoll(value);
    }
    else {
        succeeded = false;
        error_code = 1;
        message = "Invalid number format exception!";
    }
    auto result = make_tuple(succeeded, error_code, message, data);
    return result;
}



bool IntegerToCurrencyUIConverter::isValidFormat(string value) {
    regex pricePattern("\\b\\d+.\\d+.\\d+\\b\\s\\w");
    bool result = regex_match(value, pricePattern);
    return result;
}

string IntegerToCurrencyUIConverter::convert(const long long& value) {
    string result = "";
    result = to_string(value);
    unsigned long i = result.size() - 3;
    while (i > 0) {
        result.insert(i, ".");
        i -= 3;
    }
    result += " " + this->_thousandSeparator;
    return result;
}

tuple<bool, int, string, long long> IntegerToCurrencyUIConverter::convertBack(string value) {
    value = value.substr(0, value.size() - 2);
    bool succeeded = true;
    int error_code = 0;
    string message = "";
    long long data = 0;

    bool isValid = isValidFormat(value);
    if (isValid) {
        succeeded = true;
        value.erase(remove(value.begin(), value.end(), '.'), value.end());
        data = stoll(value);
    }
    else {
        succeeded = false;
        error_code = 1;
        message = "Invalid number format exception!";
    }
    auto result = make_tuple(succeeded, error_code, message, data);
    return result;
}


bool nameFilter(Product* product, string searchname) {
    bool result = false;
    string productName = product->getName();
    if (productName.find(searchname) != -1) {
        result = true;
    }
    return result;
}

bool rangeFilter(Product* product, string range) {
    IntegerToThousandSeparatedUIConverter convertor;
    bool result = false;
    string buffer = product->getPrice();
    tuple<bool, int, string, long long> priceProduct = convertor.convertBack(buffer);
    string rangeBuffer = range.substr(1, range.size() - 2);
    vector<string> slashRange = split(rangeBuffer, ", ");
    tuple<bool, int, string, long long> priceStart = convertor.convertBack(slashRange[0]);
    tuple<bool, int, string, long long> priceEnd = convertor.convertBack(slashRange[1]);
    if (get<3>(priceProduct) >= get<3>(priceStart) && get<3>(priceProduct) <= get<3>(priceEnd)) {
        result = true;
    }
    return result;
}

tuple<bool, int, string, vector<Product*>> DbSetProduct::find(bool (*predicate)(Product*, string), string str) {
    string pattern = "\\[\\d{1,3}(\\.\\d{3})*\\.\\d{3}\\,\\s\\d{1,3}(\\.\\d{3})*\\.\\d{3}\\]";
    regex rangePattern(pattern);
    tuple<bool, int, string, vector<Product*>> result;
    vector<Product*> target;
    bool check = regex_match(str, rangePattern);
    if (check == true) {
        for (auto u : this->_products) {
            if (predicate(u,str)) {
                target.push_back(u);
            }
        }
        result = make_tuple(true, 0, "", target);
        return result;
    }
    result = make_tuple(false, 1, "Format Error!", target);
    return result;
    //return result;
}

vector<Product*> DbSetProduct::all() {
    vector<Product*> values;
    ifstream fin(this->_filename);
    string buffer;
    while (!fin.eof()) {
        vector<string> temp;
        getline(fin, buffer);
        temp = split(split(buffer, ": ")[1], ", ");
        int productId = stoi(split(temp[0], "=")[1]);
        string productName = split(temp[1], "=")[1];
        string price = split(temp[2], "=")[1];
        string categoryBuffer = split(temp[3], "=")[1];
        char checkEnterValue = categoryBuffer[categoryBuffer.size() - 1];
        if (checkEnterValue == '\r') {
            categoryBuffer = categoryBuffer.substr(0, categoryBuffer.size() - 1);
        }
        int category = stoi(categoryBuffer);
        Product* p = new Product(productId, productName, price, category);
        values.push_back(p);
    }
    fin.close();
    return values;
}



vector<Category*> DbSetCategory::all() {
    vector<Category*> values;
    ifstream fin(this->_filename);
    string buffer;
    while (getline(fin, buffer)) {
        vector<string> temp;
        temp = split(split(buffer, ": ")[1], ", ");
        int categoryId = stoi(split(temp[0], "=")[1]);
        string categoryName = split(temp[1], "=")[1];
        char checkEnterValue = categoryName[categoryName.size() - 1];
        if (checkEnterValue == '\r') {
            categoryName = categoryName.substr(0, categoryName.size() - 1);
        }
        Category* p = new Category(categoryId, categoryName);
        values.push_back(p);
    }
    fin.close();
    return values;
}

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

