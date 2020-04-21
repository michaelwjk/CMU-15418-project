#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <list>
#include <map>
#include <sstream>
#include <string>

// This code is just for reference!!!


/*
We use a greedy coloring algorithm. 
 - The first fit coloring selects at random an uncolored vertex and assign the smallest
 color available. The algorithm finish when there is no uncolored vertices. This algorithm
 runs in O(n) steps
- Largest degree ordering Select the uncolored vertex with highest degree
   and color such vertex with the smallest possible color. This algorithm runs in 
   O(n2) steps
- In this version we implement the first fit algorithm. A possible assignment is to
implement the largest degree ordering
*/

using namespace std;

int N; //Size of the graph

ifstream edge;

struct vertex{
    int id;
    int *neighborhood; //Id of its neighbors
    bool is_color;
    int color;
    int degree;
};	

//
class Graph
{
	// Parameters 
	int V;			// no. of vertices
	vertex *vertice;	// adjacency List
	int **A;  //Adj Matrix. We need to do it more dynamic
	map <int,int> * xCol;	// A List to note what colors can't be used to color a particular vertex
	map <int,int> col;	//vertex number to color used map 
	int colors;	// number of colors used
	int * deg;	//array to store degree
	multimap <int,int> deg_ver; // for sorting degree
	int* ver_order;
	
public:
	Graph(int V,char* fname)    //contructor
	{
		this->V = V;
		A= new int*[V];
		vertice = (vertex*) malloc (sizeof(vertex)*V); //This is the list of vertices
		xCol = new map<int,int> [V];  //This is part of the algorithm
		deg = (int*) malloc(sizeof(int)*V); //The degree of each vertex, initially 0
		deg_ver.clear(); 
		for(int i = 0;i<V;i++)
			deg[i] = 0;
		colors = 0;
		readGraph(fname);
		for(int i=0;i<V;i++)
			deg_ver.insert(make_pair(deg[i],i));
		ver_order = (int*) malloc(sizeof(int)*V);
		printAdjMatrix(V);
		printList(V);
		//printDegrees(V);
	}

    //vertex b is neighbor of vertex a
	void addEdge(int a,int iNeighbor,int b) //Add edge a,b to the Graph
	{
        
        cout<<"vertice"<<a<<"pos:"<<iNeighbor<<"="<<b<<endl;
        vertice[a].neighborhood[iNeighbor]=b;
      	//a.neighborhood.push_back(b);
		//b.neighborhood.push_back(a);
        
		//cout<<"("<<a<<","<<b<<")"<<endl;
		//cout<<"("<<b<<","<<a<<")"<<endl;
	}

/*
	void addEdge(int a,int b) //Add edge a,b to the Graph
	{
		vertex u;
		vertex v;
		u.id=a;
		//u.neighborhood.push_back(v);
		v.id=b;
		//v.neighborhood.push_back(v);
		adj[a].push_back(u);
		adj[b].push_back(v);
		//cout<<"("<<a<<","<<b<<")"<<endl;
		//cout<<"("<<b<<","<<a<<")"<<endl;
	}
*/
	///Added by Joel

  void initAdjMatrix(int size){
  	for (int i = 0; i < size; ++i){
      A[i] = new int[size];
  	}
    for (int i=0;i<size;i++){
  	  for (int j=0;j<size;j++){
  		A[i][j]=0;
  	  }
    }
  }

  void readGraph(char *fileName){
  	initAdjMatrix(this->V);
    string line;
    int degree;
    int contRow;
    int contCol;
    istringstream iss;
    ifstream myfile (fileName);
    if (myfile.is_open())
     {
       contRow=0;
       while ( myfile.good() )
         { 
           getline (myfile,line);
           cout << line << endl;
           degree=countNeighbors(line);
           //cout<<"degree:"<<degree<<endl;
           createVertex(contRow,degree);
           istringstream iss(line); 
           contCol=0;           
    	   do {
               string val;
               iss >> val;
               if (!val.empty()){
                 cout << "Substring: " << val << endl;
                 A[contRow][stoi(val)-1]=1;
                 addEdge(contRow, contCol ,stoi(val)-1);
                 (deg[contRow])++;
			     (deg[stoi(val)-1])++;
                 contCol++;
               }
           } while (iss);
           contRow++;
         }
       myfile.close();
      }
   else 
     {  
       cout << "Unable to open file"; 
     }
 }


