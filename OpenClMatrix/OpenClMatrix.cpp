#include <CL/cl2.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <cassert>
#include <random>
#include <ctime>
#include <chrono>

int main() {
    int N;
    std::cout << "Write format matrix N: ";
    std::cin >> N;


    std::mt19937 rng(static_cast<unsigned>(time(nullptr)));
    std::uniform_int_distribution<int> dist(1, 10);


    std::vector<int> A(N * N);
    std::vector<int> B(N * N);
    std::vector<int> C(N * N);

    for (int i = 0; i < N * N; ++i) {
        A[i] = dist(rng);
        B[i] = dist(rng);
    }

 
    std::cout << "\nMatrix A:\n";
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j)
            std::cout << A[i * N + j] << "\t";
        std::cout << "\n";
    }


    std::cout << "\nMatrix B:\n";
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j)
            std::cout << B[i * N + j] << "\t";
        std::cout << "\n";
    }

    std::ifstream sourceFile("matrix_mul.cl");
    std::string kernelSource((std::istreambuf_iterator<char>(sourceFile)),
        std::istreambuf_iterator<char>());

    cl::Program::Sources sources = {
        {kernelSource.c_str(), kernelSource.length()}
    };


    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    cl_context_properties props[] = { CL_CONTEXT_PLATFORM, (cl_context_properties)(platforms[0])(), 0 };

    cl::Context context(CL_DEVICE_TYPE_GPU, props);
    auto devices = context.getInfo<CL_CONTEXT_DEVICES>();
    cl::CommandQueue queue(context, devices[0]);


    cl::Buffer bufferA(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int) * A.size(), A.data());
    cl::Buffer bufferB(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int) * B.size(), B.data());
    cl::Buffer bufferC(context, CL_MEM_WRITE_ONLY, sizeof(int) * C.size());

 
    cl::Program program(context, sources);
    program.build(devices);
    cl::Kernel kernel(program, "mat_mul");


    kernel.setArg(0, bufferA);
    kernel.setArg(1, bufferB);
    kernel.setArg(2, bufferC);
    kernel.setArg(3, N);

    //текущее
    auto start = std::chrono::high_resolution_clock::now();

    cl::NDRange global(N, N);
    queue.enqueueNDRangeKernel(kernel, cl::NullRange, global, cl::NullRange);
    queue.enqueueReadBuffer(bufferC, CL_TRUE, 0, sizeof(int) * C.size(), C.data());
    //текущее
    auto end = std::chrono::high_resolution_clock::now();
    // конец минус начало
    std::chrono::duration<double, std::milli> duration = end - start;


    std::cout << "\nResult of Multiply matrix A * B:\n";
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j)
            std::cout << C[i * N + j] << "\t";
        std::cout << std::endl;
    }

    std::cout << "\nTime to Multiply: "
        << duration.count() << " ms\n";

    return 0;
}
