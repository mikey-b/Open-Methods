#include <cstdio>
#include <cassert>
enum typeT { classA, classB, classC };

class I {
 public:
    typeT const typeId;
};

class A: public I {
 public:
    constexpr A(): I(classA) { }
};

class B: public I {
 public:
    constexpr B(): I(classB) { }
};

class C: public I {
 public:
	// Change to I* x; to see inlining of the dispatch code into doIt(C*)
	B* x;               
	constexpr C(): I(classC) {
		x = new B();
	}
};

int doIt(A* self);
int doIt(B* self);
int doIt(C* self);

template<class Resolve, class As>
static As select_overload(Resolve fnc) {
#ifndef NDEBUG
	// Sanity check on the dispatch table correctness -
	{ A item{}; auto ptr = &item; assert(ptr == static_cast<I*>(ptr)); }
	{ B item{}; auto ptr = &item; assert(ptr == static_cast<I*>(ptr)); }
	{ C item{}; auto ptr = &item; assert(ptr == static_cast<I*>(ptr)); }        
#endif
	// static_cast selects the correct overload, 
	// reinterpret_cast stores the address into the table
	return reinterpret_cast<As>( static_cast<Resolve>(fnc)); 
}

static int (*const dispatch[])(I*) = {
    select_overload<int(*)(A*), int(*)(I*)>(doIt),
    select_overload<int(*)(B*), int(*)(I*)>(doIt),
    select_overload<int(*)(C*), int(*)(I*)>(doIt)
};

int doIt(I* self) {
	assert(self->typeId < (sizeof(dispatch) / sizeof(dispatch[0])));
	auto res = dispatch[self->typeId];
	assert(res != nullptr);
	return res(self);
}

int doIt(A* self) {
    puts("A\n");
    return 1;
}
int doIt(B* self) {
    puts("B\n");
    return 2;    
}
int doIt(C* self) {
    puts("C\n");
    doIt(self->x);
    return 3;
}

int main() {
    I* x = new C();
    auto y = doIt(x);
    printf("y = %d\n", y);
}