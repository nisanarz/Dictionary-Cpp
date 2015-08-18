/*
 * map.h
 *
 *  Created on: 8 αιπε 2014
 *      Author: nis
 */


#ifndef MAP_H_
#define MAP_H_


#include <iostream>
#include <exception>
using std::ostream;
using std::exception;

namespace mtm {

class MappingDoesntExist : public exception {
public:
	  virtual const char* what() const throw()
	  {
	    return "Mapping for key was not found!\n";
	  }
};
class MappingAlreadyExists : public exception {
public:
	  virtual const char* what() const throw()
	  {
	    return "Key already exists\n";
	  }
};


/*
 * Generic Node structure
 */
template<class K, class V>
struct Node {
	K keyElement;
	V valueElement;
	Node* next;

	friend std::ostream& operator<< (std::ostream& os, const Node<K,V>* it){
		return os << it->keyElement << ":" << it->valueElement;
	}
};

/*
 * Generic Map class
 * Methods:
 *
 */

template<class K, class V>
class Map{
public:
	class iterator;
	class const_iterator;

	Map();						//C'tor
	Map(const Map& map); 		//copy c'tor
	~Map();						//D'tor
	void insert(K key,V value);
	void remove(K key);
	bool isIn(K key);
	int size() const;
	void clear();
	bool isEmpty();

	K getFirstKey();

	iterator begin();
	const_iterator begin() const;
	iterator end();
	const_iterator end() const;

	V& operator[](K keyIndex);
	bool operator==(const Map& map) const;

private:
	Node<K,V>* Head;
	int mapSize;
};

/*
 * C'tor
 */
template<class K, class V>
Map<K,V>::Map() : Head(NULL) , mapSize(0){}

/*
 * Copy C'tor
 */
template<class K, class V>
Map<K,V>::Map(const Map& map) : Head(NULL) , mapSize(0){
	Node<K,V>* nodeHelper = Head;
	while (nodeHelper != NULL){
		insert(nodeHelper->keyElement, nodeHelper->valueElement);
		nodeHelper=nodeHelper->next;
	}
}

/*
 * D'tor
 */
template<class K, class V>
Map<K,V>::~Map() {
	clear();
}

/*
 * operator ==
 */
template<class K, class V>
bool Map<K,V>::operator==(const Map& map) const {
	for (const_iterator it1 = map.begin(), it2 = begin();
	    (it1 != map.end() && it2 != end()); ++it1, ++it2) {
		if (*it1 != *it2) {
			return false;
		}
	}
	return (mapSize == map.mapSize);
}
/*
 * operator []
 */
template<class K, class V>
V& Map<K,V>::operator[](K keyIndex){
	if (isEmpty() || !(isIn(keyIndex))){
		throw MappingDoesntExist();
	}
	Node<K,V>* nodeHelper = Head;
	while (nodeHelper != NULL){
		if (nodeHelper->keyElement == keyIndex){
			return nodeHelper->valueElement;
		}
		nodeHelper=nodeHelper->next;
	}
	return nodeHelper->valueElement;
}

/*
 * insert()
 */
template<class K, class V>
void Map<K,V>::insert(K key,V value) {
	if (isIn(key)) {
		throw MappingAlreadyExists();
	}
	mapSize++;
	Node<K,V>* newNode = new Node<K,V>;
	newNode->keyElement = key;
	newNode->valueElement = value;
	newNode->next = NULL;
	Node<K,V>* nodeHelper = Head;
	if (Head == NULL){
		Head = newNode;			 // map empty, initialize head
		return;
	}

	while (nodeHelper != NULL){
		if (nodeHelper->next == NULL) {
			nodeHelper->next = newNode;
			return;
		}
		nodeHelper = nodeHelper->next;
	}
}

/*
 * remove()
 */
template<class K, class V>
void Map<K,V>::remove(K key){
	if (!(isIn(key)) || isEmpty()){
		throw MappingDoesntExist();
	}
	mapSize--;
	Node<K,V>* nodeToRemove = Head;
	if (Head->keyElement == key){
		Head = Head->next; 					// remove the head
		delete nodeToRemove;
		return;
	}
	Node<K,V>* nodeHelper = nodeToRemove;
	nodeToRemove=nodeToRemove->next;
	while (nodeToRemove != NULL){
		if (nodeToRemove->keyElement == key){
			nodeHelper->next=nodeToRemove->next;
			delete nodeToRemove;
			return;
		}
		nodeToRemove=nodeToRemove->next;
		nodeHelper=nodeHelper->next;
	}
}

/*
 * isIn()
 */
template<class K, class V>
bool Map<K,V>::isIn(K key){
	if (mapSize == 0){
		return false; //or throw?
	}
	Node<K,V>* nodeToMatch = Head;
	while(nodeToMatch != NULL){
		if (nodeToMatch->keyElement == key){
			return true;
		}
		nodeToMatch=nodeToMatch->next;
	}
	return false;
}

/*
 * size()
 */
template<class K, class V>
int Map<K,V>::size() const{
	return mapSize;
}

/*
 * isEmpty()
 */
template<class K, class V>
bool Map<K,V>::isEmpty(){
	if (mapSize == 0){
		return true;
	}
	return false;
}

/*
 * clear()
 */
template<class K, class V>
void Map<K,V>::clear(){
	if (Head == NULL){
		return;
	}
	Node<K,V>* nodeToRemove = Head;
	while (nodeToRemove != NULL){
		remove(nodeToRemove->keyElement);
		nodeToRemove=Head;
	}
}

/*
 * getFirstKey()
 */
template<class K, class V>
K Map<K,V>::getFirstKey(){
	if (Head == NULL){
		throw MappingDoesntExist();
	}
	return this->Head->keyElement;
}


/*
 * begin()
 */
template<class K, class V>
typename Map<K,V>::iterator Map<K,V>::begin(){
	return iterator(this->Head);
}

template<class K, class V>
typename Map<K,V>::const_iterator Map<K,V>::begin() const{
	return const_iterator(this->Head);
}

/*
 * end()
 */
template<class K, class V>
typename Map<K,V>::iterator Map<K,V>::end(){
	return iterator(NULL);
}

template<class K, class V>
typename Map<K,V>::const_iterator Map<K,V>::end() const{
	return const_iterator(NULL);
}

/* ----------------------------------------------------------------------------
 * ----------------------------------------------------------------------------
 * Generic Const Iterator class
 * inheritance from Iterator class
 * should act the same as Iterator class, only difference is
 * that the dereference returns a const reference to element
 * ----------------------------------------------------------------------------
 */
template <class K, class V>
class Map<K,V>::const_iterator {
	Node<K,V>* node;
	friend class Map<K,V>;
	friend class Map<K,V>::iterator;
public:
	explicit const_iterator(Node<K,V>* node = NULL); // C'tor
	const_iterator(const iterator& iterator); // Copy C'tor
	friend std::ostream& operator<< (std::ostream& os,const const_iterator& it){
		return os << it.node->keyElement << ":" << it.node->valueElement;
	}

