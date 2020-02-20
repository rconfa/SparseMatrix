#ifndef SparseMatrix_H
#define SparseMatrix_H

#include <algorithm>  // std::swap
#include <iostream>
#include <iterator> // std::forward_iterator_tag
#include <cstddef>  // std::ptrdiff_t

/**
	@file SparseMatrix.h 
	@brief Dichiarazione della classe templata SparseMatrix
*/


/**
	Classe eccezione custom che deriva da std::logic_error
	Viene generata quando si tenta di accedere ad indici fuori dalla
	matrice sparsa.

	@brief index out of bounds exception
*/
class index_out_of_bounds_exception : public std::logic_error {
public:
	/**
		Costruttore di default 
	*/
    index_out_of_bounds_exception() : std::logic_error("Index i or j out of bounds") {}
};

/**
	Classe che implementa una matrice sparsa di dati generici T. 
	Vengono fisicamente memorizzati soltanto gli elementi esplicitamente
    inseriti nella matrice dall'utente. 
    Gli elementi non inseriti hanno valore di default di tipo T.

	@brief Matrice sparsa

	@param T tipo del dato
*/
template <typename T>
class SparseMatrix{

public: 
    ///< Definzione del tipo corrispondente a size, nRows, nCols
    typedef unsigned int sm_size;
	typedef T value_type; ///< Definzione del tipo contenuto nella matrice sparsa

    /**
		Struttura che implementa un elemento della matrice sparsa

		@brief elemento della matrice sparsa.
	*/
    struct element {
        // const perche (i,j) non deve essere modificabile
        const sm_size i, j; ///< coordinate dell'elemento nella matrice
        value_type value; ///< dato inserito nella matrice
       

        /**
			@brief Costruttore secondario

		    Costruttore secondario che istanzia un element 

			@param ii indice della riga dell'elemento
            @param jj indice della colonna dell'elemento
            @param v valore dell'elemento
		*/
        element(const sm_size ii,const sm_size jj, const value_type &v) : i(ii), j(jj), value(v) {
            #ifndef NDEBUG
                std::cout << "element::element(sm_size ii, sm_size jj, const value_type &v)" << std::endl;
            #endif
        }

        // NOTA: per tutti gli altri metodi fondamentali (operator=, distruttore, copy constructor) vanno 
		//       bene quelli di default di default
    }; 

private:
	/**
		Struttura di supporto interna che implementa un nodo della lista.
		Utilizzata per memorizzare i valori inseriti dall'utente nella matrice.
		
		@brief Nodo della lista
	*/
    struct node {
		element field; ///< elemento inserito nella lista
        
		node *next; ///< puntatore al nodo successivo della lista

        /**
		    @brief Costruttore di default 
            Costruttore di default
		*/
		node() : next(nullptr) {}
        
        /**
			@brief Costruttore secondario

		    Costruttore secondario che inizializza il nodo
			@param e1 valore dell'elemento.
		*/
        node(const element &e1) : field(e1), next(nullptr) {}

        /**
			@brief Costruttore secondario

		    Costruttore secondario che inizializza il nodo
			@param e1 valore dell'elemento.
			@param n puntatore al nodo successivo
		*/
		node(const element &e1,const node *n) : field(e1), next(n) {}

        // NOTA: per tutti gli altri metodi fondamentali vanno bene quelli di default
		//       di default
	}; 


    //Attributi della classe 
    node *_head;  ///< puntatore al primo nodo della lista
    value_type _D;  ///< valore di default per gli elmenti non inseriti nella matrice sparsa
    sm_size _size;  ///< numero di elementi della lista
    sm_size _nRows;  ///< numero di righe della matrice sparsa
    sm_size _nCols;  ///< numero di colonne della matrice sparsa


    /**
		Funzione helper per la rimozioni di tutti i nodi nella lista

		@brief eliminazione di tutti i nodi
	*/
    void clear(){
        node *currNode = _head;
		node *nextNode = nullptr;

        // scorro tutta la lista fino alla fine
		while(currNode != nullptr) {
			nextNode = currNode -> next; // salvo il puntatore al prossimo nodo altrimenti andrebbe perso
			delete currNode; // elimino il nodo corrente
			currNode = nextNode; // proseguo dal nodo successivo
		}

        // azzero tutti i valori, cosi sono in una condizione coerente a fine clear
		_head = nullptr;
		_size =0;
        _nCols = 0;
        _nRows = 0;
    }

