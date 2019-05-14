function c = EnCode2(H,s,z)
    [m,n] = size(H);
    mb = m/z;
    Hs = H(1:end,(m+1):end);  %得到编码矩阵Hs
    x = mod(s * (Hs'),2);
    p = zeros(1,m);
    p(1) = x(1);
    jj=1;
    for count = 1:m-1
        ii = jj+1-(mb-1)*z;   %在已知p(jj)的情况下计算下一个可以算出的p的ii值
        if (ii>1) && (ii<=z)  %通过判定ii的范围确定如何计算p(ii)
            p(ii) = mod(x(ii)+p(jj),2);
        else
            ii = jj+z;
            p(ii) = mod(x(ii)+p(jj),2);
        end;
        jj=ii;
    end;
    c = [p,s];     %输出编码序列，校验位在前，信息位在后
end
    