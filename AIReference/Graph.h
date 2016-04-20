#pragma once
#include <cassert>
#include <list>
#include <map>
#include <algorithm>


template<typename T>
class Graph
{
public:
    struct Edge
    {
        size_t self;
        size_t prev, next;
        float cost;
    };

    struct Node
    {
        size_t self;
        T data;
        std::list<size_t> edges, _incoming;
    };
private:

    std::map<size_t, Node> nodes;
    std::map<size_t, Edge> edges;
    
    size_t nextNodeIDX = 0;
    size_t nextEdgeIDX = 0;


public:
//    auto &getNodes() const { return nodes; }
//    auto &getEdges() const { return edges; }

    size_t addNode(const T &data) { nodes.emplace(nextNodeIDX, Node{nextNodeIDX, data}); return nextNodeIDX++; }
    void   addEdge(size_t prev, size_t next, float cost = 1)
    {
        addDirectedEdge(prev, next, cost);
        addDirectedEdge(next, prev, cost);
    }

    size_t addDirectedEdge(size_t prev, size_t next, float cost = 1)
    {
        assert(nodes.count(prev) && nodes.count(next) && " Node ID invalid");

        edges.emplace(nextEdgeIDX, Edge{nextEdgeIDX, prev, next, cost});
        nodes[prev].edges.push_back(nextEdgeIDX);
        nodes[next]._incoming.push_back(nextEdgeIDX);
        return nextEdgeIDX++;
    }


    void   removeNode(size_t node)
    {
        assert(nodes.count(node) && " Node ID invalid.");
        
        std::list<size_t> edge_del; // queue for edges to delete

        for each(auto e in nodes[node].edges)
        {
            edge_del.push_back(e);
            nodes[edges[e].next]._incoming.remove(e);
        }

        for each(auto e in nodes[node]._incoming)
        {
            edge_del.push_back(e);
            nodes[edges[e].prev].edges.remove(e);
        }
        
        for each(auto e in edge_del) edges.erase(e);

        nodes.erase(node);
    }
  
    // Nodes within the range of min and max are removed.   
    void   removeRange(const T &min, const T &max, bool (*LessThan)(const T&, const T&))
    {
        std::list<size_t> node_del;
        for each(auto np in nodes)
            if (LessThan(min, np.second.data) && LessThan(np.second.data, max))
                node_del.push_back(np.second.self);
            
        for each(auto node in node_del) removeNode(node);
    }


    // Search for a node that best matches the query.
    // query    : value we're looking for.
    // Distance : comparison function that returns how close the data is.
    // Range    : Returns -1 if the closest node to the query exceeds the range.
    // returns the node ID
    int findNode(const T& query, float (*Distance)(const T&, const T&), float range = 0.0001f)  const
    {
        float sd = FLT_MAX;
        int   id = -1;

        for each(auto np in nodes)
        {
            float d = Distance(np.second.data, query);

            if (d < sd && d <= range)
            {                
                sd = d;
                id = np.second.self;
            }
        }   
        return id;
    }

    // provides modifiable access to the data in a node.
    T getNodeData(size_t node) const { return nodes.at(node).data; }
    Edge getEdgeData(size_t edge) const { return edges.at(edge); }
    std::list<size_t> getNodeEdges(size_t node) const { return nodes.at(node).edges; }

};