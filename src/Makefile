SRCS = cg.c decl.c expr.c gen.c main.c misc.c scan.c stmt.c sym.c tree.c
SRCN = cgn.c decl.c expr.c gen.c main.c misc.c scan.c stmt.c sym.c tree.c
RCS  = cg.c decl.c expr.c gen.c misc.c scan.c stmt.c sym.c tree.c

hulacomp: ${RCS}
	cc -c ${RCS}
	ar rcs hulacomp.a *.o;

clean:
	rm -f *.out *.o *.s *.a out

test1: hulacomp main.c
	cc -o test1.out main.c -L. hulacomp.a
	./test1.out input01
	cc -o out out.s
	./out

test2: hulacomp main.c
	cc -o test2.out main.c -L. hulacomp.a
	./test2.out input02
	cc -o out out.s
	./out

test3: hulacomp main.c 
	cc -o test3.out main.c -L. hulacomp.a
	./test3.out input03
	cc -o out out.s
	./out

testall: 
	make -s hulacomp

	cc -o test1.out main.c -L. hulacomp.a
	./test1.out input01
	cc -o out out.s
	./out

	./test1.out input02
	cc -o out out.s
	./out

	./test1.out input03
	cc -o out out.s
	./out
