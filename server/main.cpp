//
// Created by dominis on 05.06.19.
//
#include "Server.h"

int main()
{
    Server server(5600);
    while(true) {
        server.perform();
    }

   return 0;
}