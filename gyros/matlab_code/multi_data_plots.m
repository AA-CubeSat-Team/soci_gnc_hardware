clear all; close all; clc

testtime = 180;
odr1 = 50;
odr2 = 200;
odr3 = 800;
data1 = importdata('data_250_50_16_180.txt');
data1 = (data1(61:odr1*testtime+60));

data2 = importdata('data_250_50_8_180.txt');
data2 = (data2(61:odr1*testtime+60));

data3 = importdata('data_250_50_4_180.txt');
data3 = (data3(61:odr1*testtime+60));

data4 = importdata('data_250_200_64_180.txt');
data4 = (data4(61:odr2*testtime+60));

data5 = importdata('data_250_200_32_180.txt');
data5 = (data5(61:odr2*testtime+60));

data6 = importdata('data_250_200_16_180.txt');
data6 = (data6(61:odr2*testtime+60));
data7 = importdata('data_2000_800_256_180.txt');
data7 = (data7(61:odr3*testtime+60));

time1 = 0:1/odr1:(testtime*odr1-1)/odr1;
time2 = 0:1/odr2:(testtime*odr2-1)/odr2;
time3 = 0:1/odr3:(testtime*odr3-1)/odr3;
figure
plot(time2,data4)
hold on
plot(time2,data5)
plot(time2,data6)
plot(time1,data1)
plot(time1,data2)
plot(time1,data3)
legend('ODR=200hz, cut-off frequency=64hz','ODR=200hz, cut-off frequency=32hz','ODR=200hz, cut-off frequency=16hz','ODR=50hz, cut-off frequency=16hz','ODR=50hz, cut-off frequency=8hz','ODR=50hz, cut-off frequency=4hz')

% plot(time3,data7)


figure
myfft(data4,odr2)
hold on
myfft(data5,odr2)
myfft(data6,odr2)
myfft(data1,odr1)
myfft(data2,odr1)
myfft(data3,odr1)
legend('ODR=200hz, cut-off frequency=64hz','ODR=200hz, cut-off frequency=32hz','ODR=200hz, cut-off frequency=16hz','ODR=50hz, cut-off frequency=16hz','ODR=50hz, cut-off frequency=8hz','ODR=50hz, cut-off frequency=4hz')

% myfft(data7,odr3)
figure
allandeviation_par(time2,data4)
hold on
allandeviation_par(time2,data5)
allandeviation_par(time2,data6)
allandeviation_par(time1,data1)
allandeviation_par(time1,data2)
allandeviation_par(time1,data3)
legend('ODR=200hz, cut-off frequency=64hz','ODR=200hz, cut-off frequency=32hz','ODR=200hz, cut-off frequency=16hz','ODR=50hz, cut-off frequency=16hz','ODR=50hz, cut-off frequency=8hz','ODR=50hz, cut-off frequency=4hz')

% allandeviation_par(time3,data7)
