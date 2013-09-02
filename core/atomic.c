/*
 * atomic.c
 * Wolfrath/Kriewall, 2013
 *
 * Implementation of atomic funcitons
 */

#include "core.h"

inline int atomic_read(const Atomic_int* v){
	return (*(volatile int*)&(v)->value);
}

inline int xchg(volatile uint* addr,int val){

	int ret;

	asm volatile("lock; xchgl %0, %1" : 
				"+m" (*addr), "=a" (ret) :
				"1" (val) :
				"cc");

	return ret;
}

inline int atomic_set(Atomic_int* v, int i){

	return xchg((volatile int *)&(v)->value,i);
}

inline void atomic_add(Atomic_int* v, int i){
	
	asm volatile("lock; addl %1,%0"
				: "+m" (v->value)
				: "ir" (i));
}

inline void atomic_sub(Atomic_int* v,int i){

	asm volatile("lock; subl %1,%0"
				: "+m" (v->value)
				: "ir" (i));
}

inline void atomic_inc(Atomic_int* v){
	asm volatile("lock; incl %0" : "+m" (v->value));
}

inline void atomic_dec(Atomic_int* v){
	asm volatile("lock; decl %0" : "+m" (v->value));
}

