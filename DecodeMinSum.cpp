#include "mex.h"
#include <math.h>
#include <stdint.h>
#include <cstring>
#include <vector>

const int maxDecodeRound = 30;
const double topValue = 100.0;
const double bottomValue = -100.0;
bool CheckStatified(double* coded, int* column, int* row, size_t codedSize, size_t HSize) {
	int lastRow = 0;
	int ones = 0;
	for (int i = 0; i < HSize; ++i) {
		if (row[i] != lastRow) {
			if ((ones & 0x01) != 0)
				return false;
			ones = 0;
			lastRow = row[i];
		}
		if (coded[column[i] - 1] < 0) {
			++ones;
		}
	}
	return true;
}

void Judgement(double* decoded, size_t codedSize) {
	for (int i = 0; i < codedSize; ++i) {
		if (decoded[i] < 0)
			decoded[i] = 1;
		else
			decoded[i] = 0;
	}
}

void SumProduct(double* coded, double* decoded, int* column, int*  row, size_t codedSize, size_t HSize, double alpha, double beta)
{
	//row是校验节点的编号，column是变量节点的编号
	std::vector<double> minValue(row[HSize - 1]);
	std::vector<double> subMinValue(row[HSize - 1]);
	std::vector<bool> sgn(row[HSize - 1]);
	std::vector<double> sum(codedSize);
	std::vector<double> v(HSize);
	std::vector<double> u(HSize);
	memcpy(decoded, coded, sizeof(double) * codedSize);
	int round;
	for (round = 0; round < maxDecodeRound && !CheckStatified(decoded, column, row, codedSize, HSize); ++round) {
		minValue.assign(row[HSize - 1], 1000);
		subMinValue.assign(row[HSize - 1], 1000);
		sgn.assign(row[HSize - 1], true);
		sum.assign(codedSize, 0);
		for (int i = 0; i < HSize; ++i) {
			sum[column[i] - 1] += u[i];
		}
		for (int i = 0; i < HSize; ++i) {
			v[i] = coded[column[i] - 1] + sum[column[i] - 1] - u[i];
		}
		for (int i = 0; i < HSize; ++i) {
			int idx = row[i] - 1;
			if (abs(v[i]) < subMinValue[idx]) {
				if (v[i] < minValue[row[i] - 1]) {
					subMinValue[idx] = minValue[idx];
					minValue[idx] = abs(v[i]);
				}
				else {
					subMinValue[idx] = abs(v[i]);
				}
			}
			if (v[i] < 0) {
				sgn[idx] = !sgn[idx];
			}
		}
		for (int i = 0; i < HSize; ++i) {
			int idx = row[i] - 1;
			double value = (abs(v[i]) == minValue[idx]) ? minValue[idx] : subMinValue[idx];
			bool sign = sgn[idx];;
			if (v[i] < 0)
				sign = !sign;
			if (!sign)
				value = -value;
			u[i] = value;
		}
		memcpy(decoded, coded, codedSize * sizeof(double));
		for (int i = 0; i < HSize; ++i) {
			decoded[column[i] - 1] += u[i];
		}
	}
	Judgement(decoded, codedSize);
}

void mexFunction(int nlhs, mxArray *plhs[],
	int nrhs, const mxArray *prhs[])
{
	//check for input
	mwSize inputNum = 4;
	mwSize outputNum = 1;
	if (nrhs != inputNum) {
		mexErrMsgIdAndTxt("MyToolbox:DecodeMinSum:nrhs", "%d inputs required", inputNum);
	}
	if (nlhs != outputNum) {
		mexErrMsgIdAndTxt("MyToolbox:DecodeMinSum:nlhs", "%d output required", outputNum);
	}
	mwSize i;
	//input should be row double
	if (!mxIsDouble(prhs[0]) || mxGetM(prhs[0]) != 1) {
		mexErrMsgIdAndTxt("MyToolBox:DecodeMinSum:Input", "Input should be a row double vector");
	}
	//H matrix
	if (mxGetClassID(prhs[1]) != mxINT32_CLASS || mxGetClassID(prhs[2]) != mxINT32_CLASS) {
		mexErrMsgIdAndTxt("MyToolBox:DecodeMinSum:Input", "Ir and Js shoud be int32");
	}
	if (!mxIsDouble(prhs[3]) || mxGetNumberOfElements(prhs[3]) != 2) {
		mexErrMsgIdAndTxt("MyToolBox:DecodeMinSum:Input", "alpha and beta should be double");
	}
	double *coded = mxGetPr(prhs[0]);
	size_t codedSize = mxGetN(prhs[0]);
	int* column = (int*)mxGetData(prhs[1]);
	int* row = (int*)mxGetData(prhs[2]);
	size_t rowSize = mxGetNumberOfElements(prhs[2]);
	size_t columnSize = mxGetNumberOfElements(prhs[1]);
	if (rowSize != columnSize) {
		mexErrMsgIdAndTxt("MyToolBox:DecodeMinSum:Input", "Row size ~= columnSize");
	}
	double *ab = mxGetPr(prhs[3]);
	double alpha = ab[0];
	double beta = ab[1];
	if (!(alpha >= 0 && alpha <= 1)) {
		mexErrMsgIdAndTxt("MyToolBox:DecodeMinSum:Input", "alpha should be (0, 1]");
	}
	if (!(beta >= 0 && beta <= 1)) {
		mexErrMsgIdAndTxt("MyToolBox:DecodeMinSum:Input", "beta should be (0, 1]");
	}
	plhs[0] = mxCreateDoubleMatrix(1, (mwSize)codedSize, mxREAL);
	double* decoded = mxGetPr(plhs[0]);
	SumProduct(coded, decoded, column, row, codedSize, rowSize, alpha, beta);

}