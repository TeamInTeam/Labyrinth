#define DATABASE_PGSQL

#include <iostream>
#include "AuthenticationManager.h"
#include <odb/database.hxx>
#include "map.hxx"
#include "map-odb.hxx"
#include "MapProvider.h"

/**
 * @author Andrey Smirnov
 *
 * DATABASE DEMO and TESTS
 */

using namespace std;
using namespace odb::core;

void user_exists() {
    model::AuthenticationManager manager;

    cout << "kupihleba:\t" << (manager.user_exists("KUPIHLEBA") ? "exist" : "do not exist") << endl;
}

void test_map_import() {
    std::vector <std::vector<int>> map;
    std::vector<int> t1;
    t1.push_back(1);
    t1.push_back(2);
    t1.push_back(3);
    std::vector<int> t2;
    t2.push_back(10);
    t2.push_back(20);
    t2.push_back(30);
    map.push_back(t1);
    map.push_back(t2);

    std::tr1::shared_ptr <Map> new_map(new Map("test", map));
    model::MapProvider provider;
    cout << "map import:\t" << (provider.import_map(new_map) ? "success" : "fail") << endl;
}

void test_map_export() {
    model::MapProvider pr;
    std::tr1::shared_ptr <Map> res = pr.get_map_by_name("test");
    if (res.get() == nullptr) {
        cout << "no such map!" << endl;
        return;
    } else {
        cout << "map found:" << endl;
    }
    for (auto &i : res->get_map()) {
        for (auto &elem : i) {
            cout << elem << '\t';
        }
        cout << endl;
    }
}

void test_registration() {
    model::AuthenticationManager manager;
    try { // REMEMBER HANDLING EXCEPTIONS !!!
        cout << "registration:\t" << (manager.reg("Kupihleba", "Strong_passwd") ? "success" : "fail") << endl;
        cout << "invalid registration:\t" << (manager.reg("\"Select * from users", "password") ? "success" : "fail")
             << endl;
    } catch (std::length_error) {
        cout << "Password should be >= 8 chars!" << endl;
    } catch (std::invalid_argument) {
        cout << "Login must contain only A-Z a-z 0-9 !" << endl;
    }
}

void change_pass() {
    try { // REMEMBER HANDLING EXCEPTIONS !!!
        model::AuthenticationManager manager;
        cout << "change pass:\t" << (manager.update_password("kupihleba", "secret++") ? "success" : "fail") << endl;
        cout << "old passw\t" << (manager.auth("Kupihleba", "Strong_passwd") ? "authenticated" : "wrong") << endl;
        cout << "new passw\t" << (manager.auth("Kupihleba", "secret++") ? "authenticated" : "wrong") << endl;
    } catch (std::length_error) {
        cout << "Password should be >= 8 chars!" << endl;
    }
}

void test_auth() {
    model::AuthenticationManager auth;
    cout << "authentication [wrong user]:\t" << auth.auth("user_that_do_not_exist", "Strong_passwd") << endl;
    cout << "authentication [wrong passwd]:\t" << auth.auth("kupihleba", "random") << endl;
    cout << "authentication [correct data]:\t" << auth.auth("KUpiHLEba", "Strong_passwd")
         << endl; // case insensitive username
}


int main(int argc, char *argv[]) {
    cout << "hi!" << endl;

    user_exists();
    test_registration();
    test_auth();
    test_map_import();
    change_pass();
    test_map_export();
    user_exists();

    return 0;
}