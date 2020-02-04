//------------------------------------------- Header Section ------------------------------------------
#include <fstream>
#include <iostream>
#include <cassert>
#include <limits>
#include <cstring>
#include <cctype>
#include <cstdlib>


//-------------------- Class, Struct, Function and Constant Declarations -------------------------------

// This section consists solely of declarations of classes, structs, functions and constants.
// Put all constants that need to be used throughout the program here.
// Put all class or struct structure related things here.
// Put all function prototypes here.

// Constants go here

#define MAX_VERTICES 10
#define MAX_NEIGHBOURS 5
#define MAX_PATHLENGTH 50
#define MAX_STRLEN 10
#define FILENAME "mapofthecity.txt"

// Classes and structs begin from here

//typedef int Vertex;

//##################### Graph Class #######################

// This class contains the framework needed to work with the nodes/vertices
// and edges of the graph.


class Graph
{
	private:
		int edgeWeight[MAX_VERTICES][MAX_VERTICES];
		// Adjacency array for retrieving the weight of each edge. 
		// This implementation is easily extendable to directed graphs as well but that
		// will not be a part of our main objective.
		int numOfVertices;
		
	public:
		// Preferably keep an integer return value for all function that do not need
		// return anything. Indicate success with return value of 0 and anything else
		// for different errors. This should help in debugging.
		
		// Indexes can be in integers in half open range [0, numOfVertices)
		
		Graph(){}
		Graph(int numOfVerticesTemp);
		
		int addEdge(int index1, int index2, int weight); // Undirected Vertex
		int getEdgeWeight(int index1, int index2);
		int getNeighbourList(int index, int neighbourList[MAX_NEIGHBOURS]);
		int getNumOfVertices();
		
		// Miscellanous Functions for use inside other functions
		int assertIndexInRange(int index);
		
		// Functions that may be defined for later extension
		//int addDirectedEdge(int indexFrom, int indexTo, int weight);
};

// ONE THING I SHOULD PROBABLY MAKE CLEAR OVER HERE
// The indexes for the vertices should not be arbitrary. They should be the integers
// from 0 to <number_of_vertices>. This will help tremendously in coding the logic
// in other parts. If you want to be able to assign real names to them then maintain
// a separate list of strings containing these names where name[i] = "Name of the 
// ith vertex". 


// ####################### Parser Class ###################################
/*Class for the parser and file I/O that reads and stores 
graphs in files.
*/

// ####################### UI Class ###################################
/*Class for the parser and file I/O that reads and stores 
graphs in files. Also interacts with the user input.
*/

typedef char string[MAX_STRLEN];

class UI{
	private:
		string table[MAX_VERTICES];
		char filePath[MAX_PATHLENGTH];
		
		int getVertexNames();
		int refer(string t);
		int makeGraph();
		int inputScreen(int &srcVertex, int &destVertex);
		int questPath(int &srcVertex, int &destVertex);
		int addDistance();
		int printVisual();
		int displayCity();
		
	public:
		Graph map;
		
		UI(const char path[MAX_PATHLENGTH]);
		int takeInput(int &srcVertex, int &destVertex);
		int outputPath(int PathList[], int pathLen);		
		Graph getMap();
		
		//Ideal Example output: Shortest Path:
		//						Home to School to Shop
		//						Total Path Length: 9 units
};

// PLEASE KEEP THIS IN MIND
// The indexes for the vertices should not be arbitrary. They should be the integers
// from 0 to <number_of_vertices>. This will help tremendously in coding the logic
// in other parts. If you want to be able to assign real names to them then maintain
// a separate list of strings containing these names where name[i] = "Name of the 
// ith vertex". 


//####################### Shortest Path Class #############################

class ShortestPath
{
	/*
	An implementation of Djikstra's Algorithm to find the shortest path
	between any two vertices.
	*/
	private:
		int distances[MAX_VERTICES];
		int visited[MAX_VERTICES];
		int parentVertex[MAX_VERTICES];
		
		Graph map;
		
		int numOfVertices;
		int destnVertex;
	
	public:
		ShortestPath(int srcVertex, int destVertex, Graph pmap);		
		int visitVertex(int vertex);
		int updateDistances(int curVertex);
		int getNextVertex();
		int getPath(int PathList[], int maxlen);
};


//----------------------- Function Definition and Actual Working Region -----------------------


