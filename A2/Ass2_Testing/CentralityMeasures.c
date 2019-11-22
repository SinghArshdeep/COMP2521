// Centrality Measures ADT implementation
// COMP2521 Assignment 2

#include <stdio.h>
#include <stdlib.h>

#include "CentralityMeasures.h"
#include "Dijkstra.h"
#include "PQ.h"

// Decleration for statis functions 
static double path(Vertex dest, ShortestPaths sps);
static double vertexFound(Vertex i, Vertex dest, ShortestPaths sps, double found);

/**
 * Finds the closeness centrality for each vertex in the given graph and
 * returns the results in a NodeValues structure.
 */
NodeValues closenessCentrality(Graph g) {
	// Calculates Number of vertices in the graph
	int vertices = GraphNumVertices(g);
	// Define and initialise the variables and struct needed
	NodeValues nvs;
	double sum, count;
	nvs.numNodes = vertices;
	nvs.values = calloc(vertices, sizeof(double));
	// Loop to iterate through all the vertices 
	for (int i = 0; i < vertices; i++)
	{
		ShortestPaths sps = dijkstra(g, i);
		sum = count = 0;
		// Calculate closeness centrality for the vertex 
		for (int j = 0; j < vertices; j++)
		{
			// Sum all the distances 
			if (sps.dist[j] > 0)
			{
				sum = sum + sps.dist[j];
				count++;
			}
		}
		if (sum != 0)
			nvs.values[i] = count / (vertices - 1) * count / sum;
	}
	// Returns the result
	return nvs;
}
/**
 * Finds  the  betweenness centrality for each vertex in the given graph
 * and returns the results in a NodeValues structure.
 */
NodeValues betweennessCentrality(Graph g) {
	// Define and initialise all variables and struct
	NodeValues nvs;
	int vertices = GraphNumVertices(g);
	nvs.numNodes = vertices;
	nvs.values = calloc(vertices, sizeof(double));
	double found, paths, sum;
	// Loop to iterate through all the vertices 
	for (int i = 0; i < vertices; i++)
	{
		sum = 0.0;
		// Loop for source
		for (int j = 0; j < vertices; j++)
		{
			if (i == j)
				continue;
			ShortestPaths sps = dijkstra(g, j);
			// Loop for destination 
			for (int k = 0; k < vertices; k++)
			{
				if (i == k || j == k)
					continue;
				found = paths = 0;
				paths = path(k, sps);
				found = vertexFound(i, k, sps, found);
				if(paths != 0)
					sum += found/paths;
			}
		}
		nvs.values[i] = sum;
	}
	return nvs;
}
/**
 * Recursive function to travese through all the possible combination of paths from a source 
   to a destination vertex. 
 * Returns the number of paths found. 
**/
static double path(Vertex dest, ShortestPaths sps)
{
	if (sps.src == dest)
		return 1;
	double ways = 0.0;
	PredNode *temp = sps.pred[dest];
	// Loop to check all the predecessors of a node
	while (temp != NULL)
	{
		ways += path(temp->v, sps);
		temp = temp->next;
	}
	// Returns number of path found
	return ways;
}
/**
 * Recursive function to find occurences of a vertex in all of the possible combination of paths 
   from a source to a destination vertex. 
 * Returns the number of paths in which the vertex was found. 
**/
static double vertexFound(Vertex i, Vertex dest, ShortestPaths sps, double found)
{
	if (dest == i)
		found = 1;

	if (sps.src == dest)
		return found;

	double count = 0;
	PredNode *temp = sps.pred[dest];
	// Loop to check all the predecessors of a node
	while (temp != NULL)
	{
		count += vertexFound(i, temp->v, sps, found);
		temp = temp->next;
	}
	// Returns the result
	return count;
}
/**
 * Finds  the  normalised  betweenness centrality for each vertex in the
 * given graph and returns the results in a NodeValues structure.
 */
NodeValues betweennessCentralityNormalised(Graph g) {
	NodeValues nvs = betweennessCentrality(g);
	// Returns the structure if condition not satisfied 
	if (nvs.numNodes <= 2)
		return nvs;
	
	double inv = ((nvs.numNodes - 1) * (nvs.numNodes - 2));
	double diff = 1 / inv;
	// Iterate through all the vertoices and changes their values
	for (int i = 0; i < nvs.numNodes; i++)
	{
		nvs.values[i] = diff * nvs.values[i];
	}
	return nvs;
}

void showNodeValues(NodeValues nvs) {
	for (int i = 0; i < nvs.numNodes; i++)
	{
		printf("%d: %f\n", i, nvs.values[i]);
	}
}

void freeNodeValues(NodeValues nvs) {

}
