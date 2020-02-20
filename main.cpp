#include <iostream>
#include <vector>
#include <cassert>
#include <string>
#include "SparseMatrix.h"

void test_element(){
    std::cout << "**********TEST ELEMENT**********" << std::endl;
    SparseMatrix<int>::element e1(3,3,3);
    std::cout << "stampa element: " << e1.i << " " << e1.j << " " << e1.value << std::endl;
    //testo se i metodi di default fondamentali vanno bene
    SparseMatrix<int>::element e2(e1);
    assert(e1.i == e2.i);
    assert(e1.j == e2.j);
    assert(e1.value == e2.value);
    
}

void test_constructor(){
    std::cout << "**********TEST COSTRUTTORI**********" << std::endl;

    SparseMatrix<float> sm2(3,3,3.3); // costruttore con parametri
    assert(sm2.getNumCols() == 3);
    assert(sm2.getNumRows() == 3);
    assert(sm2.getDefaultValue() == (float)3.3);
    assert(sm2.getNumElement() == 0); 

    //copy constructor
    SparseMatrix<float> sm3(sm2); 
    assert(sm3.getNumCols() == 3);
    assert(sm3.getNumRows() == 3);
    assert(sm3.getDefaultValue() == (float)3.3);
    assert(sm3.getNumElement() == 0);

    // controllo che abbia effettivamente creato una copia
    sm2.add(2,2,3.3);
    assert(sm2.getNumElement() == sm3.getNumElement() +1); 

     // costruttore =
    SparseMatrix<float> sm4(6,6,10.9);
    assert(sm4.getDefaultValue() == (float)10.9);
    sm4 = sm2;
    assert(sm4.getNumCols() == 3);
    assert(sm4.getNumRows() == 3);
    assert(sm4.getDefaultValue() == (float)3.3);
    assert(sm4.getNumElement() == 1);

    //costruttore con matrice di altro tipo Q
    SparseMatrix<int> sm5(sm4);
    assert(sm5.getNumCols() == 3);
    assert(sm5.getNumRows() == 3);
    assert(sm5.getDefaultValue() == 3); //controllo che abbia convertito il tipo
    assert(sm5.getNumElement() == 1);
}

// controllo che l'accesso in lettura sia const, per semplicita nella scrittura del
// metodo definisco T = int
void test_constness(const SparseMatrix<int> &sm){
    std::cout << "**********TEST CONSTNESS**********" << std::endl;

    sm(2,2);
    sm.getNumCols();
    sm.getNumRows();
    int dv = sm.getDefaultValue();
    sm.getNumElement();
    dv = 999;
    assert(sm.getDefaultValue() == 9);
}

void test_add_getter(){
    std::cout << "**********TEST ADD & GETTER**********" << std::endl;

    //matrice 3x3
    SparseMatrix<int> sm(3,3,9); 

    try{

        sm.add(0,0,8); //inserimento in testa alla matrice
        sm.add(2,2,8); //inserimento in fondo alla matrice
        sm.add(1,2,8); //inserimento casuale
        assert(sm.getNumElement() == 3);
        // sovrascrivo un elemento già inserito e controllo che non aumenti la size
        sm.add(1,2,7); 
        assert(sm.getNumElement() == 3);

        // provo ad inserire un elemento al di fuori degli indici e controllare l'eccezione
        try{
            sm.add(3,5,3);
        }
        catch(index_out_of_bounds_exception e){
            std::cerr << e.what() << std::endl;
            assert(sm.getNumElement() == 3); // controllo che non sia aumentata la size
        }
    }
    catch(...){} // catcho gli errori sulla memoria

    // prova dei get
    assert(sm.getNumCols() == 3);
    assert(sm.getNumRows() == 3);
    assert(sm.getDefaultValue() == 9);
    assert(sm(2,2) == 8); // accesso in lettura
    // controllo che operator() ritorni un valore costante
    SparseMatrix<int>::value_type tmp = sm(2,2);
    tmp = 11;
    assert(sm(2,2) == 8);
    assert(sm(1,1) == 9); //leggo un valore di default

    test_constness(sm); 
}

class car{
public: // tutto pubblico per semplicita!!!!
    int year;
    double km;


    car() : year(0), km(0.0){}
    car(int y, double k) : year(y), km(k){}

