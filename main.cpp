#include"matrix.h"
#include<iostream>

int main()
{
    vec3 v1(0.0f, 1.618f, 1.0f);
    vec3 v2(-1.0f, 0.0f, 1.618f);
    vec3 aux;

    aux = (v1 +v2).normalize()*1.902;

    for(int i = 0; i<3; i++)
    {
        std::cout<<aux(i,0)<<std::endl;
    }


}