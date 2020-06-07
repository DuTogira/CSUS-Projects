// Austin Smothers
// Modified public static <T extends Comparable<T>> void sort(T[] a)
// October 3, 2018

package edu.csus.csc130.fall2018.assignment2;

import java.util.Stack;

import org.junit.Assert;

public class QuickSortNonRecursive {
	public static <T extends Comparable<T>> void sort(T[] a) {
		// Check if the array has only 1 element.
		if(a.length == 1) {
			return;
		}
		// Array has more than 1 element. This code will work. Continue
		int low = 0;
		int high = a.length-1;
		int pivot = 0;
		Stack<Integer> s = new Stack<Integer>();
		while(true) {
			while(low <= high) {
				pivot = partition(a, low, high);
				s.push(pivot+1);
				s.push(high);
				high = pivot-1;
			}
			if(!s.isEmpty()) {
				high = s.pop();
				if(!s.isEmpty()) {
					low = s.pop();
				}
			}
			if(SortUtils.isNonDecreasingOrder(a)) {
				break;
			}
		}
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
