#1. feladat a)
atletika<-c(79.2, 64.6, 86.8, 73.7, 74.9, 62.3)
probafv<-(mean(atletika)-84.8)/(12.8/sqrt(length(atletika)))

#1. megoldás p-érték alapján:
pnorm(probafv)
#A p-érték 0.016 kisebb, mint az 5%-os szignifikancia szint,
#így elvetjük a nullhipotézisünket, tehát az átlagos vérnyomás
#szignifikánsan kisebb, mint 84.8.

#2. megoldás kritikus érték alapján:
qnorm(0.05)
#A kritikus érték -1.645, a próbafüggvény -2.15, tehát a kritikus
#tartományba esik, így elvetjük a nullhipotézisünket.

#Ha kétoldali próba lett volna, p-érték alapján:

2*(1-pnorm(abs(probafv)))
#A p-érték 0.032, kisebb, mint 0.05, tehát elvetjük a nullhipotézisünket.

#Kritikus érték alapján, 2. megoldás kétoldali próbánál:
#Felső kritikus érték:
qnorm(1-0.05/2)
#Alsó kritikus érték:
qnorm(0.05/2)

#A próbafüggvény -2.15, kisebb, mint az elsó kritikus érték,
#tehát elvetjük a nullhipotézisünket.

#b)

sakk<-c(84.6, 93.2, 104.6, 106.7, 76.3, 78.2)
probafv<-(mean(sakk)-mean(atletika))/(sqrt((12.8^2)/length(sakk))+
                                      (12.8^2)/length(atletika))

#1.megoldás p-érték alapján:
1-pnorm(probafv)
#A p-érték 0.3>0.05 szignifikancia szint, így nem tudjuk elvetni a 
#nullhipotézisünket, tehát a sakkozók átlagos vérnyomása nem nagyobb
#szignifikánsan, mint az atlétáké.

#2. megoldás, Kritikus érték alapján:

qnorm(1-0.05)
#A próbafüggvény nem nagyobb, mint a kritikus érték, tehát nem tudjuk
#elvetni a nullhipotézisünket.

#2. feladat a)

minta<-c(14.8, 12.2, 16.8, 11.1)
sigma<-2
n<-length(minta)
alpha<-1-0.95
Also_hatar<-mean(minta)-sigma/sqrt(n)*qnorm(1-alpha/2)
Felso_hatar<-mean(minta)+sigma/sqrt(n)*qnorm(1-alpha/2)

#b)
Also_hatar<-mean(minta)-sd(minta)/sqrt(n)*qt(1-alpha/2, n-1)
Felso_hatar<-mean(minta)+sd(minta)/sqrt(n)*qt(1-alpha/2, n-1)

#c)

(2*sigma*qnorm(1-alpha/2)/1.6)^2
#Hogy meglegyen a teljes lefedés felfelé kerekítünk, legalább 25 mintaelem kell.


#3. feladat

mokka<-c(8.2, 5.0, 6.8, 6.7, 5.8, 7.3, 6.4, 7.8)
koffe<-c(5.1, 4.3, 3.4, 3.7, 6.1, 4.7)

var.test(mokka, koffe)
#A p-érték 0.93, nem tudjuk elvetni a nullhipotézisünket, tehát
#a szórások nem szignifikánsan különbözők.

t.test(mokka, koffe, alternative='greater', paired=FALSE, var.equal=TRUE)
#A p-érték 0.0009<0.05, tehát elvetjük a nullhipotézisünket,
#a Mokka kávé szignifikánsan lassabban oldódik.