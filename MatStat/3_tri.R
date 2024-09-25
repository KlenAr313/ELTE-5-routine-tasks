# https://amiklos.web.elte.hu/Oktatas/2024osz/Matstat/stat_a24_gyak2.html

#1 a)
dobasok <-c(1, 3, 6, 1)

atlag <- mean(dobasok) #Átlag
sqrt(mean((dobasok-atlag)^2)) #Tapasztalati szórás
sqrt(mean((dobasok-atlag)^2)/(length(dobasok)-1)) # Korrigált szórás
sd(dobasok) #Szórás együttható

mean(dobasok^2)

#b)

elmeleti_kimenetelek <-c(1,2,3,4,5,6)
elmeleti_elosztlasfv <- ecdf(elmeleti_kimenetelek)

par(mfrow=c(2,1))
png('Elmeleti:tapasztalati_elsozlasfv.png')

plot(elmeleti_elosztlasfv)

#c)

ruinf #Véletlenszám generálás
punif #Eloszlásfüggvény
qunif #Eloszlásfiggvény inverze
duinf #Sűrűségfüggvény

minta <- floor(runif(100,1,7))
table(minta)
minta_tapasztalati_elsozlas <- ecdf(minta)
plot(minta_tapasztalati_elsozlas, new=FALSE)
