%%
load('Matrix(2016,1008)Block56.mat')
mex DecodeSumProduct.cpp
mex DecodeMinSum.cpp
H = generateH(H_block);%sparse matrix
Hs = H(:, LDPC.mB*LDPC.z+1:end);
[column, row, ~] = find(H');
column = int32(column);row=int32(row);
SNR = -2;
alpha = 0.1:0.1:1;
needErrorFrame = 100;
errorRate = zeros(1, length(alpha));
frameBit = LDPC.kB * LDPC.z;
%%
for ii = 1:length(alpha)
    errorBit = 0;
    errorFrame = 0;
    totalFrame = 0;
    %%
    while errorFrame < needErrorFrame
        %%
        %Encode
        s = randi([0,1], 1, LDPC.kB * LDPC.z);
        coded = Encode(s, Hs);
        codeError = mod(coded* H', 2);
        %%
        %BPSK
        modulated = coded .* -2 +1;
        %Add Noise
        afterChannel = awgn(modulated, SNR+10*log10(2), 'measured');
        Lc = afterChannel .* 4 .* (10^(SNR / 10) );%SNR to liner
        %%
        decoded = DecodeMinSum(Lc, column, row, [alpha(ii); 0]);
        y = decoded(:, LDPC.mB*LDPC.z + 1:end);
        bits = sum(sum(y~=s));
        totalFrame = totalFrame + 1;
        if bits > 0
            errorFrame = errorFrame + 1;
            errorBit = errorBit + bits;
        end
    end
    errorRate(1, ii) = errorBit / (totalFrame * LDPC.kB * LDPC.z);
end
plot(alpha, errorRate);