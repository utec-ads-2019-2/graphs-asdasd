#include <string>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <stack>
#include <queue>
#include "graph.h"
#include <map>
#include <climits>

graph::graph(bool directed) {
    nodes = 0;
    edges = 0;
    is_directed = directed;
}

void graph::createNodes(int _id, const string& _name, double _la, double _lo) {
    Node* auxNode = new Node;
    auxNode->Id = _id;
    auxNode->Name = _name;
    auxNode->Lat = _la;
    auxNode->Long = _lo;
    vector<Node*> aux;
    aux.push_back(auxNode);
    LA.push_back(aux);
    nodes++;
}

double deg2rad(double deg) {
    return deg * (M_PI/180);
}

double graph::calculatedistance(int key1, int key2) {
    int R = 6371;
    auto x = findNode(key1);
    auto y = findNode(key2);
    double diflat = deg2rad(x->Lat-y->Lat);
    double diflong = deg2rad(x->Long-y->Long);
    double z =
            sin(diflat/2)*sin(diflat/2)+
            cos(deg2rad(y->Lat))*cos(deg2rad(x->Lat))*
            sin(diflong/2)*sin(diflong/2);
    double c= 2*atan2(sqrt(z),sqrt(1-z));
    double d= R*c;
    return d;
}

void graph::createNodes(int _id, const string& _name, double _la, double _lo, double _pond) {
    Node* auxNode = new Node;
    auxNode->Id = _id;
    auxNode->Name = _name;
    auxNode->Lat = _la;
    auxNode->Long = _lo;
    auxNode->pond = _pond;
    vector<Node*> aux;
    aux.push_back(auxNode);
    LA.push_back(aux);
    nodes++;
}

Node* graph::findNodeById(int id) {
    for (int i=0; i<LA.size(); ++i) {
        if (i == id){
            return LA[i][0];
        }
    }
    return nullptr;
}

int graph::findIndexNode(int key) {
    for (unsigned long i=0; i<LA.size(); ++i) {
        if (LA[i][0]->Id == key){
            return i;
        }
    }
    return INT_MAX;
}

void graph::createConection(int origin, int end) {
    auto auxEdge = new Edge;
    auxEdge->origin = findNode(origin);
    auxEdge->end = findNode(end);
    for (auto i : LA){
        if (i[0] == auxEdge->origin){
            i.push_back(auxEdge->end);
        }
    }
    auxEdge->origin->nexts.push_back(auxEdge);
    if (!is_directed){
        for (auto i : LA){
            if (i[0] == auxEdge->end){
                i.push_back(auxEdge->origin);
            }
        }
        auto auxEdge2 = new Edge;
        auxEdge2->origin = auxEdge->end;
        auxEdge2->end = auxEdge->origin;
        auxEdge2->pond = auxEdge->pond;
        auxEdge->end->nexts.push_back(auxEdge2);
    }
    edges++;
}

vector<double> graph::dijkstra(int OriginKey)
{
    vector<double> dist(LA.size(), INT_MAX);
    priority_queue<pair<int, int>, vector< pair<int, int> >, greater< > > Q;

    Q.push(make_pair(OriginKey, 0));
    dist[findIndexNode(OriginKey)] = 0;

    while(!Q.empty())
    {
        int u = Q.top().first;
        Q.pop();
        for(unsigned long i = 0; i < LA[findIndexNode(u)].size()-1; i++)
        {
            int v = LA[findIndexNode(u)][i+1]->Id;
            double weight = findArista(u,v)->pond;

            if(dist[findIndexNode(v)] > dist[findIndexNode(u)] + weight)
            {
                dist[findIndexNode(v)] = dist[findIndexNode(u)] + weight;
                Q.push(make_pair(v, dist[findIndexNode(v)]));
            }
        }
    }
    return dist;
}

void graph::printSolution(vector<double> dist) {
    cout << ("Vertex \t\t Distance from Source\n");
    for (int i = 0; i < LA.size(); ++i) {
        cout << findNodeById(i)->Id << "\t\t\t\t" << dist[i] << endl;
    }
}

