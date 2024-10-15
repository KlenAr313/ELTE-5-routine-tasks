#1. feladat
pontszamok<-c(41, 34, 41, 39, 42, 37, 38, 41, 33, 37, 28, 37, 36, 29, 27, 50, 0)
#a)
mean(pontszamok)
sum(pontszamok)/length(pontszamok)

sqrt(sum((pontszamok-mean(pontszamok))^2)/length(pontszamok)) #Tapasztalati szórás
sqrt(sum((pontszamok-mean(pontszamok))^2)/(length(pontszamok)-1)) #Korrigált tapasztalati szórás
sd(pontszamok)

quantile(pontszamok, 0.75)

#b)
sum((pontszamok-mean(pontszamok))^3)/(sd(pontszamok)^3)/length(pontszamok)
sum((pontszamok-mean(pontszamok))^4)/(sd(pontszamok)^4)/length(pontszamok)-3

#d)
boxplot(pontszamok)

#3. feladat

mu<-10
sigma<-3
n<-c(10, 20, 40, 80, 160)
n_szim<-10000
for (i in n){
  
  sigma_becs_1<-c()
  sigma_becs_2<-c()
  
  for (j in 1:n_szim){
    minta<-rnorm(i, mu, sigma)
    sigma_becs_1<-c(sigma_becs_1, sd(minta)^2)
    sigma_becs_2<-c(sigma_becs_2, (sd(minta)*(i-1)/i)^2)
  }
  
  print(mean(sigma_becs_1))
  print(mean(sigma_becs_2))
}

#4. feladat
minta<-c(3,4,4,4,5)
mean(minta)
sum(minta^2)/length(minta)

(8+sqrt(4.8))/2
(8-sqrt(4.8))/2

#5. feladat

steps = c(7415, 6525, 7066, 6498, 6174, 7167, 6104, 7168, 7355, 6948, 6804, 6351, 5890, 7849, 7022, 7831, 5756, 7169, 7673, 6974, 6809, 6218, 9058, 7751, 8824, 7080, 6369, 5487, 6364, 7226, 8014, 7253, 5828, 7669, 5350, 6634, 6684, 5052, 7920, 6377, 6666, 8395, 7637, 6892, 7514, 7399, 8663, 7276, 7506, 7348, 6623, 7098, 8639, 6124, 7122, 8362, 6765, 5947, 6130, 6610, 6153, 6739, 6586, 6817, 7407, 7625, 8678, 6931, 6679, 8471, 8704, 7043, 6667, 5178, 8411, 6162, 5876, 10044, 7235, 6967, 4268, 6900, 7976, 7414, 7912, 8984, 8169, 6491, 7704, 6802, 6462, 4144, 6210, 7488, 9168, 7501, 7620, 6034, 7163, 4922)
mean(steps) #Mu
sqrt(sum((steps-mean(steps))^2)/length(steps)) #Sigma

