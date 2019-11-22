// Centrality Measures ADT implementation
// COMP2521 Assignment 2

#include <stdio.h>
#include <stdlib.h>

#include "CentralityMeasures.h"
#include "Dijkstra.h"
#include "PQ.h"

static double path(Vertex dest, ShortestPaths sps);

static double pathsv(Vertex i, Vertex dest, ShortestPaths sps,int found);
NodeValues closenessCentrality(Graph g) {
	NodeValues nvs;
	int vertices = GraphNumVertices(g);
	double sum, count;
	nvs.numNodes = vertices;
	nvs.values = calloc(vertices, sizeof(double));
	for (int i = 0; i < vertices; i++)
	{
		ShortestPaths sps = dijkstra(g, i);
		sum = count = 0;
		for (int j = 0; j < vertices; j++)
		{
			if (sps.dist[j] > 0)
			{
				sum = sum + sps.dist[j];
				count++;
			}
		}
		if (sum != 0)
			nvs.values[i] = count / (vertices - 1) * count / sum;
	}
	return nvs;
}

NodeValues betweennessCentrality(Graph g) {
	NodeValues nvs;
	int vertices = GraphNumVertices(g);

	int paths = 0;

	nvs.numNodes = vertices;
	nvs.values = calloc(vertices, sizeof(double));
	double direct = 0;
	for (int i = 0; i < vertices; i++)
	{
		// Loop for source
		double sum = 0.0;
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
				
				paths = path(k, sps);
				direct = pathsv(i, k, sps, 0);
				if(paths != 0)
					sum += direct/paths;
			}
		}
		nvs.values[i] = sum;
	}
	return nvs;
}

static double path(Vertex dest, ShortestPaths sps)
{
	if (sps.src == dest)
		return 1;
	
	double ways = 0.0;
	PredNode *temp = sps.pred[dest];
	while (temp != NULL)
	{
		ways += path(temp->v, sps);
		temp = temp->next;
	}
	return ways;
}

static double pathsv(Vertex i, Vertex dest, ShortestPaths sps, int found)
{
	if (dest == i)
	{
		found = 1;
	}
	if (sps.src == dest)
	{
		return found;
	}
	double ways = 0;
	PredNode *temp = sps.pred[dest];
	while (temp != NULL)
	{
		ways += pathsv(i, temp->v, sps, found);
		temp = temp->next;
	}
	return ways;
}

NodeValues betweennessCentralityNormalised(Graph g) {
	NodeValues nvs = betweennessCentrality(g);
	if (nvs.numNodes <= 2)
		return nvs;
	
	double inv = ((nvs.numNodes - 1)*(nvs.numNodes - 2));
	double diff = 1/inv;
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

// printf("REACHE %d %d %d\n", i, j, k);
				// if (i == k || j == k)
				// 	continue;
				
				// len = 0;
				// for (int l = k; l != j; l = sps.pred[l]->v)
				// {
				// 	reverse[len] = l;
				// 	len++;
				// 	direct++;
					// if (l == i && direct > 1)
					// {
					// 	count[i]++;
					// }
				// }
				// reverse[len] = j;
				// len++;
				// if (len > 2)
				// {
				// 	for (int l = 0; l < len; l++)
				// 	{
				// 		if (reverse[l] == i)
				// 		{
				// 			count[i]++;
				// 		}
				// 	}
				// 	totalCount[i]++;
				// }
				
				// if (direct > 1)
				// 	totalCount[i]++;
				
				// printf("\nTotal is %f and count is %f\n", totalCount[i], count[i]);