    bool equal(car c2){
        return (year == c2.year && km == c2.km);
    }
};

void test_custom(){
    std::cout << "**********TEST CUSTOM TYPE**********" << std::endl;
    car temp(10,10000);

    SparseMatrix<car> sm(2,2,temp);
    assert(temp.equal(sm(0,0))); // controllo che gli elementi non inseriti corrispondano al valore di default
    try{
        sm.add(0,1,car(10,99));
    }
    catch(...){}
    assert(sm.getNumElement() == 1);
    assert(car(10,99).equal(sm(0,1)));

}

// mi dice se un intero è pari
struct is_even {
	bool operator()(int value) const {
		return (value%2) == 0;
	}
};

// mi dice se ha fatto piu di 100 km
struct old_car {
	bool operator()(const car &tmp) const {
		return tmp.km > 100;
	}
};

struct People{
    int anno;
    double peso;

public:
    People() : anno(0), peso(0){}
    People(int a, double p) : anno(a), peso(p){}
    // altri metodi uso quelli di default

};

struct test_people{
	bool operator()(const People &tmp) const {
		return tmp.anno > 9 ;
	}
};


void test_evaluate(){
    std::cout << "**********TEST EVALUATE**********" << std::endl;
    try{
        SparseMatrix<int> sm(5,5,8); 
        assert(evaluate(sm,is_even()) == 25);
        sm.add(0,0,1);
        sm.add(0,2,9);
        assert(evaluate(sm,is_even()) == 23);
        sm.add(4,4,6); //era già pari ma gli cambio il valore rispetto al default
        assert(evaluate(sm,is_even()) == 23);

        // provo con tipi custom
        car temp(10,10);
        SparseMatrix<car> zero(2,2,temp);
        assert(evaluate(zero,old_car()) == 0);
        
        car temp2(10,101);
        SparseMatrix<car> sm2(2,2,temp2);
        assert(evaluate(sm2,old_car()) == 4);
        sm2.add(0,1,car(9,99));
        assert(evaluate(sm2,old_car()) == 3);
    }
    catch(...){}

    // creo una matrice con una struct e provo ad applicare evaluate
    People p0(10,11.3);
    SparseMatrix<People> smp(3,3,p0);
    assert(evaluate(smp,test_people()) == 9);
}   

void test_iterator_const(const SparseMatrix<int> &sm) {
    std::cout << "**********TEST ITERATOR CONST**********" << std::endl;
   
    SparseMatrix<int> ::const_iterator i, ie;

    i = sm.begin();
	ie = sm.end();

	// stampo tutti i valori
	while(i != ie){   
        SparseMatrix<int>::element e1(*i);
        std::cout  << (*i).value << " "; 
        i ++ ; // testato anche i++
    }
	std::cout << std::endl;
}

void test_iterator_NO_const(SparseMatrix<int> &sm) {
    std::cout << "**********TEST ITERATOR NO CONST**********" << std::endl;
   
    SparseMatrix<int> ::iterator i, ie;

    i = sm.begin();
	ie = sm.end();

	// stampo tutti i valori
	while(i != ie){   
        (*i).value = 100; // modifico i valori
        SparseMatrix<int>::element e1(*i);
        std::cout  << e1.value << " "; 
        ++i; // testato anche i++
    }
	std::cout << std::endl;
}

void test_iterator(){
    SparseMatrix<int> sm(3,3,1);

    try{
    // inserisco valori in tutta la matrice
    for(unsigned int i = 0; i<3; ++i)
        for(unsigned int j = 0; j<3; ++j)
            sm.add(i,j, (j+i));
    }
    catch(...){} //catcho possibili errori di memoria, gli indici sono sicuro che siano corretti

    // stampo tutto con iteratori COSTANTI
    test_iterator_const(sm);
    // stampo e modifico con iteratori NON costanti
    test_iterator_NO_const(sm);
    assert(sm(0,1) == 100); // controllo se sono cambiati i valori
}


int main(){
    
    test_element(); // ma element va privato????!
    test_constructor();
    test_add_getter();
    test_custom(); 
    test_evaluate();

    test_iterator();
   
   /*  
    std::vector<SparseMatrix<int>> sm(5);
    SparseMatrix<int> temp(3,3,3);
    sm.push_back(temp); */

    return 0;
}