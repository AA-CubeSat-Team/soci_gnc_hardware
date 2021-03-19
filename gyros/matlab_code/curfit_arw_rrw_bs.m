clear all; close all; clc
load('tau_adev_2000_200_64_20000.mat');

loglog(tau,adev);

n = (1:length(tau));
T0 = 1/200;
fun = @(coe) sqrt(+coe(1).^2.*n.*T0./3+coe(2).^2.*(2./pi).*log(2)+coe(3).^2./(n.*T0)+3*coe(4)^2./(n.^2*T0^2))- adev;
x0 = [0 0 0 0];
x = lsqnonlin(fun,x0);
y = fun(x)+adev;
hold on; 
loglog(tau,y);
coe1 = [1.4101e-4 0.0046 0.018 0];
loglog(tau,fun(coe1)+adev)
legend();
% function curvefitpoly(coe,tau,adev)
