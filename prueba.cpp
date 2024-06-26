#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <ctime>
#include <cstdlib>
#include <chrono>

using namespace std;

class Laberinto {
public:
    Laberinto(int ancho, int alto) : ancho(ancho), alto(alto) {
        laberinto.resize(alto, vector<char>(ancho, '#'));
        srand(time(0));
        generarLaberinto();
    }

    void mostrarLaberinto() {
        for (int y = 0; y < alto; y++) {
            for (int x = 0; x < ancho; x++) {
                cout << laberinto[y][x];
            }
            cout << endl;
        }
    }

    bool resolverLaberinto() {
        return bfsResolver();
    }

private:
    int ancho, alto;
    vector<vector<char>> laberinto;

    void generarLaberinto() {
        stack<pair<int, int>> pila;
        int inicioX = 1, inicioY = 1;
        laberinto[inicioY][inicioX] = ' ';
        pila.push({inicioX, inicioY});

        while (!pila.empty()) {
            int x = pila.top().first;
            int y = pila.top().second;
            pila.pop();

            vector<pair<int, int>> vecinos;
            if (x > 2 && laberinto[y][x - 2] == '#') vecinos.push_back({x - 2, y});
            if (x < ancho - 3 && laberinto[y][x + 2] == '#') vecinos.push_back({x + 2, y});
            if (y > 2 && laberinto[y - 2][x] == '#') vecinos.push_back({x, y - 2});
            if (y < alto - 3 && laberinto[y + 2][x] == '#') vecinos.push_back({x, y + 2});

            if (!vecinos.empty()) {
                pila.push({x, y});
                int randIndex = rand() % vecinos.size();
                int nx = vecinos[randIndex].first;
                int ny = vecinos[randIndex].second;

                laberinto[ny][nx] = ' ';
                laberinto[ny + (y - ny) / 2][nx + (x - nx) / 2] = ' ';
                pila.push({nx, ny});
            }
        }

        laberinto[0][1] = ' ';
        laberinto[alto - 1][ancho - 2] = ' ';
    }

    bool bfsResolver() {
        vector<vector<bool>> visitado(alto, vector<bool>(ancho, false));
        vector<vector<pair<int, int>>> padre(alto, vector<pair<int, int>>(ancho, {-1, -1}));
        queue<pair<int, int>> cola;
        cola.push({0, 1});
        visitado[0][1] = true;

        vector<pair<int, int>> direcciones = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

        while (!cola.empty()) {
            int x = cola.front().first;
            int y = cola.front().second;
            cola.pop();

            if (x == ancho - 2 && y == alto - 1) {
                marcarCamino(padre, x, y);
                return true;
            }

            for (auto dir : direcciones) {
                int nx = x + dir.first;
                int ny = y + dir.second;

                if (nx >= 0 && nx < ancho && ny >= 0 && ny < alto && !visitado[ny][nx] && laberinto[ny][nx] == ' ') {
                    cola.push({nx, ny});
                    visitado[ny][nx] = true;
                    padre[ny][nx] = {x, y};
                }
            }
        }

        return false; // No se encontró solución
    }

    void marcarCamino(vector<vector<pair<int, int>>> &padre, int x, int y) {
        while (x != 0 || y != 1) { // Marca el camino de vuelta a la entrada
            laberinto[y][x] = '*';
            pair<int, int> p = padre[y][x];
            x = p.first;
            y = p.second;
        }
        laberinto[0][1] = '*'; // Marca la entrada
    }
};

int main() {
    int ancho, alto;

    cout << "Ingrese el ancho del laberinto (debe ser impar): ";
    cin >> ancho;
    cout << "Ingrese el alto del laberinto (debe ser impar): ";
    cin >> alto;
    
    if (ancho % 2 == 0 || alto % 2 == 0) {
        cout << "El ancho y el alto deben ser números impares." << endl;
        return 1;
        
    }

    auto inicio_gen = chrono::high_resolution_clock::now();
    Laberinto laberinto(ancho, alto);
    auto fin_gen = chrono::high_resolution_clock::now();
    chrono::duration<double> duracion_gen = fin_gen - inicio_gen;

    auto inicio_res = chrono::high_resolution_clock::now();
    bool resuelto = laberinto.resolverLaberinto();
    auto fin_res = chrono::high_resolution_clock::now();
    chrono::duration<double> duracion_res = fin_res - inicio_res;

    laberinto.mostrarLaberinto();

    cout << "Tiempo de generación: " << duracion_gen.count() << " segundos" << endl;
    cout << "Tiempo de resolución: " << duracion_res.count() << " segundos" << endl;

    if (resuelto) {
        cout << "El laberinto tiene solución." << endl;
    } else {
        cout << "El laberinto no tiene solución." << endl;
    }

    return 0;
}
