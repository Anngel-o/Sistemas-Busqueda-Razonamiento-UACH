#include <iostream>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <vector>

using namespace std;

// Estructura de un Nodo
struct Nodo {
    string nombre;

    set<string> vecinos;

};

//Procedimiento de insercion de nodos al nuestro mapa (El grafo)
//Es necesario mandarle como referencia los 2 nodos que necesitamos relacionar de manera bilateral
//Ademas necesitamos enviarle el mapa al cual van a ser insertados, los grafos no son dirigidos, en
// caso de que sea dirigido, podriamos hacer otro procedimiento para solo insertar una conexion del nodo A al B
void agregarArista(map<string, Nodo>& grafo, const string& nodo1, const string& nodo2) {
    grafo[nodo1].vecinos.insert(nodo2);
    grafo[nodo2].vecinos.insert(nodo1);
}

//Nuestra busqueda en amplitud, requiere el grafo con los nodos contenidos dentro del mapa, necesita un nodo
//  inicio y un nodo objetivo
vector<string> busquedaAmplitud(map<string, Nodo>& grafo, const string& inicio, const string& objetivo) {
    //En cada busqueda necesitamos un contenedor de los nodos visitados, para no quedarnos en bucles sin fin
    set<string> visitados;
    //La siguiente cola nos permitira mantener un orden del camino de la busqueda
    //Nuestra cola es de tipo vector de strings
    // le insertamos nuestro primer nodo a la cola y le añadimos nuestro nodo inicio

    queue<vector<string>> cola;
    cola.push({inicio});
    //La busqueda va a continuar si es que todavia existen nodos por visitar (nuestra cola no este vacia)
    while (!cola.empty()) {
        //Extraemos la referencia del primer elemento de la cola para continuar desde el nodo siguiente a manipular
        //  despues vamos a sacar el nodo a manipular de la cola, para que no lo vayamos a manipular despues otra vez
        vector<string> camino = cola.front();
        cola.pop();

        //Del camino que extragimos de la cola, sacamos el nodo que esta al final del camino (es el frente de la cola,
        //  solo que como es un vector este se postra al final)
        string nodo = camino.back();

        //Realizamos una verificacion si es que el nodo que estamos buscando no es el que estamos manipulando
        //  si es asi retornamos el camino que tenemos hasta el momento
        if (nodo == objetivo) {
            return camino;
        }
        //En caso de que no sea el nodo que buscamos, vamos a verificar que el nodo no este dentro de nuestro
        //  arreglo de visitados, si es que llegamos al final del vector de visitados (visitados.end()), vamos
        // a asumir que no ha sido visitado nuestro nodo y vamos a continuar
        if (visitados.find(nodo) == visitados.end()) {
            //Vamos a insertar el nodo en el vector de visitados
            visitados.insert(nodo);

            //Luego iteramos entre todos los nodos "hijos" o vecinos como los llamamos nosotros y vamos a verificar
            //  que no esten en el vector de visitados y si es asi y no estan, vamos a insertar en un nuevo camino el camino que
            //  ya teniamos y luego vamos a meter hasta el final este nodo, para cuando ya hayamos verificado todos los hijos
            //  de los nodos de la anterior generacion, vamos a tener ya en nuestro camino, los siguientes nodos a buscar
            for (const string& vecino : grafo[nodo].vecinos) {
                if (visitados.find(vecino) == visitados.end()) {
                    vector<string> nuevoCamino = camino;
                    nuevoCamino.push_back(vecino);
                    //Por eso añadimos a la cola, este camino que luego vamos a consultar
                    cola.push(nuevoCamino);
                }
            }
        }
    }
    return {}; //En caso de no encontrar nuestro nodo objetivo vamos a retornar un vector vacio representado por {}
}




