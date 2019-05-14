load('Matrix(2016,1008)Block56.mat')
H = generateH(H_block);%sparse matrix
Hs = H(:, LDPC.mB*LDPC.z+1:end);
times = 1e3;
% for ii = 1:times
%     s = randi([0,1], 1, LDPC.kB * LDPC.z);
%     c = Encode(s, Hs);
%     y = mod(c * H', 2);
%     error = sum(sum(y~=0));
%     if(error~= 0)
%         disp(error)
%     end
% end
H2 = GenH(LDPC.mB*LDPC.z, LDPC.nB*LDPC.z, LDPC.mB, LDPC.nB, LDPC.z);
sum(sum(H~=H2))