    /**
		Funzione helper privata per aggiungere un elemento nella matrice sparsa.
		L'elemento viene inserito rispettando le coordinate (i,j) in ordine
		crescente. Se la cella è già inizializzata sostituisce soltanto il valore

		@brief Inserisce un elemento nella matrice

		@param e elemento da inserire

		@throw index_out_of_bounds_exception
		@throw eccezione di allocazione di memoria (runtime) 
	*/
    void add(const element &e){
        // controllo che mi abbia passato indici validi, altrimenti genero un'eccezione
        if (e.i < _nRows && e.j < _nCols && e.i >= 0 && e.j >= 0){
			
			node *tmp;

            // se la lista è vuota aggiungo l'elemento all'inizio della lista
            if(_head == nullptr){

				try{
					// creo il nuovo nodo da aggiungere
					tmp = new node(e);
				}
				catch(...){
					tmp = nullptr; // per essere piu sicuro
					throw;
				}
                _head = tmp;
                ++_size;
                return;
            }


            // se la lista non è vuota, ciclo sui nodi finchè non trovo la posizione dove voglio inserire

            node *currNode = _head; // nodo per ciclare
            // tengo in memoria qual è il nodo prima di currNode, cosi possso fare l'inserimento
            node *prevNode = _head; 

            while(currNode != nullptr){ // finche non sono arrivato all'ultimo nodo
                // se l'indice di riga o di colonna è maggiore del nodo che sto analizzando vado avanti
                if(currNode -> field.i < e.i || currNode -> field.j < e.j) {
                    prevNode = currNode;
                    currNode = currNode -> next;
                }
                else{	// inserisco il nuovo elemento
                    // sto inserendo un elemento in una posizione che esiste già, lo sovrascrivo
                    if (currNode -> field.i == e.i && currNode -> field.j == e.j) {
                        currNode -> field.value = e.value;
                        //delete tmp; // lo cancello perchè ho solo modificato il valore quindi disalloco lo spazio
                        return; // non aumento size perchè non ho realmente aggiunto un elemento
                    }

					try{
						// creo il nuovo nodo da aggiungere
						tmp = new node(e);
					}
					catch(...){
						tmp = nullptr; // per essere piu sicuro
						throw;
					}

                    // se devo inserirlo davanti devo sistemare l'head!
                    if (_head == currNode){
                        _head = tmp;
                    }
                    else{
                        prevNode -> next = tmp; 
                    }

                    tmp -> next = currNode;
                    ++_size;
                    return;
                    
                }
            }

			try{
				// creo il nuovo nodo da aggiungere
				tmp = new node(e);
			}
			catch(...){
				tmp = nullptr; // per essere piu sicuro
				throw;
			}

            // caso in cui voglio aggiungere un valore più grande di tutti
            prevNode -> next = tmp; 
            ++_size; 
        }
        else
            throw index_out_of_bounds_exception();
	}


public:
    /**
        @brief Costruttore secondario

        Costruttore secondario. Permette di istanziare una matrice sparsa con una 
        data dimensione e un valore di default.

        @param r numero di righe della matrice
        @param c numero di colonne della matrice
        @param dv valore di default degli elementi della matrice
    */
	SparseMatrix(const sm_size r,const sm_size c, const value_type &dv) 
        : _size(0), _head(nullptr), _nCols(c), _nRows(r), _D(dv) {

        
        #ifndef NDEBUG
            std::cout << "SparseMatrix::SparseMatrix(sm_size r, sm_size c, const value_type &dv)" << std::endl;
        #endif

    };


	/**
		@brief Distruttore (METODO FONDAMENTALE)

		Distruttore. Rimuove la memoria allocata dalla matrice sparsa.
	**/
	~SparseMatrix(){
        clear();
        #ifndef NDEBUG
            std::cout << "SparseMatrix::~SparseMatrix()" << std::endl;
        #endif
    }

