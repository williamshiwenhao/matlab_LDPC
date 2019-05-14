function c = EnCode2(H,s,z)
    [m,n] = size(H);
    mb = m/z;
    Hs = H(1:end,(m+1):end);  %�õ��������Hs
    x = mod(s * (Hs'),2);
    p = zeros(1,m);
    p(1) = x(1);
    jj=1;
    for count = 1:m-1
        ii = jj+1-(mb-1)*z;   %����֪p(jj)������¼�����һ�����������p��iiֵ
        if (ii>1) && (ii<=z)  %ͨ���ж�ii�ķ�Χȷ����μ���p(ii)
            p(ii) = mod(x(ii)+p(jj),2);
        else
            ii = jj+z;
            p(ii) = mod(x(ii)+p(jj),2);
        end;
        jj=ii;
    end;
    c = [p,s];     %����������У�У��λ��ǰ����Ϣλ�ں�
end
    