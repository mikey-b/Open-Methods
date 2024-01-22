#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

enum type_t {
    class_A, class_B, class_C
};

struct I {
    enum type_t typeId;
};

struct A {
    struct I;
};

struct A init_a() {
    return (struct A){ .typeId = class_A };
}

struct B {
    struct I;
};

struct B init_b() {
    return (struct B){ .typeId = class_B };
}

struct C {
    struct I;
    struct B* x;
};

struct C init_C() {
    return (struct C){ .typeId = class_C };
}

// -----------------------------------

int doIt_A(struct A*);
int doIt_B(struct B*);
int doIt_C(struct C*);
int doIt_I(struct I*);

#define doIt(x) _Generic(x, struct A*: doIt_A, struct B*: doIt_B, struct C*: doIt_C, struct I*: doIt_I)(x)


int doIt_A(struct A* self) {
    puts("A\n");
    return 1;
}

int doIt_B(struct B* self) {
    puts("B\n");
    return 2; 
}

int doIt_C(struct C* self) {
    puts("C\n");
    return doIt(self->x);
}

static int (*const dispatch[])(struct I*) = {
    (int (*)(struct I*))doIt_A,
    (int (*)(struct I*))doIt_B,
    (int (*)(struct I*))doIt_C
};

int doIt_I(struct I* self) {
    int (*res)(struct I*) = dispatch[self->typeId];
	assert(res != 0);
	return res(self);
}

int main() {
    struct I* x = (struct I*)malloc(sizeof(struct C));
    *(struct C*)x = init_C();
    int y = doIt(x);
    printf("y = %d\n", y);
}
