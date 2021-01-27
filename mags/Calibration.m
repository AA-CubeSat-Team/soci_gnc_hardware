%% Hard-iron offset: Origin to (0,0,0)

dx=M(:,1); dy=M(:,2); dz=M(:,3);
plot3(dx, dy, dz, '.')
hold on;
x1 = max(dx); x2 = min(dx);
X = (x1 + x2)/2;
y1 = max(dy); y2 = min(dy);
Y = (y1 + y2)/2;
z1 = max(dz); z2 = min(dz);
Z = (z1 + z2)/2;
% plot3(X, Y, Z, 'k*')
M1 = dx - X;
M2 = dy - Y;
M3 = dz - Z;

plot3(M1, M2, M3, 'd')
legend('Raw Data', 'Hard-iron correction')
Q = [M1, M2, M3];


%% Soft-iron offset: Ellipse to sphere 
n=size(dx,1);
D=[dx.*dx, dy.*dy,  dz.*dz, 2.*dy.*dz, 2.*dx.*dz, 2.*dx.*dy, ...
        2.*dx, 2.*dy, 2.*dz, ones(n,1)]';

S=D*D';

v=FindFit4(S);

minX=min(dx);  maxX=max(dx);
minY=min(dy);  maxY=max(dy);
minZ=min(dz);  maxZ=max(dz);


% draw fitting
a=100; b=100; c=100;
nStep=20;
stepA=a/nStep; stepB=b/nStep; stepC=c/nStep;

[x, y, z]=meshgrid(-a:stepA:a, -b:stepB:b, -c:stepC:c);


SolidObj=v(1)*x.*x+v(2)* y.*y+v(3)*z.*z+ 2*v(4)*y.*z + 2*v(5)*x.*z + 2*v(6)*x.*y...
    + 2*v(7)*x + 2*v(8)*y + 2*v(9)*z + v(10)* ones(size(x));


clf;
       p = patch(isosurface(x,y,z,SolidObj, 0.0));
       isonormals(x,y,z,SolidObj, p);
       set(p, 'FaceColor', 'y', 'EdgeColor', 'k');
       daspect([1 1 1]);
       view(3);
       camlight ;
       lighting phong;

       hold on;
plot3(dx, dy, dz, 'm^');
title('Soft-Iron Correction')