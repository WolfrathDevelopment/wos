/*
 * atomic.c
 * Wolfrath/Kriewall, 2013
 *
 * Implementation of atomic funcitons
 */

#include "core.h"

inline int atomic_read(const struct w_atomic* v){
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

inline int atomic_set(struct w_atomic* v, int i){

	return xchg((volatile int *)&(v)->value,i);
}

inline void atomic_add(struct w_atomic* v, int i){
	
	asm volatile("lock; addl %1,%0"
				: "+m" (v->value)
				: "ir" (i));
}

inline void atomic_sub(struct w_atomic* v,int i){

	asm volatile("lock; subl %1,%0"
				: "+m" (v->value)
				: "ir" (i));
}

inline void atomic_inc(struct w_atomic* v){
	asm volatile("lock; incl %0" : "+m" (v->value));
}

inline void atomic_dec(struct w_atomic* v){
	asm volatile("lock; decl %0" : "+m" (v->value));
}

