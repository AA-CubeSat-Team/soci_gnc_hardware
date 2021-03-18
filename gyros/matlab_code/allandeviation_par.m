function [tau,adev] = allandeviation_par(time, data)
[N,L] = size(data);
M = floor((N-1)/2);
tau = (zeros(M,1));
parfor a = 1:M
    tau(a) = time(a+1)-time(1);
end
tau0 = tau(1);

theta = (zeros(N,L));
tic
parfor b = 1:N
%     disp(['b =', num2str(b)])
    theta(b,:)= sum(data(1:b,:))*tau0;
end
toc
avar = (zeros(M,L));
tic
parfor m = 1:M
%     disp(['m =', num2str(m)])
    avar(m,:) = sum((theta((1+2*m):N,:) - 2*theta((1+m):(N-m),:) + theta(1:(N-2*m),:)).^2);
    avar(m,:) = avar(m,:)./(2*tau(m).^2.*(N-2*m));
end
toc
adev = sqrt(avar);
loglog(tau,adev);