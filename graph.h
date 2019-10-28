#ifndef GRAPHS_ASDASD_GRAPH_H
#define GRAPHS_ASDASD_GRAPH_H

#include <vector>

using namespace std;


struct Node;

struct Edge{
    double pond = 0;
    Node* origin = nullptr;
    Node* end = nullptr;
};


struct Node{
    int Id;
    string Name;
    double Lat,Long;
    vector<Edge*> nexts;

    double pond = 0;
};

class graph {
private:
    unsigned int nodes,edges{};
    bool is_directed;
    static vector<vector<Node*>> LA;

public:
    explicit graph(bool isdirected);

    void createNodes(int _id, const string& _name, double _la, double _lo);
    void createNodes(int _id, const string&  _name, double _la, double _lo, double _pond);
    void createConection(int origin, int end);
    void createConection(int origin, int end, double pond);
    static void removeNode(int _id);
    static void removeConnection(int OriginKey, int EndKey);

    static Node* findNode(int key);
    static Edge* findArista(int OriginKey, int EndKey);

    double calculateDensity();  
    static string denseOrDispersed(double densidad, double cota);
    static bool isConexo();
    static bool isFuertementeConexo();
    static bool is_bipartite(graph G,int n);

    static string getPrim();
    static string getKruskal();

    static int getNodeIdByName(const string& Name);
    static double calculatedistance(int key1, int key2);
    void printNode(int key);
    void printArista(int OriginKey, int EndKey);



};



#endif //GRAPHS_ASDASD_GRAPH_H
