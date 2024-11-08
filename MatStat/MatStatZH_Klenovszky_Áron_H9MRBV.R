#1. feladat
hozamok<-c(-1.02, -0.4, 2.99, 0.17, 0.28, 3.28, 0.91, -2.35, -1.26, -0.8)

#a)
mean(hozamok) # Átlag

sqrt(sum((hozamok-mean(hozamok))^2)/length(hozamok)) #Tapasztalati szórás
sqrt(sum((hozamok-mean(hozamok))^2)/(length(hozamok)-1)) #Korrigált tapasztalati szórás
sd(hozamok)

sd(hozamok)/mean(hozamok) # Szórási együttható

#b)
novelt <-c()
novelt = hozamok + 0.02

mean(novelt) # Átlag
sqrt(sum((novelt-mean(novelt))^2)/length(novelt)) #Tapasztalati szórás
sqrt(sum((novelt-mean(novelt))^2)/(length(novelt)-1)) #Korrigált tapasztalati szórás
sd(novelt)
sd(novelt)/mean(novelt) # Szórási együttható



novelt2 <-c()
novelt2 = hozamok * 1.2

mean(novelt2) # Átlag
sqrt(sum((novelt2-mean(novelt2))^2)/length(novelt2)) #Tapasztalati szórás
sqrt(sum((novelt2-mean(novelt2))^2)/(length(novelt2)-1)) #Korrigált tapasztalati szórás
sd(novelt2)
sd(novelt2)/mean(novelt2) # Szórási együttható

#c)

sum((hozamok-mean(hozamok))^3)/(sd(hozamok)^3)/length(hozamok) # Ferdeség
# + => Jobb oldali asszimetria: Az átlagtól felfelé eltérő értékek nagyobb arányban fordulnak elő, mint a negatív irányba

sum((hozamok-mean(hozamok))^4)/(sd(hozamok)^4)/length(hozamok)-3 # Lapultság
# - => 00A normális eloszlásnál lapultabb

#d)
boxplot(hozamok)
# Téglalap felső oldala -> 3.Quartilis , 75. percentilis
# Ábra alján lévő vízszintes vonal -> max{min , Q1 - 1.5 * (Q3 - Q1)}


#4. feladat

# Torítatlanság lellenőrzése

lambda<-5 
n<-c(10, 20, 40, 80, 160)
n_szim<-10000
for (i in n){
  
  lambda_becs<-c()
  
  for (j in 1:n_szim){
    minta<-rpois(i, lambda)
    lambda_becs<-c(lambda_becs, mean(minta)) 
  }
  
  print(mean(lambda_becs))
}

# Mivel jó közelítéssel visszakapjuk a lambdát, így bebizonyosodik, hogy a becslések torzítatlanok