 ///count number of neighbors in vertex

 int countNeighbors(string line){
   int degree;
   istringstream iss(line); 
   degree=0;           
   do {
        string val;
        iss >> val;
        if (!val.empty()){
                 degree++;
            }
       } while (iss);
    return degree;
 }

  ///count number of neighbors in vertex


 ////Create vertex

void createVertex(int xPos, int xDegree){
   vertex vectorx;
   vertice[xPos].id=xPos;
   vertice[xPos].degree=xDegree;
   vertice[xPos].is_color=false;
   vertice[xPos].color=-1;
   vertice[xPos].neighborhood =  (int*) malloc (sizeof(int)*xDegree); //This is the list of vertices

   //vertice[xPos].neighborhood=new list<int>[xDegree];
}

 ////create vertex

  void printAdjMatrix(int size){
  	for (int i=0;i<size;i++){
		for (int j=0;j<size;j++){
		  cout<<A[i][j];	
		}
		cout<<""<<endl;
	}
  }

   void printList(int size){
  	int xid,z;
    for (int i=0;i<size;i++){
         cout<<"Vertex"<<i<<": ";
         z=vertice[i].degree;
         for (int j=0;j<z;j++){
         	cout<<"Degree:"<<vertice[i].degree<<" ";
         	cout<<vertice[i].neighborhood[j]<<" ";
         }
         cout<<" "<<endl;
      }
    }  

 /* void printList(int size){
  	int xid;
    list<vertex>::iterator list_iter;
    for (int i=0;i<size;i++){
        cout<<"Vertex_"<<i<<": ";
       list_iter=adj[i].begin();
       while( list_iter!=adj[i].end() ){
         xid=(*list_iter).id;
         //cout << (*list_iter).id<<"  ";
         cout << xid<<"  ";
         list_iter++;
       }
      cout<<""<<endl;
     }
  } */

    void printDegrees(int size){
    list<int>::iterator list_iter;
    for (int i=0;i<size;i++){
       cout<<"Degree_"<<i<<": "<<deg[i]<<endl;
     }
  }
  ///End added by Joel	

	/*void readGraph(char* fname)
	{
		edge.open(fname);
		int a,b;
		while(edge>>a>>b)
		{
			cout<<"a:"<<a<<endl;
			cout<<"b:"<<b<<endl;
			addEdge(a,b);
			(deg[a])++;
			(deg[b])++;
		}
		edge.close();
	}*/


/*
	
	void rmVertex(int x,int c) //Remove vertex number x which shall be colored with c 
	{
		while(!adj[x].empty())
		{
			xCol[adj[x].back()].insert(make_pair(c,c));
			adj[x].pop_back();
		}
		col[x] = c;	
	}
	
	int colMin(int x) // min Color needed to color vertex number x
	{
		map<int,int>::iterator itr;
		if(xCol[x].empty()) return 1;
		else
		{
			int i = 1;
			for(itr = xCol[x].begin() ; itr!=xCol[x].end() ; itr++)
			{
				if((itr->first) != i)
					return i;
				i++;
			}
			return i;	
		}	
	}
	
	void vertexOrder()
	{
		multimap<int,int>::reverse_iterator itr;
		int i =0;
		for(itr = deg_ver.rbegin(); itr != deg_ver.rend() ; itr++)
			ver_order[i++] = itr->second;
	}
	
	void colorGraph()	// coloring algorithm
	{
		int c;
		vertexOrder();
		for(int i = 0 ; i<V ; i++)
		{
			c = colMin(ver_order[i]);
			if( c > colors) colors = c;
			rmVertex(ver_order[i],c);
		}		
	}
	
	void printColoring() //prints the coloring
	{
		map<int,int>::iterator itr;
		cout << "Colors Used : "<<colors <<endl;
		for(itr = col.begin(); itr!= col.end(); itr++)
			cout << itr->first << " " <<itr->second<<endl;
	}
	*/
};



int main(int argc,char* argv[])
{
	if(argc!=3) { 
		cerr<<"Usage : ./color fileName N "<<endl;return 0;
	}
	N = atoi(argv[2]);
	Graph G(N,argv[1]);
	//G.printAdjMatrix(N);
	return 0;	
	/*
	if(argc!=2) { 
		cerr<<"Indique el nombre del archivo"<<endl;
		return 0;
     }
    readFile(argv[1]);
    printAdjMatrix(5);
	return 0;*/
}