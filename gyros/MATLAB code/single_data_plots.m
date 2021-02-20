clear all; close all; clc

testtime1 = 20000;
odr1 = 200;

Data1 = importdata('data_2000_200_64_20000.txt');
Data1 = Data1(61:end);
% xdata1 = Data1.data(:,1);
% ydata1 = Data1.data(:,2);
% zdata1 = Data1.data(:,3);

Bias = mean(Data1);

time1 = 0:1/odr1:(testtime1*odr1-1)/odr1;
plot(time1,Data1-Bias)
legend();
figure
[tau1,adev1] = allandeviation_par_gpu(time1,Data1-Bias);
legend

