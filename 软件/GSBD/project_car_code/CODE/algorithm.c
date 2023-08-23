

#include "algorithm.h"

 
#define MAXN 7
typedef struct {							//n*m����
	int m,n;
	double data[MAXN][MAXN];
}mat;
 
mat CreateMatrix(int p,int q);
double arry2matrix(mat *a,mat *b,mat *r,double *x,double *y,double *beta,int m,int n);
mat trans(mat a);
mat mul(mat a,mat b);
double *SolveMatrix(mat c,mat d);
void arryans(double *beta,mat c,mat d);
 
int gas_new(){
	double x[]={0.038, 0.194, 0.425, 0.626, 1.253, 2.500, 3.740};
	double y[]={0.050, 0.127, 0.094, 0.2122, 0.2729, 0.2665, 0.3317};
	mat r = {0,0,0};
	mat a = {0,0,0};
	mat b = {0,0,0};
	mat c = {0,0,0};
	mat d = {0,0,0};
	double beta[2] = {0.9,0.2};
	double rss = 0;
	int T;
 
	for(T=0;T<10;++T){
		rss = arry2matrix(&a,&b,&r,x,y,beta,7,2);
		//printf("%d %f %f %f\n",T,beta[0],beta[1],rss);
		c = mul(b,a);
		d = mul(b,r);
		arryans(beta,c,d);
	}
	while(1){}
	//system("pause");
}
 
 
// Input x[],y[],m,n
// Output a,b,r[]
double arry2matrix(mat *a,mat *b,mat *r,double *x,double *y,double *beta,int m,int n){
	int i;
	double rss = 0;
	(*a).m = m;		// �ſ˱Ⱦ���a
	(*a).n = n;
	(*b).m = n;		// a��ת�þ���b
	(*b).n = m;
	(*r).m = m;
	(*r).n = 1;
	
	for(i=0;i<m;++i){
		(*a).data[i][0] = -x[i]/(beta[1]+x[i]);
		(*a).data[i][1] = beta[0]*x[i]/(beta[1]+x[i])/(beta[1]+x[i]);
		(*b).data[0][i] = (*a).data[i][0];
		(*b).data[1][i] = (*a).data[i][1];
		(*r).data[i][0] = y[i]-beta[0]*x[i]/(beta[1]+x[i]);
		rss += (*r).data[i][0]*(*r).data[i][0];  
	}
	return rss;
}
 
/********************************************************************
*Fuction: 
*Input��  a,b
*Output:  c
********************************************************************/
mat mul(mat a,mat b){
	int i,j,k;
	mat c = {0,0,0};
	c.m = a.m;
	c.n = b.n;
	for(i=0;i<c.m;++i){
		for(j=0;j<c.n;++j){
			c.data[i][j] = 0;
			for(k=0;k<a.n;k++){
				c.data[i][j] += a.data[i][k]*b.data[k][j];
			}
		}
	}
	return c;
}
 
/********************************************************************
*Fuction: 
*Input��  c,d,beta
*Output:  new_beta
********************************************************************/
void arryans(double *beta,mat c,mat d){					//��һ��n�׵����Է�����
    int i, j, k, mi;
    double mx, tmp;
	double beta_tmp[2];
	beta_tmp[0] = beta[0];
	beta_tmp[1] = beta[1];
    for (i = 0; i < c.m-1; i++) {
		mi = i;
		mx = fabs(c.data[i][i]);
        for (j = i + 1; j < c.m; j++)			//����Ԫ��
            if (fabs(c.data[j][i]) > mx) {
                mi = j;							//��¼����������һ���о���ֵ���ֵ���к� 
                mx = fabs(c.data[j][i]);		//��¼����������һ���о���ֵ���ֵ 
            }
        if (i < mi) {							//��������
            tmp = d.data[i][0];
            d.data[i][0] = d.data[mi][0];
            d.data[mi][0] = tmp;
            for (j = i; j < c.m; j++) {
                tmp = c.data[i][j];
                c.data[i][j] = c.data[mi][j];
                c.data[mi][j] = tmp;
            }
        }
		//---��˹��Ԫ---//
        for (j = i + 1; j < c.m; j++) {				//��i�У���i+1��i+2������n-1��n��
            tmp = -c.data[j][i] / c.data[i][i];		//��j�г��Ե�i��
            d.data[j][0] += d.data[i][0] * tmp;
            for (k = i; k < c.m; k++)				//����i�е�tmp���ӵ���j��
                c.data[j][k] += c.data[i][k] * tmp;		
        }
    }
    beta[c.m - 1] = d.data[c.m - 1][0] / c.data[c.m - 1][c.m - 1];		//��ⷽ��
    for (i = c.m - 2; i >= 0; i--) {
        beta[i] = d.data[i][0];
        for (j = i + 1; j < c.m; j++)
            beta[i] -= c.data[i][j] * beta[j];
        beta[i] /= c.data[i][i];
    }
	beta[0] = beta_tmp[0]-beta[0];			
	beta[1] = beta_tmp[1]-beta[1];
}
 
 