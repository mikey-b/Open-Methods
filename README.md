This technique is an alternative to the Visitor Pattern, which has several downsides.
* It allocates memory. The visitor is a structure.
* It has a double indirection for each visit call.
* It complicates the inheritance hierarchy. The base class requires knowledge of all subclasses that are visited.

https://www.stroustrup.com/multimethods.pdf is a great paper discussing open methods and open multimethods. Along with potential implementation methods.

This code demonstrates Open Methods (Single virtual argument only).
