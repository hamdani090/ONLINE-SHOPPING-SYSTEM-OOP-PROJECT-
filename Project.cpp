#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cctype>

using namespace std;

const int MAX_PRODUCTS = 50;
const int MAX_USERS = 50;
const int MAX_ORDERS = 200;
const int MAX_CART_ITEMS = 20;
const int MAX_ORDER_ITEMS = 20;

class Product {
protected:
    int id;
    string name;
    string category;
    double pricePKR;

public:
    Product(int i = 0, const string& n = "", const string& cat = "", double p = 0.0)
        : id(i), name(n), category(cat), pricePKR(p) {}

    virtual ~Product() {}

    virtual void displayDetails() const = 0;
    virtual double calculatePrice(int quantity) const {
        return pricePKR * quantity;
    }

    int getId() const { return id; }
    string getName() const { return name; }
    string getCategory() const { return category; }
    double getBasePrice() const { return pricePKR; }
};

class FashionProduct : public Product {
    string subCategory;
public:
    FashionProduct(int i, const string& n, double p, const string& sub)
        : Product(i, n, "Fashion", p), subCategory(sub) {}
    void displayDetails() const override {
        cout << "[ID: " << id << "] " << name << " | Category: " << category
             << " (" << subCategory << ") | Price: PKR " << fixed << setprecision(2) << pricePKR << endl;
    }
};

class EducationProduct : public Product {
    string subCategory;
public:
    EducationProduct(int i, const string& n, double p, const string& sub)
        : Product(i, n, "Education", p), subCategory(sub) {}
    void displayDetails() const override {
        cout << "[ID: " << id << "] " << name << " | Category: " << category
             << " (" << subCategory << ") | Price: PKR " << fixed << setprecision(2) << pricePKR << endl;
    }
};

class AutomobileProduct : public Product {
    string subCategory;
public:
    AutomobileProduct(int i, const string& n, double p, const string& sub)
        : Product(i, n, "Automobiles", p), subCategory(sub) {}
    void displayDetails() const override {
        cout << "[ID: " << id << "] " << name << " | Category: " << category
             << " (" << subCategory << ") | Price: PKR " << fixed << setprecision(2) << pricePKR << endl;
    }
    double calculatePrice(int quantity) const override {
        double base = Product::calculatePrice(quantity);
        return base + base * 0.05;
    }
};

class ElectronicsProduct : public Product {
    string subCategory;
public:
    ElectronicsProduct(int i, const string& n, double p, const string& sub)
        : Product(i, n, "Electronics", p), subCategory(sub) {}
    void displayDetails() const override {
        cout << "[ID: " << id << "] " << name << " | Category: " << category
             << " (" << subCategory << ") | Price: PKR " << fixed << setprecision(2) << pricePKR << endl;
    }
};

class CartItem {
    Product* product;
    int quantity;
public:
    CartItem() : product(NULL), quantity(0) {}
    CartItem(Product* p, int q) : product(p), quantity(q) {}

    void set(Product* p, int q) { product = p; quantity = q; }
    Product* getProduct() const { return product; }
    int getQuantity() const { return quantity; }
    double getTotalPrice() const {
        if (!product) return 0.0;
        return product->calculatePrice(quantity);
    }
    bool isEmpty() const { return product == NULL || quantity <= 0; }
};

class Order {
    static int nextOrderId;
    int orderId;
    string customerUsername;
    string deliveryAddress;
    CartItem items[MAX_ORDER_ITEMS];
    int itemsCount;
    double totalCost;
    string deliveryType;
    double deliveryCharge;
    string paymentMethod;
    string status;

public:
    Order()
        : orderId(0), customerUsername(""), deliveryAddress(""), itemsCount(0),
          totalCost(0.0), deliveryType("Normal"), deliveryCharge(0.0),
          paymentMethod(""), status("Placed") {}

