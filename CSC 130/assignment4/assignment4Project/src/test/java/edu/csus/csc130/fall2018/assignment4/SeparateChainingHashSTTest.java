package edu.csus.csc130.fall2018.assignment4;

import org.junit.Assert;
import org.junit.jupiter.api.Test;

class SeparateChainingHashSTTest {
	double delta = 0.00001;
	
	@Test
	void testGetLoadFactor_1() {
		SeparateChainingHashST<Integer, Integer> st = new SeparateChainingHashST<Integer, Integer>(10);
		Integer[] a = {28, 83, 21, 60, 79, 22, 1};
		
		double[] expected1 = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7};
		for (int i=0; i<a.length; i++) {
			st.put(a[i], i);
			Assert.assertEquals(expected1[i], st.getLoadFactor(), delta);
		}
		
		double[] expected2 = {0.6, 0.5, 0.4, 0.3, 0.2, 0.1, 0}; 
		for (int i=0; i<a.length; i++) {
			st.delete(a[i]);
			Assert.assertEquals(expected2[i], st.getLoadFactor(), delta);
		}
	}
	
	@Test
	void testGetLoadFactor_2() {
		SeparateChainingHashST<Integer, Integer> st = new SeparateChainingHashST<Integer, Integer>(10);
		Integer[] a = {28, 83, 28, 21, 60, 83, 79, 22, 1};
		
		double[] expected1 = {0.1, 0.2, 0.2, 0.3, 0.4, 0.4, 0.5, 0.6, 0.7};
		for (int i=0; i<a.length; i++) {
			st.put(a[i], i);
			Assert.assertEquals(expected1[i], st.getLoadFactor(), delta);
		}
		
		double[] expected2 = {0.6, 0.5, 0.5, 0.4, 0.3, 0.3, 0.2, 0.1, 0}; 
		for (int i=0; i<a.length; i++) {
			st.delete(a[i]);
			Assert.assertEquals(expected2[i], st.getLoadFactor(), delta);
		}
	}

	@Test
	void testGetLoadFactor_3() {
		SeparateChainingHashST<Integer, Integer> st = new SeparateChainingHashST<Integer, Integer>(10);
		Integer[] a = {46, 36, 3, 21, 62, 44, 55, 93, 69, 20, 37, 5, 79, 17, 34};
		
		double[] expected1 = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75 };
		for (int i=0; i<a.length; i++) {
			st.put(a[i], i);
			Assert.assertEquals(expected1[i], st.getLoadFactor(), delta);
		}
		
		double[] expected2 = {0.7, 0.6499999999999999, 0.5999999999999999, 0.5499999999999998, 0.49999999999999983, 0.44999999999999984, 
				0.39999999999999986, 0.34999999999999987, 0.2999999999999999, 0.2499999999999999, 0.1999999999999999, 0.1499999999999999, 
				0.09999999999999991, 0.049999999999999906, -9.71445146547012E-17};
		for (int i=0; i<a.length; i++) {
			st.delete(a[i]);
			Assert.assertEquals(expected2[i], st.getLoadFactor(), delta);
		}
	}
	
	@Test
	void testGetLoadFactor_4() {
		SeparateChainingHashST<Integer, Integer> st = new SeparateChainingHashST<Integer, Integer>(10);
		Integer[] a = {49, 72, 40, 94, 92, 58, 90, 47, 59, 24, 35, 9, 69, 11, 68, 81, 5, 95, 47, 58};
		
		double[] expected1 = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.4, 0.425, 0.45, 0.45, 0.45};
		for (int i=0; i<a.length; i++) {
			st.put(a[i], i);
			Assert.assertEquals(expected1[i], st.getLoadFactor(), delta);
		}
		
		double[] expected2 = {0.425, 0.4, 0.375, 0.35, 0.325, 0.3, 0.275, 0.25, 0.225, 0.2, 0.175, 0.15, 0.125, 0.1, 0.075, 0.05, 0.025, 0.0, 0.0, 0.0};
		for (int i=0; i<a.length; i++) {
			st.delete(a[i]);
			Assert.assertEquals(expected2[i], st.getLoadFactor(), delta);
		}
	}
	


}
