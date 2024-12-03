#1. feladat
n_sim<-1000
n<-90
lambda<-2

lambda_MLE_vect<-c()
lambda_reciprok_becsles_vect<-c()

for (i in 1:n_sim){
  minta<-rexp(n, rate=2)
  lambda_MLE<-(n-1)/sum(minta)
  lambda_reciprok_becsles<-mean(minta)
  
  lambda_MLE_vect<-c(lambda_MLE_vect, lambda_MLE)
  lambda_reciprok_becsles_vect<-c(lambda_reciprok_becsles_vect, 
                                  lambda_reciprok_becsles)
}
mean(lambda_MLE_vect)
mean(lambda_reciprok_becsles_vect)

sd(lambda_MLE_vect)^2
(lambda^2)/n

sd(lambda_reciprok_becsles_vect)^2
1/((lambda^2)*n)

#2. feladat

#1. feladat
n_sim<-1000
n<-100
a<-10

a_MLE_vect<-c()
a_negyzet_becsles_vect<-c()

for (i in 1:n_sim){
  minta<-runif(n, max=a)
  a_MLE<-max(minta)*(n+1)/n
  a_negyzet_becsles<-a_MLE^2
  
  a_MLE_vect<-c(a_MLE_vect, a_MLE)
  a_negyzet_becsles_vect<-c(a_negyzet_becsles_vect, 
                            a_negyzet_becsles)
}
mean(a_MLE_vect)
mean(a_negyzet_becsles_vect)

sd(a_MLE_vect)^2
(a^2)/(n^2)

sd(a_negyzet_becsles_vect)^2
(2*(a^3))/n^2

#3. feladat

minta<-c(6, 4.5, 2.5, 2, 1)

also_hatar<-mean(minta)-qnorm(1-0.05/2)*2/sqrt(length(minta))
felso_hatar<-mean(minta)+qnorm(1-0.05/2)*2/sqrt(length(minta))
also_hatar
felso_hatar
