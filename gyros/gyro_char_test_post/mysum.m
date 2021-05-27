function theta = mysum(data,g,i)
[N,L] = size(data);
n = floor(N/g);

if (i== g) 
    theta = (zeros(n+N-n*g,L));
else
    theta = (zeros(n,L));
end

k = n*(i-1)+1;
theta(1,:) = sum(data(1:k,:));
for b = 2:length(theta)
    theta(b,:)= data(k+b-1,:)+ theta(b-1,:);
end

end