//########################## Graph Class ##########################
Graph :: Graph(int numOfVerticesTemp)
{
	/*
	Constructor for Graph Class
	*/
	
	numOfVertices = numOfVerticesTemp;
	// Set all the edges to 0 initially
	int i, j;
	for(i = 0; i < numOfVertices; i++)
	{
		for(j = 0; j < numOfVertices; j++)
			edgeWeight[i][j] = 0;
	}
}

int Graph :: addEdge(int index1, int index2, int weight)
{
	/*
	Adds an undirected edge of given weight between the 2 nodes whose indexes are given.
	*/
	
	assertIndexInRange(index1);
	assertIndexInRange(index2);
	assert(weight > 0);
	
	edgeWeight[index1][index2] = edgeWeight[index2][index1] = weight;

	return(0);
}

int Graph :: getEdgeWeight(int index1, int index2)
{
	/*
	Returns the weight of edge between vertices index1 and index2
	*/
	
	int weight = edgeWeight[index1][index2];
	return(weight);
}

int Graph :: getNeighbourList(int index, int neighbourList[MAX_NEIGHBOURS])
{
	/*
	Returns by reference the index list of neigbhbours of the vertex of given
	index.
	*/
	
	int neighbourIndex;
	int i = 0;
	
	for(neighbourIndex = 0; neighbourIndex < numOfVertices; neighbourIndex++)
	{
		if(getEdgeWeight(index, neighbourIndex) > 0)
			neighbourList[i++] = neighbourIndex;
	}
	
	// All other spaces are set to -1 to avoid confusion with garbage values
	for(; i < MAX_NEIGHBOURS; i++)
		neighbourList[i] = -1;
	
	return(0);
}

int Graph :: getNumOfVertices()
{
	return(numOfVertices);
}


int Graph :: assertIndexInRange(int index)
{
	/*
	Ensures that given index is in appropriate range i.e it is an
	integer in half open interval [0, numOfVertices). Raises error
	if it is not in range.
	*/
	
	int indexInRange = ( index >= 0 && index < numOfVertices );
	assert(indexInRange);	
	
	return(0);
}


//######################### Shortest Path Class #####################################

ShortestPath :: ShortestPath(int srcVertex, int destVertex, Graph pmap) 
{	
	// Set map to pmap
	map = pmap;
	numOfVertices = map.getNumOfVertices();
	
	destnVertex = destVertex; 
	
	// set all vertices' distance to infinity and state to unvisited			
	for(int i = 0; i < numOfVertices; i++)
	{
		distances[i] = std::numeric_limits<int>::max(); // Sets it to max possible
		visited[i] = 0; // see http://bit.ly/inttoinf
	}
	
	// set src vertex to 0 and visited
	distances[srcVertex] = 0;
	parentVertex[srcVertex] = -1;
	visitVertex(srcVertex);
	
	int ctr = 0;
	
	// cut -----set up loop to update distances till all vertices are covered
	int nextVertex = getNextVertex();
	while(ctr++ < numOfVertices - 1)
	{
		visitVertex(nextVertex);
		nextVertex = getNextVertex();
	}
}

int ShortestPath :: visitVertex(int vertex)
{
	/*
	Visits the given vertex and updates its neighbours.
	*/
	
	//std::cout << "Visiting Vertex " << vertex << std::endl;
	visited[vertex] = 1;
	updateDistances(vertex);
	
	/*std::cout << "Neighbour List: ";
	int nList[MAX_NEIGHBOURS];\
	map.getNeighbourList(vertex, nList);
	for(int j = 0; j < MAX_NEIGHBOURS; j++)
		std::cout << nList[j] << " ";
	std::cout << std::endl;
	
	std::cout << "Distance List: ";
	for(int i = 0; i < numOfVertices;i++)
		std::cout << distances[i] << " ";
	
	std:: cout << std::endl << std::endl;
	*/
	
	return(0);
}

int ShortestPath:: updateDistances(int curVertex)
{
	/*
	Updates the distances to all the neighbour nodes if a shorter
	path exists through the current node.
	*/
	
	int neighbourList[MAX_NEIGHBOURS];
	map.getNeighbourList(curVertex, neighbourList);
	
	int i = 0;
	while(neighbourList[i] != -1)
	{
		int nVertex = neighbourList[i];
		
		int distanceViaCurVertex = distances[curVertex] + map.getEdgeWeight(curVertex, nVertex);
		
		if(distanceViaCurVertex < distances[nVertex] && !visited[nVertex])
		{
			distances[nVertex] = distanceViaCurVertex;		
			parentVertex[nVertex] = curVertex;
		}
		
		i++;
	}
	
	return(0);
}

