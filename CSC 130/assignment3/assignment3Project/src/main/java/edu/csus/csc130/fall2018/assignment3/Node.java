package edu.csus.csc130.fall2018.assignment3;

/**
 * Updated by: Austin Smothers
 */
public class Node <Key extends Comparable<Key>, Value> {
	Key key; // key
	Value val; // associated value
	Node<Key, Value> left, right; // links to subtrees
	int n; // # nodes in subtree rooted here

	public Node(Key key, Value val, int n) {
		this.key = key;
		this.val = val;
		this.n = n;
	}
	
	/**
	 * @param node root of the sub-tree
	 * @return true if for each node in the sub-tree, n field value matches the number of nodes in its sub-tree, otherwise false
	 */
	public static <Key extends Comparable<Key>, Value> boolean isBinaryTree(Node<Key, Value> node) {
		// provide your implementation here
		
		//check for a null root. If so, return true because that's what test 1 expects
		if(node == null) {
			return true;
		}
		
		// an integer variable to track the sum of node.left.n & node.right.n
		// this will be compared to node.n later
		int n_count = 1;
		
		// check to make sure left node exists. if so, begin recursively going down & left
		if( node.left != null && isBinaryTree(node.left) ) {
			n_count += node.left.n;
		}
		
		// we must be done going left for now. check if right node exists. if so, recursion begins going down and right
		if( node.right != null && isBinaryTree(node.right) ) {
			n_count += node.right.n;
		}
		
		// Compare the value of n_count to node.n. If the two values are the same, the tree is consistent.
		if( n_count == node.n ) {
			return true;
		}
		
		// if we got here the tree must be inconsistent. Return false
		return false;
		
		//throw new UnsupportedOperationException("Not implemented yet");
		
	}
	
}
