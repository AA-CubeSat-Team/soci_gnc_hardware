clear all; close all; clc

fsr = 250;
testtime = 60;
odr = 12.5;
bw = 4;
numrun = NaN;

fileName = "data_"+ fsr+ "_"+ odr+ "_" + bw +"_" + testtime + "." + "txt"
Data = importdata(fileName);
% data = Data.data;
data = Data;

Bias = mean(data);

time = 0:1/odr:(testtime*odr-1)/odr;

plot(time,data)
% plot(time,data-Bias)
legend();
figure
% [tau,adev] = allandeviation_par(time,data-Bias);
% legend

