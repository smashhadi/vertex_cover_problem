#include <iostream>
#include <stdio.h>
#include <string>
#include <list>
#include <sstream>
#include <vector>
#include <queue>
#include <algorithm>
#include "minisat/core/Solver.h"
#include "minisat/core/SolverTypes.h"
#include "minisat/mtl/Vec.h"


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


bool check_edges(std::vector<int> edges,int vert){
	bool flag = true;

	for(std::vector<int>::size_type c=0; c<edges.size() ;c++){
		if(!((0<= edges[c]) && (edges[c]< vert))){
			flag=false;
			break;
		}
	}

	return flag;
}


std::vector<int> CheckVC(int vertexCount, std::vector<int> edge){
	int lowerbound = 1;
	int upperbound = vertexCount;
	int k = (int)(vertexCount/2);
	int u=0, v=0;
	bool result;
	Minisat::vec<Minisat::Lit> ps;
	std::vector<int> VertexCover;
	
	while (lowerbound <  upperbound){
		Minisat::Solver *solver = new Minisat::Solver();
		Minisat::Var literal;
		std::vector<std::vector<Minisat::Var> > x;

		for (int i = 0; i<vertexCount; i++){
			x.push_back(std::vector<Minisat::Var> ());
			for (int j =0; j < k; j++){
				literal = solver -> newVar();
				x[i].push_back(literal);
			}
		}

		//clause 1
		for(int vertexCoverId =0; vertexCoverId < k; vertexCoverId++){
			ps.clear();
			for ( int i=0; i < vertexCount; i++){
				ps.push(Minisat::mkLit(x[i][vertexCoverId]));
			}
			solver->addClause(ps);
		}

		//clause 2
		for(int vertexId=0; vertexId < vertexCount ; vertexId++){
			for (int i=0; i < k; i++){
				for (int j=i+1; j < k; j++ ){
					solver->addClause(~Minisat::mkLit(x[vertexId][i]),~Minisat::mkLit(x[vertexId][j]));
				}
			}
		}

		//clause 3
		for(int vertexCoverId =0; vertexCoverId < k; vertexCoverId++){
			for ( int i=0; i < vertexCount; i++){
				for ( int j=i+1; j < vertexCount; j++){
					solver->addClause(~Minisat::mkLit(x[i][vertexCoverId]),~Minisat::mkLit(x[j][vertexCoverId]));
				}
			}
		}

		//clause 4
		for(std::vector<int>::size_type i = 0; i < edge.size(); i=i+2) {
		    u = edge[i];
		    v = edge[i+1];
		    ps.clear();
		    for (int j =0; j < k; j++){
		    	ps.push(Minisat::mkLit(x[u][j]));
		    	ps.push(Minisat::mkLit(x[v][j]));
		    }
		    solver->addClause(ps);
		}

		//End of clauses
		result = solver->solve();
		if (result == true){
			VertexCover.clear();
			for(int i=0; i<vertexCount; i++){
				for(int j=0; j<k; j++){
					if (solver->modelValue(x[i][j]) == Minisat::l_True){
						VertexCover.push_back(i);
					}
				}
			}
			upperbound = k-1;
		}
		else{
			lowerbound = k+1;
		}
		k = (upperbound + lowerbound)/2;
		delete solver;
		
	}
	
	return VertexCover;

}

int main(int argc, char **argv) {

	std::string input;
	int vertices;
	std::list<int> adj;
	std::vector<int> edges;
	std::vector<int> VC;

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
				 
				VC = CheckVC(vertices, edges);
			    	std::sort(VC.begin(), VC.end());
			    	for (auto x : VC)
		                std::cout << x << " ";
					std::cout<<std::endl;
				break;
			}
			 
			default:
				break;
		}
		std::getline(std::cin,input);
	}
	return 0;
}
