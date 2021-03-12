% Magnetometer Calibration 
%
% This script performs two different ways of doing magnetometer
% calibration, and checks that both give the same answers. 
%
% Note that method 1 using CVX works well regardless of the ellipticity
% measure of the "true" A matrix in a random trial. Method 2, based on an
% analytical solution to the LS problem, seems to be quite sensitive to
% ellipticity. This is mentioned in the original paper(s):
%   (1) https://ieeexplore.ieee.org/document/1290055
%   (2) https://ieeexplore.ieee.org/document/765658
% but I've not implemented any fixes because mag data does not appear to
% ever be skewed enough for this to matter.
%
% You'll need to install CVX to run this script, which can be done here:
%   http://cvxr.com/cvx/doc/index.html
%
% Setting << test = false >> will pull in the real mag data to run the
% calibration, and the script will save the coefficients in a .mat file.
%
% T. P. Reynolds
clear; 

test = false;

% create truth data
if (test)
    d_true = randn(3,1);
    A_true = rand_psd(3);
    fprintf('Ellipticity measure of true A: %5.2f\n',cond(A_true))
    A_true = A_true./norm(A_true);

    % generate some data
    N = 100;
    data = zeros(3,N);
    for k = 1:N
        temp = randn(3,1);
        temp = temp./norm(temp);
        data(:,k) = A_true * temp + d_true;
    end
    % corrupt data a bit
    data = data + 1e-3.*randn(3,N);
    % mean field strength
    B_str = NaN(1,N);
    for k = 1:N
        temp = data(:,k);
        if (norm(temp)>0)
            B_str(k)  = norm(temp);
        end
    end
    B = mean(B_str);
else
    CalibrationData;
    N = size(M,1);
    % remove zero measurements & mean field strength
    B_str = NaN(1,N);
    data  = NaN(3,N);
    for k = 1:N
        temp = M(k,:)';
        if (norm(temp)>0)
            B_str(k)  = norm(temp);
            data(:,k) = temp;
        end
    end
    B_str = rmmissing(B_str);
    data  = rmmissing(data','MinNumMissing',3)';
    N     = size(data,2);
    B = mean(B_str);
end
   
%% Method 1: Using CVX
cvx_begin quiet sdp
    cvx_solver('sdpt3')
    variable M(3,3) symmetric
    variable n(3,1)
    variable r(1,1)
    
    expression res(N,1)
    for k = 1:N
        temp = data(:,k);
        res(k) = temp'*M*temp + dot(n,temp) + r;
    end
    
    minimize( sum_square(res) )
    
    M >= 0.0;
cvx_end

A  = pinv(sqrtm(M));
A  = A./norm(A);
d  = -0.5*(M\n);

if (test)
    A_err = norm(A-A_true);
    d_err = norm(d-d_true);
    fprintf('CVX Errors:\n')
    fprintf('A: %2.2e\n',A_err)
    fprintf('d: %2.2e\n',d_err)
end

%% Method 2: Using Least Squares
[A_ls,d_ls] = mag_calibration(data);

if (test)
    A_err = norm(A_ls-A_true);
    d_err = norm(d_ls-d_true);
    fprintf('LS Errors:\n')
    fprintf('A: %2.2e\n',A_err)
    fprintf('d: %2.2e\n',d_err)
end

%% Plot uncalibrated data
figure(1), clf, hold on, box on, grid on
% subplot(1,2,1), hold on, grid on, box on
plot3(data(1,:),data(2,:),data(3,:),'r*')
%
% subplot(1,2,2), hold on, grid on, box on
temp = A \ (data - d); 
plot3(temp(1,:),temp(2,:),temp(3,:),'b*')
temp = A_ls \ (data - d_ls); 
plot3(temp(1,:),temp(2,:),temp(3,:),'k*')
legend('Uncalibrated','Calibrated: CVX','Calibrated: LS','Interpreter','latex','FontSize',15)
axis equal
xlabel('X [uT]','FontSize',18,'Interpreter','latex')
ylabel('Y [uT]','FontSize',18,'Interpreter','latex')
zlabel('Z [uT]','FontSize',18,'Interpreter','latex')

%% Save calibration coefficients
if (~test)
    A_cal = A \ eye(3);
    b_cal = - A_cal * d;

    save('MagCalibrationCoefficients','A_cal','b_cal')
end