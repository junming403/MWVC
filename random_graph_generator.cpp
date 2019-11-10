// A C++ Program to generate test cases for 
// an unweighted directed graph 
#include <cstdio>
#include <set>
#include <stdlib.h>
using namespace std; 
  
// Define the number of runs for the test data 
// generated 
#define RUN 1
  
// Define the maximum number of vertices of the graph 
#define MAX_VERTICES 20 
  
// Define the maximum number of edges 
#define MAX_EDGES 200 
  
int main(int argv, char** args) 
{ 
    set<pair<int, int>> container; 
    set<pair<int, int>>::iterator it; 
      
    // Uncomment the below line to store 
    // the test data in a file 
    // freopen ("Test_Cases_Directed_Unweighted_Graph.in", "w", stdout); 
      
    //For random values every time 
    srand(time(NULL)); 
      
    int NUM = atoi(args[1]);;    // Number of Vertices 
    int NUMEDGE = atoi(args[2]); // Number of Edges 
    int maxWeight = atoi(args[3]);
    if (NUMEDGE > NUM*(NUM-1)/2) {
            printf("Invalid number of edges\n");
            return 0;
    }
                  
    // First print the number of vertices and edges 
    printf("%d %d\n", NUM, NUMEDGE); 

    for (int i = 0; i < NUM; i++) {
        printf("%d ", 1+(rand() % maxWeight));
    }
    printf("\n");
        
    // Then print the edges of the form (a b) 
    // where 'a' is connected to 'b' 
    for (int j=1; j<=NUMEDGE; j++) 
    { 
        int a = rand() % NUM; 
        int b = rand() % NUM; 
        pair<int, int> p = make_pair(a, b); 
        auto q = make_pair(b, a);
            
        // Search for a random "new" edge everytime 
        // Note - In a tree the edge (a, b) is same  
        // as the edge (b, a) 
        while (a == b || container.find(p) != container.end() || container.find(q) != container.end()) 
        { 
            a = rand() % NUM; 
            b = rand() % NUM; 
            p = make_pair(a, b);
            q = make_pair(b, a); 
        } 
        container.insert(p); 
    } 
            
    for (it=container.begin(); it!=container.end(); ++it) 
        printf("%d %d\n", it->first, it->second); 
            
    container.clear();       
    
    // Uncomment the below line to store 
    // the test data in a file 
    // fclose(stdout); 
    return(0); 
} 