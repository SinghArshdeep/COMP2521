// Dijkstra ADT implementation
// COMP2521 Assignment 2

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Dijkstra.h"
#include "PQ.h"
// Declaration for static function
// static void freeList(PredNode *temp);

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
        paths.dist[i] = 999999;
        paths.pred[i] = NULL;
    }
    paths.dist[src] = 0;
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
        ItemPQ v = PQDequeue(q);
        AdjList list = GraphOutIncident(g, v.key);
        while (list != NULL)
        {
            if (paths.dist[list->v] > (paths.dist[v.key] + list->weight))
            {
                ItemPQ item;
                item.key = list->v;
                item.value = paths.dist[v.key] + list->weight;
                PQAdd(q, item);

                // Change distance array 
                paths.dist[list->v] = list->weight + paths.dist[v.key];
                // freeList(paths.pred[list->v]);

                // Change the pred array 
                PredNode *temp = malloc(sizeof(struct PredNode));
                temp->v = v.key;
                temp->next = NULL;
                paths.pred[list->v] = temp;
            }
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
    for (int i = 0; i < vertices; i++)
    {
        if (paths.dist[i] == 999999)
        {
            paths.dist[i] -= 999999;
        }
    }

    return paths;
}

void showShortestPaths(ShortestPaths sps) {

}

void freeShortestPaths(ShortestPaths sps) {

}

// Free up memory used by the 
// static void freeList(PredNode *temp)
// {
//     if (temp == NULL)
//         return;

//     while (temp != NULL)
//     {
//         PredNode *del = temp;
//         temp = temp->next;
//         del->next = NULL;
//         free(del);
//     }
// }
