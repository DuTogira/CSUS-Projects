package edu.csus.csc130.fall2018.assignment4;


import java.util.ArrayList;
import java.util.List;

public class SequentialSearchST<Key, Value> 
			extends AbstractSymbolTable<Key, Value> {
	
	// linked-list node
	private class Node { 
		Key key;
		Value val;
		Node next;

		public Node(Key key, Value val, Node next) {
			this.key = key;
			this.val = val;
			this.next = next;
		}
	}		

	private Node first = null; // first node in the linked list
	int size = 0;
	
	public SequentialSearchST() {
		first = null;
		size = 0;
	}

	@Override
	public void put(Key key, Value val) {
		if (key==null) return; // do not allow null key
		
		// Search for key. Update value if found; grow table if new.
		for (Node node = first; node != null; node = node.next) {
			if (key.equals(node.key)) {
				// Search hit: update val.
				node.val = val;
				return;
			}
		}
		first = new Node(key, val, first); // Search miss: add new node
		size++;
	}

	@Override
	public Value get(Key key) {
		// Search for key, return associated value.
		for (Node node = first; node != null; node = node.next) {
			if (key.equals(node.key)) {
				return node.val; // search hit
			}
		}
		return null; // search miss
	}

	@Override
	public int size() {
		return size;
	}

	@Override
	public void delete(Key key) {
		if (key==null || first==null) return;
		if (key.equals(first.key)) {
			first = first.next;
			size--;
		} else {
			for (Node prev = first; prev.next != null; prev = prev.next) {
				if (key.equals(prev.next.key)) {
					prev = prev.next.next;
					size--;
					return;
				}
			}
		}
	}
	
	@Override
	public Iterable<Key> keys() {
		// unimplemented
		List<Key> list = new ArrayList<Key>();
		Node node =first;
		while (node!=null) {
			list.add(node.key);
			node = node.next;
		}
		
		return list;
	}	
	
	public String toString() {
		StringBuilder sb = new StringBuilder();
		Node node = first;
		while (node!=null) {
			sb.append(String.format("(%s, %s) ", node.key, node.val));
			node = node.next;
		}
		return sb.toString();
	}


}
