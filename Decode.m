function [ x ] = Decode( y, H )
%Decode LDPC����
%   y: soft input
%   H: matrix H
u = 4 .* y;%���Ŷ�
for ii = 1:LDPC.decodeRound
    s = y * H';
    
end

end