    void initialize(const string& uname, const string& addr, CartItem cart[],
                    int cartCount, const string& pMethod, const string& dType, double baseCost) {
        orderId = nextOrderId++;
        customerUsername = uname;
        deliveryAddress = addr;
        itemsCount = 0;
        for (int i = 0; i < cartCount && i < MAX_ORDER_ITEMS; ++i) {
            items[i] = cart[i];
            itemsCount++;
        }
        paymentMethod = pMethod;
        deliveryType = dType;
        deliveryCharge = (dType == "Urgent") ? 500.0 : 0.0;
        totalCost = baseCost + deliveryCharge;
        status = "Placed";
    }

    int getId() const { return orderId; }
    string getUsername() const { return customerUsername; }
    string getAddress() const { return deliveryAddress; }
    string getStatus() const { return status; }
    double getTotalCost() const { return totalCost; }

    void setStatus(const string& s) { status = s; }

    void displayOrder() const {
        cout << "\n--- Order ID: " << orderId << " ---" << endl;
        cout << "  Customer: " << customerUsername << endl;
        cout << "  Address: " << deliveryAddress << endl;
        cout << "  Delivery Type: " << deliveryType << " (" << (deliveryType == "Urgent" ? "3 days" : "5 days") << ")" << endl;
        cout << "  Payment: " << paymentMethod << endl;
        cout << "  Status: " << status << endl;
        cout << "  Items:" << endl;
        for (int i = 0; i < itemsCount; ++i) {
            Product* p = items[i].getProduct();
            if (p) {
                cout << "    - " << p->getName()
                     << " x " << items[i].getQuantity()
                     << " @ PKR " << fixed << setprecision(2) << items[i].getTotalPrice() << endl;
            }
        }
        cout << "  Delivery Charge: PKR " << fixed << setprecision(2) << deliveryCharge << endl;
        cout << "  FINAL TOTAL: PKR " << fixed << setprecision(2) << totalCost << endl;
    }
};

int Order::nextOrderId = 1001;

class NTSHOP;

class User {
protected:
    string username;
    string password;
    string address;
public:
    User(const string& u = "", const string& p = "", const string& a = "")
        : username(u), password(p), address(a) {}
    virtual ~User() {}
    virtual void startSession() = 0;
    string getUsername() const { return username; }
    string getAddress() const { return address; }
    void setAddress(const string& a) { address = a; }
};

class Customer : public User {
    CartItem shoppingCart[MAX_CART_ITEMS];
    int cartCount;
    NTSHOP* shopSystem;

public:
    Customer(const string& u = "", const string& p = "", NTSHOP* shop = NULL)
        : User(u, p, ""), cartCount(0), shopSystem(shop) {}

    void startSession() override;
    void viewCart() const;
    void checkout();
    void viewOrderHistory() const;
    double calculateCartTotal() const;
    bool addToCart(Product* p, int q);
    void clearCart();
};

class Admin : public User {
    NTSHOP* shopSystem;
public:
    Admin(const string& u = "", const string& p = "", NTSHOP* shop = NULL)
        : User(u, p, ""), shopSystem(shop) {}
    void startSession() override;
    void viewOrders() const;
    void markOrderDelivered();
    void searchCustomer() const;
};

class NTSHOP {
    Product* allProducts[MAX_PRODUCTS];
    int productCount;
    User* allUsers[MAX_USERS];
    int userCount;
    Order allOrders[MAX_ORDERS];
    int orderCount;

public:
    NTSHOP() : productCount(0), userCount(0), orderCount(0) {
        allUsers[userCount++] = new Admin("admin", "admin123", this);
        addProduct(new FashionProduct(1, "Slim Fit Jeans", 3500.0, "Male Clothings"));
        addProduct(new FashionProduct(2, "Leather Handbag", 6800.0, "Female Accessories"));
        addProduct(new EducationProduct(3, "Basic Geometry Box", 550.0, "Writing Materials"));
        addProduct(new AutomobileProduct(4, "Brake Pads (Set of 4)", 12500.0, "Car/Motorbike Spare"));
        addProduct(new ElectronicsProduct(5, "43-inch 4K Smart TV", 75000.0, "TV"));
        addProduct(new ElectronicsProduct(6, "Core i5 Laptop", 98000.0, "Laptops"));
    }

