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

void SumProduct(double* coded, double* decoded, int* column, int*  row, size_t codedSize, size_t HSize)
{
	//row是校验节点的编号，column是变量节点的编号
	std::vector<double> product(row[HSize - 1]);
	std::vector<double> sum(codedSize);
	std::vector<double> v(HSize);
	std::vector<double> u(HSize);
	memcpy(decoded, coded, sizeof(double) * codedSize);
	int round;
	for (round = 0; round < maxDecodeRound && !CheckStatified(decoded, column, row, codedSize, HSize); ++round) {
		product.assign(column[HSize - 1], 1);
		sum.assign(codedSize, 0);
		for (int i = 0; i < HSize; ++i) {
			sum[column[i] - 1] += u[i];
		}
		for (int i = 0; i < HSize; ++i) {
			v[i] = coded[column[i] - 1] + sum[column[i] - 1] - u[i];
		}
		for (int i = 0; i < HSize; ++i) {
			product[row[i] - 1] *= tanh(v[i] / 2);
		}
		for (int i = 0; i < HSize; ++i) {
			double tmp = product[row[i] - 1] / tanh(v[i] / 2);
			if (tmp >= 1) {
				tmp = topValue;
			}
			else if (tmp <= -1) {
				tmp = bottomValue;
			}
			else {
				tmp = 2 * atanh(tmp);
			}
			u[i] = tmp;
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
	mwSize inputNum = 3;
	mwSize outputNum = 1;
	if (nrhs != inputNum) {
		mexErrMsgIdAndTxt("MyToolbox:DecodeSumProduct:nrhs", "%d inputs required", inputNum);
	}
	if (nlhs != outputNum) {
		mexErrMsgIdAndTxt("MyToolbox:DecodeSumProduct:nlhs", "%d output required", outputNum);
	}
	mwSize i;
	//input should be row double
	if (!mxIsDouble(prhs[0]) || mxGetM(prhs[0]) != 1) {
		mexErrMsgIdAndTxt("MyToolBox:DecodeSumProduct:Input", "Input should be a row double vector");
	}
	//H matrix
	if (mxGetClassID(prhs[1]) != mxINT32_CLASS || mxGetClassID(prhs[2]) != mxINT32_CLASS) {
		mexErrMsgIdAndTxt("MyToolBox:DecodeSumProduct:Input", "Ir and Js shoud be int32");
	}
	double *coded = mxGetPr(prhs[0]);
	size_t codedSize = mxGetN(prhs[0]);
	int* column = (int*)mxGetData(prhs[1]);
	int* row = (int*)mxGetData(prhs[2]);
	size_t rowSize = mxGetNumberOfElements(prhs[2]);
	size_t columnSize = mxGetNumberOfElements(prhs[1]);
	if (rowSize != columnSize) {
		mexErrMsgIdAndTxt("MyToolBox:DecodeSumProduct:Input", "Row size ~= columnSize");
	}
	plhs[0] = mxCreateDoubleMatrix(1, (mwSize)codedSize, mxREAL);
	double* decoded = mxGetPr(plhs[0]);
	SumProduct(coded, decoded, column, row, codedSize, rowSize);

}