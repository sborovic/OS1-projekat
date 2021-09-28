#ifndef _list_h_
#define _list_h_

#include "thr_mem.h"

// Napomena: Sabloni za klase su i deklarisani i definisani u ovom .h fajlu

/*
 * Node<T> - Deklaracija
 */
template <class T>
class Node : public ThrMem  {
public:
	Node *prev, *next;
	T* value;
	~Node() {//syncPrintf("\ndtor node : %d", *value);
		}

private:
	class List<T>;
	// Node<T> moguce je konstruisati jedino iz List<T> i nije ga moguce kopirati
	Node(List<T>& list, Node* prev, T* value, Node* next);
	Node(Node const&);
	Node& operator=(Node const&);
	friend class List<T>;
};

/*
 * List<T> - Definicija
 */
template <class T>
class List : public ThrMem {
public:
	class Iterator {
	public:
		Iterator(Node<T>* node) : node(node) {}
		Iterator& operator++() {
			node = node->next;
			return *this;
		}
		Iterator operator++(int) {
			Iterator temp(*this);
			node = node->next;
			return temp;
		}
		int operator==(Iterator const& rhs) const {
			return node == rhs.node;
		}
		int operator!=(Iterator const& rhs) const {
			return node != rhs.node;
		}
		T* operator*() {
			return node->value;;
		}
	private:
		// Pokazivacu na Node<T> moguce je pristupiti samo iz List<T>
		// Objektu value tipa T sadrzanom u node moguc je javni pristup putem iteratora
		Node<T>* node;
		friend class List<T>;
	};

	List() : head(0), tail(0), size(-1) {
		new Node<T>(*this, 0, 0, 0);
	}
	Iterator remove(Iterator& iterator) {
		if (iterator == end()) return end();
		Node<T>* temp = iterator.node;
		temp->next->prev = temp->prev;
		if (temp->prev == 0) head = temp->next;
		else temp->prev->next = temp->next;
		Iterator next(temp->next);
		delete temp;
		--size;
		return next;
	}
	List<T>& add(T* t) {
		new Node<T>(*this, tail->prev, t, tail);
		return *this;
	}
	int is_empty() const {
		return size == 0;
	}
	int get_size() const {
		return size;
	}
	Iterator begin() {
		return Iterator(head);
	}
	Iterator end() {
		return Iterator(tail);
	}
	~List() {
		Iterator it = begin(), end_it = end();
		while (it != end_it) {
			Node<T>* temp = it.node;
			++it;
			delete temp;
		}
		delete tail;
	}

private:
	// Zabranjeno je kopiranje List<T>
	List(List const&);
	List& operator=(List const&);
	Node<T> *head, *tail;
	int size;
	friend class Node<T>;
};

/*
 * Node<T> - Definicija
 */

template <class T>
Node<T>::Node(List<T>& list, Node<T>* prev, T* value, Node<T>* next)
	: prev(prev), value(value), next(next) {
	if (prev == 0) list.head = this;
	else prev->next = this;

	if (next == 0) list.tail = this;
	else next->prev = this;

	++list.size;
}




#endif /* _list_h_ */
