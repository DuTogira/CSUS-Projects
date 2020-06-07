package edu.csus.csc130.fall2018.assignment4;

import java.util.ArrayList;
import java.util.List;

/**
 * Modified by: Austin Smothers
 *
 */
public class SeparateChainingHashST<Key extends Comparable<Key>, Value> implements SymbolTable<Key, Value>{
	private int n; // number of key value pairs
	private int m; // hash table size
	double threshold = 0.75;
	// array of unordered linked list ST objects
	private SequentialSearchST<Key, Value>[] st; 

	public SeparateChainingHashST() {
		this(997);
	}

	@SuppressWarnings({ "unchecked", "rawtypes" })
	public SeparateChainingHashST(int m) { 
		this.n= 0;
		this.m = m;
		// create m linked lists.
		st = (SequentialSearchST<Key, Value>[]) new SequentialSearchST[m];
		for (int i = 0; i < m; i++) {
			st[i] = new SequentialSearchST();
		}
	}

	private int hash(Key key) {
		return (key.hashCode() & 0x7fffffff) % m;
	}

	public Value get(Key key) {
		return (Value) st[hash(key)].get(key);
	}

	public Iterable<Key> keys() {
		// unimplemented
		return null; 
	}

	@Override
	public boolean contains(Key key) {
		return get(key)!=null;
	}

	@Override
	public boolean isEmpty() {
		return (n==0);
	}

	@Override
	public int size() {
		return n;
	};
	
	// update this method to keep track of n
	public void put(Key key, Value val) {
		if (getLoadFactor() >= threshold) {
			resize(2*m);
		}
		
		if(!contains(key)) {
			n += 1;
		}

		st[hash(key)].put(key, val);
	}
	
	// update this method to keep track of n
	public void delete(Key key) {
		if(contains(key)) {
			n -= 1;
		}

		st[hash(key)].delete(key);
	}
	
	/**
	 * @return load factor of the symbol table: (number of key value pairs)/(table size)
	 */
	public double getLoadFactor() {
		return ((double) n/m);
	}
	
	/**
	 * Resize method creates a new table with the given capacity and rehashes existing key value pairs 
	 * to the new table in the order as they appear in the existing table
	 */
	@SuppressWarnings({"unchecked", "rawtypes"})
	public void resize(int capacity) {
		// create capacity linked lists.
		SequentialSearchST<Key, Value>[] t;
		t = (SequentialSearchST<Key, Value>[]) new SequentialSearchST[capacity];
		for (int i = 0; i < capacity; i++) {
			t[i] = new SequentialSearchST();
		}
		for (int i = 0; i < m; i++) {
			if(st[i] != null) {
				for (Key key : st[i].keys()) {
					// we can't call hash() here because we need to hash for t[] and hash() uses m,
					// which is half the size of t[]. We can't update m to equal capacity yet because that
					// causes an out of bounds error for the put and get functions (which use m on st[])
					t[(key.hashCode() & 0x7fffffff) % capacity].put(key, get(key));
				}
			}
		}
		st = t;
		m = capacity;

	}
}