	/**
		@brief Operatore di assegnamento (METODO FONDAMENTALE)

        Operatore di assegnamento. Permette la copia tra dbuffer.
		
		@param other matrice sparsa da copiare
		@return reference a this

		@throw index_out_of_bounds_exception
		@throw eccezione di allocazione di memoria (runtime)
	*/
	SparseMatrix& operator=(const SparseMatrix &other){
		if(this != &other){
			SparseMatrix tmp(other); // copio l'altra matrice
            // swappo tutti i valori
            std::swap(this -> _nCols, tmp._nCols);
			std::swap(this -> _nRows, tmp._nRows);
            std::swap(this -> _D, tmp._D);
			std::swap(this -> _head, tmp._head);
			std::swap(this -> _size, tmp._size);
		}

        #ifndef NDEBUG
            std::cout << "SparseMatrix::operator=(const SparseMatrix &other)" << std::endl;
        #endif

		return *this;
	}


	/**
        @brief Copy constructor (METODO FONDAMENTALE)

		Costruttore di copia. Permette di istanziare una matrice sparsa 
        con i valori presi da un altra matrice spara.

		@param other matrice sparsa da copiare

		@throw index_out_of_bounds_exception
		@throw eccezione di allocazione di memoria (runtime)
	*/
    SparseMatrix(const SparseMatrix &other) : _size(0), _head(nullptr), _nCols(0), _nRows(0){
        //TODO devo gestire la new che c'è nella add, potrebbe fallire

            node *currNode = other._head; // salvo il puntatore alla testa
            // usando la add devo aver già definito tutti i valori
            _nCols = other._nCols;
            _nRows = other._nRows;
            _D = other._D;
            
			try {
				while(currNode != nullptr) {
					add(currNode -> field); // sistema già anche la size
					currNode = currNode -> next; // mi sposto al nodo successivo
				}	
			}
			//NOTA: sto catchando anche index_out_of_bounds ma questa eccezione non avverrà mai perchè sto copiando
			// un altra matrice valida.
			catch(...) { 
				clear(); // se qualche add non va a buon fine svuoto tutta la matrice
				throw;
			}
            
        #ifndef NDEBUG
            std::cout << "SparseMatrix::SparseMatrix(const SparseMatrix &other)" << std::endl;
        #endif
    }
    
        
    /**
		@brief Costruttore secondario

		Costruttore secondario che costruisce la matrice sparsa a partire
		da un'altra matrice sparsa di tipo generico Q.

		@param other matrice sparsa da copiare

		@throw index_out_of_bounds_exception
		@throw eccezione di allocazione di memoria (runtime)
	*/
	template <typename Q>
	SparseMatrix(const SparseMatrix<Q> &other) : _size(0), _head(nullptr), _nCols(0), _nRows(0) {
        // sfrutto gli operatori
        typename SparseMatrix<Q> :: const_iterator ib, ie;

        ib = other.begin();
        ie = other.end();

        _nCols = other.getNumCols();
        _nRows = other.getNumRows();
		_D = static_cast<value_type>(other.getDefaultValue()); // casto il valore di default 

		try{
			while(ib != ie){
				element tmpE(ib -> i, ib -> j, static_cast<value_type>(ib -> value));
				add(tmpE);
				++ib;
			}
        }
		//NOTA: sto catchando anche index_out_of_bounds ma questa eccezione non avverrà mai perchè sto copiando
		// un altra matrice valida.
		catch(...) { 
			clear(); // se qualche add non va a buon fine svuoto tutta la matrice
			throw;
		}

        #ifndef NDEBUG
            std::cout << "SparseMatrix(const SparseMatrix<Q> &other)" << std::endl;
        #endif	    
	}

