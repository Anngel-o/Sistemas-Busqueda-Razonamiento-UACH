#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <limits>
#include <algorithm>

using namespace std;


struct Nodo {
    char estado;     
    float costo_g;   
    float costo_h;    
    Nodo* padre;    


    Nodo(char estado, float costo_g, float costo_h, Nodo* padre = nullptr)
        : estado(estado), costo_g(costo_g), costo_h(costo_h), padre(padre) {}

    // Sobrecarga del operador '>' para ordenar los nodos en la cola de prioridad
    // La comparación se basa en la suma de costo_g y costo_h
    bool operator>(const Nodo& otro) const {
        return (costo_g + costo_h) > (otro.costo_g + otro.costo_h);
    }
};

// Mapa para almacenar las distancias en línea recta (dlr) entre nodos
// Este mapa es un unordered_map donde la clave es un carácter que representa el nodo
// El valor es otro unordered_map que contiene las distancias a los nodos vecinos
unordered_map<char, unordered_map<char, float>> dlr = {
    {'A', {{'B', 1.8}, {'C', 2.5}, {'D', 2.8}, {'E', 4.5}, {'G', 3.0}}},
    {'B', {{'A', 1.8}, {'C', 4.4}, {'D', 5.6}, {'E', 3.5}, {'F', 2.3}, {'G', 4.5}, {'H', 3.2}}},
    {'C', {{'A', 2.5}, {'B', 4.4}, {'D', 3.8}, {'E', 3.4}, {'F', 8.0}, {'G', 6.3}, {'H', 7.7}}},
    {'D', {{'A', 2.8}, {'B', 5.6}, {'C', 3.8}, {'E', 7.7}, {'F', 8.0}, {'G', 2.8}, {'H', 10.0}}},
    {'E', {{'A', 4.5}, {'B', 3.5}, {'C', 3.4}, {'D', 7.7}, {'F', 6.7}, {'G', 6.8}, {'H', 4.0}}},
    {'F', {{'B', 2.3}, {'C', 8.0}, {'D', 8.0}, {'E', 6.7}, {'G', 5.3}, {'H', 3.5}}},
    {'G', {{'A', 3.0}, {'B', 4.5}, {'C', 6.3}, {'D', 2.8}, {'E', 6.8}, {'F', 5.3}, {'H', 6.6}}},
    {'H', {{'B', 3.2}, {'C', 7.7}, {'D', 10.0}, {'E', 4.0}, {'F', 3.5}, {'G', 6.6}}}
};

// Mapa para almacenar las conexiones entre los nodos en el grafo
// Este mapa es un unordered_map donde la clave es un carácter que representa el nodo
// El valor es un vector de caracteres que representan los nodos vecinos
unordered_map<char, vector<char>> grafo = {
    {'A', {'B', 'C', 'D', 'E', 'G'}},
    {'E', {'H'}},
    {'H', {'F'}}
};

// Función heurística que devuelve la distancia en línea recta entre dos nodos
// Esta función se utiliza para estimar el costo restante desde el nodo actual hasta el nodo meta
float heuristica(char actual, char meta) {
    return dlr[actual][meta];
}

// Función para reconstruir e imprimir el camino desde el nodo inicial hasta el nodo meta
// Esta función sigue los punteros a los padres desde el nodo meta hasta el nodo inicial
void reconstruir_camino(Nodo* nodo) {
    vector<char> camino;  // Vector para almacenar el camino
    while (nodo) {
        camino.push_back(nodo->estado);  // Agrega el estado actual al camino
        nodo = nodo->padre;  // Mueve al nodo padre
    }
    reverse(camino.begin(), camino.end());  // Invierte el camino para obtener el orden correcto
    for (char estado : camino) {
        cout << estado << " ";  // Imprime cada estado en el camino
    }
    cout << endl;
}

// Implementación del algoritmo de búsqueda primero por lo mejor
// Esta función busca el camino óptimo desde el nodo inicial hasta el nodo meta
void busqueda_primero_por_lo_mejor(char inicio, char meta) {
    // Cola de prioridad para nodos a explorar, ordenada por el costo total (g + h)
    priority_queue<Nodo, vector<Nodo>, greater<Nodo>> conjunto_abierto;
    // Mapa para almacenar todos los nodos visitados
    unordered_map<char, Nodo*> todos_nodos;

    // Añadir el nodo inicial al conjunto abierto con costo g = 0 y heurística calculada
    conjunto_abierto.emplace(inicio, 0.0, heuristica(inicio, meta));
    // Almacenar el nodo inicial en el mapa de todos los nodos
    todos_nodos[inicio] = new Nodo(inicio, 0.0, heuristica(inicio, meta));

    // Bucle principal de búsqueda
    while (!conjunto_abierto.empty()) {
        // Obtener el nodo con el menor costo total (g + h) del conjunto abierto
        Nodo actual = conjunto_abierto.top();
        conjunto_abierto.pop();  // Eliminar el nodo del conjunto abierto

        // Verificar si se ha alcanzado el nodo meta
        if (actual.estado == meta) {
            reconstruir_camino(&actual);  // Si se alcanza el nodo meta, reconstruir el camino
            return;
        }

        // Explorar los vecinos del nodo actual
        for (char vecino : grafo[actual.estado]) {
            // Calcular el costo g (costo acumulado desde el nodo inicial hasta el vecino)
            float costo_g = actual.costo_g + dlr[actual.estado][vecino];
            // Calcular el costo h (heurística estimada desde el vecino hasta el nodo meta)
            float costo_h = heuristica(vecino, meta);
            // Crear un nuevo nodo vecino con los costos calculados y el nodo actual como padre
            Nodo* nodo_vecino = new Nodo(vecino, costo_g, costo_h, new Nodo(actual));
            // Añadir el vecino al conjunto abierto
            conjunto_abierto.push(*nodo_vecino);
            // Almacenar el nodo vecino en el mapa de todos los nodos
            todos_nodos[vecino] = nodo_vecino;
        }
    }

    // Si se vacía el conjunto abierto sin encontrar el nodo meta, imprimir mensaje de error
    cout << "No se encontró un camino." << endl;
}

// Función principal
int main() {
    char inicio, meta;  // Variables para almacenar el estado inicial y el estado meta
    cout << "Ingrese el estado inicial: ";
    cin >> inicio;  // Solicitar al usuario que ingrese el estado inicial
    cout << "Ingrese el estado meta: ";
    cin >> meta;  // Solicitar al usuario que ingrese el estado meta

    // Llamar a la función de búsqueda primero por lo mejor con los estados proporcionados por el usuario
    busqueda_primero_por_lo_mejor(inicio, meta);
}
