double calmpq(int ip, int iq, BYTE **img, int width, int height) {

	double mpq = 0;	//pq�� moment �ʱ�ȭ

	int i, j;
	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
			if (img[j][i] > 128) { //������ ȭ�Ұ��� 128���� ū ���� ��ü�� �ǹ�
				mpq += pow((double)i, ip) * pow((double)j, iq);	//moment�� ����ϱ�
			}
		}
	}

	return (mpq);
}

double calmupq(int ip, int iq, BYTE **img, int width, int height, double xb, double yb) {

	double mupq = 0; //pq�� central moment �ʱ�ȭ

	int i, j;
	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
			if (img[j][i] > 128) { //������ ȭ�Ұ��� 128���� ū ���� ��ü�� �ǹ�
				mupq += pow((double)(i - xb), ip) * pow((double)(j - yb), iq);	//central moment�� ����ϱ�
			}
		}
	}
	return (mupq);
}

void Compute_Moment_Invariant_Function(BYTE **img, int width, int height, double *M2, double *M3)
{
	//���� ���� �ʱ�ȭ
	//xm: moment, xmu: central moment
	double xm00, xm01, xm10, xmu20, xmu02, xmu30, xmu03;
	double xmu11, xmu12, xmu21, r, r4, r6, xb, yb;

	//�ʿ��� moment ����ϱ�
	xm00 = calmpq(0, 0, img, width, height);
	xm01 = calmpq(0, 1, img, width, height);
	xm10 = calmpq(1, 0, img, width, height);

	//��ü�� �߽��� ��ǥ
	xb = xm10 / xm00;
	yb = xm01 / xm00;

	//�ʿ��� central moment ����ϱ�
	xmu20 = calmupq(2, 0, img, width, height, xb, yb) / xm00;
	xmu02 = calmupq(0, 2, img, width, height, xb, yb) / xm00;
	xmu11 = calmupq(1, 1, img, width, height, xb, yb) / xm00;
	xmu30 = calmupq(3, 0, img, width, height, xb, yb) / xm00;
	xmu03 = calmupq(0, 3, img, width, height, xb, yb) / xm00;
	xmu12 = calmupq(1, 2, img, width, height, xb, yb) / xm00;
	xmu21 = calmupq(2, 1, img, width, height, xb, yb) / xm00;

	r = sqrt(xmu20 + xmu02);
	r4 = pow((double)r, 4); //r�� 4���� ���
	r6 = r4 * r * r;		//r�� 6���� ���

	//moment invariant function m2, m3����ϱ�
	*M2 = (pow((double)(xmu20 - xmu02), 2) + 4.*pow((double)xmu11, 2)) / r4;
	*M3 = (pow((double)(xmu30 - 3.*xmu12), 2) + pow((double)(3.*xmu21 - xmu03), 2)) / r6;
}
void CHomework::OnTest()
{
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();
	CChildFrame *pChildFrame = (CChildFrame *)pMainFrame->MDIGetActive(NULL);
	CImageProcessingView *pView = (CImageProcessingView *)pChildFrame->GetActiveView();
	CDC *dc = pView->GetDC();

	int i, x0, y0, w, h, count;
	char fname[128];	//�Է� bmp���� �̸��� ������ ����
	BYTE **img;
	double M2, M3;		//Moment Invariant Function m2, m3
	double **feature;	//MIF �� ������ ����
	char msg[128];		//��ü �̹��� �Ʒ� MIF�� ǥ���� ������ ����
	char FIle_Name[12][15] = { "Class1_1.bmp", "Class1_2.bmp", "Class1_3.bmp", "Class2_1.bmp", "Class2_2.bmp", "Class2_3.bmp",
								"Class3_1.bmp", "Class3_2.bmp", "Class3_3.bmp", "Class4_1.bmp", "Class4_2.bmp", "Class4_3.bmp" };

	feature = dmatrix(12, 2); //��ü���� 12���� ���� ������ M2,M3 �� ������� �Ҵ��ϱ�
	x0 = 10;
	y0 = 10;
	count = 0;
	for (i = 0; i < 12; i++) {

		sprintf(fname, "c:\\%s", FIle_Name[i]);
		Display_BMP_File(fname, x0, y0);		//�Է� BMP �̹��� display�ϱ�	
		GetCurrentImageInfo(&w, &h, &x0, &y0);	//�Է� ���� ���� �б�
		img = cmatrix(h, w);		// BYTE** �ڷ��� �Ҵ��ϱ�
		GetCurrentImageGray(img);	// ȸ���� ���� �б�

		Compute_Moment_Invariant_Function(img, w, h, &M2, &M3);	//M2, M3�� ����ؼ� return

		sprintf(msg, "%9.3f, %9.3f   ", M2, M3);
		dc->TextOut(x0, y0 + h + 5, msg, strlen(msg));	//M2, M3 ����ϱ�
		free_cmatrix(img, h, w);	//BYTE** �Ҵ�����
		count++;

		feature[i][0] = M2; //i��° ������ M2�� �����ϴ� ����
		feature[i][1] = M3; //i��° ������ M3�� �����ϴ� ����

		//�̹��� ���� 3���� ���ٿ� ����ϱ�
		if (count == 3) {
			x0 = 10;
			y0 += 150;
			count = 0;
		}
		else {
			x0 += 190;
		}
	}

	//Display_Feature_Space(feature, 530, 10); //feature���� M2, M3������ ���. (530,10)��ġ�� feature map display�ϱ�
	free_dmatrix(feature, 12, 2);
	pView->ReleaseDC(dc);
}