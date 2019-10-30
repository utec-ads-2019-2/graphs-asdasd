#include <string>
#include <iostream>
#include <cmath>
#include "graph.h"

graph::graph(bool directed) {
    nodes = 0;
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
        auxEdge->end->nexts.push_back(auxEdge);
    }
    edges++;
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
        auxEdge->end->nexts.push_back(auxEdge);
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
    delete temp;
}

void graph::removeNode(int _id) {
    auto temp = findNode(_id);
    //Borrar todos los edges que salen del NODO
    for (auto i : temp->nexts){
        removeConnection(i->origin->Id, i->end->Id);
    }
    //Borrar todos los edges que llegan al NODO y su rastro en la lista de adyacencia LA si el grafo es dirigido
    if(is_directed) {
        for (auto j : LA) {
            auto it1 = j.begin();
            for (auto k : j) {
                //cout << k->Name << endl;
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
    delete temp;
}

void graph::printLA(){
    cout << "Lista de adyacencia de un grafo ";
    if (!is_directed) cout << "no ";
    cout << "dirigido:" << endl;
    for (int i = 0; i < 50; ++i) cout << "=";
    cout << endl;
    for (auto & x : LA){
        for (auto & y : x){
            cout << y->Name << " - " << y->Id << " | ";
        }
        cout << " => Numero de conexiones : " << x.size()-1 ;
        cout << endl;
    }
}

template <typename T>
bool is_in(T element, vector <T> place){
    for (T x : place)
        if(element == x) return true;
    return false;
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


