package edu.csus.csc130.fall2018.assignment4;


public interface SymbolTable<Key, Value> {
	//put key-value pair into the table, remove key from table if value is null
	public void put(Key key, Value val); 
	
	//get value paired with key, null if key is absent
	public Value get(Key key); 
	
	//remove key (and its value) from table
	public void delete(Key key); 
	
	//is there a value paired with key?
	public boolean contains(Key key); 
	
	//is the table empty?
	public boolean isEmpty(); 
	
	//number of key-value pairs in the table
	public int size(); 
	
	//get all the keys in the table
	public Iterable<Key> keys(); 
}

