all:
	nvcc -o parallel.exe parallel.cu
	del parallel.exp parallel.lib

parallel:
	nvcc -o parallel.exe parallel.cu
	del parallel.exp parallel.lib

sequential:

clean:
	del parallel.exe sequential.exe