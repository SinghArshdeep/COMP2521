// Dijkstra ADT implementation
// COMP2521 Assignment 2

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Dijkstra.h"
#include "PQ.h"
/*
 * The  function  returns  a 'ShortestPaths' structure with the required
 * information:
 * - the number of vertices in the graph
 * - the source vertex
 * - distance array
 * - array of predecessor lists
 */
ShortestPaths dijkstra(Graph g, Vertex src)
{
    // Initialise the memory needed
    ShortestPaths paths;
    int vertices = GraphNumVertices(g);
    paths.numNodes = vertices;
    paths.src = src;
    paths.dist = calloc(vertices, sizeof(int));
    paths.pred = calloc(vertices, sizeof(PredNode *));

    // Initialise the distance and pred array 
    for (int i = 0; i < vertices; i++)
    {
        paths.dist[i] = 999999999;
        paths.pred[i] = NULL;
    }
    paths.dist[src] = 0;
    PQ q = PQNew();
    ItemPQ item;
    // Add all elements into the queue to be accessed later 
    for (int i = 0; i < vertices; i++)
    {
        item.key = i;
        item.value = 999999999;
        PQAdd(q, item);
    }
    // Add the source element with distance as 0
    item.key = src;
    item.value = 0;
    PQAdd(q, item);
    
    // Find all possible vertices and add into the queue
    while (!PQIsEmpty(q))
    {
        ItemPQ v = PQDequeue(q);
        AdjList list = GraphOutIncident(g, v.key);
        // Iterate through the graph edges 
        while (list != NULL)
        {
            // Check if the distance for new node found is less than the current distance 
            if (paths.dist[list->v] > (paths.dist[v.key] + list->weight))
            {
                // Make a new item to be added to the queue
                item.key = list->v;
                item.value = paths.dist[v.key] + list->weight;
                PQAdd(q, item);

                // Change distance array 
                paths.dist[list->v] = list->weight + paths.dist[v.key];
                // Change the pred array 
                PredNode *temp = malloc(sizeof(struct PredNode));
                temp->v = v.key;
                temp->next = NULL;
                paths.pred[list->v] = temp;
            }
            // If the new distance is same add vertex to the pred array 
            else if (paths.dist[list->v] == (paths.dist[v.key] + list->weight))
            {
                PredNode *temp = malloc(sizeof(struct PredNode));
                temp->v = v.key;
                temp->next = paths.pred[list->v];
                paths.pred[list->v] = temp;
            }
            list = list->next;
        }
    }
    // For isolated nodes change the distance to 0
    for (int i = 0; i < vertices; i++)
    {
        if (paths.dist[i] == 999999999)
            paths.dist[i] = 0;
    }
    return paths;
}

void showShortestPaths(ShortestPaths sps) {

}

void freeShortestPaths(ShortestPaths sps) {

}