    /**
		@brief Inserimento di un elemento nella matrice

        Inserisce un elemento nella matrice in posizione (i,j) con valore value
		L'elemento viene inserito rispettando le coordinate (i,j) in ordine
		crescente. Se la cella è già inizializzata sostituisce soltanto il valore

		@param value valore da inserire
		@param ii indice della riga in cui inserire l'elemento        
		@param jj indice della colonna in cui inserire l'elemento  

		@throw index_out_of_bounds_exception
		@throw eccezione di allocazione di memoria (runtime)
	*/
    void add(const sm_size ii, const sm_size jj, const value_type& value){
        element tmpE(ii,jj,value);
        add(tmpE); // richiamo il metodo privato di add 
	}
	
    /**
		@brief Accesso ai dati in lettura

        Metodo per leggere il valore dell'elemento in posizione (i,j) della
        matrice. Se l'elemento non è inserito ritorna il valore di default.

		@param ii indice della riga    
		@param jj indice della colonna

        @return valore dell'elemento in posizione (ii,jj)
		
		@throw index_out_of_bounds_exception
	*/
    const value_type& operator()(const sm_size ii,const sm_size jj) const {
       
		#ifndef NDEBUG
            std::cout << "SparseMatrix::operator()(sm_size ii, sm_size jj)" << std::endl;
        #endif	
        
        if (ii < _nRows && jj < _nCols && ii >= 0 && jj >= 0){

            node *currNode = _head;
            while(currNode != nullptr){
                if(currNode -> field.i < ii || currNode -> field.j < jj) {
                    currNode = currNode -> next;
                }
                else if (currNode -> field.i == ii && currNode -> field.j == jj){
                    
                    return currNode -> field.value;
                }
                else
                    return _D;
                
            }

            
            return _D; // non è salvato quindi ritorno il valore di default
        }
        else
            throw index_out_of_bounds_exception();
        
	}
    
   
    /**
		@brief numero di righe della matrice

		Ritorna il numero di righe della matrice

		@return numero di righe della matrice
	*/
    sm_size getNumRows() const{
        return _nRows;
    }

    /**
		@brief numero di colonne della matrice

		Ritorna il numero di colonne della matrice

		@return numero di colonne della matrice
	*/
    sm_size getNumCols() const{
        return _nCols;
    }

    
	/**
		@brief numero di elementi inseriti nella matrice

		Ritorna il numero di elementi inseriti nella matrice sparsa

		@return numero di elementi inseriti
	*/
    sm_size getNumElement() const{
        return _size;
    }

	/**
		@brief valore di default della matrice

		Ritorna il valore di default della matrice

		@return valore di default
	*/
    const value_type& getDefaultValue() const{
        return _D;
    }



    // ------------- ITERATOR ----------------
    class const_iterator; // forward declaration

    /**
		Iteratore della matrice

		@brief Iteratore della matrice
	*/
	class iterator {
		//	
	public:
		typedef std::forward_iterator_tag iterator_category;
		typedef element value_type;
		typedef ptrdiff_t difference_type;
		typedef element* pointer;
		typedef element& reference;

		/**
			Costruttore dell'iteratore
			@brief Setta il nodo a nullptr
		*/
		iterator() : _nPtr(nullptr) {}

		/**
			Copy constructor dell'iteratore

			@brief copy constructor 

			@param un altro iteratore costante other
		*/		
		iterator(const iterator &other) : _nPtr(other._nPtr) {}

		/**
			Ridefinizione operator =

			@brief operatore di assegnamento
			@param un altro iteratore costante other
			@return riferimento all'iteratore
		*/
		iterator& operator=(const iterator &other) {
			_nPtr = other._nPtr;
            return *this;
		}

		/**
			Distruttore dell'iteratore

			@brief distruttore
		*/
		~iterator() {}

		/** 
			ritorna il dato riferito dall'iteratore (dereferenziamento)
			
			@brief operatore di deferenziamento

			@return struct element
		*/
		reference operator*() const {
			return _nPtr -> field;
		}

		/** 
			ritorna il dato riferito dall'iteratore

			@brief operatore ->

			@return puntatore ad un element
		*/
		pointer operator->() const {
			return &(_nPtr -> field);
		}

		/** 
			Operatore di post-incremento dell'iteratore

			@brief operatore di post-incremento 

			@return l'iteratore pre incremento
		*/
		iterator operator++(int) {
			iterator tmp(*this);
			_nPtr = _nPtr -> next;
			return tmp;
		}

