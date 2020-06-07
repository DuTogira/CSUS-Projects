package edu.csus.csc130.fall2018.assignment2;

import java.util.Arrays;

import org.junit.jupiter.api.Test;

class QuickSortRandomizedTest {
	String[] messages = {"random", "sorted", "reverse sorted"};

	@Test
	void testSort1() {
		for (int i=0; i<3; i++) {
			System.out.println("\nResults for " + messages[i] + " inputs");
			for (int n=1000; n<=10000000; n=n*10) {
				Integer[] a = null;
				switch (i) {
					case 0:
						a =  SortUtils.getRandomInput(n, n*100);
						break;
					case 1:
						a = SortUtils.getSortedInput(n);
						break;
					case 2: 
						a = SortUtils.getReverseSortedInput(n);
						break;
				}
				Integer[] b = Arrays.copyOf(a, a.length);
				
				long startTime = System.nanoTime();
				QuickSortRandomized.sort1(a);
				long endTime = System.nanoTime();
				System.out.format("Method1: n=%d, duration=%d%n", n, endTime-startTime);
				
				startTime = System.nanoTime();
				QuickSortRandomized.sort2(b);
				endTime = System.nanoTime();
				System.out.format("Method2: n=%d, duration=%d%n", n, endTime-startTime);
			}
		}
	}
	
	@Test
	void testSort2() {
		for (int i=0; i<3; i++) {
			System.out.println("\nResults for " + messages[i] + " inputs");
			for (int n=1000; n<=10000000; n=n*10) {
				Integer[] a = null;
				switch (i) {
					case 0:
						a =  SortUtils.getRandomInput(n, n*100);
						break;
					case 1:
						a = SortUtils.getSortedInput(n);
						break;
					case 2: 
						a = SortUtils.getReverseSortedInput(n);
						break;
				}
				Integer[] b = Arrays.copyOf(a, a.length);
				
				long startTime = System.nanoTime();
				QuickSortRandomized.sort2(b);
				long endTime = System.nanoTime();
				System.out.format("Method2: n=%d, duration=%d%n", n, endTime-startTime);
				
				startTime = System.nanoTime();
				QuickSortRandomized.sort1(a);
				endTime = System.nanoTime();
				System.out.format("Method1: n=%d, duration=%d%n", n, endTime-startTime);
			}
		}
	}
}