void graph::createConection(int origin, int end, double pond) {
    auto auxEdge = new Edge;
    auxEdge->origin = findNode(origin);
    auxEdge->end = findNode(end);
    auxEdge->pond = pond;
    for (auto & i : LA){
        if (i[0] == auxEdge->origin){
            i.push_back(auxEdge->end);
        }
    }
    auxEdge->origin->nexts.push_back(auxEdge);
    if (!is_directed){
        for (auto i : LA){
            if (i[0] == auxEdge->end){
                i.push_back(auxEdge->origin);
            }
        }
        auto auxEdge2 = new Edge;
        auxEdge2->origin = auxEdge->end;
        auxEdge2->end = auxEdge->origin;
        auxEdge2->pond = auxEdge->pond;
        auxEdge->end->nexts.push_back(auxEdge2);
    }
    edges++;
}

void graph::printNode(int key) {
    Node* temporal = findNode(key);
    if(temporal){
        cout
                << "Nombre : " << temporal->Name
                << " - ID: " << temporal->Id
                << endl;
    } else {
        cout << "No se encontró lo que buscaba. " << endl;
    }
}

Node* graph::findNode(int key) {
    for (auto i : LA) {
        if (i[0]->Id == key){
            return i[0];
        }
    }
    return nullptr;
}

Edge* graph::findArista(int key1, int key2) {
    for (auto i : LA) {
        if (i[0]->Id == key1){
            vector <Edge*> tempRecorrido = i[0]->nexts;
            for (auto & y : tempRecorrido){
                if(y->end->Id==key2){
                    return y;
                }
            }
            return nullptr;
        }
    }
    return nullptr;
}

double graph::calculateDensity() {
    double densidad = static_cast<double>(edges)/ (nodes * (nodes - 1));
    if (!is_directed) {
        densidad *= 2;
    }
    return densidad;
}

string graph::denseOrDispersed(double cota){
    double densidad = calculateDensity();
    if(densidad >= cota){
        return "Es denso";
    }
    return "Es disperso";
}

int graph::getNodeIdByName(const string &Name) {
    for (auto i : LA) {
        if (i[0]->Name == Name){
            return i[0]->Id;
        }
    }
    return 0;
}

void graph::printArista(int OriginKey, int EndKey) {
    Edge* temporal = findArista(OriginKey, EndKey);
    if(temporal){
        cout
                << "Origen : " << temporal->origin->Name
                << " - Destino : " << temporal->end->Name;
        if (!is_directed) cout << " y viceversa ";
        cout
                << " - Distancia : " << temporal->pond
                << endl;
    }

    else {
        cout << "No se encontró lo que buscaba. " << endl;
    }
}

void graph::removeConnection(int OriginKey, int EndKey) {
    auto temp = findArista(OriginKey, EndKey);
    for (auto i : LA){
        if(i[0]->Id == OriginKey){
            auto it1 = i[0]->nexts.begin();
            for (auto x : i[0]->nexts){
                if(x == temp){
                    i[0]->nexts.erase(it1);
                }
                ++it1;
            }
            auto it2 = i.begin();
            for(auto y : i){
                if (y->Id == EndKey){
                    i.erase(it2);
                }
                ++it2;
            }
        }
    }
    temp = nullptr;
    delete temp;
}

void graph::removeNode(int _id) {
    auto temp = findNode(_id);
    //Borrar todos los edges que salen del NODO
    for (auto i : temp->nexts){
        if(findNode(i->origin->Id) == nullptr or findNode(i->end->Id) == nullptr){
            //cout<<"already removed";
        }
        else {
        //cout<<"i->origin->id: "<<i->origin->Id<<", i->end->Id: "<<i->end->Id<<endl;
        removeConnection(i->origin->Id, i->end->Id);
        }
    }
    //Borrar todos los edges que llegan al NODO y su rastro en la lista de adyacencia LA si el grafo es dirigido
    if(is_directed) {
        for (auto j : LA) {
            auto it1 = j.begin();
            for (auto k : j) {
                if (k == temp) {
                    auto it2 = j[0]->nexts.begin();
                    for (auto m : j[0]->nexts) {
                        if (m->end == temp) {
                            j[0]->nexts.erase(it2);
                            break;
                        }
                        ++it2;
                    }
                    j.erase(it1);
                    break;
                }
                ++it1;
            }
        }
    }

    //Borrar el nodo de la lista de adyacencia
    auto itSupremo = LA.begin();
    for (auto n : LA){
        if(n[0] == temp){
            n.clear();
            LA.erase(itSupremo);
            break;
        }
        ++itSupremo;
    }

    //Borrar el nodo
    temp= nullptr;
    delete temp;
}

