#include <stdio.h>
#include "hello.h"
static int y;
void  helloPhat(){
    y = y+ 1;
    printf("%s %d y = %d\n", __FILE__, __LINE__, ++y);
}

void  staticPhat(){
    //y = 0;
    y= y+10;
    printf("%s %d y = %d \n", __FILE__, __LINE__ , y);
}