int ShortestPath :: getNextVertex()
{
	/*
	Returns the unvisited node that is least distant, -1 if none exist
	*/
	
	int minDistantVertex = -1;
	int minDistance = std::numeric_limits<int>::max();
	
	for(int i = 0; i < numOfVertices; i++)
	{
		if(distances[i] < minDistance && !visited[i])
		{	
			minDistantVertex = i;
			minDistance = distances[i];
		}
	}
	
	return(minDistantVertex);
}

int ShortestPath :: getPath(int pathList[], int maxlen)
{
	/*
	Returns the required shortest path in sequence of vertices to
	be traversed in the array given. If length of path is greater than maxlen then -1
	is returned. Otherwise length of path is returned.
	*/
	
	/*std::cout << "Parent List: ";
	for(int k=0;k<numOfVertices;k++)
		std::cout << parentVertex[k] << " ";
	std::cout << std::endl;*/
	
	int ctr = 0;
	
	int curVertex = destnVertex;
	
	//std::cout << "curVertex is" << curVertex << std::endl;
	
	int parent = parentVertex[curVertex];
	
	while(parent != -1 && ctr < maxlen)
	{
		pathList[ctr++] = curVertex;
		curVertex = parent;
		parent = parentVertex[curVertex];
	}
	
	if(ctr >= maxlen)
		return(-1);

	pathList[ctr++] = curVertex;
	
	/*for(int j = 0; j <  ctr; j++)
		std::cout << PathList[j] << " "; 

	std::cout << std::endl;*/
	
	for(int i = 0; i < ctr/2 ; i++)
	{	
		int tmp = pathList[i];
		pathList[i] = pathList[ctr - 1 - i];
		pathList[ctr - i -1] = tmp;
	}
	
	return(ctr);

}




//###################### UI Class ##################################################
UI :: UI(const char path[MAX_PATHLENGTH])
{	
	std::strcpy(filePath, path);
	
	for(int i = 0; i < MAX_VERTICES; i++)
		std::strcpy(table[i],"\0");
	
	getVertexNames();
	makeGraph();
}

int UI :: refer(string t){									
	//returns index value of given node
	
	for (int i = 0 ; std::strcmp(table[i],"\0") != 0; i++){
		if (std::strcmp(t,table[i])==0){
			return(i);
		}
	}
	//std::cout << "refer: No such vertex found!:" << t << std::endl;
	return(-1);
}

int UI :: getVertexNames(){
	/*
	Creates a table of all the vertex names in the file and assigns
	distinct integer indices to each of them.
	*/
	
	string temp;
	int i=0, flag=0;
	std::ifstream file;
	file.open(filePath);

	while(!file.eof()){               												//while the file is open,
		file>>temp;																//input single word into temp
		if (!std::isdigit(temp[0])){    //ctype header									//if the word is not a number
			for (i=0; std::strcmp(table[i],"\0") != 0; i++){										
				if (std::strcmp(temp,table[i]) == 0){    //string header 				//check if name already in table
					flag = 1;													//if it is in table, set flag to 1
				}
			}
			if (flag == 0){														//if not in table,
				std::strcpy(table[i],temp);					//then add to table
				//std::cout << "Entering " << temp << " into the table with index" << i << std::endl;
			}
		}
		flag = 0;																	//set flag to 0 for next word
	}
	return(0);
}

int UI :: outputPath(int pathList[], int len){
	/*
	Outputs the path computed by the algorithm
	*/
	int x;
	int weight = 0;
	
	for(x = 0; x < len - 1; x++){
		weight += map.getEdgeWeight(pathList[x],pathList[x+1]);
	}
	std :: cout<<"Shortest Path:"<<std::endl;
	
	for(x = 0; x < len - 1; x++){
		std::cout<<table[pathList[x]]<<" to ";
	}
	std::cout<<table[pathList[x]] << std::endl;  
	std::cout<<"Total Path Length: " << weight << " units" << std::endl;
	
	std::cout << std::endl << "Thank You for using Fastest Pizza Delivery Services!" << std::endl;
	char end;
	std::cin >> end; // To hold the console window open
	
	return(0);																		
}

int UI :: makeGraph()
{
	/*
	Creates the actual graph object from the data file
	*/
	
	int i;
	for(i = 0; std::strcmp(table[i], "\0") != 0; i++);												//counting total number of vertices
	Graph g(i);														   			//graph object has i vertices

	std::ifstream file;
	file.open(filePath);
	
	char temp[MAX_STRLEN];
	
	while (file>>temp){
		int index1=refer(temp);
		file>>temp;
		int index2=refer(temp);
		file>>temp;
		int weight=atoi(temp);
		
		//std::cout << "Adding edge with " << index1 << " " << index2 << " " << weight << std::endl;
		
		g.addEdge(index1, index2, weight);
	}
	
	file.close();
	
	map = g;
	
	return(0);
}