void graph::printLA(){
    cout << "Lista de adyacencia de un grafo ";
    if (!is_directed) cout << "no ";
    cout << "dirigido:" << endl;
    for (int i = 0; i < 50; ++i) cout << "=";
    cout << endl;
    for (auto & x : LA){
        cout << x.size() << " = ";
        for (auto & y : x){
            cout << y->Name << " - " << y->Id << " | ";
        }
        cout << " => Numero de conexiones : " << x.size()-1 ;
        cout << endl;
    }
}

vector<Edge*> graph::filledges(){
    vector<Edge*> compilado;
    for (auto & i : LA) {
        for (auto & j : i) {
            for (auto next : j->nexts) {
                compilado.push_back(next);
            }
        }
    }
    return compilado;
}
unsigned int graph::getNodes() {
    return nodes;
}


void graph::aux_bipartite(Node* N, bool prevColor, map<Node*,bool> &checked, int &contador){
        if (checked[N]) return;
        else if (!checked[N]) return;
        else {
            checked[N] = !prevColor;
            ++contador;
            for (auto x : N->nexts){
                aux_bipartite(x->end,prevColor,checked,contador);
            }
        }
}



bool graph::is_bipartite(){
    int contador=0;
    map<Node*,bool> checked;
    for (auto & i : LA){
        if(checked[i[0]]){
            for (auto j : i[0]->nexts){
                aux_bipartite(j->end, true, checked, contador);
            }
        } else if (!checked[i[0]]){
            for (auto j : i[0]->nexts){
                aux_bipartite(j->end, false, checked, contador);
            }
        } else {
            checked[i[0]] = true;
            ++contador;
            for (auto j : i[0]->nexts){
                aux_bipartite(j->end, false, checked, contador);
            }
        }
        if (contador >=LA.size()) break;
    }
    for (auto i : LA){
        for (int j = 1; j < i.size(); ++j) {
            if (checked[i[0]] == checked[i[j]]){
                return false;
            }
        }
    }
    return true;

}


bool graph::CheckBoucle(Edge * arista){
    int temp=arista->origin->Id;
    int temp2=arista->end->Id;
    return findArista(temp,temp2)->end->Id==temp;
}


void graph::getKruskal() {
    double pesototal=0;
    if (is_directed) {
        cout << "kruskal no funciona para dirigidos" << endl;
    } else {
        vector<double> pesos;
        vector<Edge *> aristas;
        aristas = filledges();
        for (int i = 0; i <= aristas.size()-1; ++i) {
            pesos.push_back(aristas[i]->pond);
        }
        sort(pesos.begin(), pesos.end());
        //Si hay dos aristas con el mismo peso, solo una es necesaria para el MSP
        pesos.erase(unique(pesos.begin(),pesos.end()),pesos.end());

        for (int i = 0; i <= aristas.size(); ++i) {
            for (int j = 0; j < aristas.size(); ++j) {
                if (pesos[i] == aristas[j]->pond) {
                    aristas[i] = aristas[j];
                }
            }
        }
        cout<<"Orden por algoritmo de Kruskal" << endl;
        for (int k = 0; k < LA.size()-1; ++k) {
            if(!CheckBoucle(aristas[k])){
                cout << "Paso " << k+1 << ": (" << aristas[k]->origin->Name << "," << aristas[k]->end->Name << ")" << endl;
                pesototal+=aristas[k]->pond;
            }
        }
        cout << endl <<" El peso total del arbol de minima exapansion es "<<pesototal<<endl;

    }
}

template <typename T>
bool is_in(T element, vector <T> place){
    for (T x : place)
        if(element == x) return true;
    return false;
}

int graph::minKey(vector<int> key, vector<bool> mstSet){
    int min =  2147483647;
    //Deberia ser int, pero como lo igualo a i, que es un unsigned long por lo que es el iterador del vector
    int min_index = 0;
    for (unsigned long i = 0; i < LA.size(); ++i) {
        if (!mstSet[i] and key[i] < min){
            min = key[i];
            min_index = i;
        }
    }
    return min_index;
}

void graph::printMST(const vector<Edge*>& parent){
    double peso=0;
    cout<<"Orden por algoritmo de Prim" << endl;
    for (int k = 0; k < parent.size(); ++k) {
        cout << "Paso " << k+1 << ": (" << parent[k]->origin->Name << "," << parent[k]->end->Name << ")" << endl;
        peso+=parent[k]->pond;
    }
    cout << endl;
    cout<<"El peso total del arbol de minima expansion es "<<peso<<endl;
    cout<<endl;
}

