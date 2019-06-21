#include<iostream>
#include<sys/time.h>
#include<time.h>

typedef int int32x4 __attribute((vector_size(16)));
typedef float f32x4 __attribute((vector_size(16)));

static inline int32x4 splat(int v0, int v1, int v2, int v3) {
    return (int32x4) {
        v0, v1, v2, v3
    };
}



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

static inline f32x4 my_f_load(float* pos) {
    return (f32x4) {
        *pos, *(pos + 1), *(pos + 2), *(pos + 3)
    };
}

static inline void my_f_store(float *pos, f32x4 vec) {
    *pos = vec[0];
    *(pos + 1) = vec[1];
    *(pos + 2) = vec[2];
    *(pos + 3) = vec[3];
    return ;
}


void gaussian_eli_SSE_no_45(float **mat , int N)
{
    f32x4 t1, t2, t3, t4;

    for (int k = 0; k < N; k++) {

        float tmp =mat[k][k];
        // mat[k][k] = 1
        for (int j = k; j < N; j++){
            mat[k][j] = mat[k][j] / tmp;
        }

        // SSE for 8 - 10 lines
        for (int i = k + 1; i < N; i++) {
            float tmp_2[4] = { mat[i][k], mat[i][k], mat[i][k], mat[i][k] };
            t1 = my_f_load(tmp_2);
            for (int j = N - 4; j >k;j -= 4) {
                t2 = my_f_load(mat[i] + j);
                t3 = my_f_load(mat[k] + j);
                // a[i,j] := a[i,j] -  a[i,k] * a[k.j]
                // t4 = _mm_sub_ps(t2,_mm_mul_ps(t1, t3));
                t4 = t2 - t1 * t3;

                my_f_store(mat[i] + j, t4);
            }
            for (int j = k + 1; j % 4 !=(N % 4); j++){
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


    //gaussian_eli_SSE_no_45(matrix, N);

    gaussian_eli(matrix, N);

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