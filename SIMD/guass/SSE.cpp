#include<iostream>
#include<sys/time.h>
#include<time.h>
#include<pmmintrin.h>
#include<time.h>
#include<xmmintrin.h>




void gaussian_eli(float **mat, int N)
{
    for (int k = 0; k < N; k++) {
        float tmp =mat[k][k];
        // mat[k][k] = 1
        for (int j = k; j < N; j++){
            mat[k][j] = mat[k][j] / tmp;
        }
        
        for (int i = k + 1; i < N; i++){
            float tmp_2 = mat[i][k];
            for (int j = k + 1; j < N; j++){
                mat[i][j] = mat[i][j] - tmp_2 * mat[k][j];
            }
            mat[i][k] = 0;
        }
    }
    return ;
}



void gaussian_eli_SSE_no_45(float **mat , int N)
{
    __m128 t1, t2, t3, t4;

    for (int k = 0; k < N; k++) {

        float tmp =mat[k][k];
        // mat[k][k] = 1
        for (int j = k; j < N; j++){
            mat[k][j] = mat[k][j] / tmp;
        }

        // SSE for 8 - 10 lines
        for (int i = k + 1; i < N; i++) {
            float tmp_2[4] = { mat[i][k], mat[i][k], mat[i][k], mat[i][k] };
            t1 = _mm_loadu_ps(tmp_2);
            for (int j = N - 4; j >k;j -= 4) {
                t2 = _mm_loadu_ps(mat[i] + j);
                t3 = _mm_loadu_ps(mat[k] + j);
                // a[i,j] := a[i,j] -  a[i,k] * a[k.j]
                t4 = _mm_sub_ps(t2,_mm_mul_ps(t1, t3));
                _mm_storeu_ps(mat[i] + j, t4);
            }
            for (int j = k + 1; j % 4 !=(N % 4); j++){
                mat[i][j] = mat[i][j] - mat[i][k] * mat[k][j];
            }
            mat[i][k] = 0;
        }
    }

}

void gaussian_eli_SSE_no_alig(float **mat, int N)
{
    __m128 t1, t2, t3, t4;

    for (int k = 0; k < N; k++) {
        float tmp[4] = { mat[k][k], mat[k][k], mat[k][k], mat[k][k] };
        // SSE for 4-5 line
        t1 = _mm_loadu_ps(tmp);
        // not always align if start at k
        int j = k;
        for (; j <k; j += 4) {
            // take out 4 numbers
            t2 = _mm_loadu_ps(mat[k] + j);
            // div
            t3 = _mm_div_ps(t2, t1);
            // store back
            _mm_storeu_ps(mat[k] + j, t3);
        }
        // if (N - k + 1) % 4 != 0  , deal with left numbers
//        if (k % 4 != (N % 4)) {
//            for (int j = k; j % 4 != ( N% 4); j++) {
//                mat[k][j] = mat[k][j] / tmp[0];
//            }
//        }
        for (; j <N; j++) {
            mat[k][j] = mat[k][j] / tmp[0];
        }

        // SSE for 8 - 10 lines
        for (int i = k + 1; i < N; i++) {
            float tmp_2[4] = { mat[i][k], mat[i][k], mat[i][k], mat[i][k] };
            t1 = _mm_loadu_ps(tmp_2);
            int j = k;
            for (; j < N;j += 4) {
                t2 = _mm_loadu_ps(mat[i] + j);
                t3 = _mm_loadu_ps(mat[k] + j);
                // a[i,j] := a[i,j] -  a[i,k] * a[k.j]
                t4 = _mm_sub_ps(t2,_mm_mul_ps(t1, t3));
                _mm_storeu_ps(mat[i] + j, t4);
            }
            for (; j < N; j++){
                mat[i][j] = mat[i][j] - mat[i][k] * mat[k][j];
            }
            mat[i][k] = 0;
        }
    }
}





int main()
{
   // long long head, tail, freq;        // timers

    struct timeval t1, t2;

    //benchmark
    const int N = 512;
    float **matrix = new float* [N];
    for (int i = 0; i<N; i++){
        matrix[i] = new float[N];
    }
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            matrix[i][j] = rand() % 100;
        }
    }
        
    


//    for(int i=0; i<N; i++){
//        for(int j=0; j<N; j++){
//            cout<<matrix[i][j]<<" ";
//        }
//        cout<<endl;
//    }




    gettimeofday(&t1, NULL);


    gaussian_eli_SSE_no_45(matrix, N);

//        for(int i=0; i<N; i++){
//            for(int j=0; j<N; j++){
//                cout<<matrix[i][j]<<" ";
//            }
//            cout<<endl;
//        }
    



	// end time
    gettimeofday(&t2, NULL);

    double time = (t2.tv_sec - t1.tv_sec) * 1000000 + t2.tv_usec - t1.tv_usec;
    printf("%f\n",time);

    return 0;
}