function [A,b] = mag_calibration(data)

k = 4;

% create design matrix
X = data(1,:);
Y = data(2,:);
Z = data(3,:);
D = [ X.^2; Y.^2; Z.^2; 2.*X.*Y; 2.*X.*Z; 2.*Y.*Z; X; Y; Z; ones(size(X)) ];

% create scatter matrix
S = D * D';
S11 = S(1:6,1:6);
S12 = S(1:6,7:10);
S22 = S(7:10,7:10);

% create constraint matrix
C1 = [ -1,       0.5*k-1,  0.5*k-1, 0,  0,  0;
       0.5*k-1, -1,        0.5*k-1, 0,  0,  0;
       0.5*k-1,  0.5*k-1, -1,       0,  0,  0;
       0,        0,        0,      -k,  0,  0;
       0,        0,        0,       0, -k,  0;
       0,        0,        0,       0,  0, -k ];
   
% solve generalized eigensystem
M = pinv(C1) * ( S11 - S12 * (S22 \ S12') );
[Mvec,Mval] = eig(M);

% find the one positive eigenvalue
[~,locPos] = find( Mval>0 & ~isinf(Mval) );

% construct solution
a1 = Mvec(:,locPos);
if (a1(1)<0)% (sum(a1<0)==6)
    a1 = -a1;
end
a2 = - (S22 \ S12') * a1;

% check to see if it is an ellipse
I = a1(1) + a1(2) + a1(3);
J = a1(1)*a1(2) + a1(2)*a1(3) + a1(1)*a1(3) - a1(4)^2 - a1(5)^2 - a1(6)^2;

if ~( 4*J-I^2 > 0 )
    error('Not an Ellipse!')
end

% Quadric parameters
M = [ a1(1), a1(4), a1(5); a1(4), a1(2), a1(6); a1(5), a1(6), a1(3) ];
n = [ a2(1); a2(2); a2(3) ];
d = a2(4);

b = -0.5.*(M\n);
A = pinv(sqrtm(M));
A = A./norm(A);
end

