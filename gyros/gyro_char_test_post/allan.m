function [tau,AVAR] = allan(time, data)
numtaus = floor(length(time)/10);
for b = 1:numtaus
    tau(b) = time(b+1)-time(1);
end

for h = 1:numtaus
    binvar = 0;
    avebin = 0;
    totalbins = floor(time(length(time)) / tau(h));
    lengthbins = floor(length(time) / totalbins);
    for j = 1:totalbins
        avebin(j) = mean(data((lengthbins * (j-1) + 1:lengthbins*(j))));
    end
    for j = 1:(totalbins-1)
        binvar(j) = (avebin(j+1)-avebin(j))^2;
    end
    
    AVAR(h) = sqrt(sum(binvar)/(2*(totalbins-1)));
end
loglog(tau,AVAR)