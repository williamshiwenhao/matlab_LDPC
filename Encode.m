function [ encoded ] = Encode( s, Hs )
%Encode LDPC encode
x = mod(s * Hs', 2);
p = zeros(size(s, 1), LDPC.mB * LDPC.z);
p(:, 1) = x(:, 1);
for jj = 2:LDPC.mB
        p(:, (jj - 1) * LDPC.z + 1) = xor( x(:, (jj - 1) * LDPC.z + 1), p(:, (jj - 2) * LDPC.z + 1));
end

for ii = 2: LDPC.z
    p(:, ii) = xor( x(:, ii) , p(:, (LDPC.mB - 1)*LDPC.z + ii -1) );
    for jj = 2:LDPC.mB
        p(:, (jj - 1) * LDPC.z + ii) = xor( x(:, (jj - 1) * LDPC.z + ii), p(:, (jj - 2) * LDPC.z + ii));
    end
end
encoded = [p, s];
end