    ~NTSHOP() {
        for (int i = 0; i < productCount; ++i) delete allProducts[i];
        for (int i = 0; i < userCount; ++i) delete allUsers[i];
    }

    bool addProduct(Product* p) {
        if (productCount >= MAX_PRODUCTS) return false;
        allProducts[productCount++] = p;
        return true;
    }

    Product* getProductById(int id) const {
        for (int i = 0; i < productCount; ++i)
            if (allProducts[i]->getId() == id) return allProducts[i];
        return NULL;
    }

    void displayAllProductsByCategory(const string& cat) const {
        bool found = false;
        cout << "\n--- Products in " << cat << " ---" << endl;
        for (int i = 0; i < productCount; ++i) {
            if (allProducts[i]->getCategory() == cat) {
                allProducts[i]->displayDetails();
                found = true;
            }
        }
        if (!found) cout << "No products found in this category." << endl;
        cout << "--------------------------------\n" << endl;
    }

    void displayAllProducts() const {
        cout << "\n--- All Products ---" << endl;
        for (int i = 0; i < productCount; ++i)
            allProducts[i]->displayDetails();
    }

    bool registerCustomer(const string& u, const string& p) {
        if (findUser(u) != NULL) return false;
        if (userCount >= MAX_USERS) return false;
        allUsers[userCount++] = new Customer(u, p, this);
        return true;
    }

    User* findUser(const string& uname) const {
        for (int i = 0; i < userCount; ++i)
            if (allUsers[i]->getUsername() == uname) return allUsers[i];
        return NULL;
    }

    bool addOrder(const Order& o) {
        if (orderCount >= MAX_ORDERS) return false;
        allOrders[orderCount++] = o;
        cout << "\n\n********************************************************" << endl;
        cout << "    Order Placed Successfully! Order ID: " << allOrders[orderCount-1].getId() << endl;
        cout << "********************************************************\n" << endl;
        return true;
    }

    int getOrderCount() const { return orderCount; }
    const Order& getOrderAt(int idx) const { return allOrders[idx]; }
    Order& getOrderAt(int idx) { return allOrders[idx]; }

    void displayAllOrders() const {
        if (orderCount == 0) { cout << "\nNo orders placed yet." << endl; return; }
        for (int i = 0; i < orderCount; ++i) allOrders[i].displayOrder();
    }

    void displayDeliveredOrders() const {
        cout << "\n--- Delivered Orders ---" << endl;
        bool found = false;
        for (int i = 0; i < orderCount; ++i) {
            if (allOrders[i].getStatus() == "Delivered") {
                allOrders[i].displayOrder();
                found = true;
            }
        }
        if (!found) cout << "No delivered orders found." << endl;
    }

    User** getUsersArray() { return allUsers; }
    int getUserCount() const { return userCount; }
    Product** getProductArray() { return allProducts; }
    int getProductCount() const { return productCount; }
};

bool Customer::addToCart(Product* p, int q) {
    if (!p || q <= 0) return false;
    if (cartCount >= MAX_CART_ITEMS) return false;
    shoppingCart[cartCount++].set(p, q);
    return true;
}

void Customer::viewCart() const {
    if (cartCount == 0) {
        cout << "\n Your cart is empty." << endl;
        return;
    }
    cout << "\n--- Your Shopping Cart ---" << endl;
    double total = 0.0;
    for (int i = 0; i < cartCount; ++i) {
        const CartItem& item = shoppingCart[i];
        cout << (i + 1) << ". " << item.getProduct()->getName()
             << " x " << item.getQuantity()
             << " | Price: PKR " << fixed << setprecision(2) << item.getTotalPrice() << endl;
        total += item.getTotalPrice();
    }
    cout << "--------------------------------" << endl;
    cout << "Subtotal: PKR " << fixed << setprecision(2) << total << endl;
    cout << "--------------------------------\n" << endl;
}

