extrude:
	gcc -Wall extrude.c stl_util.c stl_io.c -o extrude

bench:
	gcc -Wall bench.c stl_util.c stl_io.c -o bench

convert:
	gcc -Wall convert.c stl_util.c stl_io.c -o convert

move:
	gcc -Wall move.c stl_util.c stl_io.c -o move

clean:
	rm -f bench extrude convert *.o