int UI :: takeInput(int &srcVertex, int &destVertex)
{
	/*
	Handles the entire end user interface.
	Returns 1 if shortest path is to be calculated
	and 0 if not.
	*/

	printVisual();
	int pathNeeded = inputScreen(srcVertex, destVertex);
	
	return(pathNeeded);
}

int UI :: inputScreen(int &srcVertex, int &destVertex)
{
	/*
	Runs the menu screen offering options.
	Returns 1 if shortest path is to be calculated and 0 if not.
	*/
	
	enum options {path=1, display, add, exit};
	int opt = 4;
	int chosen = 1;
	int pathChosen = 0; // do you want the shortest path
	int rerun = 0;
	
	std::cout << std::endl;
	std::cout << " Main Menu\n-----------\n";
	std::cout<<"1. If you wish to find the shortest path, press 1 \n";
	std::cout<<"2. If you wish to see locations in the city, press 2 \n";
	std::cout<<"3. If you wish to add places to the directory, press 3 \n";
	std::cout<<"4. If you wish to exit, press 4\n";
	std::cout<<"Choice: ";

	std::cin>>opt;
	
	switch(opt)
	{
		case path:
			questPath(srcVertex, destVertex);
			pathChosen = 1;
			break;
			
		case display:
			displayCity();
			rerun = 1;
			break;
			
		case add:
			addDistance();
			rerun = 1;
			break;
			
		case exit:
			std::cout << "Thank You for using our service!" << std::endl;
			std::exit(0);
			break;
			
		default:
			chosen = 0;
	}
	
	if(!chosen || rerun)
	{
		if(!chosen && !rerun)
			std::cout << "Only numbers 1-4 are allowed." << std::endl;

		pathChosen = inputScreen(srcVertex, destVertex);
	}
	
	return(pathChosen);
}

int UI :: questPath(int &srcVertex, int &destVertex){
	/*
		Takes user input for source vertex and destination vertex for shortest path.
	*/
	
	string src, dest;
	srcVertex = destVertex = -1;
	
	std::cout << "In order to avail this service please enter your starting point" << std::endl;
	std::cout << "(like your shop or a house you may be at) and your customer's address" << std::endl;
	std::cout << "as the end point" << std::endl << std::endl; 
	
	std::cout << "Please enter Starting Point: ";
	std::cin >> src;
	std::cout << "Please enter End Point: ";
	std::cin >> dest; 
	
	for(int i = 0; i < map.getNumOfVertices(); i++)
	{
		if(std::strcmp(src, table[i]) == 0)
		{
			srcVertex = i;
		}
	}
	
	if(srcVertex == -1)
	{
		std::cout << "No such point exists on the map: " << src << std::endl;
		return(-1);
	}
	
	for(int j = 0; j < map.getNumOfVertices(); j++)
	{
		if(std::strcmp(dest, table[j]) == 0)
		{
			destVertex = j;
		}
	}
	
	if(destVertex == -1)
	{
		std::cout << "No such point exists on the map: " << dest << std::endl;
		return(-1);
	}
	
	
	return 0;
}

int UI :: addDistance()
{
	/*
	Adds a new edge to the data file 
	*/
	
	string name1,name2;
	int dist;
	
	if (map.getNumOfVertices() == MAX_VERTICES){
		std::cout<<"Cannot add any more locations" << std::endl;
	}
	
	else{
		std::cout << "Proceeding to add new path on the map" << std::endl;
		
		std::cout<<"Please enter first location: ";
		std::cin>>name1;
		
		std::cout<<"Enter name of neighbouring location: ";
		std::cin>>name2;
		
		std::cout<<"Enter distance between locations: ";
		std::cin>>dist;
		
		int index1 = refer(name1);
		int index2 = refer(name2);
		
		if(index1 != -1 && index2 != -1 && map.getEdgeWeight(index1, index2) > 0) // This won't give errors
		{
			std::cout << "WARNING: Existing path is getting changed!" << std::endl;
			std::cout << "It is NOT recommended to update existing paths this way. " << std::endl;
			std::cout << "Contact Administrator if you want to update the map not extend it." << std::endl;
		}
		
		std::ofstream file;
		file.open(filePath, std::ios::app);    			//define at the start
		
		file << std::endl;
		file << name1 << " " << name2 << " " << dist;
		file.close();
		
		char outputMsg[] = "Please not that these changes will NOT be in effect till the program is restarted.";
		std::cout << outputMsg << std::endl;
		
		char option;
		std::cout << "Exit Program [y/n]: ";
		std::cin >> option;
		
		if(std::tolower(option) == 'y')
			std::exit(0);
		
		// in all other cases the program doesn't stop
	}

	std::cout << "Returning to home page." << std::endl;	
	
	return(0);
}

