#include "graph.h"
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <unordered_map>
#include <queue>
#include <limits>
#include <algorithm>
#include <fstream>

bool graph::insert(int a, int b, double d, double s, bool file){

    //firstly, check rather the edge is already there between a and b
    //the following if statement will indicate rather there is already edge by interating through the map.
    //end will be returned if there is not edge
    if (mymap[a].find(b) != mymap[a].end()){
        //mymap[a][b] is used since our map has double layer

        //updating distance and speed_limit value
        mymap[a][b].distance = d;
        mymap[a][b].speed_limit = s;

        //need to also update the opposite direction since our map is not directed
        mymap[b][a].distance = d;
        mymap[b][a].speed_limit = s;

        //if statement used for file input to not print success during load function 
        if (file){
            std::cout << "success" << std::endl;
        };
        return true;
    } else {
        //if edge does not exists

        //define new edge and set distance, speed_limit, and adjustment_factor value
        edge newedge;
        newedge.distance = d;
        newedge.speed_limit = s;
        newedge.adjustment_factor = 1;

        //inserting new edge to the map / both [a][b] and [b][a] direction needs to be updated since undirected graph
        mymap[a][b] = newedge;
        mymap[b][a] = newedge;

        if (file){
            std::cout << "success" << std::endl;
        };
        return true;
    };
};

//function load that takes a,b,d,s input from a file
//it will open the file just as in lab 2 & 3 and put it into the insert function.
void graph::load (std::string filename){
    std::ifstream file;
    file.open(filename);
    int a;
    int b;
    double d;
    double s;

    while (!file.eof()){
        file >> a;
        file >> b;
        file >> d;
        file >> s;
        insert(a, b, d, s, false);
    };

    std::cout << "success" << std::endl;
};


void graph::traffic (int a, int b, double A, bool file){
    //firstly, check that edge exists

    //checking that vertex a exists in the first & vertex b exists in the second layer
    if (mymap.find(a) != mymap.end() && mymap[a].find(b) != mymap[a].end()){

        //updating adjustment factor value / both [a][b] and [b][a] direction needs to be updated since undirected graph
        mymap[a][b].adjustment_factor = A;
        mymap[b][a].adjustment_factor = A;

        if (file){
            std::cout << "success" << std::endl;
        };

        successupdate = true;
        return;
    } else {    //if edge does not exists in the map
        if (file){
            std::cout << "failure" << std::endl;
        };
    };
};

//bool successupdate is used to print out success if one or more adjustment_factor is updated
void graph::update (std::string filename){
    successupdate = false;
    std::ifstream file;
    file.open(filename);
    int a;
    int b;
    double A;

    while (!file.eof()){
        file >> a;
        file >> b;
        file >> A;

        traffic(a, b, A, false);
    };

    //print success only if one or more adjustment factor was updated
    if(successupdate){
        std::cout << "success" << std::endl;
    } else {
        std::cout << "failure" << std::endl;
    };

};


void graph::print (int a){

    //stating a variable for 'a' in map
    auto loc_a = mymap.find(a);

    //check rather vertex a exists
    if (loc_a != mymap.end()){
        //since 'a' is not empty, check if there is any edges from 'a'
        if(loc_a->second.empty() == false){

            //iterate through all elements in 'a'
            for (const auto& isedge : loc_a->second){
                //print the value stored
                std::cout << isedge.first << " ";
            };

            std::cout << std::endl;
        } else {    //if 'a' exists but has nothing connected
            std::cout << "failure" << std::endl;
        };
    } else {    //if 'a' does not exists
        std::cout << "failure" << std::endl;
    };
};


void graph::f_delete (int a){
    auto loc_a = mymap.find(a);

    //check rather vertex a exists
    //if a exists
    if (loc_a != mymap.end()){

        //removing the vertex a which will result removal of all edges from a. But still needs to remove all edges to a
        mymap.erase(loc_a);

        //removing all edges to a
        //loop through all first layer verticies
        for (auto& to_a : mymap){
            //variable set to access the second layer of current vertex
            auto& inside = to_a.second;

            //check if there is a in second layer of current vertex
            if (inside.find(a) != inside.end()){
                inside.erase(a);
            };
        };
        std::cout << "success" << std::endl;
        
    } else {    //if a does not exists
        std::cout << "failure" << std::endl;
    };
};


