# 1)

olimp1<-c(25, 29, 17, 32, 37, 16)
olimp2<-c(23, 18, 20, 35, 36, 18)
# h0: mu1 = mu2
# h1: mu1 > mu2
# adatok nem függetlenek, szórás nem ismert: egymitás t-próba 
t.test(olimp1, olimp2, alternative='greater', paired=TRUE)
# p-val = 0.3325 > 0.05 nem vethetjük el a nullhipotézisünket
# nem teljestettek szignifikánsan rosszabbul

#2)
nezok1 <- c(623,700,782, 957, 789)
nezok2 <- c(782, 2161, 880, 822, 1500)
# a két minta független, szórások nem ismertek
# F-próba
# h0: szig1 = szig2
# h1: szig1 != szig2
var.test(nezok1, nezok2)
# p-val = 0.01008 < 0.05  elvetjük a nullhipotézisünket
# a szórások szignifikánsn különböző
# kétmintás t-próba
# h0: mu1 = mu2
# h0: mu1 < mu2
t.test(nezok1, nezok2, alternative='less', paired=FALSE, var.equal=FALSE)
# p-val = 0.08125 > 0.05, nem vetjük el a nullhipotézisunket
# szignifikánsan nem fejlődött a felcsúti foci


#3)
minta<-c(6, 3, 9, 7, 5, 6, 4, 7, 1, 8)
n<-length(minta)
alpha<-1-0.95
Also_hatar<-mean(minta)-sd(minta)/sqrt(n)*qt(1-alpha/2, n-1)
Felso_hatar<-mean(minta)+sd(minta)/sqrt(n)*qt(1-alpha/2, n-1)











