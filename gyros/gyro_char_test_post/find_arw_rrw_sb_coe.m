clear all; close all; clc
load('tau_adev_250_200_64_20000.mat');

figure 
hold on
grid on
set(gca, 'XScale', 'log', 'YScale', 'log');
xyzColor = ['r', 'b', 'g'];
N1 = zeros(1,3);
B1 = zeros(1,3);
for i = 1:3 
    [N1(i), B1(i)] = calNB(tau, adev(:,i), xyzColor(i));
end
load('tau_adev_250_50_16_20000.mat');

N2 = zeros(1,3);
B2 = zeros(1,3);
for i = 1:3 
    [N2(i), B2(i)] = calNB(tau, adev(:,i), xyzColor(i));
end

load('tau_adev_250_12.5_4_20000.mat');

N3 = zeros(1,3);
B3 = zeros(1,3);
for i = 1:3 
    [N3(i), B3(i)] = calNB(tau, adev(:,i), xyzColor(i));
end
legend('x', '', '','y', '', '','z','', '','', '', '')

NN = [N1; N2; N3];
BB = [B1; B2; B3];
mean(NN)
mean(BB)*3600
std(NN)
std(BB*3600)
function [N,B] = calNB(tau, adev, color)
    % Define the slopes of each lines
    slope1 = -0.5; %angle random walk
    % slope2 = 0.5; %rate random walk
    slope3 = 0; %static bias

    % Transfer the data to a log plot
    logtau = log10(tau);
    logadev = log10(adev);
    dlogadev = diff(logadev) ./ diff(logtau);

    [~, i] = min(abs(dlogadev(1:50000/8) - slope1));
    % [~, j] = min(abs(dlogadev(1:50000) - slope2));
    [~, k] = min(abs(dlogadev(1:50000/2) - slope3));

    % Find the y-intercept of the lines.
    b1 = logadev(i) - slope1*logtau(i);
    % b2 = logadev(j) - slope2*logtau(j);
    b3 = logadev(k) - slope3*logtau(k);

    % Determine the angle coefficients from the lines.
    logN = slope1*log(1) + b1;
    N = 10^logN

    % logK = slope2*log10(3) + b2;
    % K = 10^logK

    scfB = sqrt(2*log(2)/pi);
    logB = b3 - log10(scfB);
    B = 10^logB
    % Plot the results.
    tauN = 1;
    lineN = N ./ sqrt(tau);

    % tauK = 3;
    % lineK = K .* sqrt(tau/3);

    tauB = tau(k);
    lineB = B * scfB * ones(size(tau));

    % loglog(tau, adev, tau, lineN, '--', tauN, N, 'o', tau, lineK, '--', tauK, K, 'o', tau, lineB, '--', tauB, scfB*B, 'o')
    loglog(tau, adev, color);
%     loglog(tau, lineN, 'k--', tauN, N, 'k.', tau, lineB, 'k--', tauB, scfB*B, 'k.')
    loglog(tauN, N, 'k.',tauB, scfB*B, 'k.','MarkerSize',10)
    
    title('Allan Deviation')
    xlabel('\tau(s)')
    ylabel('\sigma(\tau)(dps)')
%     legend('\sigma', '\sigma_N', '\sigma_K','sigma_B')

    text(tauN, N, 'N')
    % text(tauK, K, 'K')
    text(tauB, scfB*B, '0.664B')
end
