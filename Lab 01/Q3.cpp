#include <iostream>
using namespace std;

int main(int argc, char const *argv[])
{
    char const *name;
    if ( argc > 1 ) {
        // A name has been passed
        name = argv[1];
    } 
    else
    {
        name = "World";
    }
    
    
    cout << "Hello " << name << "!" << endl;
    return 0;
}
