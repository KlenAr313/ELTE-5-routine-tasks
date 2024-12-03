#7. gyakorlat fajl, 3 / b

minta<-c(6, 4.5, 2.5, 2, 1)
sigma<-3
n<-length(minta)
alpha<-1-0.95

also_hatar<-mean(minta)-sigma/sqrt(n)*qnorm(1-alpha/2)
felso_hatar<-mean(minta)+sigma/sqrt(n)*qnorm(1-alpha/2)

#c)

minta_c<-(minta-5)/2 
sigma_c<-sigma/2 #Ezutan N(m, 1^2) a minta
also_hatar<-mean(minta_c)-sigma_c/sqrt(n)*qnorm(1-alpha/2)
felso_hatar<-mean(minta_c)+sigma_c/sqrt(n)*qnorm(1-alpha/2)

#4. feladat

p_minta<-88/1000
n<-1000
alpha<-1-0.95

also_hatar<-p_minta-sqrt(p_minta*(1-p_minta)/n)*qnorm(1-alpha/2)
felso_hatar<-p_minta+sqrt(p_minta*(1-p_minta)/n)*qnorm(1-alpha/2)

#8. gyakorlat, 1. feladat
#a)
kritikus_ertek<-qnorm(0.95, 18, 1)
#Ha a kitamin tartalom 19.64-nel nagyobb, akkor elvetjuk
#nullhipotezist, hogy a paciensunk nem szenved butitizmusban.
#b)
masodfaju_hiba<-pnorm(kritikus_ertek, 20, 2)

#c)
n_sim<-10000
kitamin<-rnorm(n_sim, 20, 2)
sum(kitamin>kritikus_ertek)/n_sim
1-masodfaju_hiba

#d)
kitamin<-rnorm(n_sim, 18, 1)
sum(kitamin<=kritikus_ertek)/n_sim

#2. feladat
#a)
1-(9/10)^5

#c)
n_sim<-1000
a_1<-9.8

minta_1<-matrix(runif(n_sim*5, 0, a_1), nrow=n_sim, ncol=5)
minta_1<-apply(minta_1, MARGIN=1, FUN=max)
sum(minta_1>9)/n_sim
1-(9/a_1)^5

a_2<-11

minta_2<-matrix(runif(n_sim*5, 0, a_2), nrow=n_sim, ncol=5)
minta_2<-apply(minta_2, MARGIN=1, FUN=max)
sum(minta_2>9)/n_sim
1-(9/a_2)^5
