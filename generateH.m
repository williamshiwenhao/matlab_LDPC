function [ H ] = generateH(HBlock)
%GENERATEH Éú³ÉH¾ØÕó
H = sparse(LDPC.mB * LDPC.z, LDPC.nB * LDPC.z);
for line=1:LDPC.kB
    for row=1:LDPC.nB
        if(HBlock(line, row) ~= 0)
            HSub = zeros(LDPC.z);
            for ii = 1:LDPC.z
                HSub(ii, mod(ii-1+HBlock(line, row)-1, LDPC.z)+1)=1;
            end
            H((line-1)*LDPC.z+1:line*LDPC.z, (row-1)*LDPC.z+1:row*LDPC.z) = HSub;
        end
    end
end
%Get mat a
a = zeros(LDPC.z);
for ii = 2:LDPC.z
    a(ii, ii -1) = 1;
end
H(1:LDPC.z, (LDPC.kB-1)*LDPC.z+1:LDPC.kB*LDPC.z) = a;
end

