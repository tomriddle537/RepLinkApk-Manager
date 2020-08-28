#ifndef NODE_H
#define NODE_H

#include <iostream>
using namespace std;

template <class T>

class NodoSE
{
    public:

    NodoSE *proximo;
    T dato;
    // Constructor por defecto


   NodoSE()
    {
        dato = NULL;
        proximo = NULL;
    }

    // Constructor por parámetro
    NodoSE(T dato_)
    {
        dato = dato_;
        proximo = NULL;
    }

    // Eliminar todos los Nodos
    void BorrarTodo()
    {
        if (proximo)
            proximo->BorrarTodo();
        delete this;
    }

    // Imprimir un Nodo
    void Imprimir()
    {
        cout << dato+" -> ";
    }

    ~NodoSE() {}
};

#endif // NODE_H
