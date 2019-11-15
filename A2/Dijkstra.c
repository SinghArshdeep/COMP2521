#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "Dijkstra.h"
#include "PQ.h"

ShortestPaths dijkstra(Graph g, Vertex src)
{
    ShortestPaths *paths = malloc(sizeof(struct ShortestPaths));
    int vertices = GraphNumVertices(g);
    paths->numNodes = vertices;
    paths->src = src;
    paths->dist = calloc(vertices, sizeof(int));
    paths->pred = calloc(vertices, sizeof(PredNode *));

    // Initialize the visited array 
    Vertex *visited = calloc(vertices, sizeof(int));
	for (int i = 0; i < vertices; i++)
	{
		visited[i] = -1;
	}

    // Initialise the distance array 
    for (int i = 0; i < vertices; i++)
    {
        paths->dist[i] = 999999;
        paths->pred[i] = NULL;
    }
    paths->dist[src] = 0;
    PQ q = PQNew();
    ItemPQ *item = malloc(sizeof(struct ItemPQ));
    item->key = src;
    item->value = 0;
    PQAdd(q, *item);
    for (int i = 1; i <= vertices; i++)
    {
        ItemPQ v = PQDequeue(q);
        AdjList list = GraphOutIncident(g, v.key);
        while (list != NULL)
        {
            if (visited[list->v] == -1)
            {
                ItemPQ *item = malloc(sizeof(struct ItemPQ));
                item->key = list->v;
                item->value = list->weight;
                PQAdd(q, *item);
                visited[list->v] = v.key;

                PredNode *temp = malloc(sizeof(struct PredNode));
                temp->v = v.key;
                temp->next = NULL;
                paths->pred[list->v] = temp;
                paths->dist[list->v] = list->weight + paths->dist[v.key];
            }
            else if (paths->dist[list->v] > (paths->dist[v.key] + list->weight))
            {
                paths->dist[list->v] = list->weight + paths->dist[v.key];
                freeList(paths->pred[list->v]->next);
                paths->pred[list->v]->v = v.key;
            }
            else if (paths->dist[list->v] == (paths->dist[v.key] + list->weight))
            {
                PredNode *temp = malloc(sizeof(struct PredNode));
                temp->v = v.key;
                temp->next = paths->pred[list->v];
                paths->pred[list->v] = temp;
            }
            
            list = list->next;
        }

    }
    
    return *paths;
}
/**
 * This  function  is  for  you to print out the ShortestPaths structure
 * while you are debugging/testing your implementation. 
 * 
 * We  will  not call this function during testing, so you may print out
 * the given ShortestPaths structure in whatever format you want.
 */
void showShortestPaths(ShortestPaths sps)
{
    printf("\nNUmber of nodes %d and src %d\n", sps.numNodes, sps.src);
    for (int i = 0; i < sps.numNodes; i++)
    {
        printf("\nNode %d: %d", i, sps.dist[i]);
    }
}
/**
 * Frees all memory associated with the given ShortestPaths structure.
 */
void freeShortestPaths(ShortestPaths sps)
{

}


static void freeList(PredNode *temp)
{
    while (temp != NULL)
    {
        PredNode *del = temp;
        temp = temp->next;
        free(del);
    }
}