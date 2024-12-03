#8. gyakorlat, 3.feladat

p=1/3
n<-24

png('Valsz_plot.png')
#plot(dbinom(0:n, n, p), type='l', col='blue')
barplot(dbinom(0:n, n, p), col='blue',
        names.arg=0:24)
dev.off()

p=0.5

png('Valsz_plot_H1.png')
#plot(dbinom(0:n, n, p), type='l', col='blue')
barplot(dbinom(0:n, n, p), col='blue',
        names.arg=0:24)
dev.off()

#b)

#Elsofaju hiba: elvetjuk H0-t, felteve hogy H0 igaz

p=1/3
#y_c=12
sum(dbinom(12:24, n, p))
1-pbinom(11, n, p)

#y_c=13
sum(dbinom(13:24, n, p))
1-pbinom(12, n, p)

#c)

png('Erofv.png')
plot(seq(1/3, 1, 0.001),1-pbinom(11, n, seq(1/3, 1, 0.001)), 
type='l', col='blue')
dev.off()

#d)

n_sim=1000
y_c<-12

H0_szerinti_mintak<-rbinom(n_sim, n, 1/3)
H1_szerinti_mintak<-rbinom(n_sim, n, 1/2)

#Szimulalt elsofaju hiba:
sum(H0_szerinti_mintak>=12)/n_sim

#Szimulalt masodfaju hiba:
sum(H1_szerinti_mintak<12)/n_sim

#Erofv p=0.5 eseten:
1-sum(H1_szerinti_mintak<12)/n_sim

#Nevezetes probak, t-eloszlas surusegfve szemleltetes

png('t_eloszlas_szemleltetes.png')
plot(seq(-4, 4, 0.001),dnorm(seq(-4, 4, 0.001)), type='l', col='blue')

dfs=c(1, 4, 10, 30)
cols=c('green', 'yellow', 'pink', 'orange')
names(cols)<-dfs

for (df in dfs){
  lines(seq(-4, 4, 0.001),dt(seq(-4, 4, 0.001), df), col=cols[df])
}
legend(x='topright', legend=c('Norm', paste0('df=', dfs, ' t')), lty=1, 
       col=c('blue', cols))
dev.off()

minta=c(171, 171, 173, 176, 176, 177, 177, 178, 183, 188)
mu=175
sigma=5
alpha=0.05
Probafv<-(mean(minta)-mu)/(sigma/sqrt(length(minta)))

#1. megoldas kritikus ertek alapjan:
qnorm(1-alpha/2)
#A probafv. abszolut erteke (1.26) nem haladja meg a kritikus erteket (1.96),
#igy nem tudjuk elvetni a H0 hipotezist.

#2. megoldas p-ertek alapjan:
2*(1-pnorm(Probafv))
#A p-ertek 0.21>0.05, igy nincs eleg bizonyitekunk H0 ellen, nem tudjuk elvetni.

#c)

#1. megoldas, Kritikus ertek alapjan:
Probafv
qnorm(1-alpha)
#A Probafv. erteke most sem esik a krtikus tartomanyba, igy nem tudjuk elvetni H0-t

#2. megoldas, p-ertek alapjan:
1-pnorm(abs(Probafv))
#A p-ertek nagyobb, mint a 0.05 szignifikancia szint igy nem tudjuk elvetni a H0 hipotezist

#d)

korrigalt_tapasztalati_szoras=sd(minta)
Probafv=(mean(minta)-mu)/(korrigalt_tapasztalati_szoras/sqrt(length(minta)))
kritikus_ertek=qt(1-alpha/2, df=length(minta)-1)
#Mivel a Probafv ertek (1.2) nem haladja meg a kritikus erteket, igy nem tudjuk
#elvetni a H0 hipotezist

#2. megoldas p-ertek alapjan:
2*(1-pt(abs(Probafv), df=length(minta)-1))
#Mivel a p-ertek (0.26) nagyobb, mint 0.05, nincs eleg bizonyitekunk a H0
#hipotezis ellen, nem tudjuk elvetni.