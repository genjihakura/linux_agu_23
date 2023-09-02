#include <stdio.h>
#include "inc/hello.h"
extern int y ;
int main(){

    helloPhat();
    helloPhat();
    helloPhat();
    printf("y=%d \n", y);
    helloWorld();
    staticPhat();
    staticPhat();
    staticPhat();
    helloPhat();
    return 0;
}