package edu.csus.csc130.fall2018.assignment2;

import org.junit.Assert;
import org.junit.jupiter.api.Test;

class QuickSortNonRecursiveTest {

	@Test
	void testSortI_1() {
		Character[] a = {'b'};
		QuickSortNonRecursive.sort(a);
		Assert.assertTrue(SortUtils.isNonDecreasingOrder(a));
	}
	
	@Test
	void testSortI_2() {
		Integer[] a = {2, 1};
		QuickSortNonRecursive.sort(a);
		Assert.assertTrue(SortUtils.isNonDecreasingOrder(a));
	}
	
	@Test
	void testSortI_3() {
		Integer[] a = {1, 2};
		QuickSortNonRecursive.sort(a);
		Assert.assertTrue(SortUtils.isNonDecreasingOrder(a));
	}
	
	@Test
	void testSortI_4() {
		String[] a = {"1", "3", "2"};
		QuickSortNonRecursive.sort(a);
		Assert.assertTrue(SortUtils.isNonDecreasingOrder(a));
	}

}
