function A = rand_psd(n)

% create random matrix
A = randn(n,n);

% symmetrize
A = 0.5 .* ( A + A' );

% make eigenvalues nonnegative
[U,V] = eig(A);

% reconstruct matrix
A = U * abs(V) * U';

end

