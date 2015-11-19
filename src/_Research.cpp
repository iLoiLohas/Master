#define _USE_MATH_DEFINES
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_statistics.h>


/********** �n�̌W���E���͏����i�s�ρj**********/
#define S0 (1./(2.*M_PI))	// whitenoise�̃p���[�X�y�N�g��
#define epsilon 0.3			// ����`���̋���
#define zeta 0.05			// �����萔

/********** �v�Z���� **********/
#define N 131072	// 131072,65536
#define dt 0.01		// ���ԍ��ݕ�
#define dx 0.1		// pdf�̉����̍��ݕ�
#define SET 100		// ���͂̕W�{��

double force[N];

// �����Q�N�b�^�Ŏg��
double f1(double force, double y1, double y2);
double f2(double force, double y1, double y2);


int main (int argc, char *argv[]) {

	// ���̓p�����[�^
	char *ends;
	double lambda	= strtod(argv[1], &ends);
	double beta2	= strtod(argv[2], &ends);
	double alpha	= strtod(argv[3], &ends);
	double sigma	= sqrt(2.*M_PI*S0 / dt);

	gsl_rng *r;
	gsl_rng *rp;

	FILE *t_force;
	FILE *t_x1;
	//	FILE *x_Gpdf;

	t_force = fopen("t_force.dat", "w");
	t_x1 = fopen("t_x1.dat", "w");
	//	x_Gpdf	= fopen("y2_Gpdf.dat","w");		//�K�E�X���z(�ψʉ�����pdf�Ɣ�r����B)

	r = gsl_rng_alloc(gsl_rng_default);
	rp = gsl_rng_alloc(gsl_rng_default);

	printf("The syatem is beta^2 = %lf , lambda = %lf  \n\n", beta2, lambda);
	printf("Creating a file of the pdf(.dat) ! \n\n");
	
	// ���͋��x
	double wSt = sqrt(alpha);
	double pSt = sqrt(1 - alpha);

	// �����Q�N�b�^�Ŏg��
	double DY1[4], DY2[4];
	double y1_buffer[N][SET], y2_buffer[N][SET];
	double y1min = 0., y1max = 0., y2max = 0., y2min = 0.;

	for (int sample_num = 0; sample_num < SET; sample_num++)
	{
		gsl_rng_set(r, time(NULL) + clock());
		gsl_rng_set(rp, time(NULL) + clock() + 1);

		/********** ���͂𐶐��i�z���C�g�m�C�Y�{�s�K���p���X�j **********/
		for (int sample_length = 0; sample_length < N; sample_length++)
		{
			// force[sample_length] = (gsl_ran_gaussian(r, sigma) + gsl_ran_bernoulli(r, dt*lambda)*gsl_ran_gaussian(rp, sqrt(beta2)))/dt;
			force[sample_length] = wSt*gsl_ran_gaussian(r, sigma) + pSt*gsl_ran_bernoulli(r, dt*lambda)*gsl_ran_gaussian(rp, sqrt(beta2)) / dt;
			// force[sample_length] = gsl_ran_gaussian(r, sigma);
			// force[sample_length] = gsl_ran_bernoulli(r, dt*lambda)*gsl_ran_gaussian(rp, sqrt(beta2))/dt;

			if (sample_num == 0)
				fprintf(t_force, "%lf %lf\n", sample_length*dt, force[sample_length]); //��U�̋L�^
		}

		/*********************** Runge-Kutta **********************************/
		// �����l
		int y1 = 0.;
		int y2 = 0.;

		////////////////////////// ��ŃJ�E���g�ϐ���ς���K�v���� ////////////////////////////////
		for (int i = 0; i<N; i++)
		{
			DY1[0] = dt*f1(force[i], y1, y2);
			DY2[0] = dt*f2(force[i], y1, y2);

			DY1[1] = dt*f1(force[i], y1 + DY1[0] / 2.0, y2 + DY2[0] / 2.0);
			DY2[1] = dt*f2(force[i], y1 + DY1[0] / 2.0, y2 + DY2[0] / 2.0);

			DY1[2] = dt*f1(force[i], y1 + DY1[1] / 2.0, y2 + DY2[1] / 2.0);
			DY2[2] = dt*f2(force[i], y1 + DY1[1] / 2.0, y2 + DY2[1] / 2.0);

			DY1[3] = dt*f1(force[i], y1 + DY1[2], y2 + DY2[2]);
			DY2[3] = dt*f2(force[i], y1 + DY1[2], y2 + DY2[2]);

			y1 = y1 + (DY1[0] + 2.*DY1[1] + 2.*DY1[2] + DY1[3]) / 6.0;
			y2 = y2 + (DY2[0] + 2.*DY2[1] + 2.*DY2[2] + DY2[3]) / 6.0;

			// �ψʂƑ��x�̍ŏ��l�E�ő�l������
			if (y1>y1max) y1max = y1;
			if (y1<y1min) y1min = y1;
			if (y2>y2max) y2max = y2;
			if (y2<y2min) y2min = y2;

			if (i == 0)
			{
				// �e���͂ɂ�����ψʂ̉������i�[
				y1_buffer[i][sample_num] = 0.;
				// �e���͂ɂ����鑬�x�̉������i�[
				y2_buffer[i][sample_num] = 0.;
			}
			else
			{
				// �e���͂ɂ�����ψʂ̉������i�[
				y1_buffer[i][sample_num] = y1;
				// �e���͂ɂ����鑬�x�̉������i�[
				y2_buffer[i][sample_num] = y2;
			}

			if (sample_num == 0)
			{
				// �����ψʂ̋L�^
				fprintf(t_x1, "%lf %lf\n", i*dt, y1);
			}
		}
	}

	/************************* �ψʂ�pdf���v�Z���� ***********************************/
	int n_dx, pdf_y1, integral_y1 = 0;
	double y1_pdf_buffer[3000][100];
	FILE *y1_pdf;
	y1_pdf = fopen("y1_pdf.dat", "w");

	for (int k = 0; k<SET; k++)
	{
		for (int j = 0; (y1min + j*dx) <= y1max; j++)
		{
			// ��dx�Ɋ܂܂���
			n_dx = 0;
			for (int i = 0; i<N; i++)
			{
				if ((y1_buffer[i][k] < (y1min + (j + 1)*dx)) && (y1_buffer[i][k] >= (y1min + j*dx))) n_dx++;
			}
			//			printf("n_dx�̒��g:%lf\n",n_dx);		
			y1_pdf_buffer[j][k] = (double)n_dx / N / dx;
		}
	}

	//	printf("Inside of the y1min:%lf, y1max:%lf\n",y1min, y1max);		

	for (int i = 0; (y1min + i*dx) <= y1max; i++)
	{
		pdf_y1 = 0.;
		for (int k = 0; k<SET; k++)
		{
			pdf_y1 += y1_pdf_buffer[i][k];
			// printf("y1_pdf_buffer�̒��g:%lf\n",y1_pdf_buffer);
		}
		pdf_y1 = (double)pdf_y1 / SET;
		integral_y1 += pdf_y1*dx;
		// �����̊m�����x�֐��̋L�^
		fprintf(y1_pdf, "%lf %lf\n", (y1min + i*dx), pdf_y1);
	}

	// pdf�̑S�ϕ��l
	printf("integral of pdf_y1 = %lf\n", integral_y1);

	fclose(y1_pdf);

	/************************* ���x��pdf���v�Z���� ***********************************/
	//	int n_dx, pdf_y2, integral_y2 = 0;
	//	double y2_pdf_buffer[3000][100];
	//	FILE *y2_pdf;
	//	y2_pdf	= fopen("y2_pdf.dat","w");
	//	
	//	for(int k=0; k<SET; k++)  
	//	{
	//		for(int j=0; (y2min+j*dx) <= y2max; j++ )
	//		{
	//			// ��dx�Ɋ܂܂���
	//			n_dx = 0;
	//			for(i=0; i<N ; i++ )
	//			{
	//				if((y2_buffer[i][k] < (y2min+(j+1)*dx)) && (y2_buffer[i][k] >= (y2min+j*dx))) n_dx++;
	//			}
	//			y2_pdf_buffer[j][k] = (double)n_dx/N/dx;
	//		}
	//	}
	//
	//	for(i=0; (y2min+i*dx) <= y2max; i++)
	//	{
	//		pdf_y2 = 0.;
	//		for(k=0; k<SET; k++)
	//		{
	//			pdf_y2 += y2_pdf_buffer[i][k];
	//		}
	//		pdf_y2 = (double)pdf_y2/SET;
	//		integral_y2 += pdf_y2*dx;
	//		// �����̊m�����x�֐��̋L�^
	//		fprintf(y2_pdf, "%lf %lf\n", (y2min+i*dx), pdf_y2);
	//	}
	//
	//	// pdf�̑S�ϕ��l
	//	printf("integral of pdf_y2 = %lf\n",integral_y2);
	//
	//	fclose(y2_pdf);
	/************************** �V�~�����[�V�������ʂ̕ψʕ��U���v�Z *******************************/
	//	int scan_file, var_y1, mo4_y1, first_row, second_row;
	//	FILE *y1_var;
	//	y1_var  = fopen("sim_y1_var.dat", "w");
	//	y1_pdf = fopen("y1_pdf.dat", "r");
	//	
	//	var_y1 = 0.;
	//	mo4_y1 = 0.;
	//	while((scan_file = fscanf(y1_pdf, "%lf %lf", &first_row, &second_row)) != EOF)
	//	{
	//		var_y1 += pow(first_row,2)*second_row;
	//		mo4_y1 += pow(first_row,4)*second_row;
	//	}
	//	
	//	fprintf(y1_var, "%lf", mo4_y1/pow(var_y1,2));
	//
	//	fclose(y1_pdf);
	//	fclose(y1_var);
	/************************** �V�~�����[�V�������ʂ̑��x���U���v�Z *******************************/
	//	int scan_file, var_y2, first_row, second_row;
	//	int var_y2;
	//	FILE *y2_var;
	//	y2_var  = fopen("sim_y2_var.dat", "w");
	//	y1_pdf = fopen("y2_pdf.dat", "r");
	//
	//	var_y2 = 0.;
	//	while((scan_file = fscanf(y2_pdf, "%lf %lf", &first_row, &second_row)) != EOF)
	//	{
	//		var_y2 += pow(first_row,2)*second_row;
	//	}
	//	
	//	fprintf(y2_var, "%lf", var_y2);
	//	fclose(y2_var);
	//	fclose(y2_pdf);

	/////////////////////////////////////////////////////////////
	fclose(t_force);
	fclose(t_x1);
	//	fclose(x_Gpdf);

	printf("\nIt is done !\n");
	return 0;
}

double f1(double force, double y1, double y2)
{
	return y2;
}

double f2(double force, double y1, double y2)
{
	return force - 2 * zeta*y2 - y1 - epsilon*y1*y1*y1;
}

