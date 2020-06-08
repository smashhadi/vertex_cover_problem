#include <iostream>
#include <stdio.h>
#include <string>
#include <list>
#include <sstream>
#include <vector>
#include <queue>

#define INF 2 << 22


class Graph{

private:
    int V;
    std::vector<std::list<int> > adjList;

public:
    Graph(int const &V);
    void addEdge(int const &u, int const &v);
    void BFS(int const &s, int const &d);
};


// Initialize the graph with node count and scale the adjacency list with node count.
Graph::Graph(int const &V){
    this->V = V;
    adjList.resize(V);
}


// Go to the (u) th vector position then add v to the linked list.
void Graph::addEdge(int const &u, int const &v){
    adjList[u].push_back( v );
}


/*
void findPath(int const &startNode, int const &endNode, int* &parent){
	if( startNode == endNode || endNode == -1){
        std::cout<<startNode<< "-";
    }
    else{
        findPath(startNode, parent[endNode], parent);
        std::cout<<endNode<< "-";
    }
}
*/

void shortest_path(int const &start, int const &end, int* &parent){

	std::vector<int> shortest_path;
	int temp = end, k;

	while (start != temp){
		shortest_path.push_back(temp);
		temp = parent[temp];
    }
	shortest_path.push_back(start);

	for(k=shortest_path.size()-1; k>0; k--)
		std::cout<<shortest_path[k]<<"-";
	std::cout<<shortest_path[0];
}


void Graph::BFS(int const &s, int const &d){

    int *dist = new int[V];
    int *parent = new int[V];

    for(int v = 0; v < V; ++v){
        dist[v] = INF;
        parent[v] = -1;
    }

    dist[s] = 0;
    std::queue<int> Q;
    Q.push(s);

    while( !Q.empty() ){
        int u = Q.front();
        Q.pop();

        std::list<int>::iterator it;
        for(it = adjList[u].begin(); it != adjList[u].end(); ++it){
            if( dist[*it] == INF ){
                Q.push(*it);
                dist[*it] = dist[u] + 1;
                parent[*it] = u;
            }
        }
    }

    if( dist[d] != INF ){
        //findPath(s, d, parent);
    	shortest_path(s, d, parent);
        std::cout<<std::endl;
    }
    else
        std::cout<<"Error: No path available\n";

}


int get_vertices(std::string line){
	int v;
	char c;

	std::istringstream some_stream(line);
	some_stream >> c >> v;
	return v;
}


std::vector<int> get_edges(std::string line){
	int n;
	size_t index = 0;
	std::vector<int> adj_list;

	while(true){
		index = line.find('{', index);
		if(index == std::string::npos) break;
		line.replace(index, 1, "");
		index+=1;
	}
	index = 0;
	while(true){
		index = line.find('<', index);
		if(index == std::string::npos) break;
		line.replace(index, 1, "");
		index+=1;
		}
	index = 0;
	while(true){
		index = line.find(',', index);
		if(index == std::string::npos) break;
		line.replace(index, 1, " ");
		index+=1;
		}
	index = 0;
	while(true){
		index = line.find('>', index);
		if(index == std::string::npos) break;
		line.replace(index, 1, "");
		index+=1;
		}
	index = 0;
	while(true){
		index = line.find('E', index);
		if(index == std::string::npos) break;
		line.replace(index, 1, "");
		index+=1;
		}

	std::istringstream str(line);
	for(n = 0; str >> n; ) {
		adj_list.push_back(n);
	}

	return adj_list;
}


void get_src_dst(std::string line, int *s, int *d){
	char c;
	int a,b;
	std::istringstream stream(line);
	stream >> c >> a >> b;
	*s=a;
	*d=b;
}


bool check_s_d(int src,int dst, int vert){
	bool flag = false;
	if((0<= src) && (src< vert) && (0<= dst) && (dst< vert))
		flag = true;
	return flag;
}


bool check_edges(std::vector<int> edges,int vert){
	bool flag = true;

	for(int c=0; c<edges.size() ;c++){
		if(!((0<= edges[c]) && (edges[c]< vert))){
			flag=false;
			break;
		}
	}

	return flag;
}


 int main(){
	 std::string input;
	 int vertices, u, v, src, dst;
	 std::list<int> adj;
	 std::vector<int> edges;

	 std::getline(std::cin,input);

	 while (!std::cin.eof()){

		 switch(input[0]){
			 case 'V':{
				 vertices = get_vertices(input);
				 break;
			 }
			 case 'E':{
				 edges = get_edges(input);
				 if(check_edges(edges,vertices) == false){
					 std::cout<<"Error: Input of edge includes wrong vertices\n";
				 }
				 break;
			 }
			 case 's':{
				 get_src_dst(input, &src, &dst);
				 if(check_s_d(src,dst,vertices) == true){
					 Graph *g = new Graph(vertices);

					 for(int i =0; i<edges.size()-1 ;i=i+2){
						 u = edges[i];
						 v = edges[i+1];
						 g->addEdge(u,v);
						 g->addEdge(v,u);
					 }
					 g->BFS(src, dst);
				 }
				 else
				 {
					 std::cout<<"Error: Shortest path source or destination includes wrong node value\n";
				 }
				 break;
			 default:
				 break;
			 }
		 }
		 std::getline(std::cin,input);
	 }
	 return 0;
 }
