adat=c(2,0,1,0,8,3,5,7,8,2,3,5,1,7,8,3,5,3,2,8)
sum(adat<3)

t_adat = table(adat)
names(t_adat)[t_adat==max(t_adat)] 

sd(adat)== sqrt(sum((adat-mean(adat))^2)/(length(adat)))

rep=rep(c("A","B"),c(10,10))
df = data.frame(adat=adat,rep=rep)
library(ggplot2)
ggplot(df, aes(x = rep, y = adat)) +
  geom_boxplot(fill = "gold") +
  scale_x_discrete (name = "A  és  B csoport")


#3.gyak
#3. feladat
n_sim<-100000
lambda<-0.5
n<-6
becslofv<-c()
becslofv_mertani<-c()

for(i in 1:n_sim){
  minta<-rexp(n, lambda)
  becslofv<-c(becslofv, n/sum(minta))
  becslofv_mertani<-c(becslofv_mertani, 1/prod(minta)^(1/n))
}

mean(becslofv)
mean(becslofv_mertani)
0.5*gamma(5/6)^6




n_sim=100000
n=6
theta=3
becslofv_egyenletes_MLE<-c()
becslofv_egyenletes_Momentum<-c()
becslofv_egyenletes_MLEkorr<-c()

for (i in 1:n_sim){
  minta<-
  becslofv_egyenletes_MLE
  becslofv_egyenletes_Momentum
  becslofv_egyenletes_MLEkorr
    
    
    
}






