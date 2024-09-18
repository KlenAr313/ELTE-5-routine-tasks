homerseklet_valtozo<-c(-5, -5,
                       1,1,1,
                       3.5,3.5,3.5,3.5,3.5,
                       7.5,7.5)

homerseklet_valtozo <-rep(c(-5,1,3.5, 7.5), times=c(2,3,5,2))

hist(homerseklet_valtozo, breaks=c(-10,0,2,5,10))

hist(homerseklet_valtozo, breaks=c(-10,0,2,5,10), freq=TRUE)


hist(homerseklet_valtozo, breaks=c(-10,0,2,5,10), freq=TRUE, col='purple', 
     xlab='Hőmérséket', ylab='Gyakoriság', main='Hőmérséklet hisztogrammja')
 
mean(homerseklet_valtozo)

minta<-c(21,35,45,61)
ECDF<-ecdf(minta)
plot(ECDF)
plot(ECDF, pch=NULL)

points(minta, seq(0,length(minta) - 1)*0.25, pch=16)

quantile(minta, 0.33)