	Node<K,V>* operator*(){
			if (!node) {
				throw MappingDoesntExist();
			}
			return this->node;
	}
	const_iterator& operator++();
	const_iterator operator++(int);
	bool operator==(const const_iterator& iterator) const;
	bool operator!=(const const_iterator& iterator) const;
};


/* ----------------------------------------------------------------------------
 * ----------------------------------------------------------------------------
 * ----------------------------------------------------------------------------
 * Iterator class generic implementation
 *
 * ----------------------------------------------------------------------------
 */
template <class K, class V>
class Map<K,V>::iterator {
	Node<K,V>* node;
	friend class Map<K,V>;
	friend class Map<K,V>::const_iterator;

public:
	explicit iterator(Node<K,V>* node = NULL); // C'tor

	friend std::ostream& operator<< (std::ostream& os, const iterator& it){
		return os << it.node->keyElement << ":" << it.node->valueElement;
	}

	Node<K,V>* operator*(){
			if (!node) {
				throw MappingDoesntExist();
			}
			return this->node;
	}
	iterator& operator++();
	iterator operator++(int);
	bool operator==(const const_iterator& iterator) const;
	bool operator!=(const const_iterator& iterator) const;
};

/* Iterator C'tor
 */
template <class K, class V>
Map<K,V>::iterator::iterator(Node<K,V>* node) :
	 node(node) {}

/* Iterator ++
 */
template <class K, class V>
class Map<K,V>::iterator& Map<K,V>::iterator::operator++()  {
//	assert(node);
	node = node->next;
	return *this;
}

/* ++ Iterator
 */
template <class K, class V>
class Map<K,V>::iterator Map<K,V>::iterator::operator++(int) {
	iterator result = *this;
	node = node->next;
	return result;
}

/* Iterator == comparison
 */
template <class K, class V>
bool Map<K,V>::iterator::operator==(const const_iterator& iterator) const {
	return (node == iterator.node);
}

/* Iterator != not equal
 */
template <class K, class V>
bool Map<K,V>::iterator::operator!=(const const_iterator& iterator) const {
	return !(*this == iterator);
}



/* Implementation of ConstIterator methods:
 */
/* ConstIterator C'tor
 */
template <class K, class V>
Map<K,V>::const_iterator::const_iterator(Node<K,V>* node) :
	node(node) {}


/* ConstIterator Copy C'tor
 */
template <class K, class V>
Map<K,V>::const_iterator::const_iterator(const iterator& iterator) :
	node(iterator.node) {}

/* ConstIterator ++
 */
template <class K, class V>
class Map<K,V>::const_iterator& Map<K,V>::const_iterator::operator++()  {
//	assert(node);
	node = node->next;
	return *this;
}

/* ++ ConstIterator
 */
template <class K, class V>
class Map<K,V>::const_iterator Map<K,V>::const_iterator::operator++(int) {
	const_iterator result = *this;
	node = node->next;
	return result;
}

/* ConstIterator == comparison
 */
template <class K, class V>
bool Map<K,V>::const_iterator::operator==(const const_iterator& iterator) const{
	return (node == iterator.node);
}

/* ConstIterator != not equal
 */
template <class K, class V>
bool Map<K,V>::const_iterator::operator!=(const const_iterator& iterator) const{
	return !(*this == iterator);
}

/*
 *MapIf function
 *Condition - generic compare function.
 *DoThat - generic function which change the map.
 */

template <class K, class V, typename Condition, typename DoThat>
void MapIf(Map<K,V>& map, Condition condition, DoThat todo){
	for (typename Map<K,V>::iterator it = map.begin(); it != map.end(); ++it){
		Node<K,V>* tmp = *it;
		if (condition(tmp->keyElement)){
			todo(map[tmp->keyElement]);
		}
	}
	return;
}

}

#endif /* MAP_H_ */
