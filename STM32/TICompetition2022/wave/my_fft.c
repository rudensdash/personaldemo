#include "my_fft.h"


uint32_t lBufInArray[NPT];
uint32_t lBufOutArray[NPT];
uint32_t lBufMagArray[NPT];

fft_info_t fft_inf;

#if defined(C_FFT_NOT_DSP) // 采用 C 语言算法而不是 DSP算法 如果没定义这个 宏就使用DSP 默认DSP

void FFT(double *pr,int n, double* amp_data)
{
	//static double amp_data[NPT*2];//定义输出幅值
	static double fr_data[NPT*2];//定义输出实部
	static double fi_data[NPT*2];//定义输出虚部
	static double pr_data[NPT];//缓存输入实部
	static double pi_data[NPT];//缓存输入虚部
	unsigned int k;//n=2^k;
	int i,j;
	int it,m,is,nv;
	double p,q,s,vr,vi,poddr,poddi;
//===============初始化================================//
	k = log10(n)/log10(2);//n=2^k;
	for(i=0;i<n;i++)
	{
		pr_data[i] = *(pr+i);//读入缓存
		pi_data[i] = 0;//读入缓存
	}
//========将pr[0]和pi[0]循环赋值给fr_data[]和fi_data[]==========//
  for (it=0; it<=n-1; it++)  
  { 
		m=it; 
		is=0;
		for(i=0; i<=k-1; i++)
    { 
			j=m/2; 
			is=2*is+(m-2*j); 
			m=j;
		}
    fr_data[it]=pr_data[is]; 
    fi_data[it]=pi_data[is];
  }
//==================================================//
	pr_data[0]=1.0; 
	pi_data[0]=0.0;
	p=6.283185306/(1.0*n);
	pr_data[1]=cos(p); //将w=e^-j2pi_data/n用欧拉公式表示
	pi_data[1]=-sin(p);
//================计算pr_data和pi_data========================//
  for (i=2; i<=n-1; i++)  
  { 
		p=pr_data[i-1]*pr_data[1]; 
		q=pi_data[i-1]*pi_data[1];
		s=(pr_data[i-1]+pi_data[i-1])*(pr_data[1]+pi_data[1]);
		pr_data[i]=p-q; pi_data[i]=s-p-q;
  }
//===================计算fr_data和fi_data=====================//
  for (it=0; it<=n-2; it=it+2)  
  { 
		vr=fr_data[it]; 
		vi=fi_data[it];
		fr_data[it]=vr+fr_data[it+1]; 
		fi_data[it]=vi+fi_data[it+1];
		fr_data[it+1]=vr-fr_data[it+1]; 
		fi_data[it+1]=vi-fi_data[it+1];
  }
//=================蝴蝶操作=========================//
	m=n/2; 
	nv=2;
  for (i=k-2; i>=0; i--)
  { 
		m=m/2; 
		nv=2*nv;
    for (it=0; it<=(m-1)*nv; it=it+nv)
		{
			for (j=0; j<=(nv/2)-1; j++)
			{ 
				p=pr_data[m*j]*fr_data[it+j+nv/2];
				q=pi_data[m*j]*fi_data[it+j+nv/2];
				s=pr_data[m*j]+pi_data[m*j];
				s=s*(fr_data[it+j+nv/2]+fi_data[it+j+nv/2]);
				poddr=p-q; 
				poddi=s-p-q;
				fr_data[it+j+nv/2]=fr_data[it+j]-poddr;
				fi_data[it+j+nv/2]=fi_data[it+j]-poddi;
				fr_data[it+j]=fr_data[it+j]+poddr;
				fi_data[it+j]=fi_data[it+j]+poddi;
			}
		}
  }
//=================计算幅值输出并存储==========================//
  for (i=0; i<=n-1; i++)
  {
		amp_data[i]=sqrt(fr_data[i]*fr_data[i]+fi_data[i]*fi_data[i]);  //幅值计算
  }

}

#else



/*
*********************************************************************************************************
*    函 数 名: PowerMag
*    功能说明: 求模值
*    形    参：_usFFTPoints  FFT点数   Output_Buffer FFT 输出数组  Mag_Buffer 存放幅值数组
*    返 回 值: 无
*********************************************************************************************************
*/
void PowerMag(void)        
{
	signed short lX,lY;
    float X,Y,Mag;
    unsigned short i;
    for(i=0; i<NPT/2; i++)
    {
        lX  = (lBufOutArray[i] << 16) >> 16;
        lY  = (lBufOutArray[i] >> 16);
        X = NPT * ((float)lX) / 32768;
        Y = NPT * ((float)lY) / 32768;
        Mag = sqrt(X * X + Y * Y) / NPT;
        if(i == 0)
            lBufMagArray[i] = (unsigned long)(Mag * 32768);
        else
            lBufMagArray[i] = (unsigned long)(Mag * 65536);
    }
}

#if 0
// 暂时不求相位
/*
*********************************************************************************************************
*    函 数 名: Power_Phase_Radians
*    功能说明: 求相位
*    形    参：_usFFTPoints  FFT点数， uiCmpValue 阀值
*    返 回 值: 无
*********************************************************************************************************
*/
void Power_Phase_Radians(uint16_t _usFFTPoints, uint32_t _uiCmpValue)        
{
    int16_t lX, lY;
    uint16_t i;
    float32_t phase;
    float32_t mag;
    
    
    for (i=0; i <_usFFTPoints; i++)
    {
        lX= (output[i]<<16)>>16;   //实部
        lY= (output[i] >> 16);    //虚部  
        
         phase = atan2(lY, lX);            // atan2求解的结果范围是(-pi, pi], 弧度制 
        arm_sqrt_f32((float32_t)(lX*lX+ lY*lY), &mag);   /* 求模 */
        
        if(_uiCmpValue > mag)
        {
            Phase[i] = 0;            
        }
        else
        {
            Phase[i] = phase* 180.0f/3.1415926f;  // 将求解的结果由弧度转换为角度 
        }
    }
}
#endif 

// DSP库 FFT 运算
void FFT(void){
	
	uint16_t i;
	uint16_t* p;
	if(modulate_flag == 0)  // AM
		p = y_input1;
	else if (modulate_flag == 1) // FM
		p = y_input2;
	for(i = 0; i < NPT; i++){
		lBufInArray[i] = p[i] << 16; // 实部
	}
	
	cr4_fft_1024_stm32(lBufOutArray, lBufInArray, NPT);
	PowerMag(); // 求幅值 存入 lBufMagArray
	
}




#endif

