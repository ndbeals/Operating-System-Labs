#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>


using namespace std;


class test
{
    public: test* func();
};

test* test::func()
{
    return NULL;
}





int main(int argc, char const *argv[])
{
    int test[2];
    test[0]=5;
    test[1]=15;

    printf("%d\n",&test);
    printf("%d\n",*test);
    printf("%d\n",*(test+2));
    printf("%d\n",&(test[0]));
    
    // int test2;
    // test2=**(test+1);
    // **(test+1)=5;
    // **test[0]=5;
    // **test[1]=15;

    // test * ex = new test();
    // if ( ex == NULL ) {
        // printf("null! \n");
    // }
    

    // printf("%d  %d",*test[0],*test[1]);
    char * s;
    string * ss;
    printf("%d\n",sizeof(s));
    printf("%d\n",nullptr);
    printf("%d\n",NULL);

    printf("%s\n", (ss == NULL)?"true":"false");

    return 0;
}
