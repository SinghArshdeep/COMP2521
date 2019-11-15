// Dijkstra ADT implementation
// COMP2521 Assignment 2

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Dijkstra.h"
#include "PQ.h"
static void freeList(PredNode *temp);

ShortestPaths dijkstra(Graph g, Vertex src)
{
    // Initialise the memory needed
    ShortestPaths *paths = malloc(sizeof(struct ShortestPaths));
    int vertices = GraphNumVertices(g);
    paths->numNodes = vertices;
    paths->src = src;
    paths->dist = calloc(vertices, sizeof(int));
    paths->pred = calloc(vertices, sizeof(PredNode *));

    // Initialize the visited array 
    // Vertex *visited = calloc(vertices, sizeof(int));
	// for (int i = 0; i < vertices; i++)
	// {
	// 	visited[i] = -1;
	// }

    // Initialise the distance and pred array 
    for (int i = 0; i < vertices; i++)
    {
        paths->dist[i] = 999999;
        paths->pred[i] = NULL;
    }
    paths->dist[src] = 0;
    PQ q = PQNew();

    // Add all elements into the queue to be accessed later 
    for (int i = 0; i < vertices; i++)
    {
        ItemPQ item;
        item.key = i;
        item.value = 999999;
        PQAdd(q, item);
    }
    // Add the source element with distance as 0
    ItemPQ item;
    item.key = src;
    item.value = 0;
    PQAdd(q, item);
    
    while (!PQIsEmpty(q))
    {
        // printf("Loop: %d\n", i);
        ItemPQ v = PQDequeue(q);
        // printf("ITEM: %d\n", v.key);
        AdjList list = GraphOutIncident(g, v.key);
        while (list != NULL)
        {

            // printf("Key is: %d\n", list->v);
            // if (visited[list->v] == -1)
            // {
            //     ItemPQ item;
            //     item.key = list->v;
            //     item.value = list->weight;
            //     PQAdd(q, item);
            //     visited[list->v] = v.key;

            //     PredNode *temp = malloc(sizeof(struct PredNode));
            //     temp->v = v.key;
            //     temp->next = NULL;
            //     paths->pred[list->v] = temp;
            //     paths->dist[list->v] = list->weight + paths->dist[v.key];
            // }
            if (paths->dist[list->v] > (paths->dist[v.key] + list->weight))
            {
                ItemPQ item;
                item.key = list->v;
                item.value = paths->dist[v.key] + list->weight;
                PQAdd(q, item);

                // Change distance array 
                paths->dist[list->v] = list->weight + paths->dist[v.key];
                freeList(paths->pred[list->v]);

                // Change the pred array 
                PredNode *temp = malloc(sizeof(struct PredNode));
                temp->v = v.key;
                temp->next = NULL;
                paths->pred[list->v] = temp;
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
    // freeList(paths->pred[src]);
    // paths->pred[src] = NULL;

    for (int i = 0; i < vertices; i++)
    {
        if (paths->dist[i] == 999999)
        {
            paths->dist[i] -= 999999;
        }
    }

    return *paths;
}

void showShortestPaths(ShortestPaths sps) {

}

void freeShortestPaths(ShortestPaths sps) {

}

static void freeList(PredNode *temp)
{
    if (temp == NULL)
        return;
    // PredNode *first = temp;
    while (temp != NULL)
    {
        PredNode *del = temp;
        temp = temp->next;
        del->next = NULL;
        free(del);
    }
    // first = NULL;
}