void graph::primMST(int key) {
    if (is_directed) {
        cout << "Prim no funciona para dirigidos" << endl;
    } else {
        vector<Edge *> conexiones;
        vector<Node *> usados;
        auto tempnode = findNode(key);
        usados.push_back(tempnode);
        double min = 2147483647;
        Edge *minN = nullptr;
        while (usados.size() < LA.size() and isConexo()) {
            for (unsigned long i = 0; i < usados.size(); ++i) {
                for (unsigned long j = 0; j < usados[i]->nexts.size(); ++j) {
                    if (min > usados[i]->nexts[j]->pond and !is_in(usados[i]->nexts[j]->end, usados)) {
                        min = usados[i]->nexts[j]->pond;
                        minN = usados[i]->nexts[j];
                    }
                }
            }
            if (min != 2147483647) {
                conexiones.push_back(minN);
                usados.push_back(minN->end);
                minN = nullptr;
                min = 2147483647;
            }
        }
        printMST(conexiones);
    }
}


bool graph::isConexo(){
    int expectedSize = LA.size();
    vector<Node*> ready;
    for(auto x : LA[0]) {
        ready.push_back(x);
        if(ready.size() == expectedSize){
            return true;
        }
    }
    for (int y = 0; y < LA.size(); ++y) {
        for (int i = 1; i < expectedSize; ++i) {
            if (is_in(LA[i][0], ready)) {
                for (int j = 1; j < LA[i].size(); ++j) {
                    if (!is_in<Node*>(LA[i][j], ready)) {
                        ready.push_back(LA[i][j]);
                        if (ready.size() == expectedSize) {
                            return true;
                        }
                    }
                }
            } else {
                for (int j = 1; j < LA[i].size(); ++j) {
                    if (is_in<Node*>(LA[i][j], ready)) {
                        ready.push_back(LA[i][0]);
                        if (ready.size() == expectedSize) {
                            return true;
                        }
                        break;
                    }
                }
            }
        }
    }
    return false;
}

bool graph::isFuertementeConexo() {
    if(!is_directed){
        cout<<"Fuertemente conexo solo funciona con dirigidos"<<endl;
        return false;
    }
    return true;
}

void graph::printAristasByNode() {
    for (auto x : LA){
        cout << "En el nodo " << x[0]->Id << " existen : ";
        for (auto y : x[0]->nexts){
            cout << y->origin->Id << " - " << y->end->Id << " pond(" << y->pond << ") | ";
        }
        cout << endl;

    }
}

vector<int> graph::DFS(int key){
    vector<bool> visited(LA.size(), false);
    stack<int> stack;
    vector<int> resultdfs;
    stack.push(key);

    while (!stack.empty())
    {
        key = stack.top();
        stack.pop();
        if (!visited[findIndexNode(key)])
        {
            resultdfs.push_back(key);
            visited[findIndexNode(key)] = true;
        }

        for (auto & next : LA[findIndexNode(key)][0]->nexts){
            int temp = next->end->Id;
            if (!visited[findIndexNode(temp)]){
                stack.push(temp);
            }
        }
    }
    return resultdfs;
}

void graph::printDFS(const vector<int>& Resultado){
    cout<<"Following is Depth First Traversal: "<<endl;
    for (int i : Resultado) {
        cout<<i<<"  ";
    }
    cout<<endl;
}

vector<int> graph::BFS(int key) {
    vector<bool> visited(LA.size(),false);
    vector<int> queue;
    vector<int> resultbfs;

    visited[findIndexNode(key)] = true;
    queue.push_back(key);

    while(!queue.empty()){
        key = queue.front();
        //cout<<"keys: "<<key<<endl;
        resultbfs.push_back(key);
        queue.erase(queue.begin());

        for (auto & next : LA[findIndexNode(key)][0]->nexts){
            int temp = next->end->Id;
            if (!visited[findIndexNode(temp)]){
                visited[findIndexNode(temp)] = true;
                queue.push_back(temp);
            }
        }
    }
    return resultbfs;
}

void graph::printBFS(const vector<int>& Resultadobfs){
    cout<<"Following is Breadth First Traversal: "<<endl;
    for (int i=0; i<Resultadobfs.size(); ++i) {
        cout<<Resultadobfs[i]<<"  ";
    }
    cout<<endl;
}

