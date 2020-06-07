package edu.csus.csc130.fall2018.assignment2;

import java.util.Random;

public class QuickSortRandomized {
	private static Random random = new Random();
	
	// Method 1: shuffle the input array before doing quick sort
	public static <T extends Comparable<T>> void sort1(T[] a) {
		if (a!=null && a.length>1) {
			SortUtils.shuffle(a);
			sort1(a, 0, a.length-1);
		}
	}
	
	private static <T extends Comparable<T>> void sort1(T[] a, int lo, int hi) {
		if (hi <= lo) {
			return;
		}
		int j = partition(a, lo, hi); // Partition
		sort1(a, lo, j - 1); // Sort left part a[lo .. j-1].
		sort1(a, j + 1, hi); // Sort right part a[j+1 .. hi]
	}
	
	// Method 2: randomly pick a pivot element during quick sort
	public static <T extends Comparable<T>> void sort2(T[] a) {
		if (a!=null && a.length>1) {
			sort2(a, 0, a.length-1);
		}
	}
	
	private static <T extends Comparable<T>> void sort2(T[] a, int lo, int hi) {
		if (hi <= lo) {
			return;
		}
		
		// randomly pick a pivot element
		int pos = lo + random.nextInt(hi-lo + 1);
		SortUtils.swap(a,  lo, pos);
		
		int j = partition(a, lo, hi); // Partition
		sort2(a, lo, j - 1); // Sort left part a[lo .. j-1].
		sort2(a, j + 1, hi); // Sort right part a[j+1 .. hi]
	}
	
	// Partition into a[lo..j-1], a[j], a[j+1..hi]
	public static <T extends Comparable<T>> int partition(T[] a, int lo, int hi) { 
		int i = lo, j = hi + 1; // left and right scan indices
		T v = a[lo]; // the pivot
		while (true) {
			while (SortUtils.isLessThan(a[++i], v)) {//++i is evaluated to i+1 
				if (i == hi) {
					break;
				}
			}
			
			while (SortUtils.isLessThan(v, a[--j])) {//--j is evaluated to j-1
				if (j == lo) {
					break;
				}
			}
			
			if (i >= j) {
				break;
			}
			
			SortUtils.swap(a, i, j);
		}
		SortUtils.swap(a, lo, j); // Put v = a[j] into position

		return j; // with a[lo..j-1] <= a[j] <= a[j+1..hi]
	}

}
