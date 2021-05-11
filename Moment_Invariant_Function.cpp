double calmpq(int ip, int iq, BYTE **img, int width, int height) {

	double mpq = 0;	//pq차 moment 초기화

	int i, j;
	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
			if (img[j][i] > 128) { //영상의 화소값이 128보다 큰 경우는 물체를 의미
				mpq += pow((double)i, ip) * pow((double)j, iq);	//moment값 계산하기
			}
		}
	}

	return (mpq);
}

double calmupq(int ip, int iq, BYTE **img, int width, int height, double xb, double yb) {

	double mupq = 0; //pq차 central moment 초기화

	int i, j;
	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
			if (img[j][i] > 128) { //영상의 화소값이 128보다 큰 경우는 물체를 의미
				mupq += pow((double)(i - xb), ip) * pow((double)(j - yb), iq);	//central moment값 계산하기
			}
		}
	}
	return (mupq);
}

void Compute_Moment_Invariant_Function(BYTE **img, int width, int height, double *M2, double *M3)
{
	//각종 변수 초기화
	//xm: moment, xmu: central moment
	double xm00, xm01, xm10, xmu20, xmu02, xmu30, xmu03;
	double xmu11, xmu12, xmu21, r, r4, r6, xb, yb;

	//필요한 moment 계산하기
	xm00 = calmpq(0, 0, img, width, height);
	xm01 = calmpq(0, 1, img, width, height);
	xm10 = calmpq(1, 0, img, width, height);

	//물체의 중심점 좌표
	xb = xm10 / xm00;
	yb = xm01 / xm00;

	//필요한 central moment 계산하기
	xmu20 = calmupq(2, 0, img, width, height, xb, yb) / xm00;
	xmu02 = calmupq(0, 2, img, width, height, xb, yb) / xm00;
	xmu11 = calmupq(1, 1, img, width, height, xb, yb) / xm00;
	xmu30 = calmupq(3, 0, img, width, height, xb, yb) / xm00;
	xmu03 = calmupq(0, 3, img, width, height, xb, yb) / xm00;
	xmu12 = calmupq(1, 2, img, width, height, xb, yb) / xm00;
	xmu21 = calmupq(2, 1, img, width, height, xb, yb) / xm00;

	r = sqrt(xmu20 + xmu02);
	r4 = pow((double)r, 4); //r의 4제곱 계산
	r6 = r4 * r * r;		//r의 6제곱 계산

	//moment invariant function m2, m3계산하기
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
	char fname[128];	//입력 bmp파일 이름을 저장할 공간
	BYTE **img;
	double M2, M3;		//Moment Invariant Function m2, m3
	double **feature;	//MIF 값 저정할 공간
	char msg[128];		//물체 이미지 아래 MIF를 표시할 저장할 공간
	char FIle_Name[12][15] = { "Class1_1.bmp", "Class1_2.bmp", "Class1_3.bmp", "Class2_1.bmp", "Class2_2.bmp", "Class2_3.bmp",
								"Class3_1.bmp", "Class3_2.bmp", "Class3_3.bmp", "Class4_1.bmp", "Class4_2.bmp", "Class4_3.bmp" };

	feature = dmatrix(12, 2); //물체사진 12개에 대한 각각의 M2,M3 값 저장공간 할당하기
	x0 = 10;
	y0 = 10;
	count = 0;
	for (i = 0; i < 12; i++) {

		sprintf(fname, "c:\\%s", FIle_Name[i]);
		Display_BMP_File(fname, x0, y0);		//입력 BMP 이미지 display하기	
		GetCurrentImageInfo(&w, &h, &x0, &y0);	//입력 영상 정보 읽기
		img = cmatrix(h, w);		// BYTE** 자료형 할당하기
		GetCurrentImageGray(img);	// 회색조 영상 읽기

		Compute_Moment_Invariant_Function(img, w, h, &M2, &M3);	//M2, M3를 계산해서 return

		sprintf(msg, "%9.3f, %9.3f   ", M2, M3);
		dc->TextOut(x0, y0 + h + 5, msg, strlen(msg));	//M2, M3 출력하기
		free_cmatrix(img, h, w);	//BYTE** 할당해제
		count++;

		feature[i][0] = M2; //i번째 영상의 M2값 저장하는 공간
		feature[i][1] = M3; //i번째 영상의 M3값 저장하는 공간

		//이미지 영상 3개씩 한줄에 출력하기
		if (count == 3) {
			x0 = 10;
			y0 += 150;
			count = 0;
		}
		else {
			x0 += 190;
		}
	}

	//Display_Feature_Space(feature, 530, 10); //feature에는 M2, M3정보가 담김. (530,10)위치에 feature map display하기
	free_dmatrix(feature, 12, 2);
	pView->ReleaseDC(dc);
}