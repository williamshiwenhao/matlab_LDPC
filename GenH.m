function H=GenH(m,n,mb,nb,z)
load Matrix(2016,1008)Block56.mat;  %load����״��ʾ��H_block
H=zeros(m,n);
for i=1:mb
    for j=1:nb
        if(H_block(i,j)>0)  %��������е�1��ƫ��
            for p=1:z
                H((i-1)*z+p,(j-1)*z+mod(p-1+H_block(i,j)-1,z)+1)=1;  %ÿ�е�ƫ������ȡģ����
            end
        end
    end
end
H(1,m)=0;  %�޸ľ���a����������У�Hp���Ͻ���0����1��
clear H_block; %�������Լ�ռ�
end