void graph::path (int a, int b){
    //firstly, check rather both vertex a and b exists in our map
    if (mymap.find(a) == mymap.end() || mymap.find(b) == mymap.end()){
        std::cout << "failure" << std::endl;
        return;
    };

    //using priority queue to save vertices ordered by their current shortest time
    //std::greater is used to set priority queue to min heap
    std::priority_queue <std::pair <double, int>, std::vector <std::pair <double, int>>,std::greater<std::pair<double,int>>> priorityQ;

    //using map to store the lowest time from a to every other vertex
    std::unordered_map <int, double> times;

    //using map to store the predecessor of each vertex in the path to it from the source
    std::unordered_map <int, int> predecessors;

    //need to start by making all time values of vertices to infinity
    //loop through all first layer vertices in mymap
    for (const auto& set_d : mymap){

        //setting value in vertex value (used as key in times map) as infinity 
        times[set_d.first] = std::numeric_limits <double> :: infinity();
    };

    //need to change the time value stored in timess map for the source to itself to 0
    times[a] = 0;
    //since time value for a to itself is set, push it to the priority queue
    priorityQ.push( {0.0, a});

    //loop through priority queue
    while (priorityQ.empty() != true){
        //extract the vertex in the priority queue with the smallest current time
        int loc_current = priorityQ.top().second;
        priorityQ.pop();

        //check rather we are already in the destination or not
        if (loc_current == b){
            break;
        };

        //exploring each adjecent vertecies to the current vertex
        for (auto adjecent : mymap[loc_current]){
            int adjecent_key = adjecent.first;
            //getting the weight (time) value
            double time = adjecent.second.time();
            double newtime = times[loc_current] + time;

            //if there is a shorter path to the adjecent vertex exists
            if (newtime < times[adjecent_key]){
                //update the time value
                times[adjecent_key] = newtime;
                //record that time value
                predecessors[adjecent_key] = loc_current;
                //push that adjecent key to the queue
                priorityQ.push( {newtime, adjecent_key});
            };
        };
    };

    //checking rather path to the destination exists or not
    //if path exists
    if (times[b] != std::numeric_limits<double>::infinity()){
        //recreating the path using predecessors map
        std::vector<int> path_found;

        //pushing all the vertecies used in the path
        for (int i = b; i != a; i = predecessors[i]){
            path_found.push_back(i);
        };

        //adding the source vertex to the path
        path_found.push_back(a);

        //printing the verticies used in the path
        int size = static_cast<int> (path_found.size());
        for (int i = 0; i < size; i++){
            std::cout << path_found[i] << " ";
        };
        std::cout << std::endl;
    } else {
        std::cout << "failure" << std::endl;
    };

};


//almost same algorithm with path function
void graph::lowest (int a, int b){
    //firstly, check rather both vertex a and b exists in our map
    if (mymap.find(a) == mymap.end() || mymap.find(b) == mymap.end()){
        std::cout << "failure" << std::endl;
        return;
    };

    //using priority queue to save vertices ordered by their current shortest time
    //std::greater is used to set priority queue to min heap
    std::priority_queue <std::pair <double, int>, std::vector <std::pair <double, int>>,std::greater<std::pair<double,int>>> priorityQ;

    //using map to store the lowest time from a to every other vertex
    std::unordered_map <int, double> times;

    //using map to store the predecessor of each vertex in the path to it from the source
    std::unordered_map <int, int> predecessors;

    //need to start by making all time values of vertices to infinity
    //loop through all first layer vertices in mymap
    for (const auto& set_d : mymap){

        //setting value in vertex value (used as key in times map) as infinity 
        times[set_d.first] = std::numeric_limits <double> :: infinity();
    };

    times[a] = 0;
    priorityQ.push( {0.0, a});

    //loop through priority queue
    while (priorityQ.empty() != true){
        int loc_current = priorityQ.top().second;
        priorityQ.pop();

        if (loc_current == b){
            break;
        };

        //exploring each adjecent vertecies to the current vertex
        for (auto adjecent : mymap[loc_current]){
            int adjecent_key = adjecent.first;
            double time = adjecent.second.time();
            double newtime = times[loc_current] + time;

            //if there is a shorter path to the adjecent vertex exists
            if (newtime < times[adjecent_key]){
                times[adjecent_key] = newtime;
                predecessors[adjecent_key] = loc_current;
                priorityQ.push( {newtime, adjecent_key});
            };
        };
    };

    //checking rather path to the destination exists or not
    //if path exists
    if (times[b] != std::numeric_limits<double>::infinity()){
        
        //printing out the sum of the time in the path
        std::cout << times[b] << std::endl;
    } else {
        std::cout << "failure" << std::endl;
    };
};