function [tau,adev] = allandeviation_par(time, data)
[N,L] = size(data);
M = floor((N-1)/2);
tau = (zeros(M,1));
parfor a = 1:M
    tau(a) = time(a+1)-time(1);
end
tau0 = tau(1);
delete(gcp('nocreate'));
g = 8;
tic
for i = 1:g
    "working"
    i
j(i) = batch(@mysum,1,{data,g,i});
end

for i = 1:g
    "wait"
    i
wait(j(i))
end

theta = [];
for i = 1:g
    a = fetchOutputs(j(i));
    a = gather(a{:});
    theta = [theta; a];
end
theta = (theta*tau0);
toc

tic
avar = (zeros(M,L));
parfor m = 1:M
    m
    avar(m,:) = sum((theta((1+2*m):N,:) - 2*theta((1+m):(N-m),:) + theta(1:(N-2*m),:)).^2);
    avar(m,:) = avar(m,:)./(2*tau(m).^2.*(N-2*m));
end
toc
adev = sqrt(avar);
loglog(tau,adev);