		/** 
			Operatore di pre-incremento dell'iteratore

			@brief operatore di pre-incremento 

			@return l'iteratore incrementato
		*/
		iterator& operator++() {
			_nPtr = _nPtr -> next;
            return *this;
		}

		/** 
		 	Ridefinizione dell'operatore di uguaglianza

			@brief Operatore di uguaglianza

			@param un altro iteratore costante other
			@return Risultato dell'uguaglianza
		*/
		bool operator==(const iterator &other) const {
			return _nPtr == other._nPtr;
		}

		/** 
		 	Ridefinizione dell'operatore di diseguaglianza

			@brief Operatore di uguaglianza

			@param un altro iteratore costante other
			@return Risultato della diseguaglianza
		*/
		bool operator!=(const iterator &other) const {
			return _nPtr != other._nPtr;
		}
		
		// Solo se serve anche const_iterator aggiungere le seguenti definizioni
		friend class const_iterator;

		/** 
		 	Ridefinizione dell'operatore di uguaglianza tra iteratore e const_iterator

			@brief Operatore di uguaglianza con const_iterator

			@param un iteratore const_iterator other
			@return Risultato dell'uguaglianza
		*/
		bool operator==(const const_iterator &other) const {
			return _nPtr == other._nPtr;
		}

		/** 
		 	Ridefinizione dell'operatore di diseguaglianza tra iteratore e const_iterator

			@brief Operatore di uguaglianza con const_iterator

			@param un iteratore const_iterator other
			@return Risultato della diseguaglianza
		*/
		bool operator!=(const const_iterator &other) const {
			return _nPtr != other._nPtr;
		}

		// Solo se serve anche const_iterator aggiungere le precedenti definizioni

	private:
		//Dati membro
        node *_nPtr; // puntatore ai nodi altrimenti l'iteratore non può accedere ai dati

		// La classe container deve essere messa friend dell'iteratore per poter
		// usare il costruttore di inizializzazione.
		friend class SparseMatrix; // !!! Da cambiare il nome!

		// Costruttore privato di inizializzazione usato dalla classe container
		// tipicamente nei metodi begin e end
		iterator(node *n) : _nPtr(n){}
	}; // classe iterator
	
	/**
		Ritorna l'iteratore all'inizio della sequenza dati
	
		@return iteratore all'inizio della sequenza
	*/
	iterator begin() {
		return iterator(_head);
	}
	
	/**
		Ritorna l'iteratore alla fine della sequenza dati
	
		@return iteratore alla fine della sequenza
	*/
	iterator end() {
		return iterator(nullptr);
	}
	
	
	/**
		Iteratore costante della lista

		@brief Iteratore costante della lista
	*/
	class const_iterator {
		//	
	public:
		typedef std::forward_iterator_tag iterator_category;
		typedef element value_type;
		typedef ptrdiff_t difference_type;
		typedef const element* pointer;
		typedef const element & reference;

		/**
			Costruttore dell'iteratore costante
			@brief Setta il nodo a nullptr
		*/
		const_iterator() : _nPtr(nullptr){}
		
		/**
			Copy constructor dell'iteratore costante

			@brief copy constructor 

			@param un altro const_iterator other
		*/	
		const_iterator(const const_iterator &other) : _nPtr(other._nPtr){}

		/**
			Ridefinizione operator =

			@brief operatore di assegnamento
			@param un altro const_iterator other
			@return riferimento all'iteratore
		*/
		const_iterator& operator=(const const_iterator &other) {
			_nPtr = other._nPtr;

            return *this;
		}

		/**
			Distruttore dell'iteratore

			@brief distruttore
		*/
		~const_iterator() {}

		/** 
			ritorna il dato riferito dal const_iterator (dereferenziamento)
			
			@brief operatore di deferenziamento

			@return struct element
		*/
		reference operator*() const {
			return _nPtr -> field;
		}

		/** 
			ritorna il dato riferito dal const_iterator 
			@brief operatore ->

			@return puntatore ad un element
		*/
		pointer operator->() const {
			return &(_nPtr -> field);
		}
		
