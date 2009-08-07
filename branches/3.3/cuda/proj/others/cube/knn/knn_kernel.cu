#ifndef _KNN_KERNEL_H_
#define _KNN_KERNEL_H_

#include "config.h"

////////////////////////////////////////////////////////////////////////////////
//! �������Ӿ��벢��С�ھ���ƽ��С��radius_square�������������������
//! @param g_idata  input data in global memory
//! @param g_odata  output data in global memory
////////////////////////////////////////////////////////////////////////////////
__global__ void
testKernel( float* g_idata, float* g_odata) 
{
	const int step = gridDim.x * blockDim.x;					//����ѭ������
	int base_index = blockIdx.x * blockDim.x + threadIdx.x;		//�������ӵ����
	float value_sum;											//���������֮��
	float square;												//����֮������ƽ��
	float axis_diff;											//����֮������֮��

	unsigned int dim_i;
	
	float obj_data[particle_dimension];

	for(;base_index < particle_num; base_index += step)
	{
		value_sum = 0;

		//��globle memory���һ�����ӵ���Ϣ������obj_data��ȥ�����ڼ���õ����������ӵľ���
		for(int i=0; i<particle_dimension; i++)
			obj_data[i] = g_idata[base_index*data_dimension+i];
		
		//����share memoryʤ�ڴ�����е���Ϣ������ֻ�ܴ��thread_num���㣬��Ҫparticle_num/thread_num���̵߳���
		__shared__ float sdata[thread_num*data_dimension];

		for(int g=0;g<particle_num/thread_num;++g)
		{
			//��������Ϣ������share memory��ȥ
			for(int i=0; i<data_dimension; i++)
				sdata[threadIdx.x*data_dimension+i] = g_idata[data_dimension*thread_num*g+threadIdx.x*data_dimension+i];

			__syncthreads();

			for(int e=0;e<thread_num;++e)			
			{	
				square = 0;

				//������������������֮�����
				for(dim_i=0;dim_i<particle_dimension;++dim_i)
				{
					axis_diff = obj_data[dim_i]-sdata[e*4+dim_i];
					square += axis_diff * axis_diff;
				}

				//�ȽϾ���
				if(square < radius_square)
				{
					value_sum += sdata[e*(particle_dimension+1) + particle_dimension];//�������������
				}
			}
			__syncthreads();
		}

		g_odata[base_index]=value_sum;//����Ϣ�ŵ�Ŀ��globe memory���������ȥ
	}
}

#endif // #ifndef _TEMPLATE_KERNEL_H_