//En el caso de la busqueda por profundidad vamos a buscar nodo por nodo sin importar los hijos, sino hasta llegar
//  al final de esa rama donde no existan hijos no visitados
vector<string> busquedaProfundidad(map<string, Nodo>& grafo, const string& inicio, const string& objetivo) {

    //El proceso es igual a como lo manejamos en la busqueda por amplitud, solamente que los caminos que vamos a revisar
    //  en la siguiente iteracion no seran lo que se hayan quedado en la cola (los primeros manipulados), sino el nodo
    //  hijo que hayamos manipulado
    set<string> visitados;
    // Vamos a hacer una piia la cual nos ayuda a manipular el ultimo valor introducido, ya que no nos importan los caminos
    //  que estaban antes sino el camino del nodo hijo, la pila nos ayuda a esto
    stack<vector<string>> pila;

    //Añadimos a la pila nuestro nodo inicial
    pila.push({inicio});

    while (!pila.empty()) {

        //En el vector camino metemos la referencia del elemento mas arriba de la pila, ya que buscamos manipular los
        //  siguientes caminos, no los anteriores
        vector<string> camino = pila.top();
        pila.pop();
        //Nuestro vector de camino guarda al final el nodo siguiente a manipular (es una de las propiedades de los vectores)
        //  por lo tanto guardamos en la variable string "nodo" nuestro ultimo elemento del camino
        string nodo = camino.back();

        //verificamos si el nodo actual es igual al del objetivo, si es asi retornamos el camino
        if (nodo == objetivo) {
            return camino;
        }

         //En caso de que no sea el nodo que buscamos, vamos a verificar que el nodo no este dentro de nuestro
        //  arreglo de visitados, si es que llegamos al final del vector de visitados (visitados.end()), vamos
        // a asumir que no ha sido visitado nuestro nodo y vamos a continuar
        if (visitados.find(nodo) == visitados.end()) {

            //Insertamos el nodo manipulado dentro de nuestro vector de visitados
            visitados.insert(nodo);

            //Luego iteratemos en cada uno de los vecinos (hijos) de nuestro nodo
            for (const string& vecino : grafo[nodo].vecinos) {
                //Realizamos la misma verificacion de que este nodo hijo no este en el vector de verificados
                if (visitados.find(vecino) == visitados.end()) {

                    //En caso de que no este entre los visitados, vamos a guardar en nuestra variable nuevoCamino
                    //  nuestro camino actual y vamos a añadir nuestro vecino al final del camino (recordemos la
                    //  propiedad de los vectores) y por ultimo añadiremos arriba de nuestra pila este camino
                    vector<string> nuevoCamino = camino;
                    nuevoCamino.push_back(vecino);
                    pila.push(nuevoCamino);
                    //El camino siguiente a manipular va a ser el que esta mas arriba en la pila, como nosotros
                    //  enviamos nuestro nuevo camino arriba en la pila, este camino sera el siguiente en ser consultado
                }
            }
            //El camino que estamos manipulando no es como en el de amplitud, en donde el camino de los hijos se iban al
            // final de la fila, sino que en este caso los caminos de los hijos se van al inicio, haciendo que sean los
            //  siguientes en manipularse y podamos verificar cada uno de los caminos posibles en profundidad

        }
    }
    return {}; // No se encontró un camino
}

int main() {
    map<string, Nodo> grafo;

    // Agregamos las aristas de nuestro grafo
    agregarArista(grafo, "F", "H");
    agregarArista(grafo, "H", "E");
    agregarArista(grafo, "E", "B");
    agregarArista(grafo, "E", "A");
    agregarArista(grafo, "E", "C");
    agregarArista(grafo, "A", "B");
    agregarArista(grafo, "A", "C");
    agregarArista(grafo, "A", "G");
    agregarArista(grafo, "D", "G");
    agregarArista(grafo, "D", "C");

    string inicio, objetivo;

    //Pedimos al usuario el nodo de inicio y el nodo final
    cout << "Ingrese el nodo de inicio: ";
    cin >> inicio;
    cout << "Ingrese el nodo que desea buscar: ";
    cin >> objetivo;

    //Realizamos las respectivas busquedas en amplitud y luego en profundidad
    vector<string> caminoAmplitud = busquedaAmplitud(grafo, inicio, objetivo);
    if (!caminoAmplitud.empty()) {
        cout << "Busqueda por Amplitud"<<endl;
        cout << "Camino de " << inicio << " a " << objetivo << ": ";
        for (const string& nodo : caminoAmplitud) {
            cout << nodo << " ";
        }
        cout << endl;
    } else {
        cout << "No hay un camino de " << inicio << " a " << objetivo << " (BFS)." << endl;
    }


    vector<string> caminoProfundidad = busquedaProfundidad(grafo, inicio, objetivo);
    if (!caminoProfundidad.empty()) {
        cout << "Busqueda por Profundidad"<<endl;
        cout << "Camino de " << inicio << " a " << objetivo << ": ";
        for (const string& nodo : caminoProfundidad) {
            cout << nodo << " ";
        }
        cout << endl;
    } else {
        cout << "No hay un camino de " << inicio << " a " << objetivo << " (DFS)." << endl;
    }
    return 0;
}