		/** 
			Operatore di post-incremento del const_iterator 

			@brief operatore di post-incremento 

			@return l'iteratore pre incremento
		*/
		const_iterator operator++(int) {
			const_iterator tmp(*this);
            _nPtr = _nPtr->next;

            return tmp;
		}

		/** 
			Operatore di pre-incremento del const_iterator

			@brief operatore di pre-incremento 

			@return l'iteratore incrementato
		*/
		const_iterator& operator++() {
			_nPtr = _nPtr -> next;
            return *this;
		}

		/** 
		 	Ridefinizione dell'operatore di uguaglianza

			@brief Operatore di uguaglianza

			@param un altro const_iterator other
			@return Risultato dell'uguaglianza
		*/
		bool operator==(const const_iterator &other) const {
			return _nPtr == other._nPtr;
		}
		
		/** 
		 	Ridefinizione dell'operatore di diseguaglianza

			@brief Operatore di uguaglianza

			@param un altro const_iterator other
			@return Risultato della diseguaglianza
		*/
		bool operator!=(const const_iterator &other) const {
			return _nPtr != other._nPtr;
		}

		// Solo se serve anche iterator aggiungere le seguenti definizioni
		
		friend class iterator;

		/** 
		 	Ridefinizione dell'operatore di uguaglianza tra const_iterator e iterator 

			@brief Operatore di uguaglianza con iterator

			@param un iteratore costante other
			@return Risultato dell'uguaglianza
		*/
		bool operator==(const iterator &other) const {
			return _nPtr == other._nPtr;
		}

		/** 
		 	Ridefinizione dell'operatore di disuguaglianza tra const_iterator e iterator 

			@brief Operatore di uguaglianza con iterator

			@param un iteratore costante other
			@return Risultato della disuguaglianza
		*/
		bool operator!=(const iterator &other) const {
			return _nPtr != other._nPtr;
		}

		/**
			Costruttore di conversione iterator -> const_iterator
			@brief conversione da iterator a const_iterator

			@param un altro iterator other
		*/		
		const_iterator(const iterator &other) : _nPtr(other._nPtr) {}

		/**
			Ridefinizione operator = da un iterator a un const_iterator

			@brief operatore di assegnamento
			@param un altro iterator other
			@return riferimento al const_iterator
		*/
		const_iterator &operator=(const iterator &other) {
			_nPtr = other._nPtr;
			return *this;
		}

		// Solo se serve anche iterator aggiungere le precedenti definizioni

	private:
		const node* _nPtr;

        friend class SparseMatrix;

        const_iterator(const node* n) : _nPtr(n) {}
		
		
	}; // classe const_iterator
	
	/**
		Ritorna l'iteratore all'inizio della sequenza dati
	
		@return iteratore all'inizio della sequenza
	*/
	const_iterator begin() const {
		return const_iterator(_head);
	}
	
	/**
		Ritorna l'iteratore alla fine della sequenza dati
	
		@return iteratore alla fine della sequenza
	*/
	const_iterator end() const {
		return const_iterator(nullptr);
	}
};

/**
    @brief numero di elementi che soddisfano il predicato


	Ritorna il numero di elementi, compresi quelli di default, della matrice
    che soddisfano un predicato.

	@param sm matrice sparsa su cui verificare il predicato
	@param pred predicato da soddisfare

	@return numero di elementi che soddisfano il predicato
*/
template <typename M, typename P>
unsigned int  evaluate(const SparseMatrix<M> &sm, P pred){
	typename SparseMatrix<M> :: const_iterator i, ie;

	i = sm.begin();
	ie = sm.end();

    unsigned int counter = 0;
    
    // controllo se soddisfa il valore di default
    if(pred(sm.getDefaultValue())){
        // conto quanti valori di default ci sono nella matrice
        unsigned int numElementMax = sm.getNumRows() * sm.getNumCols();
        counter = numElementMax - sm.getNumElement();
    }
	while(i != ie){
       
        // se il predicato rispetta il valore aumento il contatore
		if(pred((*i).value)){
			counter ++;
		}
		++i;
	}

    return counter;
}


#endif