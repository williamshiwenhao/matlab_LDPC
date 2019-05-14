function H=GenH(m,n,mb,nb,z)
load Matrix(2016,1008)Block56.mat;  %load出块状表示的H_block
H=zeros(m,n);
for i=1:mb
    for j=1:nb
        if(H_block(i,j)>0)  %矩阵块首行的1有偏移
            for p=1:z
                H((i-1)*z+p,(j-1)*z+mod(p-1+H_block(i,j)-1,z)+1)=1;  %每行的偏移量可取模计算
            end
        end
    end
end
H(1,m)=0;  %修改矩阵a的特殊情况中，Hp右上角是0不是1；
clear H_block; %清除，节约空间
end