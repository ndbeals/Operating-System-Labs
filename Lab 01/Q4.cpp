#include <iostream>
#include <cstdlib>
using namespace std;

int main(int argc, char const *argv[])
{
    int input1;
    int input2;

    if ( argc > 2 ) {
        // 2 variables have been passed
        input1 = strtol(argv[1],NULL,0);
        input2 = strtol(argv[2],NULL,0);
    } 
    
    cout << "Sum of ( " << input1 << " + " << input2 << " ) = " << input1 + input2  << endl;
    return 0;
}
