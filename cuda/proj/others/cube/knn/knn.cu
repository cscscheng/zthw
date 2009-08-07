// includes, system
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// includes, project
#include <cutil_inline.h>

////////////////////////////////////////////////////////////////////////////////
// declaration, forward
void runTest( int argc, char** argv);
void randomInit(float* data, int size);

extern "C"
void computeGold( float* reference, float* idata, const unsigned int len, const unsigned int dimension);
bool hComparef( const float* reference, const float* data, const unsigned int len);

// includes, kernels
#include <knn_kernel.cu>

////////////////////////////////////////////////////////////////////////////////
// Program main
////////////////////////////////////////////////////////////////////////////////
int
main( int argc, char** argv) 
{
	printf("particle num is:%d \n",particle_num);
    runTest( argc, argv);

    cutilExit(argc, argv);
}

////////////////////////////////////////////////////////////////////////////////
//runTest
////////////////////////////////////////////////////////////////////////////////
void
runTest( int argc, char** argv) 
{
	// use command-line specified CUDA device, otherwise use device with highest Gflops/s
	if( cutCheckCmdLineFlag(argc, (const char**)argv, "device") )
		cutilDeviceInit(argc, argv);
	else
		cudaSetDevice( cutGetMaxGflopsDeviceId() );

	srand((unsigned int)time(0));

	unsigned int i_data_size = particle_num * (particle_dimension + 1);		//�������������������С �� ��������������ά����1��
	unsigned int i_mem_size = sizeof( float) * i_data_size;					//���ڴ洢������������ռ��ֽ���				
	unsigned int o_mem_size = sizeof( float) * particle_num;				//���ڴ洢�������ռ��ֽ���

    unsigned int timer = 0;
    cutilCheckError( cutCreateTimer( &timer));
    cutilCheckError( cutStartTimer( timer));

    // ���� host memory
    float* h_idata = (float*) malloc( i_mem_size);		//�������ݿռ���h_idata��
    
    randomInit(h_idata, i_data_size);					//�����������ݷ���h_idata


    // ����device memory
    float* d_idata;
    cutilSafeCall( cudaMalloc( (void**) &d_idata, i_mem_size));

    // ���ڴ��е�h_idata�������Դ��е�d_idata��ȥ
    cutilSafeCall( cudaMemcpy( d_idata, h_idata, i_mem_size,
                                cudaMemcpyHostToDevice) );

    // ���� device memory ���ڴ洢���
    float* d_odata;
    cutilSafeCall( cudaMalloc( (void**) &d_odata, o_mem_size));

    // �����ں�Grid��Block����һά��
    dim3  grid( block_num, 1, 1);
    dim3  threads( thread_num, 1, 1);

    // ִ��kernel
    testKernel<<< grid, threads >>>( d_idata, d_odata);

    // check if kernel execution generated and error
    cutilCheckMsg("Kernel execution failed");

    // �����ڴ�ռ�洢device�ϼ���Ľ��
    float* h_odata = (float*) malloc( o_mem_size);
    
	// ��device�ϵĽ��������host�ϵ�h_odata������ȥ
    cutilSafeCall( cudaMemcpy( h_odata, d_odata, o_mem_size,
                                cudaMemcpyDeviceToHost) );


    cutilCheckError( cutStopTimer( timer));
    printf( "Processing time in GPU: %f (ms)\n", cutGetTimerValue( timer));

	/////////////////////////////////////////////////////////////////////////////
	//////CPU�ϵļ���
	////////////////////////////////////////////////////////////////////////////
    cutilCheckError( cutStartTimer( timer));
	
	// �����ڴ�ռ����ڴ洢CPU�ϵļ�����
    float* reference = (float*) malloc( o_mem_size);

    computeGold( reference, h_idata, particle_num, particle_dimension);

	cutilCheckError( cutStopTimer( timer));
    printf( "Processing time in CPU: %f (ms)\n", cutGetTimerValue( timer));
    
	cutilCheckError( cutDeleteTimer( timer));

    bool res = hComparef( reference, h_odata, particle_num);
    printf( "\n GPU����CPU�ϲ��Խ���Ƿ�ͨ���� %s\n", (1 == res) ? "PASSED" : "FAILED");

    // �����ڴ�ռ�
    free( h_idata);
    free( h_odata);
    free( reference);
    cutilSafeCall(cudaFree(d_idata));
    cutilSafeCall(cudaFree(d_odata));

    cudaThreadExit();
}

//����������ݺ���
void randomInit(float* data, int size)
{
    for (int i = 0; i < size; ++i)
      data[i] = rand() / (float)RAND_MAX;
}

//�Ƚϲ��Խ��������������
bool hComparef( const float* reference, const float* data, const unsigned int len)
{        
        bool result = true;
		int error_num;
		error_num = 0;

        for( unsigned int i = 0; i < len; ++i) {

            float diff = reference[i] - data[i];
            bool comp = (diff <= 0.0f) && (diff >= -0.0f);
            result &= comp;

			if( ! comp) 
				error_num++;
        }
		printf("ERROR_NUM = %d ,\n", error_num);
        return (result) ? true : false;
}