#include <iostream>
#include <chrono>

using namespace std;
using namespace chrono;

void naive_algorithm(int* in, int* out, int size)
{
    auto start = high_resolution_clock::now();
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++)
            out[i * size + j] = in[j * size + i];
    }
    auto end = high_resolution_clock::now();
    cout << "naive algorithm time elapsed: " << duration_cast<milliseconds>(end - start).count() << "ms" << endl; //change to nanoseconds for small-sized matrices

    //checking the correctness of the algorithm on small-sized matrices:
    /*cout << "naive algorithm" << endl << "in:" << endl;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++)
            cout << in[i * size + j] << " ";
        cout << endl;
    }
    cout << endl << endl << "out:" << endl;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++)
            cout << out[i * size + j] << " ";
        cout << endl;
    }
    cout << endl << endl;*/
}

void recursion(int* in, int* out, int ind_rows1, int ind_rows2, int ind_columns1, int ind_columns2, int size)
//divide the side that is longer until the block is 4x4
{
    int length_rows =  ind_rows2 - ind_rows1;
    int length_columns = ind_columns2 - ind_columns1;
    if (length_rows == 4 && length_columns == 4) {
        for (int i = ind_rows1; i < ind_rows2; i++) {
            for (int j = ind_columns1; j < ind_columns2; j++) {
                out[i * size + j] = in[j * size + i];
            }
        }
    }
    else if (length_rows >= length_columns) {
        recursion(in, out, ind_rows1, ind_rows1 + length_rows / 2, ind_columns1, ind_columns2, size);
        recursion(in, out, ind_rows1 + length_rows / 2, ind_rows2, ind_columns1, ind_columns2, size);
    }
    else {
        recursion(in, out, ind_rows1, ind_rows2, ind_columns1, ind_columns1 + length_columns / 2, size);
        recursion(in, out, ind_rows1, ind_rows2, ind_columns1 + length_columns / 2 , ind_columns2, size);
    }
}

void cache_oblivious_algorithm(int* in, int* out, int size)
{
    auto start = high_resolution_clock::now();
    recursion(in, out, 0, size, 0, size, size);
    auto end = high_resolution_clock::now();
    cout << "cache-oblivious algorithm time elapsed: " << duration_cast<milliseconds>(end - start).count() << "ms" << endl; //change to nanoseconds for small-sized matrices
    //checking the correctness of the algorithm on small-sized matrices:
    /*cout << "cache-oblivious:" << endl << "in:" << endl;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++)
            cout << in[i * size + j] << " ";
        cout << endl;
    }
     cout << "out:" << endl;
    cout << endl << endl;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++)
            cout << out[i * size + j] << " ";
        cout << endl;
    }*/
}

int main() {
    int size = 8192;
    int* in = new int[size * size];
    int* out = new int[size * size];
    srand((unsigned int) time(nullptr));
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            in[i * size + j] = rand();
            out[i * size + j] = 0;
        }
    }

    naive_algorithm(in, out, size);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++)
            out[i * size + j] = 0;
    }

    cache_oblivious_algorithm(in, out, size);

    delete [] in;
    delete [] out;
    return 0;
}