__kernel void mat_mul(
    __global const int* A,
    __global const int* B,
    __global int* C,
    const int N) {

    int row = get_global_id(0);
    int col = get_global_id(1);

    int sum = 0;
    for (int k = 0; k < N; ++k) {
        sum += A[row * N + k] * B[k * N + col];
    }

    C[row * N + col] = sum;
}
