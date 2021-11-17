#include "ProductManager.hpp"

int main() {
    TextDbContext database("Category=/Users/macbookpro/Documents/Workspace/OOP/Project_W5/Project_W5/categories.txt; Product=/Users/macbookpro/Documents/Workspace/OOP/Project_W5/Project_W5/products.txt");
    database.Products.setFileName(database.getFileName("Product"));
    database.Categories.setFileName(database.getFileName("Category"));
    vector<Product*> products = database.Products.all();
    vector<Category*> categories = database.Categories.all();
    bool flag = true;
    while(flag) {
        cout << "What do you want to do? \n" ;
        cout << "1. Display all categories\n";
        cout << "2. Find products by name\n";
        cout << "3. Find products by price range\n";
        cout << "0. Quit\n";
        cout << "Please enter your choice: ";
        int choices; cin >> choices;
        switch (choices) {
            case 1:
            {
                for (auto category : categories) {
                    vector<shared_ptr<Product>> buffer;
                    for (auto product : products ) {
                        if (product->getCategory() == category->getId()) {
                            shared_ptr<Product> temp(product);
                            buffer.push_back(temp);
                        }
                    }
                    category->setProducts(buffer);
                }

                for (auto category : categories) {
                    int cnt = 1;
                    cout << "Type " <<  category->getId()<< endl;
                    vector<shared_ptr<Product>> temp = category->getProducts();
                    for (auto u : temp) {
                        cout << cnt++ <<". " << "Category: " << category->getName() << ", ID: " << u->getId() << ", Name: " << u->getName() << ", Price: " << u->getPrice() << endl;
                    }
                    cnt = 1;
                }
                break;
            }
            case 2:
            {
                string namesearch;
                cout << "\nEnter Name: ";
                cin.ignore();
                getline(cin, namesearch);
                auto result = database.Products.find(nameFilter, namesearch);
                int cnt = 1;
                bool isSuccess;
                int error_code = 0;
                string msg = "";
                vector<Product*> products;
                tie(isSuccess, error_code, msg, products) = result;
                for (auto product : products) {
                    cout << cnt++ <<". " << "Category: " << product->getName() << ", ID: " << product->getId() << ", Name: " << product->getName() << ", Price: " << product->getPrice() << endl;
                }
                break;
            }
            case 3:
            {
                string rangesearch;
                cout << "\nEnter Range : ";
                cin.ignore();
                getline(cin, rangesearch);
                cout << rangesearch[rangesearch.size() - 1];
                auto result = database.Products.find(rangeFilter, rangesearch);
                bool isSuccess;
                int error_code = 0;
                string msg = "";
                vector<Product*> products;
                tie(isSuccess, error_code, msg, products) = result;
                if (isSuccess) {
                    int cnt = 1;
                    for (auto product : products) {
                        cout << cnt++ <<". " << "Category: " << product->getName() << ", ID: " << product->getId() << ", Name: " << product->getName() << ", Price: " << product->getPrice() << endl;
                    }
                }
                else {
                    cout << msg << endl;
                }
                break;
            }
            case 0:
                flag = 0;
                break;
            default:
                break;
        }
    }
	return 0;
}