double Customer::calculateCartTotal() const {
    double total = 0.0;
    for (int i = 0; i < cartCount; ++i) total += shoppingCart[i].getTotalPrice();
    return total;
}

void Customer::clearCart() {
    for (int i = 0; i < cartCount; ++i) shoppingCart[i] = CartItem();
    cartCount = 0;
}

void Customer::checkout() {
    if (cartCount == 0) {
        cout << "\n Cannot checkout. Your cart is empty." << endl;
        return;
    }

    string tempUsername, tempAddress;
    cout << "\n--- Checkout Process ---" << endl;
    cout << "Enter your delivery username: ";
    cin >> tempUsername;
    cout << "Enter your full delivery address: ";
    cin.ignore();
    getline(cin, tempAddress);
    this->address = tempAddress;

    int paymentChoice;
    string paymentMethod;
    cout << "\nSelect Payment Method (1 for Advance, 2 for Cash on Delivery): ";
    cin >> paymentChoice;

    if (paymentChoice == 1) {
        paymentMethod = "Advance Payment";
        string accountName, accountNumber;
        cout << "Enter Account Name: ";
        cin.ignore();
        getline(cin, accountName);
        cout << "Enter Account Number: ";
        getline(cin, accountNumber);
        cout << "Payment details secured for advance payment." << endl;
    } else {
        paymentMethod = "Cash on Delivery (COD)";
    }

    int deliveryChoice;
    string deliveryType;
    double baseTotal = calculateCartTotal();
    cout << "\nSelect Delivery Type:" << endl;
    cout << "1. Normal Delivery (5 days, No extra charge)" << endl;
    cout << "2. Urgent Delivery (3 days, PKR 500 extra charge)" << endl;
    cout << "Your choice: ";
    cin >> deliveryChoice;

    if (deliveryChoice == 2) {
        deliveryType = "Urgent";
        cout << "Urgent Delivery selected (PKR 500 added to total)." << endl;
    } else {
        deliveryType = "Normal";
    }

    cout << "\n\n--- Order Summary ---" << endl;
    viewCart();
    cout << "Payment: " << paymentMethod << endl;
    cout << "Delivery: " << deliveryType << endl;

    char confirm;
    cout << "Proceed with placing order? (y/n): ";
    cin >> confirm;

    if (tolower(confirm) != 'y') {
        cout << "\nOrder cancelled by user. Cart retained." << endl;
        return;
    }

    Order newOrder;
    newOrder.initialize(this->username, this->address, shoppingCart, cartCount, paymentMethod, deliveryType, baseTotal);

    if (shopSystem->addOrder(newOrder)) {
        clearCart();
    } else {
        cout << "Failed to add order to system ." << endl;
    }
}

void Customer::viewOrderHistory() const {
    cout << "\n--- Your Order History ---" << endl;
    bool found = false;
    int oc = shopSystem->getOrderCount();
    for (int i = 0; i < oc; ++i) {
        const Order& o = shopSystem->getOrderAt(i);
        if (o.getUsername() == this->username) {
            o.displayOrder();
            found = true;
        }
    }
    if (!found) cout << "You have no orders yet." << endl;
}

