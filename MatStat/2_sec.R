homerseklet_valtozo<-c(-5, -5,
                       1,1,1,
                       3.5,3.5,3.5,3.5,3.5,
                       7.5,7.5)

homerseklet_valtozo <-rep(c(-5,1,3.5, 7.5), times=c(2,3,5,2))

hist(homerseklet_valtozo, breaks=c(-10,0,2,5,10))

hist(homerseklet_valtozo, breaks=c(-10,0,2,5,10), freq=TRUE)


hist(homerseklet_valtozo, breaks=c(-10,0,2,5,10), freq=TRUE, col='red', 
     xlab='Hőmérséket', ylab='Gyakoriság', main='Hőmérséklet hisztogrammja')
 