int UI :: displayCity(){
	/*
	Displays the list of locations on the map.
	*/
	
	std::cout<<"Locations: \n";
	
	for (int i=0;i<map.getNumOfVertices();i++){
		std::cout << i+1 << ". " << table[i] << std::endl;
	}
	
	std::cout << "Returning to home page." << std::endl;	

	return(0);
}

int UI :: printVisual()
{
	/*
	Displays starting visual
	*/
	
	std::cout<<"                        ___              \n";
	std::cout<<"                        |  ~~--.         \n";
	std::cout<<"                        |\%=@\%\%/          \n";
	std::cout<<"                        |o\%\%\%/           \n";
	std::cout<<"                     __ |\%\%o/            \n";
	std::cout<<"               _,--~~ | |(_/ ._          \n";
	std::cout<<"            ,/'  m\%\%\%\%| |o/ /  `\\.       \n";
	std::cout<<"           /' m\%\%o(_)\%| |/ /o\%\%m `\\      \n";
	std::cout<<"         /' \%\%@=\%o\%\%\%o|   /(_)o\%\%\% `\\    \n";
	std::cout<<"        /  \%o\%\%\%\%\%=@\%\%|  /\%\%o\%\%@=\%\%  \\   \n";
	std::cout<<"       |  (_)\%(_)\%\%o\%\%| /\%\%\%=@(_)\%\%\%  |  \n";
	std::cout<<"       | \%\%o\%\%\%\%o\%\%\%(_|/\%o\%\%o\%\%\%\%o\%\%\% |  \n";
	std::cout<<"       | \%\%o\%(_)\%\%\%\%\%o\%(_)\%\%\%o\%\%o\%o\%\% |  \n";
	std::cout<<"       |  (_)\%\%=@\%(_)\%o\%o\%\%(_)\%o(_)\%  |  \n";
	std::cout<<"        \ ~\%\%o\%\%\%\%\%o\%o\%=@\%\%o\%\%@\%\%o\%~ /   \n";
	std::cout<<"         \. ~o\%\%(_)\%\%\%o\%(_)\%\%(_)o~ ,/    \n";
	std::cout<<"           \_ ~o\%=@\%(_)\%o\%\%(_)\%~ _/      \n";
	std::cout<<"             `\_~~o\%\%\%o\%\%\%\%\%~~_/'        \n";
	std::cout<<"                `--..____,,--'           \n";
	std::cout<<"\n \n \n";
	
	std::cout<<"WELCOME TO FASTEST PIZZA SERVICES! \n";
	std::cout<<"We help you reach your customers in the fastest manner possible, so that you may serve them your pizzas as fresh as possible. \n";
	
	return(0);
}

Graph UI :: getMap()
{
	/*
	Returns the map graph prepared from the file. 
	*/
	return(map);
}


// ------------------------- Testing Functions -------------------------------------

int testGraph()
{
	Graph map(4);
	map.addEdge(0,1,10);
	map.addEdge(1,2,3);
	map.addEdge(0,2,4);
	map.addEdge(2,3,9);
	map.addEdge(1,3,2);
	
	ShortestPath path(0,3,map);
	
	int p[10];
	int l = path.getPath(p, 10);
	
	for(int i = 0; i < l; i++)
		std :: cout << p[i] << " ";
	
	return(0);
}


// ------------------------- Final main Function -----------------------------------

int main()
{
	// This should preferably consist only of function calls and no actual working
	int srcVertex, destVertex;
	Graph map;
	int pathList[MAX_PATHLENGTH];
	int pathLen;
	int pathNeeded = 0;
	
	UI interface(FILENAME);
	pathNeeded = interface.takeInput(srcVertex, destVertex);
	
	if(pathNeeded)
	{
		map = interface.getMap();
		ShortestPath path(srcVertex, destVertex, map);
		pathLen = path.getPath(pathList, MAX_PATHLENGTH);
		interface.outputPath(pathList, pathLen);
	}
		
	return(0);
}