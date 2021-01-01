#include <iostream>
#include "VK/src/api.h"
using namespace std;

VK::Client api;
int main(){
    if(api.auth("87761796911", "as991gaa")){
        cout << "Auth ok" << endl;
        cout << "Access token: " << api.access_token() << endl << endl;  
        cout << api.call("wall.post", "owner_id=12345&message=Test");
    }
    else{
        cout << "Auth fail" << endl;
    }
}