void Customer::startSession() {
    int choice;
    while (true) {
        cout << "\n--- Welcome, " << username << " to N&T SHOP ---" << endl;
        cout << "1. Browse Products by Category" << endl;
        cout << "2. View Cart and Checkout" << endl;
        cout << "3. View Order History " << endl;
        cout << "4. Logout" << endl;
        cout << "Enter choice: ";
        if (!(cin >> choice)) {
            cin.clear(); cin.ignore(10000, '\n');
            cout << "Invalid input. Please try again." << endl;
            continue;
        }
        if (choice == 4) break;

        if (choice == 1) {
            int catChoice;
            string catName;
            cout << "\n--- Select Category ---" << endl;
            cout << "1: Fashion\n2: Education\n3: Automobiles\n4: Electronics\nYour choice: ";
            cin >> catChoice;
            switch (catChoice) {
                case 1: catName = "Fashion"; break;
                case 2: catName = "Education"; break;
                case 3: catName = "Automobiles"; break;
                case 4: catName = "Electronics"; break;
                default: cout << "Invalid category." << endl; continue;
            }
            shopSystem->displayAllProductsByCategory(catName);

            int productId, quantity;
            cout << "Enter Product ID to add to cart (0 to skip): ";
            if (!(cin >> productId)) {
                cin.clear(); cin.ignore(10000, '\n'); continue;
            }
            if (productId == 0) continue;

            Product* selectedProduct = shopSystem->getProductById(productId);
            if (selectedProduct) {
                cout << "Enter quantity: ";
                if (!(cin >> quantity) || quantity <= 0) {
                    cin.clear(); cin.ignore(10000, '\n');
                    cout << "Invalid quantity. Skipped." << endl;
                    continue;
                }
                if (addToCart(selectedProduct, quantity)) {
                    cout << " Added " << quantity << " x " << selectedProduct->getName() << " to cart." << endl;
                } else {
                    cout << "Failed to add item to cart ." << endl;
                }
            } else {
                cout << " Invalid Product ID." << endl;
            }
        } else if (choice == 2) {
            viewCart();
            if (cartCount > 0) {
                char confirm;
                cout << "Ready to checkout? (y/n): ";
                cin >> confirm;
                if (tolower(confirm) == 'y') checkout();
            }
        } else if (choice == 3) {
            viewOrderHistory();
        } else {
            cout << "Invalid option." << endl;
        }
    }
}

void Admin::viewOrders() const {
    shopSystem->displayAllOrders();
}

void Admin::markOrderDelivered() {
    int id;
    cout << "Enter Order ID to mark as delivered: ";
    if (!(cin >> id)) {
        cin.clear(); cin.ignore(10000, '\n');
        cout << "Invalid ID." << endl;
        return;
    }
    int oc = shopSystem->getOrderCount();
    for (int i = 0; i < oc; ++i) {
        Order& o = shopSystem->getOrderAt(i);
        if (o.getId() == id) {
            if (o.getStatus() == "Placed") {
                o.setStatus("Delivered");
                cout << " Order ID " << id << " marked as 'Delivered'." << endl;
                return;
            } else {
                cout << " Order ID " << id << " is already " << o.getStatus() << "." << endl;
                return;
            }
        }
    }
    cout << " Order ID " << id << " not found." << endl;
}

void Admin::searchCustomer() const {
    string searchKey;
    int searchType;

    cout << "\n--- Search Customer ---" << endl;
    cout << "Search by: 1) Username or 2) Address: ";
    if (!(cin >> searchType)) {
        cin.clear(); cin.ignore(10000, '\n'); return;
    }

    if (searchType == 1) {
        cout << "Enter Username: ";
        cin >> searchKey;
    } else if (searchType == 2) {
        cout << "Enter Address : ";
        cin.ignore();
        getline(cin, searchKey);
    } else {
        cout << "Invalid search type." << endl;
        return;
    }

    cout << "\n--- Search Results ---" << endl;
    bool found = false;

    User** users = shopSystem->getUsersArray();
    int uc = shopSystem->getUserCount();

    for (int i = 0; i < uc; ++i) {
        Customer* customer = dynamic_cast<Customer*>(users[i]);
        if (customer) {
            bool matches = false;
            if (searchType == 1 && customer->getUsername() == searchKey) matches = true;
            if (searchType == 2 && customer->getAddress().find(searchKey) != string::npos && !searchKey.empty()) matches = true;

            if (matches) {
                found = true;
                cout << "Found Customer: " << customer->getUsername() << endl;
                cout << "  - Last Known Address: " << customer->getAddress() << endl;

                double totalSpent = 0.0;
                int ordersCount = 0;
                int oc = shopSystem->getOrderCount();
                for (int j = 0; j < oc; ++j) {
                    const Order& o = shopSystem->getOrderAt(j);
                    if (o.getUsername() == customer->getUsername() && o.getStatus() != "Cancelled") {
                        totalSpent += o.getTotalCost();
                        ordersCount++;
                    }
                }

                cout << "  - Total Orders Placed : " << ordersCount << endl;
                cout << "  - Total Amount Shopped: PKR " << fixed << setprecision(2) << totalSpent << endl;
            }
        }
    }

    if (!found) {
        cout << "No customers found matching the criteria." << endl;
    }
}

