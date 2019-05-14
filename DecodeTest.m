%%
load('Matrix(2016,1008)Block56.mat')
mex DecodeSumProduct.cpp
mex DecodeMinSum.cpp
H = generateH(H_block);%sparse matrix
Hs = H(:, LDPC.mB*LDPC.z+1:end);
[column, row, ~] = find(H');
column = int32(column);row=int32(row);
times = 1e3;
SNR = 0;
%%
for ii = 1:times
    %%
    %Encode
    s = randi([0,1], 1, LDPC.kB * LDPC.z);
%     coded = Encode(s, Hs);
    coded = EnCode2(H, s, LDPC.z);
    codeError = mod(coded* H', 2);
    %%
    %BPSK
    modulated = coded .* -2 +1;
    %Add Noise
    afterChannel = awgn(modulated, SNR+10*log10(2));
    Lc = afterChannel .* 4 .* (10^(SNR / 10) );%SNR to liner
    decoded = DecodeSumProduct(Lc, column, row);
    decoded2 = DecodeMinSum(Lc, column, row, [1.0; 1.0]);
    y = decoded(:, LDPC.mB*LDPC.z + 1:end);
    y2 = decoded(:, LDPC.mB*LDPC.z + 1:end);
    errorBit = sum(sum(y~=s));
    errorBit2 = sum(sum(y2~=s));
    if(errorBit ~= 0 || errorBit2 ~= 0)
        disp(errorBit);
        disp(errorBit2);
    end
end