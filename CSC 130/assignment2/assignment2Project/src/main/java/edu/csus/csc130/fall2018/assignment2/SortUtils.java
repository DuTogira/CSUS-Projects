package edu.csus.csc130.fall2018.assignment2;

import java.util.Random;

public class SortUtils {
	
	public static<T extends Comparable<T>> boolean isLessThan(T v, T w) {
		return v.compareTo(w) < 0;
	}

	public static<T> void swap(T[] a, int i, int j) {
		T t = a[i];
		a[i] = a[j];
		a[j] = t;
	}
	
	@SuppressWarnings("rawtypes")
	public static void shuffle(Comparable[] a) {
		Random random = new Random();
		int n = a.length;
		for (int i = 0; i < n; i++) { 
			// Exchange a[i] with random element in a[i..N-1]
			int r = i + random.nextInt(n-i);
			Comparable temp = a[i];
			a[i] = a[r];
			a[r] = temp;
		}
	}
	
	public static <T extends Comparable<T>> boolean isNonDecreasingOrder(T[] a) {
		for (int i=1; i<=a.length-1; i++) {
			if (a[i-1].compareTo(a[i])>0) {
				return false;
			}
		}
		return true;
	}
	
	public static Integer[] getRandomInput(int len, int limit) {
		Random random = new Random();
		Integer[] a = new Integer[len];
		for (int i=0; i<len; i++) {
			int n = random.nextInt(limit);
			a[i] = n;
		}	
		
		return a;
	}
	
	public static Integer[] getSortedInput(int len) {
		Random random = new Random();
		Integer[] a = new Integer[len];
		a[0] = 0;
		for (int i=1; i<len; i++) {
			a[i] = a[i-1] + random.nextInt(10);
		}		
		return a;
	}
	
	public static Integer[] getReverseSortedInput(int len) {
		Random random = new Random();
		Integer[] a = new Integer[len];
		a[0] = len;
		for (int i=1; i<len; i++) {
			a[i] = a[i-1] - random.nextInt(5);
		}		
		return a;
	}

}
