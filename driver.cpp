// include libraries here
#include <iostream>
#include <string>
#include "graph.h"


int main(){
    
    std::string command = " ";

    //variable used to run a while loop that will run till program is over
    bool program_run = true;

    //variables used for function intake
    int a;
    int b;
    double d;
    double s;
    std::string filename;
    double A;

    //creating object for graph header file
    graph graphobject;

    while(program_run){
        std::cin >> command;

        if (command == "exit"){
            program_run = false;
        }else if (command == "insert"){
            std::cin >> a;
            std::cin >> b;
            std::cin >> d;
            std::cin >> s;
            graphobject.insert(a, b , d, s, true);
        }else if (command == "load"){
            std::cin >> filename;
            graphobject.load (filename);
        }else if (command == "traffic"){
            std::cin >> a;
            std::cin >> b;
            std::cin >> A;
            graphobject.traffic(a, b, A, true);
        }else if (command == "update"){
            std::cin >> filename;
            graphobject.update(filename);
        }else if (command == "print"){
            std::cin >> a;
            graphobject.print (a);
        }else if (command == "delete"){
            std::cin >> a ;
            graphobject.f_delete(a);
        }else if (command == "path"){
            std::cin >> a;
            std::cin >> b;
            graphobject.path(a, b);
        }else if (command == "lowest"){
            std::cin >> a;
            std::cin >> b;
            graphobject.lowest(a, b);
        };
    };

};