void Admin::startSession() {
    int choice;
    while (true) {
        cout << "\n--- Welcome, Admin (" << username << ") ---" << endl;
        cout << "1. View All Orders" << endl;
        cout << "2. View Delivered Orders" << endl;
        cout << "3. Mark Order as Delivered" << endl;
        cout << "4. Search Customer Information" << endl;
        cout << "5. Logout" << endl;
        cout << "Enter choice: ";
        if (!(cin >> choice)) {
            cin.clear(); cin.ignore(10000, '\n');
            cout << "Invalid input. Please try again." << endl;
            continue;
        }
        if (choice == 5) break;

        switch (choice) {
            case 1: viewOrders(); break;
            case 2: shopSystem->displayDeliveredOrders(); break;
            case 3: markOrderDelivered(); break;
            case 4: searchCustomer(); break;
            default: cout << "Invalid option." << endl;
        }
    }
}

void runSystem(NTSHOP* shop) {
    string username, password;
    int roleChoice;

    cout << "\n=======================================" << endl;
    cout << "  WELCOME TO N&T SHOP ONLINE SYSTEM" << endl;
    cout << "=======================================" << endl;

    while (true) {
        cout << "Are you a Customer or an Admin?" << endl;
        cout << "1. Customer\n2. Admin\n3. Register New Customer\n4. Exit" << endl;
        cout << "Enter choice: ";
        if (!(cin >> roleChoice)) {
            cin.clear(); cin.ignore(10000, '\n');
            cout << "Invalid input. Please try again." << endl;
            continue;
        }
        if (roleChoice == 4) break;

        if (roleChoice == 3) {
            cout << "Enter new username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;
            if (shop->registerCustomer(username, password)) {
                cout << "\n Customer '" << username << "' registered successfully!" << endl;
            } else {
                cout << "\n Error: Could not register (username exists or capacity reached)." << endl;
            }
            continue;
        }

        cout << "Enter Username: ";
        cin >> username;
        cout << "Enter Password: ";
        cin >> password;

        User* currentUser = shop->findUser(username);

        if (currentUser) {
            bool authenticated = false;
            Admin* admin = dynamic_cast<Admin*>(currentUser);
            Customer* cust = dynamic_cast<Customer*>(currentUser);

            if (admin) {
                if (username == "admin" && password == "admin123") authenticated = true;
            } else if (cust) {
                authenticated = true;
            }

            if (authenticated) {
                if (roleChoice == 1 && cust) {
                    cust->startSession();
                } else if (roleChoice == 2 && admin) {
                    admin->startSession();
                } else {
                    cout << " Role mismatch. Please log in with the correct role." << endl;
                }
            } else {
                cout << " Authentication failed. Incorrect password." << endl;
            }
        } else {
            cout << " User not found. Please try again or register." << endl;
        }
    }
    cout << "\nThank you for using N&T SHOP. Goodbye!" << endl;
}

int main() {
    cout << fixed << setprecision(2);
    NTSHOP* shop = new NTSHOP();
    runSystem(shop);
    delete shop;
    return 0;
}