void printMatrix(const vector<vector<long>>& a){
    for (const auto& x : a){
        for (auto y : x){
            cout << y << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void graph::printFW(){
    cout << "FLOYD - WARSHALL :" << endl;
    printMatrix(FloydWarshall());
}



vector<vector<long>> graph::FloydWarshall() {
    map<Node*, int> aux;
    for (unsigned long i = 0; i < LA.size(); ++i) {
        aux.insert(pair<Node*, int>(LA[i][0],i));
    }
    vector<vector<long>> ans;
    for (auto x : LA){
        vector<long> temp;
        temp.reserve(LA.size());
        for (auto y : LA) temp.push_back(INT_MAX);
        ans.push_back(temp);
    }
    for (unsigned long z = 0 ; z < LA.size(); ++z){
        ans[z][z] = 0;
        for (auto n : LA[z][0]->nexts){
            int indexA = aux.find(n->origin)->second;
            int indexB = aux.find(n->end)->second;
            ans[indexA][indexB] = n->pond;
        }
    }
    for (unsigned long a = 0; a < LA.size(); ++a)
        for (unsigned long b = 0; b < LA.size(); ++b)
            for (unsigned long c = 0; c < LA.size(); ++c) {
                if (ans[b][c] > ans[b][a] + ans[a][c]) ans[b][c] = ans[b][a] + ans[a][c];
                printMatrix(ans);
            }

    return ans;
}

void graph::printBF(int StartID) {
    auto X = findNode(StartID);
    auto x = BellmanFord(StartID);
    cout << "BELLMAN-FORD : " << endl;
    for (auto y : x){
        cout << y.first->Name << " shortest path size to " << X->Name << " is " << y.second << endl;
    }
}

map<Node*, int> graph::BellmanFord(int StartID) {
    map<Node*, int> ans;
    auto FirstNode = findNode(StartID);
    for (auto& i : LA) {
        ans.insert(pair<Node*,int>(i[0], INT_MAX));
    }
    ans.find(FirstNode)->second = 0;

    bool changed = true;
    for(unsigned long count = 0 ; count < LA.size()-1   ; ++count) {
        for (auto &j : ans) {
            for (auto k : j.first->nexts) {
                if (k->pond + j.second < ans.find(k->end)->second) {
                    ans.find(k->end)->second = k->pond + j.second;
                    changed = true;
                }
            }
            if (changed) changed = false;
        }

    }
    return ans;

}



double graph::distancetoendnode(int nodo, int end) {

    auto final= findIndexNode(end);
    auto camino = calculatedistance(nodo,end);
   return camino;




}
double graph::Aasterix(int start,int end) {
    auto go = findNode(start);
    vector<double> pesos;

    double peso=0;

    if (distancetoendnode(start, end) == 0) {
        return 0;
    }
    for (int k= 0; k <= nodes; ++k) {


        if (distancetoendnode(go->nexts[0]->end->Id, end) != 0 and go->nexts[0]->end->Id!=end) {
            for (int i = 0; i <= go->nexts.size() - 1; ++i) {
                if (distancetoendnode(go->nexts[i]->end->Id, end) != 0) {
                    pesos.push_back(go->nexts[i]->pond + distancetoendnode(go->nexts[i]->end->Id, end));
                }

            }
            if (!pesos.empty()) {
                sort(pesos.begin(), pesos.end());
            }
            double min = pesos[0];
            for (int i = 0; i <= go->nexts.size() - 1; ++i) {
                if (min == go->nexts[i]->pond + distancetoendnode(go->nexts[i]->end->Id, end)) {

                    peso += go->nexts[i]->pond;
                    go = go->nexts[i]->end;
                    pesos.clear();
                    pesos.push_back(10000);

                }
            }

        }
        for (int j = 0; j <= go->nexts.size()-1; ++j) {
            if(go->nexts[j]->end->Id==end){
                peso+=go->nexts[j]->pond;
                return peso;
            }
        }

    }
return 0;
}

void graph::printa(int begin,int end, double peso){
    cout<<"A*: "<<endl;
    cout<<"Start: "<<begin<<" - End: "<<" "<<end<<" - Peso: "<<" "<<peso<<endl;
}

/*
graph::~graph() {
    int i=0;
    while(not LA[i][0]->nexts.empty()){
        removeNode(LA[i][0]->Id);
        i++;
    }
    LA[i][0]= nullptr;
    delete LA[i][0];
}
 */
