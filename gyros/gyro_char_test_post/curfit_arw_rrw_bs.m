clear all; 
% close all; clc
load('tau_adev_250_200_64_20000.mat');
tau = gather(tau);
adev = gather(adev);
% 
% tau = tau(1:300000)
% adev = adev(1:300000,:)

loglog(tau,adev);

n = (1:length(tau));
n = [n' n' n'];
T0 = 1/200;
fun = @(coe) sqrt(+coe(1,:).^2.*n.*T0./3+coe(2,:).^2.*(2./pi).*log(2)+coe(3,:).^2./(n.*T0)+3*coe(4,:).^2./(n.^2*T0^2))- adev;
x0 = [0 0 0; 0 0 0; 0 0 0; 0 0 0];
x = lsqnonlin(fun,x0);
y = fun(x)+adev;
hold on; 
% loglog(tau,y);
legend('x axis','y axis', 'z axis');
xlabel('\tau')
ylabel('\sigma (\tau)')
title('12.5 Hz')

