package edu.csus.csc130.fall2018.assignment3;

import org.junit.Assert;
import org.junit.jupiter.api.Test;

class NodeTest {

	@Test
	void testIsBinaryTree_1() {
		Node<Integer, String> node = null;
		Assert.assertTrue(Node.isBinaryTree(node));
	}
	
	@Test
	void testIsBinaryTree_2() {
		Node<Integer, String> node = null;
		
		node = new Node<Integer, String>(1, "Fall", 0);
		Assert.assertFalse(Node.isBinaryTree(node));
		
		node = new Node<Integer, String>(1, "Fall", 2);
		Assert.assertFalse(Node.isBinaryTree(node));
		
		node = new Node<Integer, String>(1, "Fall", 1);
		Assert.assertTrue(Node.isBinaryTree(node));
	}
	
	@Test
	void testIsBinaryTree_3() {
		Character[] keys = {'S', 'E', 'X', 'A', 'R', 'C', 'H', 'M'};
		Node<Character, Integer> root = null;
		for (int i=0; i<keys.length; i++) {
			root = put(root, keys[i], i);
		}
		
		Assert.assertTrue(Node.isBinaryTree(root));
		
		updateN(root, 'R', 2);
		Assert.assertFalse(Node.isBinaryTree(root));
		
		updateN(root, 'R', 4);
		Assert.assertFalse(Node.isBinaryTree(root));
	}
	
	@Test
	void testIsBinaryTree_4() {
		Character[] keys = {'S', 'E', 'X', 'A', 'R', 'C', 'H', 'M', 'Z', 'Y'};
		Node<Character, Integer> root = null;
		for (int i=0; i<keys.length; i++) {
			root = put(root, keys[i], i);
		}
		
		Assert.assertTrue(Node.isBinaryTree(root));
		
		updateN(root, 'S', 11);
		Assert.assertFalse(Node.isBinaryTree(root));
		
		updateN(root, 'S', 12);
		Assert.assertFalse(Node.isBinaryTree(root));
	}
	
	private static <Key extends Comparable<Key>, Value> Node<Key, Value> put(Node<Key, Value> node, Key key, Value val) {
		if (node == null)
			return new Node<Key, Value>(key, val, 1);

		int cmp = key.compareTo(node.key);
		if (cmp < 0)
			node.left = put(node.left, key, val);
		else if (cmp > 0)
			node.right = put(node.right, key, val);
		else
			node.val = val;
		node.n = size(node.left) + size(node.right) + 1;

		return node;
	}
	
	private static <Key extends Comparable<Key>, Value>  void updateN(Node<Key, Value> node, Key key, int n) {
		if (node == null) return;

		int cmp = key.compareTo(node.key);
		if (cmp < 0)
			updateN(node.left, key, n);
		else if (cmp > 0)
			updateN(node.right, key, n);
		else
			node.n = n;
	}
	
	private static <Key extends Comparable<Key>, Value> int size(Node<Key, Value> node) {
		return node==null? 0: node.n;
	}

}
