%%
load('Matrix(2016,1008)Block56.mat')
H = generateH(H_block);%sparse matrix
Hs = H(:, LDPC.mB*LDPC.z+1:end);
%%