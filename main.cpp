#include"matrix.h"
#include<iostream>

int main()
{
    vec3 v1(1, 2, 3);
    vec3 v2(3, 2, 1);
    vec3 aux;

    aux = vec3::crossProduct(v1,v2);

    aux = v1;

    for(int i = 0; i<3; i++)
    {
        std::cout<<aux(i)<<std::endl;
    }


}