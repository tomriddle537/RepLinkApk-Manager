#ifndef LIST_H
#define LIST_H

#include <fstream>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <nodoSE.h>


using namespace std;

template <class T>

class ListaSE
{

protected:
    NodoSE<T> *cabeza;
    int noNodos;
public:
    // Constructor por defecto
    ListaSE()
    {
        noNodos = 0;
        cabeza = NULL;
    }

    int Longitud()
    {
        return noNodos;
    }

    bool Vacia(){
        return noNodos==0;
    }


    T ObtenerPorPos(int pos){
        NodoSE<T> *temp = cabeza;

        if (pos < 1 || pos > noNodos) {
            cout << "Fuera de rango " << endl;
            throw exception();
        } else if (pos == 1) {
            return cabeza->dato;
        } else {
            NodoSE<T> *temp1 = temp->proximo;
            for (int i = 2; i <= pos; i++) {
                if (i == pos) {
                    return temp1->dato;
                }
                //temp = temp->next;
                temp1 = temp1->proximo;
            }
        }
        return cabeza->dato;
    }

    T* ObtenerPorPosMem(int pos){
        NodoSE<T> *temp = cabeza;

        if (pos < 1 || pos > noNodos) {
            cout << "Fuera de rango " << endl;
            throw exception();
        } else if (pos == 1) {
            return &cabeza->dato;
        } else {
            NodoSE<T> *temp1 = temp->proximo;
            for (int i = 2; i <= pos; i++) {
                if (i == pos) {
                    return &temp1->dato;
                }
                //temp = temp->next;
                temp1 = temp1->proximo;
            }
        }
        return &cabeza->dato;
    }


    // Insertar al inicio

    void AdicionarCabeza(T dato_)
    {
        NodoSE<T> *nuevoNodo = new NodoSE<T> (dato_);
        NodoSE<T> *temp = cabeza;

        if (!cabeza) {
            cabeza = nuevoNodo;
        } else {
            nuevoNodo->proximo = cabeza;
            cabeza = nuevoNodo;

            while (temp) {
                temp = temp->proximo;
            }
        }
        noNodos++;
    }

    // Insertar al final

    void AdicionarCola(T dato_)
    {
        NodoSE<T> *nuevoNodo = new NodoSE<T> (dato_);
        NodoSE<T> *temp = cabeza;

        if (!cabeza) {
            cabeza = nuevoNodo;
        } else {
            while (temp->proximo != NULL) {
                temp = temp->proximo;
            }
            temp->proximo = nuevoNodo;
        }
        noNodos++;
    }

    // Concatenar a otra Lista

    void Concatenar(ListaSE lista)
    {
        NodoSE<T> *temp2 = lista.cabeza;

        while (temp2) {
            AdicionarCola(temp2->dato);
            temp2 = temp2->proximo;
        }
    }

    // Eliminar todos los nodos
    void BorrarTodo()
    {
        cabeza->BorrarTodo();
        cabeza = 0;
    }

    // Eliminar por dato en el nodo
    void BorrarPorDato(T data_)
    {
        NodoSE<T> *temp = cabeza;
        NodoSE<T> *temp1 = cabeza->proximo;

        int cont = 0;

        if (cabeza->dato == data_) {
            cabeza = temp->proximo;
        } else {
            while (temp1) {
                if (temp1->dato == data_) {
                    NodoSE<T> *aux_node = temp1;
                    temp->proximo = temp1->proximo;
                    delete aux_node;
                    cont++;
                    noNodos--;
                }
                temp = temp->proximo;
                temp1 = temp1->proximo;
            }
        }
    }

    // Eliminar por posición del nodo
    void BorrarPorPos(int pos)
    {
        NodoSE<T> *temp = cabeza;
        NodoSE<T> *temp1 = temp->proximo;

        if (pos < 1 || pos > noNodos) {
            throw exception();
        } else if (pos == 1) {
            noNodos--;
            cabeza = temp->proximo;
        } else {
            for (int i = 2; i <= pos; i++) {
                if (i == pos) {
                    NodoSE<T> *nodoAux = temp1;
                    temp->proximo = temp1->proximo;
                    delete nodoAux;
                    noNodos--;
                }
                temp = temp->proximo;
                temp1 = temp1->proximo;
            }
        }
    }

    // Imprimir la Lista
    void Imprimir()
    {
        NodoSE<T> *temp = cabeza;
        if (!Vacia()) {
            while (temp) {
                temp->Imprimir();
                if (!temp->proximo) cout << "NULL";
                temp = temp->proximo;
            }
        }
        cout << endl << endl;
    }

    // Buscar el dato de un nodo
    int Buscar(T data_)
    {
        NodoSE<T> *temp = cabeza;
        int cont = 1;

        while (temp) {
            if (temp->dato == data_) {
                return cont;
            }
            temp = temp->proximo;
            cont++;
        }

        return -1;
    }


    ~ListaSE() {}

};

#endif // LIST_H
